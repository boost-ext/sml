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

namespace sml = boost::sml;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};

struct sub {
  auto operator()() const {
    using namespace sml;
    // clang-format off
    return make_transition_table(
     *"idle"_s + event<e3> = "sub1"_s
    , "sub1"_s + event<e4> = X
    );
    // clang-format on
  }
};

struct composite {
  auto operator()() const {
    using namespace sml;
    // clang-format off
    return make_transition_table(
     *"idle"_s   + event<e1> = "s1"_s
    , "s1"_s     + event<e2> = state<sub>
    , state<sub> + event<e5> = X
    );
    // clang-format on
  }
};

template <class TSM>
class state_name_visitor {
 public:
  explicit state_name_visitor(const TSM& sm) : sm_{sm} {}

  template <class TSub>
  void operator()(boost::sml::aux::string<boost::sml::sm<TSub>>) const {
    std::cout << boost::sml::aux::get_type_name<TSub>() << ':';
    sm_.template visit_current_states<boost::sml::aux::identity<TSub>>(*this);
  }

  template <class TState>
  void operator()(TState state) const {
    std::cout << state.c_str() << '\n';
  }

 private:
  const TSM& sm_;
};

int main() {
  sml::sm<composite> sm{};

  const auto state_name = state_name_visitor<decltype(sm)>{sm};

  sm.process_event(e1{});
  sm.visit_current_states(state_name);  // s1

  sm.process_event(e2{});
  sm.visit_current_states(state_name);  // sub:idle

  sm.process_event(e3{});
  sm.visit_current_states(state_name);  // sub:sub1

  sm.process_event(e4{});
  sm.visit_current_states(state_name);  // sub:terminate

  sm.process_event(e5{});
  sm.visit_current_states(state_name);  // terminate
}
