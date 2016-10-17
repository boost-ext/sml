//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/msm-lite.hpp>
#include <string>
#include <utility>

namespace msm = boost::msm::lite;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};

const auto idle = msm::state<class idle>;
const auto s1 = msm::state<class s1>;
const auto s2 = msm::state<class s2>;
const auto s3 = msm::state<class s3>;
const auto s4 = msm::state<class s4>;
const auto s5 = msm::state<class s5>;
const auto end = msm::state<class end>;

test operators = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      auto yes = [] { return true; };
      auto no = [] { return false; };
      auto action = [](int &i) { i++; };

      // clang-format off
      return make_transition_table(
         *idle + event<e1> [ yes || no ] / (action, action) = s1
        , s1 + event<e1> [ !no && yes ] / action = s2
        , s2 + event<e1> [ no && yes ] / (action, [](int&i) {i++;}) = s3
        , s2 + event<e2> [ yes && [] { return true; } ] / (action, [](int&i) {i++;}) = s3
      );
      // clang-format on
    }
  };

  int i = 0;
  msm::sm<c> sm{i};

  {
    i = 0;
    sm.process_event(e1{});
    expect(sm.is(s1));
    expect(2 == i);
  }

  {
    i = 0;
    sm.process_event(e1{});
    expect(sm.is(s2));
    expect(1 == i);
  }

  {
    i = 0;
    sm.process_event(e1{});
    expect(sm.is(s2));
    expect(0 == i);
  }

  {
    i = 0;
    sm.process_event(e2{});
    expect(sm.is(s3));
    expect(2 == i);
  }
};

test uml_notation = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> = X
      );
      // clang-format on
    }
  };

  msm::sm<c> sm;
  sm.process_event(e1{});
  expect(sm.is(s1));
  sm.process_event(e2{});
  expect(sm.is(msm::X));
};

struct c_guard {
  template <class T>
  bool operator()(const T &) const noexcept {
    return true;
  }
};

struct c_action {
  explicit c_action(int) {}
  template <class T>
  void operator()(const T &) noexcept {}
};

test transition_table_types = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      auto guard1 = [] { return true; };
      auto guard2 = [](auto) { return false; };
      auto guard3 = [=](int v) { return [=] { return guard2(v); }; };
      auto action1 = [] {};
      auto action2 = [](int, auto, float &) {};

      // clang-format off
      return make_transition_table(
         *idle + event<e1> [guard1] / (action1, []{}) = s1
        , s1 + event<e2> / ([] { }) = s2
        , s2 + event<e4> / defer
        , s1 / [] {}
        , s1 [guard1 && guard3(42)]
        , s1 [guard1] / action1
        , s1 + event<e3> / action1
        , idle = s1
        , s3 + event<e4> / [] { }
        , s3 + event<e5> [guard1] / action1
        , s1 [guard1 && guard1 && [] { return true; }] = s2
        , (*s3) [guard1] = s2
        , s2 [guard1 && !guard2] = s3
        , s3 [guard1] / action1 = s4
        , s4 / action1 = s5
        , s5 / (action1, action2) = end
        , idle(H) + event<e1> = s1
        , idle(H) + event<e1> [c_guard{} && guard1] = s1
        ,*idle + event<e1> [guard1] / action1 = s1
        , idle + event<e1> / action1 = s1
        , idle + event<e1> / (action1, c_action{1}) = s1
        , idle + event<e1> [guard1 && guard2] / (action1, action2) = s1
        , idle + event<e1> [guard1 && guard2] / (action1, action2, []{}) = s1
        , idle + event<e1> [guard1 || !guard2] / (action1, action2, []{}, [](auto){}) = s1
        , idle + event<e2> [guard1 || guard2] / (action1, action2, []{}, [](int, auto, float){}) = s1
        , idle + event<e1> [guard1 && guard2 && [] { return true; } ] / (action1, action2, []{}, [](int, auto, float){}) = X
        , idle + event<e1> [guard1 && guard2 && [] { return true; } && [] (auto) { return false; } ] / (action1, action2, []{}, [](int, auto, double){}) = X
        , s1 <= *idle + event<e1> [guard1] / (action1, []{})
        , s2 <= s1 + event<e2> / ([] { })
        , s1 <= s2 + event<e4> / defer
        , s2 <= s1 [guard1 && guard1 && [] { return true; }]
        , s2 <= (*s3) [guard1]
        , s2 <= s3 [guard1 && !guard2]
        , s3 <= s4 [guard1] / action1
        , s4 <= s5 / action1
        , end <= s5 / (action1, action2)
        , s1 <= idle(H) + event<e1>
        , s1 <= idle(H) + event<e1> [c_guard{} && guard1]
        ,*s1 <= idle + event<e1> [guard1] / action1
        , s1 <= idle + event<e1> / action1
        , s1 <= idle + event<e1> / (action1, c_action{1}, c_action{2})
        , s1 <= idle + event<e1> [guard1 && guard2] / (action1, action2)
        , s1 <= idle + event<e1> [guard1 && guard2] / (action1, action2, []{})
        , s1 <= idle + event<e1> [guard1 || !guard2] / (action1, action2, []{}, [](auto){})
        , s1 <= idle + event<e2> [guard1 || guard2] / (action1, action2, []{}, [](int, auto, float){})
        , X <= idle + event<e1> [guard1 && guard2 && [] { return true; } ] / (action1, action2, []{}, [](int, auto, float){})
        , X <= idle + event<e1> [guard1 && guard2 && [] { return true; } && [] (auto) { return false; } ] / (action1, action2, []{}, [](int, auto, double){})
      );
      // clang-format on
    }
  };

  float f = 12.0;
  msm::sm<c> sm{f, 42, 87.0, 0.0f};
};
