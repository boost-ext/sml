//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include <iostream>
#include <string>
#include <typeinfo>
#include "boost/msm-lite.hpp"

namespace msm = boost::msm::lite;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};

struct guard {
  bool operator()() const { return true; }
} guard;

struct action {
  void operator()() {}
} action;

struct plant_uml {
  auto operator()() const noexcept {
    using namespace msm;
    // clang-format off
    return make_transition_table(
       *"idle"_s + event<e1> = "s1"_s
      , "s1"_s + event<e2> [ guard ] / action = "s2"_s
      , "s2"_s + event<e3> [ guard ] = "s1"_s
      , "s2"_s + event<e4> / action = X
    );
    // clang-format on
  }
};

template <class T>
void dump_transition() noexcept {
  auto src_state = std::string{msm::state<typename T::src_state>.c_str()};
  auto dst_state = std::string{msm::state<typename T::dst_state>.c_str()};
  if (dst_state == "X") {
    dst_state = "[*]";
  }

  if (T::initial) {
    std::cout << "[*] --> " << src_state << std::endl;
  }

  std::cout << src_state << " --> " << dst_state;

  const auto has_event = !msm::aux::is_same<typename T::event, msm::detail::anonymous>::value;
  const auto has_guard = !msm::aux::is_same<typename T::guard, msm::detail::always>::value;
  const auto has_action = !msm::aux::is_same<typename T::action, msm::detail::none>::value;

  if (has_event || has_guard || has_action) {
    std::cout << " :";
  }

  if (has_event) {
    std::cout << " " << typeid(typename T::event).name();
  }

  if (has_guard) {
    std::cout << " [" << typeid(typename T::guard).name() << "]";
  }

  if (has_action) {
    std::cout << " / " << typeid(typename T::action).name();
  }

  std::cout << std::endl;
}

template <template <class...> class T, class... Ts>
void dump_transitions(const T<Ts...>&) noexcept {
  int _[]{0, (dump_transition<Ts>(), 0)...};
  (void)_;
}

template <class SM>
void dump(const SM&) noexcept {
  std::cout << "@startuml" << std::endl << std::endl;
  dump_transitions(typename SM::transitions{});
  std::cout << std::endl << "@enduml" << std::endl;
}

int main() {
  msm::sm<plant_uml> sm;
  dump(sm);
}
