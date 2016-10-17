//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// clang-format off
#if __has_include(<boost/di.hpp>)
// clang-format on
#include "boost/msm-lite.hpp"
#include <boost/di.hpp>
#include <cassert>
#include <typeinfo>
#include <iostream>

namespace msm = boost::msm::lite;

struct e1 {};
struct e2 {};
struct e3 {};

auto guard = [](int i, double d) {
  assert(42 == i);
  assert(87.0 == d);
  std::cout << "guard" << std::endl;
  return true;
};

auto action = [](int i, auto e) {
  assert(42 == i);
  std::cout << "action: " << typeid(e).name() << std::endl;
};

struct example {
  auto operator()() const noexcept {
    using namespace msm;
    // clang-format off
    return make_transition_table(
       *"idle"_s + event<e1> = "s1"_s
      , "s1"_s + event<e2> [ guard ] / action = "s2"_s
      , "s2"_s + event<e3> / [] { std::cout << "in place action" << std::endl; } = X
    );
    // clang-format on
  }
};

namespace di = boost::di;

class controller {
 public:
  explicit controller(msm::sm<example>& sm) : sm(sm) {}

  void start() {
    sm.process_event(e1{});
    sm.process_event(e2{});
    sm.process_event(e3{});
    assert(sm.is(msm::X));
  }

 private:
  msm::sm<example>& sm;
};

int main() {
  auto injector = di::make_injector(di::bind<>.to(42), di::bind<>.to(87.0));
  injector.create<controller>().start();
}
#else
int main() {}
#endif
