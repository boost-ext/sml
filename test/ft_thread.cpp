//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <thread>
#include "boost/msm-lite.hpp"

namespace msm = boost::msm::lite;

struct e1 {};
struct e2 {};

struct actions_guards {
  auto configure() {
    using namespace msm;
    // clang-format off
    return make_transition_table(
       *"idle"_s + event<e1> [([this]{ guard1_calls++; return true; })] / [this] { action1_calls++; } = "s1"_s
      , "idle"_s + event<e2> [([this]{ guard2_calls++; return true; })] / [this] { action2_calls++; } = "s2"_s
    );
    // clang-format on
  }

  int guard1_calls = 0;
  int guard2_calls = 0;
  int action1_calls = 0;
  int action2_calls = 0;
};

test process_the_same_event = [] {
  actions_guards ag;
  msm::sm<actions_guards> sm{ag};
  std::thread t1{[&] { sm.process_event(e1{}); }};
  std::thread t2{[&] { sm.process_event(e2{}); }};
  t1.join();
  t2.join();
  using namespace msm;
  expect(1 == ag.guard1_calls + ag.guard2_calls);
  expect(1 == ag.action1_calls + ag.action2_calls);
  expect(sm.is("s1"_s) || sm.is("s2"_s));
};
