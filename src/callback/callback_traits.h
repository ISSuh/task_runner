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
template <typename R, typename Class, typename... Args>
struct FunctorTraits<R (Class::*)(Args...)> {
  using return_type = R;
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
  using return_type = R;
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
template< class T >
using decay_t = typename std::decay<T>::type;

template <typename Functor>
using MakeFunctorTraits = FunctorTraits<decay_t<Functor>>;

}  // namespace runner

#endif  // CALLBACK_CALLBACK_TRAITS_H_
