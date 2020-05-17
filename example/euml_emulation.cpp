//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <cassert>

namespace sml = boost::sml;

namespace {
struct e1 {};
struct e2 {};
struct e3 {};

auto event1 = sml::event<e1>;
auto event2 = sml::event<e2>;
auto event3 = sml::event<e3>;

auto idle = sml::state<class idle>;
auto s1 = sml::state<class s1>;
auto s2 = sml::state<class s2>;

class euml_emulation;

struct Guard {
  template <class TEvent>
  bool operator()(euml_emulation&, const TEvent&) const;
} guard;

struct Action {
  template <class TEvent>
  void operator()(euml_emulation&, const TEvent&);
} action;

class euml_emulation {
 public:
  auto operator()() const {
    using namespace sml;
    // clang-format off
    return make_transition_table(
      s1 <= *idle + event1,
      s2 <= s1    + event2 [ guard ],
      X  <= s2    + event3 [ guard ] / action
    );
    // clang-format on
  }

  template <class TEvent>
  bool call_guard(const TEvent&) {
    return true;
  }

  void call_action(const e3&) {}
};

template <class TEvent>
bool Guard::operator()(euml_emulation& sm, const TEvent& event) const {
  return sm.call_guard(event);
}

template <class TEvent>
void Action::operator()(euml_emulation& sm, const TEvent& event) {
  sm.call_action(event);
}
}  // namespace

int main() {
  euml_emulation euml;
  sml::sm<euml_emulation> sm{euml};
  assert(sm.is(idle));
  sm.process_event(e1{});
  assert(sm.is(s1));
  sm.process_event(e2{});
  assert(sm.is(s2));
  sm.process_event(e3{});
  assert(sm.is(sml::X));
}
