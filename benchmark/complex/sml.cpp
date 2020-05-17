//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <cstdlib>
#include "benchmark.hpp"

namespace sml = boost::sml;

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

auto guard = [] { return true; };
auto action = [] {};

int main() {
  struct c {
    auto operator()() const noexcept {
      using namespace sml;
      auto idle = state<class idle>;
      auto s1 = state<class s1>;
      auto s2 = state<class s2>;
      auto s3 = state<class s3>;
      auto s4 = state<class s4>;
      auto s5 = state<class s5>;
      auto s6 = state<class s6>;
      auto s7 = state<class s7>;
      auto s8 = state<class s8>;
      auto s9 = state<class s9>;
      auto s10 = state<class s10>;
      auto s11 = state<class s11>;
      auto s12 = state<class s12>;
      auto s13 = state<class s13>;
      auto s14 = state<class s14>;
      auto s15 = state<class s15>;
      auto s16 = state<class s16>;
      auto s17 = state<class s17>;
      auto s18 = state<class s18>;
      auto s19 = state<class s19>;
      auto s20 = state<class s20>;
      auto s21 = state<class s21>;
      auto s22 = state<class s22>;
      auto s23 = state<class s23>;
      auto s24 = state<class s24>;
      auto s25 = state<class s25>;
      auto s26 = state<class s26>;
      auto s27 = state<class s27>;
      auto s28 = state<class s28>;
      auto s29 = state<class s29>;
      auto s30 = state<class s30>;
      auto s31 = state<class s31>;
      auto s32 = state<class s32>;
      auto s33 = state<class s33>;
      auto s34 = state<class s34>;
      auto s35 = state<class s35>;
      auto s36 = state<class s36>;
      auto s37 = state<class s37>;
      auto s38 = state<class s38>;
      auto s39 = state<class s39>;
      auto s40 = state<class s40>;
      auto s41 = state<class s41>;
      auto s42 = state<class s42>;
      auto s43 = state<class s43>;
      auto s44 = state<class s44>;
      auto s45 = state<class s45>;
      auto s46 = state<class s46>;
      auto s47 = state<class s47>;
      auto s48 = state<class s48>;
      auto s49 = state<class s49>;

      // clang-format off
      return make_transition_table(
        s1 <= *idle + event<e1> [guard] / action,
        s2 <= s1 + event<e2> [guard] / action,
        s3 <= s2 + event<e3> [guard] / action,
        s4 <= s3 + event<e4> [guard] / action,
        s5 <= s4 + event<e5> [guard] / action,
        s6 <= s5 + event<e6> [guard] / action,
        s7 <= s6 + event<e7> [guard] / action,
        s8 <= s7 + event<e8> [guard] / action,
        s9 <= s8 + event<e9> [guard] / action,
        s10 <= s9 + event<e10> [guard] / action,
        s11 <= s10 + event<e11> [guard] / action,
        s12 <= s11 + event<e12> [guard] / action,
        s13 <= s12 + event<e13> [guard] / action,
        s14 <= s13 + event<e14> [guard] / action,
        s15 <= s14 + event<e15> [guard] / action,
        s16 <= s15 + event<e16> [guard] / action,
        s17 <= s16 + event<e17> [guard] / action,
        s18 <= s17 + event<e18> [guard] / action,
        s19 <= s18 + event<e19> [guard] / action,
        s20 <= s19 + event<e20> [guard] / action,
        s21 <= s20 + event<e21> [guard] / action,
        s22 <= s21 + event<e22> [guard] / action,
        s23 <= s22 + event<e23> [guard] / action,
        s24 <= s23 + event<e24> [guard] / action,
        s25 <= s24 + event<e25> [guard] / action,
        s26 <= s25 + event<e26> [guard] / action,
        s27 <= s26 + event<e27> [guard] / action,
        s28 <= s27 + event<e28> [guard] / action,
        s29 <= s28 + event<e29> [guard] / action,
        s30 <= s29 + event<e30> [guard] / action,
        s31 <= s30 + event<e31> [guard] / action,
        s32 <= s31 + event<e32> [guard] / action,
        s33 <= s32 + event<e33> [guard] / action,
        s34 <= s33 + event<e34> [guard] / action,
        s35 <= s34 + event<e35> [guard] / action,
        s36 <= s35 + event<e36> [guard] / action,
        s37 <= s36 + event<e37> [guard] / action,
        s38 <= s37 + event<e38> [guard] / action,
        s39 <= s38 + event<e39> [guard] / action,
        s40 <= s39 + event<e40> [guard] / action,
        s41 <= s40 + event<e41> [guard] / action,
        s42 <= s41 + event<e42> [guard] / action,
        s43 <= s42 + event<e43> [guard] / action,
        s44 <= s43 + event<e44> [guard] / action,
        s45 <= s44 + event<e45> [guard] / action,
        s46 <= s45 + event<e46> [guard] / action,
        s47 <= s46 + event<e47> [guard] / action,
        s48 <= s47 + event<e48> [guard] / action,
        s49 <= s48 + event<e49> [guard] / action,
        idle <= s49 + event<e50> [guard] / action
    );
      // clang-format on
    }
  };

  sml::sm<c> sm;

  benchmark_execution_speed([&] {
    for (auto i = 0; i < 1'000'000; ++i) {
      if (rand() % 2) sm.process_event(e1());
      if (rand() % 2) sm.process_event(e2());
      if (rand() % 2) sm.process_event(e3());
      if (rand() % 2) sm.process_event(e4());
      if (rand() % 2) sm.process_event(e5());
      if (rand() % 2) sm.process_event(e6());
      if (rand() % 2) sm.process_event(e7());
      if (rand() % 2) sm.process_event(e8());
      if (rand() % 2) sm.process_event(e9());
      if (rand() % 2) sm.process_event(e10());
      if (rand() % 2) sm.process_event(e11());
      if (rand() % 2) sm.process_event(e12());
      if (rand() % 2) sm.process_event(e13());
      if (rand() % 2) sm.process_event(e14());
      if (rand() % 2) sm.process_event(e15());
      if (rand() % 2) sm.process_event(e16());
      if (rand() % 2) sm.process_event(e17());
      if (rand() % 2) sm.process_event(e18());
      if (rand() % 2) sm.process_event(e19());
      if (rand() % 2) sm.process_event(e20());
      if (rand() % 2) sm.process_event(e21());
      if (rand() % 2) sm.process_event(e22());
      if (rand() % 2) sm.process_event(e23());
      if (rand() % 2) sm.process_event(e24());
      if (rand() % 2) sm.process_event(e25());
      if (rand() % 2) sm.process_event(e26());
      if (rand() % 2) sm.process_event(e27());
      if (rand() % 2) sm.process_event(e28());
      if (rand() % 2) sm.process_event(e29());
      if (rand() % 2) sm.process_event(e30());
      if (rand() % 2) sm.process_event(e31());
      if (rand() % 2) sm.process_event(e32());
      if (rand() % 2) sm.process_event(e33());
      if (rand() % 2) sm.process_event(e34());
      if (rand() % 2) sm.process_event(e35());
      if (rand() % 2) sm.process_event(e36());
      if (rand() % 2) sm.process_event(e37());
      if (rand() % 2) sm.process_event(e38());
      if (rand() % 2) sm.process_event(e39());
      if (rand() % 2) sm.process_event(e40());
      if (rand() % 2) sm.process_event(e41());
      if (rand() % 2) sm.process_event(e42());
      if (rand() % 2) sm.process_event(e43());
      if (rand() % 2) sm.process_event(e44());
      if (rand() % 2) sm.process_event(e45());
      if (rand() % 2) sm.process_event(e46());
      if (rand() % 2) sm.process_event(e47());
      if (rand() % 2) sm.process_event(e48());
      if (rand() % 2) sm.process_event(e49());
      if (rand() % 2) sm.process_event(e50());
    }
  });
  benchmark_memory_usage(sm);
}
