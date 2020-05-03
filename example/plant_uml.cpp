//
// Copyright (c) 2016-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <cassert>
#include <iostream>
#include <string>
#include <typeinfo>
#include <set>

namespace sml = boost::sml;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};

struct guard {
  bool operator()() const { return true; }
} guard;

struct action {
  void operator()() {}
} action;


struct sub {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
        *"idle"_s + event<e3> /action = "s1"_s
        , "s1"_s + event<e4> / action = X
    );
    // clang-format on
  }
};

struct plant_uml {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
        *"idle"_s + event<e1> = state<sub>
        , "idle"_s + event<e5> = state<sub>
        , state<sub> + event<e2> [ guard ] / action = "s2"_s
        , "s2"_s + event<e3> [ guard ] = "s1"_s
        , "s2"_s + event<e4> / action = X

        ,*"idle2"_s + event<e2> = "s17"_s
        , "s17"_s + event<e3> = X
    );
    // clang-format on
  }
};


template <typename>
struct is_sub_state_machine : std::false_type
{};

template <class T, class... Ts>
struct is_sub_state_machine<boost::sml::back::sm<boost::sml::back::sm_policy<T, Ts...>>>
    : std::true_type
{};

using strset_t = std::set<std::string>;

template <class T>
void dump_transition(strset_t& substates_handled, int& starts) noexcept {
  auto src_state = std::string{sml::aux::string<typename T::src_state>{}.c_str()};
  auto dst_state = std::string{sml::aux::string<typename T::dst_state>{}.c_str()};
  if (dst_state == "terminate") {
    dst_state = "[*]";
  }

  if (T::initial) {
    std::cout <<  (starts++ ? "--\n" : "") <<"[*] --> " << src_state << std::endl;
  }

  if constexpr (is_sub_state_machine<typename T::dst_state>::value) {

    auto [loc, suc] = substates_handled.insert(dst_state);

    if (suc) {
      std::cout << "\nstate " << dst_state << " {\n";
      int new_starts{0};
      dump_transitions(substates_handled, new_starts, typename T::dst_state::transitions{});
      std::cout << "}\n";
    }
  }

  std::cout << src_state << " --> " << dst_state;

  const auto has_event = !sml::aux::is_same<typename T::event, sml::anonymous>::value;
  const auto has_guard = !sml::aux::is_same<typename T::guard, sml::front::always>::value;
  const auto has_action = !sml::aux::is_same<typename T::action, sml::front::none>::value;

  if (has_event || has_guard || has_action) {
    std::cout << " :";
  }

  if (has_event) {
    std::cout << " " << boost::sml::aux::get_type_name<typename T::event>();
  }

  if (has_guard) {
    std::cout << " [" << boost::sml::aux::get_type_name<typename T::guard::type>() << "]";
  }

  if (has_action) {
    std::cout << " / " << boost::sml::aux::get_type_name<typename T::action::type>();
  }

  std::cout << std::endl;
}

template <template <class...> class T, class... Ts>
void dump_transitions(strset_t& substates_handled, int& starts, const T<Ts...>&) noexcept {
  int _[]{0, (dump_transition<Ts>(substates_handled, starts), 0)...};
  (void)_;
}

template <class SM>
void dump(const SM&) noexcept {
  std::set<std::string> substates_handled;  // guarantee only one dump per sub-machine
  int starts{0};                            // '--' is required between ortho states

  std::cout << "@startuml" << std::endl << std::endl;
  dump_transitions(substates_handled, starts, typename SM::transitions{});
  std::cout << std::endl << "@enduml" << std::endl;
}

int main() {
  sml::sm<plant_uml> sm;
  dump(sm);
}
