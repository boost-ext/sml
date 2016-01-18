//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#define MSM_DSL_DST_STATE_FIRST  // dst == src + event [ guard ] / action
#include "msm/msm.hpp"
#include <cassert>

struct e1 {};
struct e2 {};
struct e3 {};

auto event1 = msm::event<e1>;
auto event2 = msm::event<e2>;
auto event3 = msm::event<e3>;

auto idle = msm::state<class idle>{};
auto s1 = msm::state<class s1>{};
auto s2 = msm::state<class s2>{};

struct {
  template <class FSM, class TEvent>
  bool operator()(msm::sm<FSM>&, const TEvent&) const noexcept {
    // return fsm.get() == 42;
    return true;
  }
} guard;

struct {
  template <class FSM, class TEvent>
  void operator()(msm::sm<FSM>&, const TEvent&) noexcept {
    // fsm.set(123);
  }
} action;

class euml_emulation {
 public:
  explicit euml_emulation(int i) : i(i) {}

  auto configure() const noexcept {
    using namespace msm;
    // clang-format off
    return make_transition_table(
	    s1 		  == idle(initial) + event1,
	    s2 		  == s1 		   + event2 [ guard ],
	    terminate == s2 		   + event3 [ guard ] / action
    );
    // clang-format on
  }

  void set(int value) noexcept { i = value; }
  int get() const noexcept { return i; }

 private:
  int i = 0;
};

int main() {
  euml_emulation euml{42};
  msm::sm<euml_emulation> sm{euml};
  assert(sm.is(idle));
  assert(sm.process_event(e1{}));
  assert(sm.is(s1));
  assert(sm.process_event(e2{}));
  assert(sm.is(s2));
  assert(sm.process_event(e3{}));
  assert(sm.is(msm::terminate));
  // assert(123 == euml.get());
}
