//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <string>
#include <utility>


#if defined(_MSC_VER) && _MSC_VER == 1933 && _MSVC_LANG == 202002L
// workaround: operator deduction failed (MSVC 19.33 /std:c++20) 
#define OP_NEG(expr) operator!(expr)
#else
#define OP_NEG(expr) !expr
#endif

namespace sml = boost::sml;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};

const auto idle = sml::state<class idle>;
const auto s1 = sml::state<class s1>;
const auto s2 = sml::state<class s2>;
const auto s3 = sml::state<class s3>;
const auto s4 = sml::state<class s4>;
const auto s5 = sml::state<class s5>;
const auto end = sml::state<class end>;

test operators = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;
      auto yes = [] { return true; };
      auto no = [] { return false; };
      auto action = [](int &i) { i++; };

      // clang-format off
      return make_transition_table(
         *idle + event<e1> [ yes || no ] / (action, action) = s1
        , s1 + event<e1> [ OP_NEG(no) && yes ] / action = s2
        , s2 + event<e1> [ no && yes ] / (action, [](int&i) {i++;}) = s3
        , s2 + event<e2> [ yes && [] { return true; } ] / (action, [](int&i) {i++;}) = s3
      );
      // clang-format on
    }
  };

  int i = 0;
  sml::sm<c> sm{i};

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
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> = X
      );
      // clang-format on
    }
  };

  sml::sm<c> sm;
  sm.process_event(e1{});
  expect(sm.is(s1));
  sm.process_event(e2{});
  expect(sm.is(sml::X));
};

test member_functions = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> [ &c::guard ] / &c::action = X
      );
      // clang-format on
    }

    auto guard() const -> bool { return true; }
    void action() { action_ = true; }
    bool action_{};
  };

  c c_{};
  sml::sm<c> sm{c_};
  expect(!c_.action_);
  sm.process_event(e1{});
  expect(sm.is(sml::X));
  expect(c_.action_);
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
      using namespace sml;
      auto guard1 = [] { return true; };
      auto guard2 = [](auto) -> bool { return false; };
      auto guard3 = [=](int v) { return [=] { return guard2(v); }; };
      auto action1 = [] {};
      auto action2 = [](int, auto, float &) -> void {};

      struct sub {
        auto operator()() noexcept {
          using namespace sml;
          return make_transition_table(*idle + event<e1> = X);
        }
      };

      // clang-format off
      return make_transition_table(
         *idle + event<e1> [guard1] / (action1, []{}) = s1
        , s1 + event<e2> / ([] { }) = s2
        , s2 + event<e4> / defer
        , s1 + event<e3> / action1
        , idle [guard3(42)] = s1
        , s3 + event<e4> / [] { }
        , s3 + event<e5> [guard1] / action1
        , s1 [guard1 && guard1 && [] { return true; }] = s2
        , (*s3) [guard1] = s4
        , s2 [guard1 && OP_NEG(guard2)] = s3
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
        , idle + event<e1> [guard1 || OP_NEG(guard2)] / (action1, action2, []{}, [](auto) -> void {}) = s1
        , idle + event<e2> [guard1 || guard2] / (action1, action2, []{}, [](int, const auto&, float) -> void{}) = s1
        , idle + event<e1> [guard1 && guard2 && [] { return true; } ] / (action1, action2, []{}, [](int, auto, float) -> void{}) = X
        , idle + event<e1> [guard1 && guard2 && [] { return true; } && [] (auto)  -> bool{ return false; } ] / (action1, action2, []{}, [](int, auto, double) -> void{}) = X
        , state<sub> + event<e1> / []{}
        , state<sub>(s1) + event<e1> / []{}
        , state<sub>(s1) = s2
        , s1 <= *idle + event<e1> [guard1] / (action1, []{})
        , s2 <= s1 + event<e2> / ([] { })
        , s1 <= s2 + event<e4> / defer
        , s2 <= s1 [guard1 && guard1 && [] { return true; }]
        , s4 <= (*s3) [guard1]
        , s4 <= s3 [guard1 && OP_NEG(guard2)]
        , s5 <= s4 [guard1] / action1
        , end <= s5 / (action1, action2)
        , s1 <= idle(H) + event<e1>
        , s1 <= idle(H) + event<e1> [c_guard{} && guard1]
        ,*s1 <= idle + event<e1> [guard1] / action1
        , s1 <= idle + event<e1> / action1
        , s1 <= idle + event<e1> / (action1, c_action{1}, c_action{2})
        , s1 <= idle + event<e1> [guard1 && guard2] / (action1, action2)
        , s1 <= idle + event<e1> [guard1 && guard2] / (action1, action2, []{})
        , s1 <= idle + event<e1> [guard1 || OP_NEG(guard2)] / (action1, action2, []{}, [](auto) -> void{})
        , s1 <= idle + event<e2> [guard1 || guard2] / (action1, action2, []{}, [](int, auto, float) -> void{})
        , X <= idle + event<e1> [guard1 && guard2 && [] { return true; } ] / (action1, action2, []{}, [](int, auto, float) -> void {})
        , X <= idle + event<e1> [guard1 && guard2 && [] { return true; } && [] (auto) -> bool { return false; } ] / (action1, action2, []{}, [](int, auto, double) -> void{})
      );
      // clang-format on
    }
  };

  float f = 12.0;
  sml::sm<c> sm{f, 42, 87.0, 0.0f};
};
