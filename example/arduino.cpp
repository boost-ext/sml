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
  T::setup();
  T::on();
  T::off();
};

template<auto Pin>
struct led final {
  static constexpr auto setup = [] { pinMode(uint8_t(Pin), OUTPUT); };
  static constexpr auto on    = [] { digitalWrite(uint8_t(Pin), HIGH); };
  static constexpr auto off   = [] { digitalWrite(uint8_t(Pin), LOW); };
};

template<auto Pin>
struct button final {
  static constexpr auto setup = [] { pinMode(uint8_t(Pin), INPUT); };
  static constexpr auto on    = [] { return digitalRead(uint8_t(Pin)) == HIGH; };
  static constexpr auto off   = [] { return digitalRead(uint8_t(Pin)) == LOW; };
};

/**
 * Implementation: https://godbolt.org/z/Ej5KeE
 * Simulation: https://www.tinkercad.com/things/9epUrFrzKP3
 */
template<component TButton, component TLed>
struct switcher {
  constexpr auto operator()() const {
    const auto event = []<class TEvent>(TEvent) { return boost::sml::event<TEvent>; };
    const auto setup = [this] { []<class... Ts>(switcher<Ts...>) { (Ts::setup(), ...); }(*this); };

    /**
     * Initial state: *initial_state
     * Transition DSL: src_state + event [ guard ] / action = dst_state
     */
    using namespace boost::sml;
    return make_transition_table(
      *"idle"_s                          / setup     = "led off"_s,
       "led off"_s + event(TButton::on)  / TLed::on  = "led on"_s,
       "led on"_s  + event(TButton::off) / TLed::off = "led off"_s
    );
  }
};

int main() {
  for (boost::sml::sm<switcher<button<2>, led<11>>> sm;;) {
    [&sm]<template<class...> class TList, class... TEvents>(TList<TEvents...>) {
      ([&sm](const auto& event) {
        if (event()) { sm.process_event(event); }
      }(TEvents{}), ...);
    }(decltype(sm)::events{});
  }
}
// clang-format on
#else
int main() {}
#endif
