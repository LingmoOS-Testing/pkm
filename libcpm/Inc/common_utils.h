/**
 *  (C) Copyright 2025 Elysia. All Rights Reserved.
 *  Description：common_utils
 *  Author：Elysia
 *  Date: 25-6-21
 *  Modify Record:
 */
#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H
#include <cstddef>
#include <string>

namespace cpkm::utils {
/**
 * Convert string to a has number, useful in switch-case
 * @param s
 * @param l
 * @param h
 * @return
 */
constexpr unsigned int str2tag_core(const char* s, const std::size_t l,
                                    const unsigned int h) {
  return l == 0
           ? h
           : str2tag_core(s + 1, l - 1,
                          (h * 33) + static_cast<unsigned char>(*s) - 'a' + 1);
}

/**
 * For easier use, use this string literal!
 * @param s
 * @param l
 * @return
 */
constexpr unsigned int operator""_tag(const char* s, const std::size_t l) {
  return str2tag_core(s, l, 0);
}

/**
 * Calculate the hash number of a std::string!
 * @param s
 * @return
 */
inline unsigned int str2tag(const std::string& s) {
  return str2tag_core(s.c_str(), s.size(), 0);
}
}
#endif //COMMON_UTILS_H