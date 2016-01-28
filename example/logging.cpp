//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <cstdio>
#include <typeinfo>

template <class SM, class TEvent>
void log_process_event(const TEvent&) {
  printf("[%s][process_event] %s\n", typeid(SM).name(), typeid(TEvent).name());
}

template <class SM, class TGuard, class TEvent>
void log_guard(const TGuard&, const TEvent&, bool result) {
  printf("[%s][guard] %s %s %s\n", typeid(SM).name(), typeid(TGuard).name(), typeid(TEvent).name(),
         (result ? "[OK]" : "[Reject]"));
}

template <class SM, class TAction, class TEvent>
void log_action(const TAction&, const TEvent&) {
  printf("[%s][action] %s %s\n", typeid(SM).name(), typeid(TAction).name(), typeid(TEvent).name());
}

template <class SM, class TSrcState, class TDstState>
void log_state_change(const TSrcState& src, const TDstState& dst) {
  printf("[%s][transition] %s -> %s\n", typeid(SM).name(), src.c_str(), dst.c_str());
}

#define BOOST_MSM_LOG(T, SM, ...) log_##T<SM>(__VA_ARGS__)
#include "boost/msm-lite.hpp"
#include <cassert>
#include <iostream>

namespace msm = boost::msm::lite;

struct e1 {};
struct e2 {};

struct guard {
  bool operator()() const { return true; }
} guard;

struct action {
  void operator()() {}
} action;

struct logging {
  auto configure() const noexcept {
    using namespace msm;

    // clang-format off
    return make_transition_table(
        "idle"_s(initial) == "s1"_s + event<e1> [ guard && guard ] / action
    );
    // clang-format on
  }
};

int main() {
  msm::sm<logging> sm;
  assert(sm.process_event(e1{}));
  assert(!sm.process_event(e2{}));
}
