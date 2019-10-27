#pragma once

#include <iostream>
#include <numeric>
#include <fstream>
#include <cstdint>
#include <date/date.h>
#include <date/tz.h>
#include <memory>
#include <functional>
#include <string>
#include <limits>
#include <exception>
#include <stdexcept>
#include <utility>
#include <chrono>
#include <vector>
#include <random>
#include <type_traits>
#include <bitset>
#include <filesystem>
#include <array>
#include <algorithm>

#include <cereal/cereal.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/chrono.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/tuple.hpp>
#include <cereal/types/bitset.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/utility.hpp>

using u32 = std::uint32_t;
using u64 = std::uint64_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

namespace detail
{

  template<class T>
  struct Length : std::integral_constant<u64, sizeof(T)> { static_assert(std::is_pod_v<T>, "Length<T> must only be used with POD types"); };

}

template<class T>
struct SizeOf;

/*
 *  Generate brand new instance of type
 **/
template<class T>
void ConstructNew(T &);

/*
 *  Get the size of the entity's content that would be stored in a file
 **/
template<class T>
struct SizeOf : std::integral_constant<u64, sizeof(T)> { };

template<template<typename, typename> class ArrayT, typename T, typename N>
struct SizeOf<ArrayT<T, N>> : std::integral_constant<u64, std::tuple_size<ArrayT>::value * sizeof(std::tuple_element<0, ArrayT>::type)> { };

/*
 *  Get length of entity's file content (in bytes)
 **/
template<class T>
inline u64 LengthOf(T &&t);

template<class ...T>
inline u64 LengthOf(T &&...t)
{
  return LengthOf(std::forward<T>(t)...);
}

/* Specialization for arithmetic types */
template<class T>
inline u64 LengthOf(T &t, typename std::enable_if<std::is_arithmetic_v<T>>::type* = 0)
{
  return detail::Length<T>::value;
}

/* Unwinding variadic calls to LengthOf - will eventually unwind to all calls to LengthOf with either an
 * arithmetic type or a custom type with LengthOf specialized for it
 * */
template<class T, class ...Ts>
inline u64 LengthOf(T &&t, Ts &&...ts)
{
  return LengthOf(std::forward<T>(t)) + LengthOf(std::forward<Ts>(ts)...);
}
/* Specialization for standard types */

/* std::vector */
template<template<class, class> class V, class T, class A>
inline u64 LengthOf(V<T, A> &vec, typename std::enable_if<std::is_same_v<std::vector<T,A>, decltype(vec)>>::type* = 0)
{
  auto fn = [](u64 x, auto &&y) { return x + LengthOf(std::forward<decltype(y)>(y)); };
  return std::accumulate(std::begin(vec), std::end(vec), u64(0), fn);
}

/* std::string */
template<template<class, class, class> class StringT, class CharT, class Traits, class Allocator>
inline u64 LengthOf(StringT<CharT, Traits, Allocator> &&Str)
{
  using char_type = typename StringT<CharT, Traits, Allocator>::char_type;
  return Str.length() * sizeof(char_type);
}

/* std::bitset */
template<template<class> class Bitset, class N>
inline u64 LengthOf(Bitset<N> &&b)
{
  return sizeof(b);
}

template<>
inline u64 LengthOf(std::bitset<16> &b)
{
  return static_cast<u64>(sizeof(b));
}

/* std::array */
template<typename T, unsigned N>
inline u64 LengthOf(const std::array<T, N> &arr)
{
  return static_cast<u64>(std::tuple_size<decltype(arr)>::value * sizeof(std::tuple_element<0, decltype(arr)>::type));
}
//template<template<typename, size_t> class ArrayT, typename T, size_t N>
//inline u64 LengthOf(ArrayT<T, N> &Array)
//{
//  return static_cast<u64>(std::tuple_size<ArrayT>::value * sizeof(T));
//}

template<class T, unsigned N>
inline u64 LengthOf(const T(&Array)[N])
{
  return static_cast<u64>(sizeof(T)) * N;
}

using sf_time_point = std::chrono::system_clock::time_point;
using sf_time_years_months_days = date::year_month_day;

template<>
inline u64 LengthOf(sf_time_point &tp)
{
  return detail::Length<typename sf_time_point::duration::rep>::value;
}

constexpr sf_time_point zero_time = sf_time_point(std::chrono::system_clock::duration(std::chrono::milliseconds(0)));

#define SPECIALIZE_CONSTRUCT_NEW_ENTITY(ENTITY_TYPE, PARAMETER_NAME)                \
template<> inline void ConstructNew<##ENTITY_TYPE##>(ENTITY_TYPE & PARAMETER_NAME)


SPECIALIZE_CONSTRUCT_NEW_ENTITY(u32, value)    { value = 0; }
SPECIALIZE_CONSTRUCT_NEW_ENTITY(u64, value)    { value = 0; }
SPECIALIZE_CONSTRUCT_NEW_ENTITY(i32, value)    { value = 0; }
SPECIALIZE_CONSTRUCT_NEW_ENTITY(i64, value)    { value = 0; }
SPECIALIZE_CONSTRUCT_NEW_ENTITY(float, value)  { value = 0.f; }
SPECIALIZE_CONSTRUCT_NEW_ENTITY(double, value) { value = 0.0; }
SPECIALIZE_CONSTRUCT_NEW_ENTITY(char, value)   { value = 0; }
