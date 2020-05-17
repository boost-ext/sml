//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <cassert>
#include <cstdio>
#include <iostream>

namespace sml = boost::sml;

namespace {
struct my_logger {
  template <class SM, class TEvent>
  void log_process_event(const TEvent&) {
    printf("[%s][process_event] %s\n", sml::aux::get_type_name<SM>(), sml::aux::get_type_name<TEvent>());
  }

  template <class SM, class TGuard, class TEvent>
  void log_guard(const TGuard&, const TEvent&, bool result) {
    printf("[%s][guard] %s %s %s\n", sml::aux::get_type_name<SM>(), sml::aux::get_type_name<TGuard>(),
           sml::aux::get_type_name<TEvent>(), (result ? "[OK]" : "[Reject]"));
  }

  template <class SM, class TAction, class TEvent>
  void log_action(const TAction&, const TEvent&) {
    printf("[%s][action] %s %s\n", sml::aux::get_type_name<SM>(), sml::aux::get_type_name<TAction>(),
           sml::aux::get_type_name<TEvent>());
  }

  template <class SM, class TSrcState, class TDstState>
  void log_state_change(const TSrcState& src, const TDstState& dst) {
    printf("[%s][transition] %s -> %s\n", sml::aux::get_type_name<SM>(), src.c_str(), dst.c_str());
  }
};

struct e1 {};
struct e2 {};

struct guard {
  bool operator()() const { return true; }
} guard;

struct action {
  void operator()() {}
} action;

struct logging {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
       *"idle"_s + event<e1> [ guard && guard ] / action = "s1"_s
    );
    // clang-format on
  }
};
}  // namespace

int main() {
  my_logger logger;
  sml::sm<logging, sml::logger<my_logger>> sm{logger};
  sm.process_event(e1{});
  sm.process_event(e2{});
}
