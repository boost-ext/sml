//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <cassert>
#include <iostream>
#include <typeinfo>

namespace sml = boost::sml;

namespace {

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};

// Allow out of header implementation
bool guard2_impl(int);

struct actions_guards {
  using self = actions_guards;

  auto operator()() {
    using namespace sml;

    auto guard1 = [] {
      std::cout << "guard1" << std::endl;
      return true;
    };

    auto guard2 = wrap(&guard2_impl);

    auto action1 = [](auto e) { std::cout << "action1: " << typeid(e).name() << std::endl; };

    struct action2 {
      void operator()([[maybe_unused]] int i) {
        assert(42 == i);
        std::cout << "action2" << std::endl;
      }
    };

    // clang-format off
    return make_transition_table(
       *"idle"_s + event<e1> = "s1"_s
      , "s1"_s + event<e2> [ guard1 ] / action1 = "s2"_s
      , "s2"_s + event<e3> [ guard1 && ![] { return false;} ] / (action1, action2{}) = "s3"_s
      , "s3"_s + event<e4> [ !guard1 || guard2 ] / (action1, [] { std::cout << "action3" << std::endl; }) = "s4"_s
      , "s3"_s + event<e4> [ guard1 ] / ([] { std::cout << "action4" << std::endl; }, [this] { action4(); } ) = "s5"_s
      , "s5"_s + event<e5> [ &self::guard3 ] / &self::action5 = X
    );
    // clang-format on
  }

  bool guard3(int i) const noexcept {
    assert(42 == i);
    std::cout << "guard3" << std::endl;
    return true;
  }

  void action4() const { std::cout << "action4" << std::endl; }

  void action5(int i, const e5&) {
    assert(42 == i);
    std::cout << "action5" << std::endl;
  }
};
bool guard2_impl(int i) {
  assert(42 == i);
  std::cout << "guard2" << std::endl;
  return false;
}
}  // namespace

int main() {
  actions_guards ag{};
  sml::sm<actions_guards> sm{ag, 42};
  sm.process_event(e1{});
  sm.process_event(e2{});
  sm.process_event(e3{});
  sm.process_event(e4{});
  sm.process_event(e5{});
  assert(sm.is(sml::X));
}
