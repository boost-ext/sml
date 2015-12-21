//
// Copyright (c) 2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <cassert>
#include <typeinfo>
#include "msm/msm.hpp"

struct e1 {
  static constexpr auto id = 0;
};
struct e2 {
  static constexpr auto id = 1;
};

struct e3 {
  static constexpr auto id = 2;
};

// auto expect_state = [](auto& sm, const auto& state) {
// sm.visit_current_states([&](const auto& s) { expect(static_cast<const msm::state_base*>(&s) == &state); });
//};

// test empty = [] {
// struct c {
// auto configure() const noexcept {
// using namespace msm;
// init_state<> idle;

// return make_transition_table(idle + event<e1> / [] {});
//}
//};

// msm::sm<c> sm{c{}};
// sm.process_event(e1{});
//};

// test transition = [] {
// struct c {
// msm::init_state<> idle;
// msm::state<> s1;

// auto configure() const noexcept {
// using namespace msm;
// return make_transition_table(idle == s1 + event<e1>);
//}
//};

// c c_;
// msm::sm<c> sm{c_};
// expect_state(sm, c_.idle);
// sm.process_event(e1{});
// expect_state(sm, c_.s1);
//};

// test action_with_event = [] {
// struct c {
// auto configure() const noexcept {
// using namespace msm;
// auto action = [](const e1&) {};
// return make_transition_table(idle == s1 + event<e1> / action);
//}

// msm::init_state<> idle;
// msm::state<> s1;
//};

// c c_;
// msm::sm<c> sm{c_};
// expect_state(sm, c_.idle);
// sm.process_event(e1{});
// expect_state(sm, c_.s1);
//};

// test action_with_parameter = [] {
// struct c {
// auto configure() const noexcept {
// using namespace msm;
// auto action = [](int i) { expect(i == 42); };
// return make_transition_table(idle == s1 + event<e1> / action);
//}

// msm::init_state<> idle;
// msm::state<> s1;
//};

// c c_;
// msm::sm<c> sm{c_, 42};
// expect_state(sm, c_.idle);
// sm.process_event(e1{});
// expect_state(sm, c_.s1);
//};

// test action_and_guad_with_parameter = [] {
// struct c {
// auto configure() const noexcept {
// using namespace msm;
// auto guard = [](double d) {
// expect(d == 87.0);
// return true;
//};
// auto action = [](int i) { expect(i == 42); };
// return make_transition_table(idle == s1 + event<e1>[guard] / action);
//}

// msm::init_state<> idle;
// msm::state<> s1;
//};

// c c_;
// msm::sm<c> sm{c_, 87.0, 42};
// expect_state(sm, c_.idle);
// sm.process_event(e1{});
// expect_state(sm, c_.s1);
//};

// test action_and_guad_with_parameters_and_event = [] {
// struct c {
// auto configure() const noexcept {
// using namespace msm;
// auto guard = [](int i, auto e, double d) {
// static_expect(msm::aux::is_same<decltype(e), e1>::value);
// expect(i == 42);
// expect(d == 87.0);
// return true;
//};
// auto action = [](int i, float& f) {
// expect(i == 42);
// expect(f == 12.0);
//};
// return make_transition_table(idle == s1 + event<e1>[guard] / action);
//}

// msm::init_state<> idle;
// msm::state<> s1;
//};

// c c_;
// float f = 12.0;
// msm::sm<c> sm{c_, 42, 87.0, f};
// expect_state(sm, c_.idle);
// sm.process_event(e1{});
// expect_state(sm, c_.s1);
//};

auto guard = [] { return true; };
auto action = [](int i) { std::cout << "action: " << i << std::endl; };

struct g1 {
  bool operator()() {
    std::cout << "g1" << std::endl;
    return false;
  }
};

struct g2 {
  bool operator()() {
    std::cout << "g2" << std::endl;
    return true;
  }
};

test basic = [] {

  struct c {
    auto configure() const noexcept {
      using namespace msm;
      state<> idle, s1, s2, s3;

      // clang-format off
	  return make_transition_table(
		idle(init) == s1 + event<e1> [g2{}] / action
	  , s1 == s2 + event<e2> [g1{}] / action
	  , s1 == idle + event<e2> [g1{}] / action
	  , s1 == s3 + event<e2> [g2{}] / action
  	  );
      // clang-format on
    }
  };

  c c_;
  msm::sm<c> sm{c_, 42};
  sm.process_event(e1());
  sm.process_event(e3());
  sm.process_event(e2());
};
