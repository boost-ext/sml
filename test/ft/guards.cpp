//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>

namespace sml = boost::sml;

struct event1 {};

const auto state1 = sml::state<class state1>;
const auto state2 = sml::state<class state2>;
const auto state3 = sml::state<class state2>;

test guards_logic_should_short_circuit = [] {
  struct c {
    auto operator()() {
      using namespace sml;
      auto guard_true = [this] {
        ++guard_true_calls;
        return true;
      };
      auto guard_false = [this] {
        ++guard_false_calls;
        return false;
      };

      // clang-format off
        return make_transition_table(
         *state1 + event<event1> [ guard_true || guard_true ] = state2,
          state2 + event<event1> [ guard_false && guard_true ] = state3
        );
        // clang-format off
    }

    int guard_true_calls = 0;
    int guard_false_calls = 0;
  };

  sml::sm<c> sm{};

  sm.process_event(event1{});
  expect(0 == static_cast<const c&>(sm).guard_false_calls);
  expect(1 == static_cast<const c&>(sm).guard_true_calls);
  expect(sm.is(state2));

  sm.process_event(event1{});
  expect(1 == static_cast<const c&>(sm).guard_false_calls);
  expect(1 == static_cast<const c&>(sm).guard_true_calls);
  expect(sm.is(state2));
};

// Issue #546: SML's operator&& / operator|| / operator! must not be selected
// for std::integral_constant<bool,V> (a.k.a. std::true_type / std::false_type)
// when 'using namespace boost::sml;' is in scope.  Before the fix, combining
// two std::true_type values with && returned front::and_<...> (an SML guard
// combinator) instead of bool, which breaks any code that expects the standard
// && semantics (e.g. Google Mock type-trait machinery on GCC 11+).
//
// These two types mimic std::integral_constant<bool,V>:
//   T::type == T  and  bool T::value  => is_integral_constant_like<T> = true
//   operator()()                      => callable (like std::integral_constant)
//   operator bool()                   => built-in &&/||/! work via conversion
// (File-scope to avoid C++ restriction on static members in local classes.)
struct ic546_true {
  using type = ic546_true;
  static constexpr bool value = true;
  constexpr bool operator()() const noexcept { return value; }
  constexpr operator bool() const noexcept { return value; }
};
struct ic546_false {
  using type = ic546_false;
  static constexpr bool value = false;
  constexpr bool operator()() const noexcept { return value; }
  constexpr operator bool() const noexcept { return value; }
};
test integral_constant_and_or_not_not_grabbed_by_sml_operators = [] {
  using namespace sml;  // NOLINT(build/namespaces)

  const ic546_true t{};
  const ic546_false f{};

  auto r_and = t && t;  // must be bool (built-in &&), not front::and_<...>
  auto r_or  = f || t;  // must be bool (built-in ||), not front::or_<...>
  auto r_not = !t;      // must be bool (built-in !),  not front::not_<...>

  // Each decltype must be bool, not an SML combinator type (#546).
  static_assert(aux::is_same<decltype(r_and), bool>::value, "&&");
  static_assert(aux::is_same<decltype(r_or),  bool>::value, "||");
  static_assert(aux::is_same<decltype(r_not), bool>::value, "!");

  expect(r_and == true);
  expect(r_or  == true);
  expect(r_not == false);
};

// Issue #515: event<e>[!guard] and *state[!guard] must compile in C++20 mode
// (MSVC C++20 failed to deduce T in operator[] when the argument type is
// front::not_<...> and the SFINAE was expressed as a default template parameter.
// Fix: move the constraint to the trailing return type.)
test negated_guard_compiles_on_event_and_state = [] {
  struct c {
    auto operator()() {
      using namespace sml;
      auto guard_true = [] { return true; };

      // clang-format off
      return make_transition_table(
        *state1 + event<event1> [ !guard_true ] = state2,   // event<e>[!guard]
         state2                 [ !guard_true ] = state3    // *state[!guard]
      );
      // clang-format on
    }
  };

  sml::sm<c> sm{};
  // !guard_true always evaluates to false, so neither transition fires.
  sm.process_event(event1{});
  expect(sm.is(state1));
};
