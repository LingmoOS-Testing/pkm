/**
 *  SPDX-License-Identifier: MulanPSL-2.0
 *  (C) Copyright 2024 Elysia. All Rights Reserved.
 *  Description：utils
 *  Author：Elysia
 *  Date: 24-8-25
 *  Modify Record:
 */

#include "utils.h"

hash_t hash_(char const* str) {
  hash_t ret{basis};

  while (*str) {
    ret ^= *str;
    ret *= prime;
    str++;
  }

  return ret;
}
