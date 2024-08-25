/**
 *  SPDX-License-Identifier: MulanPSL-2.0
 *  (C) Copyright 2024 Elysia. All Rights Reserved.
 *  Description：utils
 *  Author：Elysia
 *  Date: 24-8-25
 *  Modify Record:
 */
#ifndef PKM_INC_UTILS_H_
#define PKM_INC_UTILS_H_

#include <cstdint>

typedef std::uint64_t hash_t;

constexpr hash_t prime = 0x100000001B3ull;
constexpr hash_t basis = 0xCBF29CE484222325ull;

// Compute the hash of a string.
hash_t hash_(char const* str);

constexpr hash_t hash_compile_time(char const* str, hash_t last_value = basis) {
  return *str ? hash_compile_time(str + 1, (*str ^ last_value) * prime)
              : last_value;
}
#endif  // PKM_INC_UTILS_H_
