//
// Copyright (c) 2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include <iostream>
#include <typeinfo>
#include "msm/msm.hpp"
#include "boost/di.hpp"

struct e1 {
  e1(int i) : i(i) {}
  int i = 0;
};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};

auto guard1 = [](auto, int i) {
  std::cout << "guard1: " << i << std::endl;
  return true;
};

auto guard2 = [](auto) { return true; };
auto guard3 = [](int i) {
  std::cout << "guard3: " << i << std::endl;
  return true;
};
auto action1 = [] { std::cout << "action1" << std::endl; };
auto action2 = [](double, float &) { std::cout << "action2" << std::endl; };

void f(int i, double) { std::cout << "f" << i << std::endl; }
auto f2(float &) { return true; }

struct q {
  bool operator()(const e1 &) { return true; }
  // bool operator()(const e2 &) { return false; }
};

struct game_over {};

auto is_key = [](int v) { return [=] { return guard3(v); }; };

class sub {
 public:
  auto configure() noexcept {
    using namespace msm;
    init_state<> idle;
    state<> s1, s2;

    // clang-format off
    return make_transition_table(
     // +-----------------------------------------------------------------+
       idle == s1 + event<e3> / [] { std::cout << "in SUB" << std::endl; }
     , s1 == s2 + event<e4> / [] { std::cout << "again in SUB sm" << std::endl; }
     //+-----------------------------------------------------------------+
    );
    // clang-format on
  }
};

class controller {
 public:
  /*  controller(int i, double d, const msm::sm<sub>& sub) : sub_(sub) {*/
  // std::cout << "i:" << i << "d:" << d << std::endl;
  /*}*/

  auto configure() noexcept {
    using namespace msm;
    init_state<> idle;
    state<> s1, s2, s3, s4, s5;
    state<game_over> end;

    // clang-format off
    return make_transition_table(
     // +-----------------------------------------------------------------+
       idle == sub_ + event<e1> [ guard1 && is_key(77) ] / (action1, f)
     , sub_ == s1 + event<e2> / ([] { std::cout << "SUB exit" << std::endl; })
	 , s1 == s2 + event<e4> / process_event(e5{})
	 , _ + event<not_handled> / [] { std::cout << "not handled" << std::endl; }
     // +-----------------------------------------------------------------+
	 //, s1 / [] {}
	 //, s1 [guard1]
	 //, s1 [guard1] / action1
	 //, s1 + event<not_handled> / action1
	 //, idle == s1
     // +-----------------------------------------------------------------+
	 , s3 + event<not_handled> / [] { std::cout << "fdsa"; }
	 , s3 + event<not_handled> [guard1] / action1
	 , s1 == s2 [guard1 && guard1 && f2]
     , s3 == s2 [guard1]
     , s2 == s3 [guard1 && !guard2]
     , s3 == s4 [guard1] / action1
     , s4 == s5 / action1
     , s5 == end / (action1, action2)
     , idle == s1 + event<e1>
     , idle == s1 + event<e1> [q{} && guard1]
     , idle == s1 + event<e1> [guard1] / action1
     , idle == s1 + event<e1> / action1
     , idle == s1 + event<e1> / (action1, action2)
     , idle == s1 + event<e1> [guard1 && guard2] / (action1, action2)
     , idle == s1 + event<e1> [guard1 && guard2] / (action1, action2, []{})
     , idle == s1 + event<e1> [guard1 && guard2] / (action1, action2, []{}, [](auto){})
     , idle == s1 + event<e2> [guard1 && guard2] / (action1, action2, []{}, [](int, auto, float){})
     , idle == s1 + event<e1> [guard1 && guard2 && [] { return true; } ] / (action1, action2, []{}, [](int, auto, float){})
     , idle == s1 + event<e1> [guard1 && guard2 && [] { return true; } && [] (auto) { return false; } ] / (action1, action2, []{}, [](int, auto, float){})
     //+-----------------------------------------------------------------+
    );
    // clang-format on
  }

  const msm::sm<sub> &sub_;
};

namespace di = boost::di;

int main() {
  auto injector = di::make_injector(di::bind<int>().to(42));
  auto sm = injector.create<msm::sm<controller>>();
  // std::cout << typeid(decltype(sm)::events).name() << std::endl;
  sm.process_event(e1(77));
  sm.process_event(e3());
  sm.process_event(e4());
  sm.process_event(e2());
  sm.process_event(e4());
  // sm.process_event(e5());
}
