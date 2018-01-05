//
// Copyright (c) 2016-2017 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <common/test.hpp>

int main() {}

#include <boost/sml.hpp>
#include <string>
#include <utility>

namespace sml = boost::sml;

struct e1 {};
struct e2 {};
struct e3 {};

const auto idle = sml::state<class idle>;
const auto s1 = sml::state<class s1>;
const auto s2 = sml::state<class s2>;
const auto s3 = sml::state<class s3>;
const auto s4 = sml::state<class s4>;

test process_event = [] {
  struct c {
    auto operator()() {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> / process(e3{})
        , s1 + event<e3> / [this] { a_called++; } = X
      );
      // clang-format on
    }

    int a_called = 0;
  };

  c c_;
  sml::sm<c> sm{c_};
  expect(sm.is(idle));
  sm.process_event(e1{});
  expect(sm.is(s1));
  expect(!c_.a_called);
  sm.process_event(e2{});  // + process(e3{})
  expect(1 == c_.a_called);
  expect(sm.is(sml::X));
};

test process_event_using_injected_sm = [] {
  struct c {
    auto operator()() const noexcept {
      using namespace sml;
      return make_transition_table(*s1 +
                                       event<e1> /
                                           [](bool value, auto event, auto& sm) {
                                             static_expect(aux::is_same<decltype(event), e1>::value);
                                             if (value) {
                                               sm.process_event(e2{});
                                             }
                                           } = s3,
                                   s1 + event<e2> = s2, s3 + event<e2> = s4);
    }
  };

  using namespace sml;
  {
    sm<c> sm{true};
    sm.process_event(e1{});
    expect(sm.is(s4));
  }
  {
    sm<c> sm{false};
    sm.process_event(e1{});
    expect(sm.is(s3));
  }
};

test process_event_from_substate = [] {
  struct sub {
    auto operator()() const noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *s1 + event<e1> = s2
        , s2 + event<e2> / process(e3{})
        , s2 + event<e1> / process(e3{}) = s3
        , s3 + event<e3> = X
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = state<sub>
        , state<sub> + event<e3> / [this] { a_called++; }
      );
      // clang-format on
    }

    int a_called = 0;
  };

  c c_;
  sml::sm<c> sm{c_};
  expect(sm.is(idle));
  expect(sm.is<decltype(sml::state<sub>)>(s1));

  sm.process_event(e1{});
  expect(sm.is(sml::state<sub>));
  expect(sm.is<decltype(sml::state<sub>)>(s1));
  expect(!c_.a_called);

  sm.process_event(e1{});
  expect(sm.is(sml::state<sub>));
  expect(sm.is<decltype(sml::state<sub>)>(s2));
  expect(!c_.a_called);

  sm.process_event(e2{});  // + process(e3{})
  expect(sm.is(sml::state<sub>));
  expect(sm.is<decltype(sml::state<sub>)>(s2));
  expect(1 == c_.a_called);

  sm.process_event(e1{});  // + process(e3{})
  expect(sm.is(sml::state<sub>));
  expect(sm.is<decltype(sml::state<sub>)>(sml::X));
  expect(1 == c_.a_called);
};

test process_event_from_substate_using_injected_sm = [] {
  struct sub {
    auto operator()() const noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *s1 + event<e1> = s2
        , s2 + event<e2> / [](auto, auto& sm) {
                sm.process_event(e3{});
            }
        , s2 + event<e1> / [](auto, auto& sm) {
                sm.process_event(e3{});
            } = s3
        , s3 + event<e3> = X
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = state<sub>
        , state<sub> + event<e3> / [this] { a_called++; }
      );
      // clang-format on
    }

    int a_called = 0;
  };

  c c_;
  sml::sm<c> sm{c_};
  expect(sm.is(idle));
  expect(sm.is<decltype(sml::state<sub>)>(s1));

  sm.process_event(e1{});
  expect(sm.is(sml::state<sub>));
  expect(sm.is<decltype(sml::state<sub>)>(s1));
  expect(!c_.a_called);

  sm.process_event(e1{});
  expect(sm.is(sml::state<sub>));
  expect(sm.is<decltype(sml::state<sub>)>(s2));
  expect(!c_.a_called);

  sm.process_event(e2{});  // + process_event(e3{})
  expect(sm.is(sml::state<sub>));
  expect(sm.is<decltype(sml::state<sub>)>(s2));
  expect(1 == c_.a_called);

  sm.process_event(e1{});  // + process_event(e3{})
  expect(sm.is(sml::state<sub>));
  expect(sm.is<decltype(sml::state<sub>)>(sml::X));
  expect(1 == c_.a_called);
};
