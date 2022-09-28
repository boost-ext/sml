//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.lite.hpp>
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
      using namespace sml::dsl;
      return transition_table{
	*"idle"_s + event<e1> [ guard ] / action = "s1"_s,
	 "s1"_s + event<e2> [ guard ] / action = "s2"_s,
	 "s2"_s + event<e3> [ guard ] / action = "s3"_s,
	 "s3"_s + event<e4> [ guard ] / action = "s4"_s,
	 "s4"_s + event<e5> [ guard ] / action = "s5"_s,
	 "s5"_s + event<e6> [ guard ] / action = "s6"_s,
	 "s6"_s + event<e7> [ guard ] / action = "s7"_s,
	 "s7"_s + event<e8> [ guard ] / action = "s8"_s,
	 "s8"_s + event<e9> [ guard ] / action = "s9"_s,
	 "s9"_s + event<e10> [ guard ] / action = "s10"_s,
	 "s10"_s + event<e11> [ guard ] / action = "s11"_s,
	 "s11"_s + event<e12> [ guard ] / action = "s12"_s,
	 "s12"_s + event<e13> [ guard ] / action = "s13"_s,
	 "s13"_s + event<e14> [ guard ] / action = "s14"_s,
	 "s14"_s + event<e15> [ guard ] / action = "s15"_s,
	 "s15"_s + event<e16> [ guard ] / action = "s16"_s,
	 "s16"_s + event<e17> [ guard ] / action = "s17"_s,
	 "s17"_s + event<e18> [ guard ] / action = "s18"_s,
	 "s18"_s + event<e19> [ guard ] / action = "s19"_s,
	 "s19"_s + event<e20> [ guard ] / action = "s20"_s,
	 "s20"_s + event<e21> [ guard ] / action = "s21"_s,
	 "s21"_s + event<e22> [ guard ] / action = "s22"_s,
	 "s22"_s + event<e23> [ guard ] / action = "s23"_s,
	 "s23"_s + event<e24> [ guard ] / action = "s24"_s,
	 "s24"_s + event<e25> [ guard ] / action = "s25"_s,
	 "s25"_s + event<e26> [ guard ] / action = "s26"_s,
	 "s26"_s + event<e27> [ guard ] / action = "s27"_s,
	 "s27"_s + event<e28> [ guard ] / action = "s28"_s,
	 "s28"_s + event<e29> [ guard ] / action = "s29"_s,
	 "s29"_s + event<e30> [ guard ] / action = "s30"_s,
	 "s30"_s + event<e31> [ guard ] / action = "s31"_s,
	 "s31"_s + event<e32> [ guard ] / action = "s32"_s,
	 "s32"_s + event<e33> [ guard ] / action = "s33"_s,
	 "s33"_s + event<e34> [ guard ] / action = "s34"_s,
	 "s34"_s + event<e35> [ guard ] / action = "s35"_s,
	 "s35"_s + event<e36> [ guard ] / action = "s36"_s,
	 "s36"_s + event<e37> [ guard ] / action = "s37"_s,
	 "s37"_s + event<e38> [ guard ] / action = "s38"_s,
	 "s38"_s + event<e39> [ guard ] / action = "s39"_s,
	 "s39"_s + event<e40> [ guard ] / action = "s40"_s,
	 "s40"_s + event<e41> [ guard ] / action = "s41"_s,
	 "s41"_s + event<e42> [ guard ] / action = "s42"_s,
	 "s42"_s + event<e43> [ guard ] / action = "s43"_s,
	 "s43"_s + event<e44> [ guard ] / action = "s44"_s,
	 "s44"_s + event<e45> [ guard ] / action = "s45"_s,
	 "s45"_s + event<e46> [ guard ] / action = "s46"_s,
	 "s46"_s + event<e47> [ guard ] / action = "s47"_s,
	 "s47"_s + event<e48> [ guard ] / action = "s48"_s,
	 "s48"_s + event<e49> [ guard ] / action = "s49"_s,
	 "s49"_s + event<e50> [ guard ] / action = "s50"_s,
    };
    }
  };

  sml::sm sm{c{}};

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
