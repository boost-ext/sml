//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Issue #715: MSVC ran out of heap (C1060) from about 28 constructor dependencies.
// The covariant try_get overloads (#467) deduced D from the pool's pool_type<D&>
// bases, which is exponential in their number on MSVC.  That deduction also failed
// on every compiler as soon as the pool held more than one reference dependency,
// so a derived dependency was only found as the only one (#723).
//
// cppcheck-suppress missingIncludeSystem
#include <boost/sml.hpp>
// cppcheck-suppress missingIncludeSystem
#include <tuple>
// cppcheck-suppress missingIncludeSystem
#include <utility>

namespace sml = boost::sml;

#ifndef NDEPS
#define NDEPS 128
#endif

template <int>
struct dep {};

struct e1 {};

const auto idle = sml::state<class idle>;

// Abstract, so a missed lookup cannot fall back to a default-constructed object.
struct iface {
  virtual ~iface() = default;
  virtual int id() const = 0;
};

struct impl : iface {
  int id() const override { return 715; }
};

struct many_deps {
  auto operator()() const noexcept {
    using namespace sml;
    const auto action = [](dep<0>&) {};
    return make_transition_table(*idle + event<e1> / action = X);
  }
};

struct derived_among_many_deps {
  auto operator()() const noexcept {
    using namespace sml;
    const auto check = [](const dep<0>&, const iface& i) { expect(715 == i.id()); };
    return make_transition_table(*idle + event<e1> / check = X);
  }
};

template <int... Ns>
void construct_with_many_deps(std::integer_sequence<int, Ns...>) {
  std::tuple<dep<Ns>...> deps;
  sml::sm<many_deps> sm{std::get<Ns>(deps)...};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
}

// All deps are const: before, a derived dep was also found as the only const one.
template <int... Ns>
void construct_with_derived_among_many_deps(std::integer_sequence<int, Ns...>) {
  const std::tuple<dep<Ns>...> deps;
  const impl derived{};
  sml::sm<derived_among_many_deps> sm{std::get<Ns>(deps)..., derived};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
}

test many_ctor_dependencies = [] { construct_with_many_deps(std::make_integer_sequence<int, NDEPS>{}); };

test derived_dep_among_many_ctor_dependencies = [] {
  construct_with_derived_among_many_deps(std::make_integer_sequence<int, NDEPS - 1>{});
};
