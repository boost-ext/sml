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

namespace {
struct connect {
  int id{};
};
struct disconnect {};
struct interrupt {};

struct Disconnected {};
struct Connected {
  int id{};  /// per state data
};
struct Interrupted {
  int id{};  /// per state data
};

class data {
 public:
  explicit data(const std::string& address) : address{address} {}

  auto operator()() {
    using namespace boost::sml;

    const auto set = [](const auto& event, Connected& state) { state.id = event.id; };

    const auto update = [](Connected& src_state, Interrupted& dst_state) { dst_state.id = src_state.id; };

    const auto print = [this](Connected& state) { std::cout << address << ':' << state.id << '\n'; };

    // clang-format off
    return make_transition_table(
      * state<Disconnected> + event<connect>    / (set, print)    = state<Connected>
      , state<Connected>    + event<interrupt>  / (update, print) = state<Interrupted>
      , state<Interrupted>  + event<connect>    / (set, print)    = state<Connected>
      , state<Connected>    + event<disconnect> / (print)         = X
    );
    // clang-format on
  }

 private:
  std::string address{};  /// shared data between states
};
}  // namespace

int main() {
  Disconnected ds{};
  Connected cs{};
  Interrupted is{};
  sml::sm<data> sm{data{"127.0.0.1"}, ds, cs, is};
  sm.process_event(connect{1024});
  sm.process_event(interrupt{});
  sm.process_event(connect{1025});
  sm.process_event(disconnect{});
  assert(sm.is(sml::X));
}
