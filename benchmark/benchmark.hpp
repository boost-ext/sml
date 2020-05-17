//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#if defined(CHECK_COMPILE_TIME)
template <class T>
void benchmark_execution_speed(const T& expr) noexcept {
  expr();
}

template <class T>
void benchmark_memory_usage(const T&) noexcept {}
#else
#include <chrono>
#include <iostream>

template <class T>
void benchmark_execution_speed(const T& expr) noexcept {
  auto start = std::chrono::high_resolution_clock::now();
  expr();
  auto finish = std::chrono::high_resolution_clock::now();
  std::cout << "execution speed: " << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() << "ms"
            << std::endl;
}

template <class T>
void benchmark_memory_usage(const T& object) noexcept {
  std::cout << "memory usage: " << sizeof(object) << "b" << std::endl;
}
#endif
