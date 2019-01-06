//
// Copyright (c) 2016-2019 Kris Jusiak (kris at jusiak dot net)
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
template <class R, class... Ts>
auto call_impl(R (*f)(Ts...)) {
  return [f](Ts... args) { return f(args...); };
}
template <class T, class R, class... Ts>
auto call_impl(T* self, R (T::*f)(Ts...)) {
  return [self, f](Ts... args) { return (self->*f)(args...); };
}
template <class T, class R, class... Ts>
auto call_impl(const T* self, R (T::*f)(Ts...) const) {
  return [self, f](Ts... args) { return (self->*f)(args...); };
}
template <class T, class R, class... Ts>
auto call_impl(const T* self, R (T::*f)(Ts...)) {
  return [self, f](Ts... args) { return (self->*f)(args...); };
}
/**
 * Simple wrapper to call free/member functions
 * @param args function, [optional] this
 * @return function(args...)
 */
auto call = [](auto... args) { return call_impl(args...); };
//->

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};

auto guard1 = [] {
  std::cout << "guard1" << std::endl;
  return true;
};

auto guard2 = [](int i) {
  assert(42 == i);
  std::cout << "guard2" << std::endl;
  return false;
};

bool guard3(int i) {
  assert(42 == i);
  std::cout << "guard3" << std::endl;
  return true;
}

auto action1 = [](auto e) { std::cout << "action1: " << typeid(e).name() << std::endl; };
struct action2 {
  void operator()(int i) {
    assert(42 == i);
    std::cout << "action2" << std::endl;
  }
};

struct actions_guards {
  auto operator()() noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
       *"idle"_s + event<e1> = "s1"_s
      , "s1"_s + event<e2> [ guard1 ] / action1 = "s2"_s
      , "s2"_s + event<e3> [ guard1 && ![] { return false;} ] / (action1, action2{}) = "s3"_s
      , "s3"_s + event<e4> [ !guard1 || guard2 ] / (action1, [] { std::cout << "action3" << std::endl; }) = "s4"_s
      , "s3"_s + event<e4> [ guard1 ] / ([] { std::cout << "action4" << std::endl; }, [this] { action4(); }) = "s5"_s
      , "s5"_s + event<e5> [ call(guard3) || guard2 ] / call(this, &actions_guards::action5) = X
    );
    // clang-format on
  }

  void action4() const { std::cout << "action4" << std::endl; }

  void action5(int i, const e5&) {
    assert(42 == i);
    std::cout << "action5" << std::endl;
  }
};
}  // namespace

int main() {
  sml::sm<actions_guards> sm{42};
  sm.process_event(e1{});
  sm.process_event(e2{});
  sm.process_event(e3{});
  sm.process_event(e4{});
  sm.process_event(e5{});
  assert(sm.is(sml::X));
}
