//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

/*
The goal of this file is only to compile a state machine with deeply nested
states that use on_entry events. The only requirement on this file is a
successful compilation without a ftemplate-depth error.
*/
#define BOOST_SML_CREATE_DEFAULT_CONSTRUCTIBLE_DEPS
#include <boost/sml.hpp>

namespace sml = boost::sml;
using namespace sml;

struct e0 {};
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

struct s0 {
  auto operator()() noexcept {
    auto idle = state<struct idle_>;
    auto run = state<struct run_>;
    return make_transition_table(
        *idle + event<e0> = run, run + on_entry<_> / [] {});
  }
};

struct s1 {
  auto operator()() noexcept {
    auto idle = state<struct idle_>;
    auto run = state<s0>;
    return make_transition_table(
        *idle + event<e1> = run, run + on_entry<_> / [] {});
  }
};

struct s2 {
  auto operator()() noexcept {
    auto idle = state<struct idle_>;
    auto run = state<s1>;
    return make_transition_table(
        *idle + event<e2> = run, run + on_entry<_> / [] {});
  }
};

struct s3 {
  auto operator()() noexcept {
    auto idle = state<struct idle_>;
    auto run = state<s2>;
    return make_transition_table(
        *idle + event<e3> = run, run + on_entry<_> / [] {});
  }
};

struct s4 {
  auto operator()() noexcept {
    auto idle = state<struct idle_>;
    auto run = state<s3>;
    return make_transition_table(
        *idle + event<e4> = run, run + on_entry<_> / [] {});
  }
};

struct s5 {
  auto operator()() noexcept {
    auto idle = state<struct idle_>;
    auto run = state<s4>;
    return make_transition_table(
        *idle + event<e5> = run, run + on_entry<_> / [] {});
  }
};

struct s6 {
  auto operator()() noexcept {
    auto idle = state<struct idle_>;
    auto run = state<s5>;
    return make_transition_table(
        *idle + event<e6> = run, run + on_entry<_> / [] {});
  }
};

struct s7 {
  auto operator()() noexcept {
    auto idle = state<struct idle_>;
    auto run = state<s6>;
    return make_transition_table(
        *idle + event<e7> = run, run + on_entry<_> / [] {});
  }
};

struct s8 {
  auto operator()() noexcept {
    auto idle = state<struct idle_>;
    auto run = state<s7>;
    return make_transition_table(
        *idle + event<e8> = run, run + on_entry<_> / [] {});
  }
};

struct s9 {
  auto operator()() noexcept {
    auto idle = state<struct idle_>;
    auto run = state<s8>;
    return make_transition_table(
        *idle + event<e9> = run, run + on_entry<_> / [] {});
  }
};

struct s10 {
  auto operator()() noexcept {
    auto idle = state<struct idle_>;
    auto run = state<s9>;
    return make_transition_table(
        *idle + event<e10> = run, run + on_entry<_> / [] {});
  }
};

struct DeepSM {
  auto operator()() noexcept { return make_transition_table(*state<struct idle> = state<s10>); }
};

test deep_sm = [] {
  sm<DeepSM> state_machine;
  state_machine.process_event(e0{});
};
