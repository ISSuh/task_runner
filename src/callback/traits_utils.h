/**
 *
 *  Copyright:  Copyright (c) 2021, ISSuh
 *
 */

#ifndef CALLBACK_TRAITS_UTILS_H_
#define CALLBACK_TRAITS_UTILS_H_

#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace runner {

template <typename T>
using PassingType = typename std::conditional<std::is_scalar<T>::value, T, T&&>::type;

template <typename...>
struct make_void {
  using type = void;
};

template <typename... Ts>
using void_t = typename make_void<Ts...>::type;

template< class T >
using decay_t = typename std::decay<T>::type;

template <typename Functor, typename SFINAE = void>
struct IsCallableObject : std::false_type {};

template <typename Callable>
struct IsCallableObject<Callable, void_t<decltype(&Callable::operator())>>
    : std::true_type {};

template <typename... Types>
struct TypeList {};

template <size_t n, typename List>
struct DropTypeListItemImpl;

template <size_t n, typename T, typename... List>
struct DropTypeListItemImpl<n, TypeList<T, List...>>
    : DropTypeListItemImpl<n - 1, TypeList<List...>> {};

template <typename T, typename... List>
struct DropTypeListItemImpl<0, TypeList<T, List...>> {
  using Type = TypeList<T, List...>;
};

template <>
struct DropTypeListItemImpl<0, TypeList<>> {
  using Type = TypeList<>;
};

template <size_t n, typename List>
using DropTypeListItem = typename DropTypeListItemImpl<n, List>::Type;

template <size_t n, typename List, typename... Accum>
struct TakeTypeListItemImpl;

template <size_t n, typename T, typename... List, typename... Accum>
struct TakeTypeListItemImpl<n, TypeList<T, List...>, Accum...>
    : TakeTypeListItemImpl<n - 1, TypeList<List...>, Accum..., T> {};

template <typename T, typename... List, typename... Accum>
struct TakeTypeListItemImpl<0, TypeList<T, List...>, Accum...> {
  using Type = TypeList<Accum...>;
};

template <typename... Accum>
struct TakeTypeListItemImpl<0, TypeList<>, Accum...> {
  using Type = TypeList<Accum...>;
};

template <size_t n, typename List>
using TakeTypeListItem = typename TakeTypeListItemImpl<n, List>::Type;

template <typename List1, typename List2>
struct ConcatTypeListsImpl;

template <typename... Types1, typename... Types2>
struct ConcatTypeListsImpl<TypeList<Types1...>, TypeList<Types2...>> {
  using Type = TypeList<Types1..., Types2...>;
};

template <typename List1, typename List2>
using ConcatTypeLists = typename ConcatTypeListsImpl<List1, List2>::Type;

template <typename R, typename ArgList>
struct MakeFunctionTypeImpl;

template <typename R, typename... Args>
struct MakeFunctionTypeImpl<R, TypeList<Args...>> {
  typedef R Type(Args...);
};

template <typename R, typename ArgList>
using MakeFunctionType = typename MakeFunctionTypeImpl<R, ArgList>::Type;

template <typename Signature>
struct ExtractArgsImpl;

template <typename R, typename... Args>
struct ExtractArgsImpl<R(Args...)> {
  using ReturnType = R;
  using ArgsList = TypeList<Args...>;
};

template <typename Signature>
using ExtractArgs = typename ExtractArgsImpl<Signature>::ArgsList;

template <typename Signature>
using ExtractReturnType = typename ExtractArgsImpl<Signature>::ReturnType;

template <typename Callable,
          typename Signature = decltype(&Callable::operator())>
struct ExtractCallableRunTypeImpl;

template <typename Callable, typename R, typename... Args>
struct ExtractCallableRunTypeImpl<Callable, R (Callable::*)(Args...)> {
  using Type = R(Args...);
};

template <typename Callable, typename R, typename... Args>
struct ExtractCallableRunTypeImpl<Callable, R (Callable::*)(Args...) const> {
  using Type = R(Args...);
};

template <typename Callable>
using ExtractCallableRunType = typename ExtractCallableRunTypeImpl<Callable>::Type;

template<typename T, T... Ints>
struct integer_sequence {
  typedef T value_type;
  static constexpr std::size_t size() { return sizeof...(Ints); }
};

template<std::size_t... Ints>
using index_sequence = integer_sequence<std::size_t, Ints...>;

template<typename T, std::size_t N, T... Is>
struct make_integer_sequence : make_integer_sequence<T, N-1, N-1, Is...> {};

template<typename T, T... Is>
struct make_integer_sequence<T, 0, Is...> : integer_sequence<T, Is...> {};

template<std::size_t N>
using make_index_sequence = make_integer_sequence<std::size_t, N>;

template<typename... T>
using index_sequence_for = make_index_sequence<sizeof...(T)>;

}  // namespace runner

#endif  // CALLBACK_TRAITS_UTILS_H_
