//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#if !__has_include(<boost / di.hpp>)
int main() {}
#else
#include "msm/msm.hpp"
#include <boost/di.hpp>
#include <cassert>
#include <typeinfo>
#include <iostream>

struct e1 {};
struct e2 {};
struct e3 {};
struct {
} terminate;

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
  auto configure() const noexcept {
    using namespace msm;
    state<class idle> idle;
    state<class s1> s1;
    state<class s2> s2;
    state<class s3> s3;

    // clang-format off
    return make_transition_table(
        idle(initial) == s1 + event<e1>
      , s1 == s2 + event<e2> [ guard ] / action
      , s2 == s3(terminate) + event<e3> / [] { std::cout << "in place action" << std::endl; }
    );
    // clang-format on
  }
};

namespace di = boost::di;

class controller {
 public:
  explicit controller(const msm::sm<example>& sm) : sm(sm) {}

  void start() {
    sm.process_event(e1{});
    sm.process_event(e2{});
    sm.process_event(e3{});
    assert(sm.is(terminate));
  }

 private:
  msm::sm<example> sm;
};

int main() {
  auto injector = di::make_injector(di::bind<>.to(42), di::bind<>.to(87.0));
  injector.create<controller>().start();
}
#endif
