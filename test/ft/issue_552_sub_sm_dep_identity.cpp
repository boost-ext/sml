//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#define BOOST_SML_CREATE_DEFAULT_CONSTRUCTIBLE_DEPS
// cppcheck-suppress missingIncludeSystem
#include <boost/sml.hpp>

namespace sml = boost::sml;

namespace {
struct start {};
struct poke {};

const void *written_by_parent_action = nullptr;
const void *read_by_sub_guard = nullptr;

struct configurable_sub {
  int n = 0;
  bool configured() const {
    read_by_sub_guard = this;
    return n == 1;
  }
  auto operator()() {
    using namespace sml;
    return make_transition_table(*state<class idle> + event<poke>[&configurable_sub::configured] = X);
  }
};

struct parent {
  auto operator()() {
    using namespace sml;
    return make_transition_table(*state<class off> + event<start> /
                                                       [](configurable_sub &s) {
                                                         s.n = 1;
                                                         written_by_parent_action = &s;
                                                       } = state<configurable_sub>);
  }
};

void expect_one_sub_object(sml::sm<parent> &sm) {
  sm.process_event(start{});
  sm.process_event(poke{});
  expect(written_by_parent_action == read_by_sub_guard);
  expect(sm.is<decltype(sml::state<configurable_sub>)>(sml::X));
}
}  // namespace

test parent_action_and_sub_guard_see_same_passed_sub = [] {
  configurable_sub passed{};
  sml::sm<parent> sm{passed};
  expect_one_sub_object(sm);
};

test parent_action_and_sub_guard_see_same_default_sub = [] {
  sml::sm<parent> sm{};
  expect_one_sub_object(sm);
};
