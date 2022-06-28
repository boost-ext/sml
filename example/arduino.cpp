//
// Copyright (c) 2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>

#if __has_include(<Arduino.h>)
#include <Arduino.h>

// clang-format off
template<class T>
concept component = requires {
  T::setup;
  T::on;
  T::off;
};

template<auto Pin>
struct led final {
  static constexpr auto setup = [] { pinMode(uint8_t(Pin), OUTPUT); };
  static constexpr auto on    = [] { digitalWrite(uint8_t(Pin), HIGH); }; 
  static constexpr auto off   = [] { digitalWrite(uint8_t(Pin), LOW); }; 
};

template<auto Pin>
struct button final {
  static constexpr auto setup   = [] { pinMode(uint8_t(Pin), INPUT); };
  static constexpr auto on      = [] (const auto& event) { return event() == HIGH; };
  static constexpr auto off     = [] (const auto& event) { return event() == LOW; };
  static constexpr auto pressed = [] { return digitalRead(uint8_t(Pin)); }; 
};

/**
 * Implementation: https://godbolt.org/z/55z858
 * Simulation: https://www.tinkercad.com/things/9epUrFrzKP3
 */
template<component Btn, component Led>
struct switcher {
  constexpr switcher() {
    []<class... Ts>(switcher<Ts...>) { (Ts::setup(), ...); }(*this);
  }

  constexpr auto operator()() const {
    const auto event = []<class TEvent>(TEvent) { return boost::sml::event<TEvent>; };

    /**
     * Initial state: *initial_state
     * Transition DSL: src_state + event [ guard ] / action = dst_state
     */
    using namespace boost::sml;
    return make_transition_table(
      *"off"_s + event(Btn::pressed) [ Btn::on  ] / Led::on  = "on"_s,
       "on"_s  + event(Btn::pressed) [ Btn::off ] / Led::off = "off"_s
    );
  }
};

int main() {
  for (boost::sml::sm<switcher<button<2>, led<11>>> sm;;) {
    [&sm]<template<class...> class TList, class... TEvents>(TList<TEvents...>) {
      (sm.process_event(TEvents{}), ...);
    }(decltype(sm)::events{});
  }
}
// clang-format on
#else
int main() {}
#endif
