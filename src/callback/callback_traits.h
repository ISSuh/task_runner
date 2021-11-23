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
  
template <typename R, typename... Args>
class TaskCallback;

template <typename T>
using PassingType = std::conditional<std::is_scalar<T>::value, T, T&&>;

// FunctorTraits<>
template <typename Functor>
struct FunctorTraits;

// For function
template <typename R, typename... Args>
struct FunctorTraits<R (*)(Args...)> {
  using return_type = R;
  using RunType = R(Args...);
  static constexpr bool is_method = false;
  static constexpr bool is_nullable = true;

  template <typename Function, typename... RunArgs>
  static R Invoke(Function&& function, RunArgs&&... args) {
    return std::forward<Function>(function)(std::forward<RunArgs>(args)...);
  }
};

// For member function
template <typename R, typename Receiver, typename... Args>
struct FunctorTraits<R (Receiver::*)(Args...)> {
  using return_type = R;
  using RunType = R(Receiver*, Args...);
  static constexpr bool is_method = true;
  static constexpr bool is_nullable = true;

  template <typename Method, typename ReceiverPtr, typename... RunArgs>
  static R Invoke(Method method,
                  ReceiverPtr&& receiver_ptr,
                  RunArgs&&... args) {
    return ((*receiver_ptr).*method)(std::forward<RunArgs>(args)...);
  }
};

// For const member function
template <typename R, typename Receiver, typename... Args>
struct FunctorTraits<R (Receiver::*)(Args...) const> {
  using return_type = R;
  using RunType = R(const Receiver*, Args...);
  static constexpr bool is_method = true;
  static constexpr bool is_nullable = true;

  template <typename Method, typename ReceiverPtr, typename... RunArgs>
  static R Invoke(Method method,
                  ReceiverPtr&& receiver_ptr,
                  RunArgs&&... args) {
    return ((*receiver_ptr).*method)(std::forward<RunArgs>(args)...);
  }
};

// For lambda fucntion
// template <typename R, typename Receiver, typename... Args>
// struct FunctorTraits<R(&Receiver::operator())(Args...) const> {
//   using return_type = R;
//   using RunType = R(const Receiver*, Args...);
//   static constexpr bool is_method = true;
//   static constexpr bool is_nullable = true;

//   template <typename Method, typename ReceiverPtr, typename... RunArgs>
//   static R Invoke(Method method,
//                   ReceiverPtr&& receiver_ptr,
//                   RunArgs&&... args) {
//     return (Receiver::operator())(std::forward<RunArgs>(args)...);
//   }
// };

// For TaskCallback.
template <typename R, typename... Args>
struct FunctorTraits<TaskCallback<R(Args...)>> {
  using return_type = R;
  using RunType = R(Args...);
  static constexpr bool is_method = false;
  static constexpr bool is_nullable = true;

  template <typename CallbackType, typename... RunArgs>
  static R Invoke(CallbackType&& callback, RunArgs&&... args) {
    return std::forward<CallbackType>(callback).Run(std::forward<RunArgs>(args)...);
  }
};

// InvokeHelper<>
template< class T >
using decay_t = typename std::decay<T>::type;

template <typename Functor>
using MakeFunctorTraits = FunctorTraits<decay_t<Functor>>;

}  // namespace runner

#endif  // CALLBACK_CALLBACK_TRAITS_H_
