//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Issue #580: back::process<E> fails with a template error when event E is not
// referenced in any transition row (only in the action parameter list).
//
// cppcheck-suppress missingIncludeSystem
#include <boost/sml.hpp>
// cppcheck-suppress missingIncludeSystem
#include <queue>

namespace sml = boost::sml;

struct e1 {};
struct e2 {};

// States at file scope so both SM definition and test share the same type.
const auto s580_s1 = sml::state<class s580_s1_t>;
const auto s580_s2 = sml::state<class s580_s2_t>;

// e2 appears in both a transition row and as the process() argument.
struct sm_580a {
  auto operator()() noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
        *s580_s1 + event<e1> / process(e2{}) = s580_s2
      , s580_s2 + event<e2>                  = X
    );
    // clang-format on
  }
};

// e2 appears ONLY as a process() argument — never in a transition-row event
// slot.  Before the fix, this produced a template error because process_t was
// built from events_t which excluded e2.
struct sm_580b {
  auto operator()() noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
        *s580_s1 + event<e1> / process(e2{}) = s580_s2
    );
    // clang-format on
  }
};

// Lambda style: [](back::process<e2> p){...} — e2 only in lambda parameter.
struct sm_580c {
  auto operator()() noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
        *s580_s1 + event<e1> /
            [](sml::back::process<e2> p) { p(e2{}); } = s580_s2
    );
    // clang-format on
  }
};

test process_event_e2_in_row = [] {
  sml::sm<sm_580a, sml::process_queue<std::queue>> sm;
  expect(sm.is(s580_s1));
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

test process_event_e2_not_in_row = [] {
  // e2 only referenced via process(e2{}); should compile and run without error.
  sml::sm<sm_580b, sml::process_queue<std::queue>> sm;
  expect(sm.is(s580_s1));
  sm.process_event(e1{});
  // e2 is queued but has no transition handler — SM stays in s580_s2.
  expect(sm.is(s580_s2));
};

test process_event_lambda_style = [] {
  // Lambda style back::process<e2>; e2 only in lambda parameter.
  sml::sm<sm_580c, sml::process_queue<std::queue>> sm;
  expect(sm.is(s580_s1));
  sm.process_event(e1{});
  // e2 is queued but has no transition handler — SM stays in s580_s2.
  expect(sm.is(s580_s2));
};
