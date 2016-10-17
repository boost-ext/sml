//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-string-literal-operator-template"
#pragma clang diagnostic ignored "-Wzero-length-array"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

#if !defined(__has_builtin)
#define __has_builtin(...) 0
#endif
#if !defined(__has_extension)
#define __has_extension(...) 0
#endif

#include <cstdio>
#include <cstdlib>

#define expect(...) (void)((__VA_ARGS__) || (expect_fail__(#__VA_ARGS__, __FILE__, __LINE__), 0))
#define static_expect(...) static_assert((__VA_ARGS__), "fail")

inline void expect_fail__(const char *msg, const char *file, int line) {
  std::printf("%s:%d:%s\n", file, line, msg);
  std::exit(-1);
}

struct test {
  template <class Test>
  test(const Test &test) {
    test();
  }
};

int main() {}
