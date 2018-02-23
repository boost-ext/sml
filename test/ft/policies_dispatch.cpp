//
// Copyright (c) 2016-2017 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>

namespace sml = boost::sml;

struct e1 {};
struct e2 {};

const auto idle = sml::state<class idle>;
const auto s1 = sml::state<class s1>;

test sm_dispatch = [] {
  struct c {
    auto operator()() {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1   + event<e2> = X
      );
      // clang-format on
    }
  };

  {
    sml::sm<c, sml::dispatch<sml::back::policies::jump_table>> sm{};
    expect(sm.is(idle));
    sm.process_event(e1{});
    expect(sm.is(s1));
    sm.process_event(e2{});
    expect(sm.is(sml::X));
  }

  {
    sml::sm<c, sml::dispatch<sml::back::policies::branch_stm>> sm{};
    expect(sm.is(idle));
    sm.process_event(e1{});
    expect(sm.is(s1));
    sm.process_event(e2{});
    expect(sm.is(sml::X));
  }

  {
    sml::sm<c, sml::dispatch<sml::back::policies::switch_stm>> sm{};
    expect(sm.is(idle));
    sm.process_event(e1{});
    expect(sm.is(s1));
    sm.process_event(e2{});
    expect(sm.is(sml::X));
  }

#if defined(__cpp_fold_expressions)
  {
    sml::sm<c, sml::dispatch<sml::back::policies::fold_expr>> sm{};
    expect(sm.is(idle));
    sm.process_event(e1{});
    expect(sm.is(s1));
    sm.process_event(e2{});
    expect(sm.is(sml::X));
  }
#endif
};
