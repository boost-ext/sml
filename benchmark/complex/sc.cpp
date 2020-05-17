//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/statechart/event.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/transition.hpp>
#include <cstdlib>
#include "benchmark.hpp"

namespace sc = boost::statechart;
namespace mpl = boost::mpl;

struct e1 : sc::event<e1> {};
struct e2 : sc::event<e2> {};
struct e3 : sc::event<e3> {};
struct e4 : sc::event<e4> {};
struct e5 : sc::event<e5> {};
struct e6 : sc::event<e6> {};
struct e7 : sc::event<e7> {};
struct e8 : sc::event<e8> {};
struct e9 : sc::event<e9> {};
struct e10 : sc::event<e10> {};
struct e11 : sc::event<e11> {};
struct e12 : sc::event<e12> {};
struct e13 : sc::event<e13> {};
struct e14 : sc::event<e14> {};
struct e15 : sc::event<e15> {};
struct e16 : sc::event<e16> {};
struct e17 : sc::event<e17> {};
struct e18 : sc::event<e18> {};
struct e19 : sc::event<e19> {};
struct e20 : sc::event<e20> {};
struct e21 : sc::event<e21> {};
struct e22 : sc::event<e22> {};
struct e23 : sc::event<e23> {};
struct e24 : sc::event<e24> {};
struct e25 : sc::event<e25> {};
struct e26 : sc::event<e26> {};
struct e27 : sc::event<e27> {};
struct e28 : sc::event<e28> {};
struct e29 : sc::event<e29> {};
struct e30 : sc::event<e30> {};
struct e31 : sc::event<e31> {};
struct e32 : sc::event<e32> {};
struct e33 : sc::event<e33> {};
struct e34 : sc::event<e34> {};
struct e35 : sc::event<e35> {};
struct e36 : sc::event<e36> {};
struct e37 : sc::event<e37> {};
struct e38 : sc::event<e38> {};
struct e39 : sc::event<e39> {};
struct e40 : sc::event<e40> {};
struct e41 : sc::event<e41> {};
struct e42 : sc::event<e42> {};
struct e43 : sc::event<e43> {};
struct e44 : sc::event<e44> {};
struct e45 : sc::event<e45> {};
struct e46 : sc::event<e46> {};
struct e47 : sc::event<e47> {};
struct e48 : sc::event<e48> {};
struct e49 : sc::event<e49> {};
struct e50 : sc::event<e50> {};

struct idle;
struct s1;
struct s2;
struct s3;
struct s4;
struct s5;
struct s6;
struct s7;
struct s8;
struct s9;
struct s10;
struct s11;
struct s12;
struct s13;
struct s14;
struct s15;
struct s16;
struct s17;
struct s18;
struct s19;
struct s20;
struct s21;
struct s22;
struct s23;
struct s24;
struct s25;
struct s26;
struct s27;
struct s28;
struct s29;
struct s30;
struct s31;
struct s32;
struct s33;
struct s34;
struct s35;
struct s36;
struct s37;
struct s38;
struct s39;
struct s40;
struct s41;
struct s42;
struct s43;
struct s44;
struct s45;
struct s46;
struct s47;
struct s48;
struct s49;

struct player : sc::state_machine<player, idle> {
  template <class TEvent>
  void action(TEvent const &) {}
};

struct idle : sc::simple_state<idle, player> {
  typedef mpl::list<sc::transition<e1, s1, player, &player::action>> reactions;
};

struct s1 : sc::simple_state<s1, player> {
  typedef mpl::list<sc::transition<e2, s2, player, &player::action>> reactions;
};

struct s2 : sc::simple_state<s2, player> {
  typedef mpl::list<sc::transition<e3, s3, player, &player::action>> reactions;
};

struct s3 : sc::simple_state<s3, player> {
  typedef mpl::list<sc::transition<e4, s4, player, &player::action>> reactions;
};

struct s4 : sc::simple_state<s4, player> {
  typedef mpl::list<sc::transition<e5, s5, player, &player::action>> reactions;
};

struct s5 : sc::simple_state<s5, player> {
  typedef mpl::list<sc::transition<e6, s6, player, &player::action>> reactions;
};

struct s6 : sc::simple_state<s6, player> {
  typedef mpl::list<sc::transition<e7, s7, player, &player::action>> reactions;
};

struct s7 : sc::simple_state<s7, player> {
  typedef mpl::list<sc::transition<e8, s8, player, &player::action>> reactions;
};

struct s8 : sc::simple_state<s8, player> {
  typedef mpl::list<sc::transition<e9, s9, player, &player::action>> reactions;
};

struct s9 : sc::simple_state<s9, player> {
  typedef mpl::list<sc::transition<e10, s10, player, &player::action>> reactions;
};

struct s10 : sc::simple_state<s10, player> {
  typedef mpl::list<sc::transition<e11, s11, player, &player::action>> reactions;
};

struct s11 : sc::simple_state<s11, player> {
  typedef mpl::list<sc::transition<e12, s12, player, &player::action>> reactions;
};

struct s12 : sc::simple_state<s12, player> {
  typedef mpl::list<sc::transition<e13, s13, player, &player::action>> reactions;
};

struct s13 : sc::simple_state<s13, player> {
  typedef mpl::list<sc::transition<e14, s14, player, &player::action>> reactions;
};

struct s14 : sc::simple_state<s14, player> {
  typedef mpl::list<sc::transition<e15, s15, player, &player::action>> reactions;
};

struct s15 : sc::simple_state<s15, player> {
  typedef mpl::list<sc::transition<e16, s16, player, &player::action>> reactions;
};

struct s16 : sc::simple_state<s16, player> {
  typedef mpl::list<sc::transition<e17, s17, player, &player::action>> reactions;
};

struct s17 : sc::simple_state<s17, player> {
  typedef mpl::list<sc::transition<e18, s18, player, &player::action>> reactions;
};

struct s18 : sc::simple_state<s18, player> {
  typedef mpl::list<sc::transition<e19, s19, player, &player::action>> reactions;
};

struct s19 : sc::simple_state<s19, player> {
  typedef mpl::list<sc::transition<e20, s20, player, &player::action>> reactions;
};

struct s20 : sc::simple_state<s20, player> {
  typedef mpl::list<sc::transition<e21, s21, player, &player::action>> reactions;
};

struct s21 : sc::simple_state<s21, player> {
  typedef mpl::list<sc::transition<e22, s22, player, &player::action>> reactions;
};

struct s22 : sc::simple_state<s22, player> {
  typedef mpl::list<sc::transition<e23, s23, player, &player::action>> reactions;
};

struct s23 : sc::simple_state<s23, player> {
  typedef mpl::list<sc::transition<e24, s24, player, &player::action>> reactions;
};

struct s24 : sc::simple_state<s24, player> {
  typedef mpl::list<sc::transition<e25, s25, player, &player::action>> reactions;
};

struct s25 : sc::simple_state<s25, player> {
  typedef mpl::list<sc::transition<e26, s26, player, &player::action>> reactions;
};

struct s26 : sc::simple_state<s26, player> {
  typedef mpl::list<sc::transition<e27, s27, player, &player::action>> reactions;
};

struct s27 : sc::simple_state<s27, player> {
  typedef mpl::list<sc::transition<e28, s28, player, &player::action>> reactions;
};

struct s28 : sc::simple_state<s28, player> {
  typedef mpl::list<sc::transition<e29, s29, player, &player::action>> reactions;
};

struct s29 : sc::simple_state<s29, player> {
  typedef mpl::list<sc::transition<e30, s30, player, &player::action>> reactions;
};

struct s30 : sc::simple_state<s30, player> {
  typedef mpl::list<sc::transition<e31, s31, player, &player::action>> reactions;
};

struct s31 : sc::simple_state<s31, player> {
  typedef mpl::list<sc::transition<e32, s32, player, &player::action>> reactions;
};

struct s32 : sc::simple_state<s32, player> {
  typedef mpl::list<sc::transition<e33, s33, player, &player::action>> reactions;
};

struct s33 : sc::simple_state<s33, player> {
  typedef mpl::list<sc::transition<e34, s34, player, &player::action>> reactions;
};

struct s34 : sc::simple_state<s34, player> {
  typedef mpl::list<sc::transition<e35, s35, player, &player::action>> reactions;
};

struct s35 : sc::simple_state<s35, player> {
  typedef mpl::list<sc::transition<e36, s36, player, &player::action>> reactions;
};

struct s36 : sc::simple_state<s36, player> {
  typedef mpl::list<sc::transition<e37, s37, player, &player::action>> reactions;
};

struct s37 : sc::simple_state<s37, player> {
  typedef mpl::list<sc::transition<e38, s38, player, &player::action>> reactions;
};

struct s38 : sc::simple_state<s38, player> {
  typedef mpl::list<sc::transition<e39, s39, player, &player::action>> reactions;
};

struct s39 : sc::simple_state<s39, player> {
  typedef mpl::list<sc::transition<e40, s40, player, &player::action>> reactions;
};

struct s40 : sc::simple_state<s40, player> {
  typedef mpl::list<sc::transition<e41, s41, player, &player::action>> reactions;
};

struct s41 : sc::simple_state<s41, player> {
  typedef mpl::list<sc::transition<e42, s42, player, &player::action>> reactions;
};

struct s42 : sc::simple_state<s42, player> {
  typedef mpl::list<sc::transition<e43, s43, player, &player::action>> reactions;
};

struct s43 : sc::simple_state<s43, player> {
  typedef mpl::list<sc::transition<e44, s44, player, &player::action>> reactions;
};

struct s44 : sc::simple_state<s44, player> {
  typedef mpl::list<sc::transition<e45, s45, player, &player::action>> reactions;
};

struct s45 : sc::simple_state<s45, player> {
  typedef mpl::list<sc::transition<e46, s46, player, &player::action>> reactions;
};

struct s46 : sc::simple_state<s46, player> {
  typedef mpl::list<sc::transition<e47, s47, player, &player::action>> reactions;
};

struct s47 : sc::simple_state<s47, player> {
  typedef mpl::list<sc::transition<e48, s48, player, &player::action>> reactions;
};

struct s48 : sc::simple_state<s48, player> {
  typedef mpl::list<sc::transition<e49, s49, player, &player::action>> reactions;
};

struct s49 : sc::simple_state<s49, player> {
  typedef mpl::list<sc::transition<e50, idle, player, &player::action>> reactions;
};

int main() {
  player sm;
  sm.initiate();

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
