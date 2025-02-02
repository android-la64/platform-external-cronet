// Copyright 2013 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file defines some bit utilities.

#ifndef BASE_BITS_H_
#define BASE_BITS_H_

#include <stddef.h>
#include <stdint.h>

#include <bit>
#include <concepts>

#include "base/check.h"

namespace base::bits {

// Bit functions in <bit> are restricted to a specific set of types of unsigned
// integer; restrict functions in this file that are related to those in that
// header to match for consistency.
template <typename T>
concept UnsignedInteger =
    std::unsigned_integral<T> && !std::same_as<T, bool> &&
    !std::same_as<T, char> && !std::same_as<T, char8_t> &&
    !std::same_as<T, char16_t> && !std::same_as<T, char32_t> &&
    !std::same_as<T, wchar_t>;

// We want to migrate all users of these functions to use the unsigned type
// versions of the functions, but until they are all moved over, create a
// concept that captures all the types that must be supported for compatibility
// but that we want to remove.
template <typename T>
concept SignedIntegerDeprecatedDoNotUse =
    std::integral<T> && !UnsignedInteger<T>;

// Returns true iff |value| is a power of 2.
//
// TODO(https://crbug.com/1414634): Replace with std::has_single_bit().
template <typename T>
  requires UnsignedInteger<T>
constexpr bool IsPowerOfTwo(T value) {
  // From "Hacker's Delight": Section 2.1 Manipulating Rightmost Bits.
  //
  // Only positive integers with a single bit set are powers of two. If only one
  // bit is set in x (e.g. 0b00000100000000) then |x-1| will have that bit set
  // to zero and all bits to its right set to 1 (e.g. 0b00000011111111). Hence
  // |x & (x-1)| is 0 iff x is a power of two.
  return value > 0 && (value & (value - 1)) == 0;
}

template <typename T>
  requires SignedIntegerDeprecatedDoNotUse<T>
constexpr bool IsPowerOfTwoDeprecatedDoNotUse(T value) {
  return value > 0 && (value & (value - 1)) == 0;
}

// Round down |size| to a multiple of alignment, which must be a power of two.
template <typename T>
  requires UnsignedInteger<T>
inline constexpr T AlignDown(T size, T alignment) {
  DCHECK(IsPowerOfTwo(alignment));
  return size & ~(alignment - 1);
}

template <typename T>
  requires SignedIntegerDeprecatedDoNotUse<T>
inline constexpr T AlignDownDeprecatedDoNotUse(T size, T alignment) {
  DCHECK(IsPowerOfTwoDeprecatedDoNotUse(alignment));
  return size & ~(alignment - 1);
}

// Move |ptr| back to the previous multiple of alignment, which must be a power
// of two. Defined for types where sizeof(T) is one byte.
template <typename T>
  requires(sizeof(T) == 1)
inline T* AlignDown(T* ptr, uintptr_t alignment) {
  return reinterpret_cast<T*>(
      AlignDown(reinterpret_cast<uintptr_t>(ptr), alignment));
}

// Round up |size| to a multiple of alignment, which must be a power of two.
template <typename T>
  requires UnsignedInteger<T>
inline constexpr T AlignUp(T size, T alignment) {
  DCHECK(IsPowerOfTwo(alignment));
  return (size + alignment - 1) & ~(alignment - 1);
}

template <typename T>
  requires SignedIntegerDeprecatedDoNotUse<T>
inline constexpr T AlignUpDeprecatedDoNotUse(T size, T alignment) {
  DCHECK(IsPowerOfTwoDeprecatedDoNotUse(alignment));
  return (size + alignment - 1) & ~(alignment - 1);
}

// Advance |ptr| to the next multiple of alignment, which must be a power of
// two. Defined for types where sizeof(T) is one byte.
template <typename T>
  requires(sizeof(T) == 1)
inline T* AlignUp(T* ptr, uintptr_t alignment) {
  return reinterpret_cast<T*>(
      AlignUp(reinterpret_cast<uintptr_t>(ptr), alignment));
}

// Returns the integer i such as 2^i <= n < 2^(i+1).
//
// A common use for this function is to measure the number of bits required to
// contain a value; for that case use std::bit_width().
//
// A common use for this function is to take its result and use it to left-shift
// a bit; instead of doing so, use std::bit_floor().
// TODO(https://crbug.com/1414634): Replace existing uses that do that.
constexpr int Log2Floor(uint32_t n) {
  return 31 - std::countl_zero(n);
}

// Returns the integer i such as 2^(i-1) < n <= 2^i.
//
// A common use for this function is to measure the number of bits required to
// contain a value; for that case use std::bit_width().
//
// A common use for this function is to take its result and use it to left-shift
// a bit; instead of doing so, use std::bit_ceil().
// TODO(https://crbug.com/1414634): Replace existing uses that do that.
constexpr int Log2Ceiling(uint32_t n) {
  // When n == 0, we want the function to return -1.
  // When n == 0, (n - 1) will underflow to 0xFFFFFFFF, which is
  // why the statement below starts with (n ? 32 : -1).
  return (n ? 32 : -1) - std::countl_zero(n - 1);
}

// Returns a value of type T with a single bit set in the left-most position.
// Can be used instead of manually shifting a 1 to the left. Unlike the other
// functions in this file, usable for any integral type.
template <typename T>
  requires std::integral<T>
constexpr T LeftmostBit() {
  T one(1u);
  return one << (8 * sizeof(T) - 1);
}

}  // namespace base::bits

#endif  // BASE_BITS_H_
