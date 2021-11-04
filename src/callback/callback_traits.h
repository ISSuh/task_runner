/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef CALLBACK_CALLBACK_TRAITS_H_
#define CALLBACK_CALLBACK_TRAITS_H_

#include <stddef.h>

#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>

#include "callback/callback.h"

namespace runner {

// FunctorTraits<>
template <typename Functor>
struct FunctorTraits;

// For function
template <typename R, typename... Args>
struct FunctorTraits<R (*)(Args...)> {
  using RunType = R(Args...);
  static constexpr bool is_method = false;
  static constexpr bool is_nullable = true;

  template <typename Function, typename... RunArgs>
  static R Invoke(Function&& function, RunArgs&&... args) {
    return std::forward<Function>(function)(std::forward<RunArgs>(args)...);
  }
};

// For member function
template <typename R, typename Class, typename... Args>
struct FunctorTraits<R (Class::*)(Args...)> {
  using RunType = R(Class*, Args...);
  static constexpr bool is_method = true;
  static constexpr bool is_nullable = true;

  template <typename Method, typename ClassPtr, typename... RunArgs>
  static R Invoke(Method method, ClassPtr&& class_ptr, RunArgs&&... args) {
    return ((*class_ptr).*method)(std::forward<RunArgs>(args)...);
  }
};

// For const member function
template <typename R, typename Class, typename... Args>
struct FunctorTraits<R (Class::*)(Args...) const> {
  using RunType = R(Class*, Args...);
  static constexpr bool is_method = true;
  static constexpr bool is_nullable = true;

  template <typename Method, typename ClassPtr, typename... RunArgs>
  static R Invoke(Method method, ClassPtr&& class_ptr, RunArgs&&... args) {
    return ((*class_ptr).*method)(std::forward<RunArgs>(args)...);
  }
};

// For lambda fucntion
template <typename T>
struct FunctorTraits : public FunctorTraits<decltype(&T::operator())> {};

// InvokeHelper<>
template <bool test, typename Functor>
using MakeFunctorTraits = FunctorTraits<std::decay_t<Functor>>;

template <typename ReturnType>
struct InvokeHelper {
  template <typename Functor, typename... RunArgs>
  static inline ReturnType TryInvoke(Functor&& functor, RunArgs&&... args) {
    using Traits = MakeFunctorTraits<Functor>;
    return Traits::Invoke(std::forward<Functor>(functor),
                          std::forward<RunArgs>(args)...);
  }
};

// Invoker<>
//
// See description at the top of the file.
template <typename StorageType, typename UnboundRunType>
struct Invoker;

template <typename StorageType, typename R, typename... UnboundArgs>
struct Invoker<StorageType, R(UnboundArgs...)> {
  static R RunOnce(BindStateBase* base,
                   PassingType<UnboundArgs>... unbound_args) {
    // Local references to make debugger stepping easier. If in a debugger,
    // you really want to warp ahead and step through the
    // InvokeHelper<>::MakeItSo() call below.
    StorageType* storage = static_cast<StorageType*>(base);
    static constexpr size_t num_bound_args =
        std::tuple_size<decltype(storage->bound_args_)>::value;
    return RunImpl(std::move(storage->functor_),
                   std::move(storage->bound_args_),
                   std::make_index_sequence<num_bound_args>(),
                   std::forward<UnboundArgs>(unbound_args)...);
  }

  static R Run(BindStateBase* base, PassingType<UnboundArgs>... unbound_args) {
    // Local references to make debugger stepping easier. If in a debugger,
    // you really want to warp ahead and step through the
    // InvokeHelper<>::MakeItSo() call below.
    const StorageType* storage = static_cast<StorageType*>(base);
    static constexpr size_t num_bound_args =
        std::tuple_size<decltype(storage->bound_args_)>::value;
    return RunImpl(storage->functor_, storage->bound_args_,
                   std::make_index_sequence<num_bound_args>(),
                   std::forward<UnboundArgs>(unbound_args)...);
  }

 private:
  template <typename Functor, typename BoundArgsTuple, size_t... indices>
  static inline R RunImpl(Functor&& functor,
                          BoundArgsTuple&& bound,
                          std::index_sequence<indices...>,
                          UnboundArgs&&... unbound_args) {
    static constexpr bool is_method = MakeFunctorTraits<Functor>::is_method;

    using DecayedArgsTuple = std::decay_t<BoundArgsTuple>;

    return InvokeHelper<R>::MakeItSo(
        std::forward<Functor>(functor),
        Unwrap(std::get<indices>(std::forward<BoundArgsTuple>(bound)))...,
        std::forward<UnboundArgs>(unbound_args)...);
  }

}  // namespace runner

#endif  // CALLBACK_CALLBACK_TRAITS_H_
