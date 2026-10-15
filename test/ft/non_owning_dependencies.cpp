//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// cppcheck-suppress missingIncludeSystem
#include <boost/sml.hpp>

namespace sml = boost::sml;

struct settings {
  bool ok = false;
};
struct context {
  int value = 0;
};

struct e1 {};

const auto idle = sml::state<class idle>;

struct root {
  auto operator()() const noexcept {
    using namespace sml;
    const auto guard = [](const settings& s, const context& c) {
      expect(c.value == 0);
      return s.ok;
    };
    const auto action = [](const settings&, context& c) {
      c.value = 42;
    };
    return make_transition_table(*idle + event<e1> [guard] / action = X);
  }
};

test const_lvalue_dependency_injection = [] {
  const settings s{true};
  context c{};
  sml::sm<root> sm{s, c};
  expect(sm.is(idle));
  sm.process_event(e1{});
  expect(sm.is(sml::X));
  expect(c.value == 42);
};

// A guard taking const T& and an action taking T& must both see the instance passed
// to the sm, not a copy: a copy keeps the original's self, so &d != d.self.
struct first_dep {
  const first_dep *self = this;
  int value = 0;
};
struct second_dep {
  const second_dep *self = this;
  int value = 0;
};
struct derived_first_dep : first_dep {};
struct derived_second_dep : second_dep {};

struct same_instance_root {
  auto operator()() const noexcept {
    using namespace sml;
    const auto guard = [](const first_dep &a, const second_dep &b) {
      expect(&a == a.self && &b == b.self);
      return true;
    };
    const auto action = [](first_dep &a, second_dep &b) {
      expect(&a == a.self && &b == b.self);
      ++a.value;
      ++b.value;
    };
    return make_transition_table(*idle + event<e1>[guard] / action = X);
  }
};

test const_and_mutable_ref_deps_are_the_passed_instances = [] {
  first_dep a{};
  second_dep b{};
  sml::sm<same_instance_root> sm{a, b};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
  expect(a.value == 1 && b.value == 1);
};

test derived_deps_as_const_and_mutable_base_refs_are_the_passed_instances = [] {
  derived_first_dep a{};
  derived_second_dep b{};
  sml::sm<same_instance_root> sm{a, b};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
  expect(a.value == 1 && b.value == 1);
};
