//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Issue #715: MSVC ran out of heap (C1060) when sm<> received many distinct
// constructor dependencies.  The covariant try_get overloads (#467) deduced D
// from the pool's pool_type<D&> bases, and MSVC's deduction through N matching
// bases is exponential in N: 24 deps needed ~8.6GB, 28 ran out of heap.
// The second test also needs the covariant lookup itself among many reference
// deps (before, it only found a derived dep as the pool's only reference dep).
//
// cppcheck-suppress missingIncludeSystem
#include <boost/sml.hpp>
// cppcheck-suppress missingIncludeSystem
#include <tuple>
// cppcheck-suppress missingIncludeSystem
#include <utility>

namespace sml = boost::sml;

#ifndef NDEPS
#define NDEPS 28
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
    const auto check = [](dep<0>&, const iface& i) { expect(715 == i.id()); };
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

template <int... Ns>
void construct_with_derived_among_many_deps(std::integer_sequence<int, Ns...>) {
  std::tuple<dep<Ns>...> deps;
  impl derived;
  sml::sm<derived_among_many_deps> sm{std::get<Ns>(deps)..., derived};
  sm.process_event(e1{});
  expect(sm.is(sml::X));
}

test many_ctor_dependencies_compile = [] { construct_with_many_deps(std::make_integer_sequence<int, NDEPS>{}); };

test derived_dep_among_many_ctor_dependencies = [] {
  construct_with_derived_among_many_deps(std::make_integer_sequence<int, NDEPS - 1>{});
};
