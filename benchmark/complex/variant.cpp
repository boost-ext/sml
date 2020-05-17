//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <cstdio>
#include <cstdlib>
#include <type_traits>
#include <variant>
#include "benchmark.hpp"

template <class... Ts>
struct overload : Ts... {
  using Ts::operator()...;
  template <class... TArgs>
  overload(TArgs&&... args) : TArgs(std::forward<TArgs>(args))... {}
};

template <class... Ts>
overload(Ts...)->overload<std::decay_t<Ts>...>;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};
struct e6 {};
struct e7 {};
struct e8 {};
struct e9 {};
struct e10 {};
struct e11 {};
struct e12 {};
struct e13 {};
struct e14 {};
struct e15 {};
struct e16 {};
struct e17 {};
struct e18 {};
struct e19 {};
struct e20 {};
struct e21 {};
struct e22 {};
struct e23 {};
struct e24 {};
struct e25 {};
struct e26 {};
struct e27 {};
struct e28 {};
struct e29 {};
struct e30 {};
struct e31 {};
struct e32 {};
struct e33 {};
struct e34 {};
struct e35 {};
struct e36 {};
struct e37 {};
struct e38 {};
struct e39 {};
struct e40 {};
struct e41 {};
struct e42 {};
struct e43 {};
struct e44 {};
struct e45 {};
struct e46 {};
struct e47 {};
struct e48 {};
struct e49 {};
struct e50 {};

struct c {
  struct S1 {};
  struct S2 {};
  struct S3 {};
  struct S4 {};
  struct S5 {};
  struct S6 {};
  struct S7 {};
  struct S8 {};
  struct S9 {};
  struct S10 {};
  struct S11 {};
  struct S12 {};
  struct S13 {};
  struct S14 {};
  struct S15 {};
  struct S16 {};
  struct S17 {};
  struct S18 {};
  struct S19 {};
  struct S20 {};
  struct S21 {};
  struct S22 {};
  struct S23 {};
  struct S24 {};
  struct S25 {};
  struct S26 {};
  struct S27 {};
  struct S28 {};
  struct S29 {};
  struct S30 {};
  struct S31 {};
  struct S32 {};
  struct S33 {};
  struct S34 {};
  struct S35 {};
  struct S36 {};
  struct S37 {};
  struct S38 {};
  struct S39 {};
  struct S40 {};
  struct S41 {};
  struct S42 {};
  struct S43 {};
  struct S44 {};
  struct S45 {};
  struct S46 {};
  struct S47 {};
  struct S48 {};
  struct S49 {};
  struct S50 {};

  bool guard() { return true; }
  void action() {}

  std::variant<S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11, S12, S13, S14, S15, S16, S17, S18, S19, S20, S21, S22, S23, S24,
               S25, S26, S27, S28, S29, S30, S31, S32, S33, S34, S35, S36, S37, S38, S39, S40, S41, S42, S43, S44, S45, S46,
               S47, S48, S49, S50> state;
public:
  void handle(const e1&) {
    std::visit(overload{
        [&](S1) { if (guard()) action(); state = S2{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e2&) {
    std::visit(overload{
        [&](S2) { if (guard()) action(); state = S3{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e3&) {
    std::visit(overload{
        [&](S3) { if (guard()) action(); state = S4{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e4&) {
    std::visit(overload{
        [&](S4) { if (guard()) action(); state = S5{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e5&) {
    std::visit(overload{
        [&](S5) { if (guard()) action(); state = S6{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e6&) {
    std::visit(overload{
        [&](S6) { if (guard()) action(); state = S7{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e7&) {
    std::visit(overload{
        [&](S7) { if (guard()) action(); state = S8{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e8&) {
    std::visit(overload{
        [&](S8) { if (guard()) action(); state = S9{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e9&) {
    std::visit(overload{
        [&](S9) { if (guard()) action(); state = S10{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e10&) {
    std::visit(overload{
        [&](S10) { if (guard()) action(); state = S11{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e11&) {
    std::visit(overload{
        [&](S11) { if (guard()) action(); state = S12{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e12&) {
    std::visit(overload{
        [&](S12) { if (guard()) action(); state = S13{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e13&) {
    std::visit(overload{
        [&](S13) { if (guard()) action(); state = S14{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e14&) {
    std::visit(overload{
        [&](S14) { if (guard()) action(); state = S15{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e15&) {
    std::visit(overload{
        [&](S15) { if (guard()) action(); state = S16{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e16&) {
    std::visit(overload{
        [&](S16) { if (guard()) action(); state = S17{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e17&) {
    std::visit(overload{
        [&](S17) { if (guard()) action(); state = S18{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e18&) {
    std::visit(overload{
        [&](S18) { if (guard()) action(); state = S19{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e19&) {
    std::visit(overload{
        [&](S19) { if (guard()) action(); state = S20{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e20&) {
    std::visit(overload{
        [&](S20) { if (guard()) action(); state = S21{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e21&) {
    std::visit(overload{
        [&](S21) { if (guard()) action(); state = S22{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e22&) {
    std::visit(overload{
        [&](S22) { if (guard()) action(); state = S23{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e23&) {
    std::visit(overload{
        [&](S23) { if (guard()) action(); state = S24{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e24&) {
    std::visit(overload{
        [&](S24) { if (guard()) action(); state = S25{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e25&) {
    std::visit(overload{
        [&](S25) { if (guard()) action(); state = S26{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e26&) {
    std::visit(overload{
        [&](S26) { if (guard()) action(); state = S27{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e27&) {
    std::visit(overload{
        [&](S27) { if (guard()) action(); state = S28{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e28&) {
    std::visit(overload{
        [&](S28) { if (guard()) action(); state = S29{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e29&) {
    std::visit(overload{
        [&](S29) { if (guard()) action(); state = S30{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e30&) {
    std::visit(overload{
        [&](S30) { if (guard()) action(); state = S31{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e31&) {
    std::visit(overload{
        [&](S31) { if (guard()) action(); state = S32{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e32&) {
    std::visit(overload{
        [&](S32) { if (guard()) action(); state = S33{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e33&) {
    std::visit(overload{
        [&](S33) { if (guard()) action(); state = S34{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e34&) {
    std::visit(overload{
        [&](S34) { if (guard()) action(); state = S35{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e35&) {
    std::visit(overload{
        [&](S35) { if (guard()) action(); state = S36{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e36&) {
    std::visit(overload{
        [&](S36) { if (guard()) action(); state = S37{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e37&) {
    std::visit(overload{
        [&](S37) { if (guard()) action(); state = S38{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e38&) {
    std::visit(overload{
        [&](S38) { if (guard()) action(); state = S39{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e39&) {
    std::visit(overload{
        [&](S39) { if (guard()) action(); state = S40{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e40&) {
    std::visit(overload{
        [&](S40) { if (guard()) action(); state = S41{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e41&) {
    std::visit(overload{
        [&](S41) { if (guard()) action(); state = S42{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e42&) {
    std::visit(overload{
        [&](S42) { if (guard()) action(); state = S43{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e43&) {
    std::visit(overload{
        [&](S43) { if (guard()) action(); state = S44{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e44&) {
    std::visit(overload{
        [&](S44) { if (guard()) action(); state = S45{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e45&) {
    std::visit(overload{
        [&](S45) { if (guard()) action(); state = S46{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e46&) {
    std::visit(overload{
        [&](S46) { if (guard()) action(); state = S47{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e47&) {
    std::visit(overload{
        [&](S47) { if (guard()) action(); state = S48{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e48&) {
    std::visit(overload{
        [&](S48) { if (guard()) action(); state = S49{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e49&) {
    std::visit(overload{
        [&](S49) { if (guard()) action(); state = S50{}; },
        [](auto) { }
    }, state);
  }

  void handle(const e50&) {
    std::visit(overload{
        [&](S50) { if (guard()) action(); state = S1{}; },
        [](auto) { }
    }, state);
  }
};

int main() {
  c sm;

  benchmark_execution_speed([&] {
    for (auto i = 0; i < 1'000'000; ++i) {
      if (rand() % 2) sm.handle(e1());
      if (rand() % 2) sm.handle(e2());
      if (rand() % 2) sm.handle(e3());
      if (rand() % 2) sm.handle(e4());
      if (rand() % 2) sm.handle(e5());
      if (rand() % 2) sm.handle(e6());
      if (rand() % 2) sm.handle(e7());
      if (rand() % 2) sm.handle(e8());
      if (rand() % 2) sm.handle(e9());
      if (rand() % 2) sm.handle(e10());
      if (rand() % 2) sm.handle(e11());
      if (rand() % 2) sm.handle(e12());
      if (rand() % 2) sm.handle(e13());
      if (rand() % 2) sm.handle(e14());
      if (rand() % 2) sm.handle(e15());
      if (rand() % 2) sm.handle(e16());
      if (rand() % 2) sm.handle(e17());
      if (rand() % 2) sm.handle(e18());
      if (rand() % 2) sm.handle(e19());
      if (rand() % 2) sm.handle(e20());
      if (rand() % 2) sm.handle(e21());
      if (rand() % 2) sm.handle(e22());
      if (rand() % 2) sm.handle(e23());
      if (rand() % 2) sm.handle(e24());
      if (rand() % 2) sm.handle(e25());
      if (rand() % 2) sm.handle(e26());
      if (rand() % 2) sm.handle(e27());
      if (rand() % 2) sm.handle(e28());
      if (rand() % 2) sm.handle(e29());
      if (rand() % 2) sm.handle(e30());
      if (rand() % 2) sm.handle(e31());
      if (rand() % 2) sm.handle(e32());
      if (rand() % 2) sm.handle(e33());
      if (rand() % 2) sm.handle(e34());
      if (rand() % 2) sm.handle(e35());
      if (rand() % 2) sm.handle(e36());
      if (rand() % 2) sm.handle(e37());
      if (rand() % 2) sm.handle(e38());
      if (rand() % 2) sm.handle(e39());
      if (rand() % 2) sm.handle(e40());
      if (rand() % 2) sm.handle(e41());
      if (rand() % 2) sm.handle(e42());
      if (rand() % 2) sm.handle(e43());
      if (rand() % 2) sm.handle(e44());
      if (rand() % 2) sm.handle(e45());
      if (rand() % 2) sm.handle(e46());
      if (rand() % 2) sm.handle(e47());
      if (rand() % 2) sm.handle(e48());
      if (rand() % 2) sm.handle(e49());
      if (rand() % 2) sm.handle(e50());
    }
  });
  benchmark_memory_usage(sm);
}
