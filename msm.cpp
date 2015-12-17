//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

//[msm
//<-
#include <cassert>
#include <iostream>
#include <typeinfo>
#include "msm.hpp"
#include "boost/di.hpp"

//->
struct e1 {
  e1(int i) : i(i) {}
  int i = 0;
};
struct e2 {};
struct e3 {};

auto guard1 = [](auto e, int i) {
  std::cout << "guard1: " << i << std::endl;
  return true;
};

auto guard2 = [](auto) { return true; };
auto action1 = [] { std::cout << "action1" << std::endl; };
auto action2 = [](double, float &) { std::cout << "action2" << std::endl; };

void f(int i, double) { std::cout << "f" << i << std::endl; }

struct q {
  bool operator()(const e1 &) { return true; }
  bool operator()(const e2 &) { return false; }
};

struct game_over {};

class controller {
public:
  auto configure() noexcept {
    using namespace msm;
    init_state idle;
    state s1, s2, s3, s4, s5;
    property_state<game_over> end;

    // clang-format off
    return make_transition_table(
     // +-----------------------------------------------------------------+
	   idle == s1
     , s1 == s2 [guard1]
     //, idle == s2 [guard1]
     , s2 == s3 [guard1 && !guard2]
     , s3 == s4 [guard1] / action1
     , s4 == s5 / action1
     , s5 == end / (action1, action2)
     , idle == s1 + event<e1>
     , idle == s1 + event<e1> [guard1]
     , idle == s1 + event<e1> [guard1] / action1
     , idle == s1 + event<e1> / action1
     , idle == s1 + event<e1> / (action1, action2)
     , idle == s1 + event<e1> [guard1 && guard2] / (action1, action2)
     , idle == s1 + event<e1> [guard1 && guard2] / (action1, action2, []{})
     , idle == s1 + event<e1> [guard1 && guard2] / (action1, action2, []{}, [](auto){})
     , idle == s1 + event<e1> [guard1 && guard2] / (action1, action2, []{}, [](int, auto, float){})
     , idle == s1 + event<e1> [guard1 && guard2 && [] { return true; } ] / (action1, action2, []{}, [](int, auto, float){})
     , idle == s1 + event<e1> [guard1 && guard2 && [] { return true; } && [] (auto) { return false; } ] / (action1, action2, []{}, [](int, auto, float){})
     //+-----------------------------------------------------------------+
    );
    // clang-format on
  }
};

namespace di = boost::di;

int main() {
  auto injector = di::make_injector(di::bind<int>().to(42));
  auto sm = injector.create<msm::sm<controller>>();
  sm.start();

  sm.process_event(e1(77));
}

//]
