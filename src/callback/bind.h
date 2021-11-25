/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef CALLBACK_BIND_H_
#define CALLBACK_BIND_H_

#include <stddef.h>

#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <utility>

#include "callback/callback.h"
#include "callback/function_traits.h"

namespace runner {

template <typename R, typename... Args>
class TaskCallback;

template <typename Functor, typename... BoundArgs>
struct BindTypeHelper {
  static constexpr size_t num_bounds = sizeof...(BoundArgs);
  using FunctorTraits = MakeFunctorTraits<Functor>;
  using RunType = typename FunctorTraits::RunType;
  using ReturnType = ExtractReturnType<RunType>;

  using RunParamsList = ExtractArgs<RunType>;
  using BoundParamsList = TakeTypeListItem<num_bounds, RunParamsList>;
  using UnboundParamsList = DropTypeListItem<num_bounds, RunParamsList>;

  using BoundArgsList = TypeList<BoundArgs...>;

  using UnboundRunType = MakeFunctionType<ReturnType, UnboundParamsList>;
};

template <typename Functor, typename... BoundArgs>
using MakeUnboundRunType = typename BindTypeHelper<Functor, BoundArgs...>::UnboundRunType;

class BindStateBase {
 public:
  using InvokeFuncStorage = void(*)();
  InvokeFuncStorage polymorphic_invoke_;

 protected:
  BindStateBase(InvokeFuncStorage polymorphic_invoke)
    : polymorphic_invoke_(polymorphic_invoke) {}

  ~BindStateBase() = default;
};

template <typename Functor, typename... BoundArgs>
struct BindState final : BindStateBase {
  template <typename ForwardFunctor, typename... ForwardBoundArgs>
  static BindState* Create(BindStateBase::InvokeFuncStorage invoke_func,
                           ForwardFunctor&& functor,
                           ForwardBoundArgs&&... bound_args) {
    return new BindState(invoke_func,
                         std::forward<ForwardFunctor>(functor),
                         std::forward<ForwardBoundArgs>(bound_args)...);
  }

  Functor functor_;
  std::tuple<BoundArgs...> bound_args_;

 private:
  template <typename ForwardFunctor, typename... ForwardBoundArgs>
  explicit BindState(BindStateBase::InvokeFuncStorage invoke_func,
                     ForwardFunctor&& functor,
                     ForwardBoundArgs&&... bound_args)
      : BindStateBase(invoke_func),
        functor_(std::forward<ForwardFunctor>(functor)),
        bound_args_(std::forward<ForwardBoundArgs>(bound_args)...) {
  }

  ~BindState() = default;
};

template <bool is_class, typename ReturnType>
struct InvokeHelper;

template <typename ReturnType>
struct InvokeHelper<false, ReturnType> {
  template <typename Functor, typename... RunArgs>
  static inline ReturnType TryInvoke(Functor&& functor, RunArgs&&... args) {
    using Traits = MakeFunctorTraits<Functor>;
    return Traits::Invoke(std::forward<Functor>(functor),
                          std::forward<RunArgs>(args)...);
  }
};

template <typename ReturnType>
struct InvokeHelper<true, ReturnType> {
  template <typename Functor, typename BoundClassPtr, typename... RunArgs>
  static inline void TryInvoke(Functor&& functor,
                              BoundClassPtr&& class_ptr,
                              RunArgs&&... args) {
    if (!class_ptr)
      return;

    using Traits = MakeFunctorTraits<Functor>;
    Traits::Invoke(std::forward<Functor>(functor),
                   std::forward<BoundClassPtr>(class_ptr),
                   std::forward<RunArgs>(args)...);
  }
};

// Used to implement MakeBindStateType.
template <bool is_method, typename Functor, typename... BoundArgs>
struct MakeBindStateTypeImpl;

template <typename Functor, typename... BoundArgs>
struct MakeBindStateTypeImpl<false, Functor, BoundArgs...> {
  using Type = BindState<decay_t<Functor>, decay_t<BoundArgs>...>;
};

template <typename Functor>
struct MakeBindStateTypeImpl<true, Functor> {
  using Type = BindState<decay_t<Functor>>;
};

template <typename Functor, typename Receiver, typename... BoundArgs>
struct MakeBindStateTypeImpl<true, Functor, Receiver, BoundArgs...> {
  using Type = BindState<decay_t<Functor>, decay_t<Receiver>, decay_t<BoundArgs>...>;
};

template <typename Functor, typename... BoundArgs>
using MakeBindStateType =
    typename MakeBindStateTypeImpl<MakeFunctorTraits<Functor>::is_method,
                                   Functor,
                                   BoundArgs...>::Type;

// Invoker<>
template <typename>
struct BindUnwrapTraits {
  template <typename T>
  static T&& Unwrap(T&& o) {
    return std::forward<T>(o);
  }
};

template <typename T>
using Unwrapper = BindUnwrapTraits<decay_t<T>>;

template <typename T>
T&& Unwrap(T&& o) {
  return Unwrapper<T>::Unwrap(std::forward<T>(o));
}

template <typename StorageType, typename UnboundRunType>
struct Invoker;

template <typename StorageType, typename R, typename... UnboundArgs>
struct Invoker<StorageType, R(UnboundArgs...)> {
  static R Run(BindStateBase* base, PassingType<UnboundArgs>... unbound_args) {
    const StorageType* storage = static_cast<StorageType*>(base);
    static constexpr size_t num_bound_args = std::tuple_size<decltype(storage->bound_args_)>::value;
    return RunImpl(storage->functor_,
                   storage->bound_args_,
                   make_index_sequence<num_bound_args>(),
                   std::forward<UnboundArgs>(unbound_args)...);
  }

 private:
  template <typename Functor, typename BoundArgsTuple, size_t... indices>
  static inline R RunImpl(Functor&& functor,
                          BoundArgsTuple&& bound,
                          index_sequence<indices...>,
                          UnboundArgs&&... unbound_args) {
    static constexpr bool is_method = MakeFunctorTraits<Functor>::is_method;
    return InvokeHelper<is_method, R>::TryInvoke(
        std::forward<Functor>(functor),
        Unwrap(std::get<indices>(std::forward<BoundArgsTuple>(bound)))...,
        std::forward<UnboundArgs>(unbound_args)...);
  }
};

template <typename Functor, typename... Args>
TaskCallback<MakeUnboundRunType<Functor, Args...>> BindImpl(Functor&& functor, Args&&... args) {
  using BindState = MakeBindStateType<Functor, Args...>;
  using UnboundRunType = MakeUnboundRunType<Functor, Args...>;
  using Invoker = Invoker<BindState, UnboundRunType>;
  using CallbackType = TaskCallback<UnboundRunType>;

  using PolymorphicInvoke = typename CallbackType::PolymorphicInvoke;
  PolymorphicInvoke invoke_func = Invoker::Run;

  using InvokeFuncStorage = BindStateBase::InvokeFuncStorage;
  return CallbackType(BindState::Create(
      reinterpret_cast<InvokeFuncStorage>(invoke_func),
      std::forward<Functor>(functor), std::forward<Args>(args)...));
}

template <typename Functor, typename... Args>
TaskCallback<MakeUnboundRunType<Functor, Args...>> Bind(Functor&& functor, Args&&... args) {
  return BindImpl(std::forward<Functor>(functor), std::forward<Args>(args)...);
}

template <typename Signature>
TaskCallback<Signature> BindRepeating(TaskCallback<Signature> closure) {
  return closure;
}

}  // namespace runner

#endif  // CALLBACK_BIND_H_