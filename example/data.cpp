//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <boost/format.hpp>
#include <cassert>
#include <iostream>

namespace sml = boost::sml;

namespace {
struct connect {
  int port{};
};
struct disconnect {};
struct interrupt {};

struct Disconnected {};
struct Connected {
  int port{};  /// per state data
};
struct Interrupted {
  int port{};  /// per state data
  static int next_id;
  int id{};

  Interrupted(int port=-1) : port(port), id(next_id++) { p("ctor"); }
  Interrupted(const Interrupted& other) : port(other.port), id(next_id++) { p((boost::format("copy ctor: %1% ->") % other.id).str().c_str(), false); };
  Interrupted& operator=(const Interrupted& other) { port = other.port; p("copy assign"); return *this;};
  Interrupted(Interrupted&& other) : port(other.port), id(next_id++) { p("move ctor"); };
  Interrupted& operator=(Interrupted&& other) { port = other.port; p("move assign"); return *this;};
  ~Interrupted() { p("dtor"); }

  void p(const char* op, bool colon=true) { printf(" %s%s %d\n", op, colon ? ":" : "", id); }
};
int Interrupted::next_id = 0;

class data {
  using Self = data;

 public:
  explicit data(const std::string& address) : address{address} {}

  auto operator()() {
    using namespace boost::sml;

    const auto set = [](const auto& event, Connected& state) { state.port = event.port; };
    const auto update = [](Connected& src_state, Interrupted& dst_state) { dst_state.port = src_state.port; };

    // clang-format off
    return make_transition_table(
      * state<Disconnected> + event<connect>    / (set, &Self::print)    = state<Connected>
      , state<Connected>    + event<interrupt>  / (update, &Self::print) = state<Interrupted>
      , state<Interrupted>  + event<connect>    / (set, &Self::print)    = state<Connected>
      , state<Connected>    + event<disconnect> / (&Self::print)         = X
    );
    // clang-format on
  }

 private:
  void print(Connected& state) { std::cout << address << ':' << state.port << '\n'; };

  std::string address{};  /// shared data between states
};
}  // namespace

int main() {
  data d{std::string{"127.0.0.1"}};
  sml::sm<data> sm{d, Connected{1}, Interrupted()};
  sm.process_event(connect{1024});
  sm.process_event(interrupt{});
  sm.process_event(connect{1025});
  sm.process_event(disconnect{});
  assert(sm.is(sml::X));
}
