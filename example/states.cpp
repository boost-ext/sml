//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/msm-lite.hpp"
#include <iostream>
#include <cassert>

namespace msm = boost::msm::lite;

struct e1 {};
struct e2 {};
struct e3 {};

struct states {
  auto configure() const noexcept {
    using namespace msm;
    state<class idle> idle;
    auto s2 = state<class s2>{};

    // clang-format off
    return make_transition_table(
       *idle + event<e1> = "s1"_s
	    , "s1"_s + msm::on_entry / [] { std::cout << "s1 on entry" << std::endl; }
	    , "s1"_s + msm::on_exit / [] { std::cout << "s1 on exit" << std::endl; }
      , "s1"_s + event<e2> = s2
      , s2 + event<e3> = X
    );
    // clang-format on
  }
};

int main() {
  msm::sm<states> sm;
  assert(sm.process_event(e1{}));
  assert(sm.process_event(e2{}));
  assert(sm.process_event(e3{}));
  assert(sm.is(msm::X));
}
