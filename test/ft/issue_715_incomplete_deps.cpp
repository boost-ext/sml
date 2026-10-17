//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Issue #715: the covariant lookup scans all reference deps of the pool, also a
// dep that is still incomplete where an sm is constructed with it.  It must not
// test whether the dep is complete.
// Not in dependencies.cpp: BOOST_SML_CREATE_DEFAULT_CONSTRUCTIBLE_DEPS tests
// whether each reference dep is constructible, and gcc 16 warns when a type is
// defined after such a test failed (-Wsfinae-incomplete).
// The state machine classes are not empty: an empty one hits an unrelated gcc 16
// -Wmaybe-uninitialized, which would hide that warning with -Werror.
//
// cppcheck-suppress missingIncludeSystem
#include <boost/sml.hpp>

namespace sml = boost::sml;

struct e1 {};

const auto idle = sml::state<class idle>;

struct base {
  int val = 1;
};

// clang instantiates the constexpr constructor of the first sm right here, and its
// lookup of the state machine class scans the incomplete dep.  That must not keep
// the later lookup of base from finding the completed dep.
struct completed_later;
struct sm_constructed_before_dep_is_complete {
  int member = 0;
  auto operator()() const noexcept {
    using namespace sml;
    const auto action = [](completed_later &, int &) {};
    return make_transition_table(*idle + event<e1> / action = X);
  }
};
void construct_sm_before_dep_is_complete(completed_later &dep, int &i) {
  sml::sm<sm_constructed_before_dep_is_complete> sm{dep, i};
  sm.process_event(e1{});
}
struct completed_later : base {
  completed_later() { val = 715; }
};

struct base_of_completed_later {
  int member = 0;
  auto operator()() const noexcept {
    using namespace sml;
    const auto check = [](const base &b, int &) { expect(715 == b.val); };
    return make_transition_table(*idle + event<e1> / check = X);
  }
};

test derived_dep_found_after_sm_constructed_while_incomplete = [] {
  completed_later dep;
  int i = 0;
  construct_sm_before_dep_is_complete(dep, i);
  sml::sm<base_of_completed_later> sm{dep, i};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
};

// gcc tries constant initialization of an sm with static storage, so it instantiates
// the constexpr constructor where the sm is defined, while the dep is incomplete.
struct defined_later;
struct sm_with_static_storage {
  int member = 0;
  auto operator()() const noexcept {
    using namespace sml;
    const auto count = [](defined_later &, int &n) { ++n; };
    return make_transition_table(*idle + event<e1> / count = X);
  }
};
void process_event_in_static_sm(defined_later &dep, int &calls) {
  static sml::sm<sm_with_static_storage> sm{dep, calls};
  sm.process_event(e1{});
}
struct defined_later {
  int val = 0;
};

test static_sm_with_ref_dep_defined_later = [] {
  defined_later dep;
  int calls = 0;
  process_event_in_static_sm(dep, calls);
  expect(1 == calls);
};
