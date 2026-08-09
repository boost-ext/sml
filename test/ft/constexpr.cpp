//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#define BOOST_SML_DISABLE_EXCEPTIONS 1
#include <boost/sml.hpp>

namespace sml = boost::sml;

struct e1 {};
struct e2 {};

const auto idle = sml::state<class idle>;
const auto s1 = sml::state<class s1>;
const auto s2 = sml::state<class s2>;

test constexpr_sm = [] {
  using namespace sml;
  struct c {
    // clang-format off
    constexpr auto operator()() noexcept {
      return make_transition_table(
        *idle + event<e1> = X
      );
    }
    // clang-format on
  };

  constexpr sml::sm<c, sml::dispatch<sml::back::policies::branch_stm>> sm{};
  (void)sm;

  constexpr auto test =  []{
    sml::sm<c, sml::dispatch<sml::back::policies::branch_stm>> sm{};
    sm.process_event(e1{});
    return sm.is(X);
  };

  static_assert(test());
};

test constexpr_anonymous_transitions_sm = [] {
  using namespace sml;
  struct c {
    // clang-format off
    constexpr auto operator()() noexcept {
      const auto guard = [] { return true; };
      const auto action = [] {};
      return make_transition_table(
        *idle + event<e1> = s1,
         s1[guard]        = s2,
         s2 / action      = X
      );
    }
    // clang-format on
  };

  constexpr sml::sm<c, sml::dispatch<sml::back::policies::branch_stm>> sm{};
  (void)sm;
};

test constexpr_unexpected_event_sm = [] {
  using namespace sml;
  struct c {
    // clang-format off
    constexpr auto operator()() noexcept {
      return make_transition_table(
        *idle + event<e1>             = s1,
         s1   + unexpected_event<e2>  = X
      );
    }
    // clang-format on
  };

  constexpr sml::sm<c, sml::dispatch<sml::back::policies::branch_stm>> sm{};
  (void)sm;
};
