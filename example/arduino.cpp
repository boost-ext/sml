//
// Copyright (c) 2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>

/**
 * Simulation: https://www.tinkercad.com/things/8rDYx57Ns7Q
 */
#if __has_include(<Arduino.h>)
#include <Arduino.h>

struct blink {
  enum class PIN : uint8_t {
    BUTTON = 2,
    LED = 11,
  };
  template <PIN Pin>
  struct pressed {
    static constexpr PIN pin = Pin;
  };

  auto operator()() const {
    using namespace boost::sml;

    constexpr auto setup = [] {
      pinMode(uint8_t(PIN::LED), OUTPUT);
      pinMode(uint8_t(PIN::BUTTON), INPUT);
    };
    constexpr auto turn_on = [](auto pin) { return [pin] { digitalWrite(uint8_t(pin), HIGH); }; };
    constexpr auto turn_off = [](auto pin) { return [pin] { digitalWrite(uint8_t(pin), LOW); }; };

    /**
     * Initial state: *initial_state
     * Transition DSL: src_state + event [ guard ] / action = dst_state
     */
    // clang-format off
    return make_transition_table(
      *"idle"_s                                  / setup              = "led off"_s,
       "led off"_s + event<pressed<PIN::BUTTON>> / turn_on (PIN::LED) = "led on"_s,
       "led on"_s  + event<pressed<PIN::BUTTON>> / turn_off(PIN::LED) = "led off"_s
    );
    // clang-format on
  }
};

template <template <class...> class TList, class... TEvents, class T>
constexpr auto dispatch(TList<TEvents...>, T& sm) -> void {
  // clang-format off
  (void)boost::sml::aux::swallow{0, ((
    [&sm](const auto& event) {
      if (digitalRead(uint8_t(event.pin))) {
        while (digitalRead(uint8_t(event.pin))) { delay(10); };
        sm.process_event(decltype(event){});
      }
    }(TEvents{})
  ), 0)...};
  // clang-format on
}

int main() {
  boost::sml::sm<blink> sm{};
  for (;;) {
    dispatch(decltype(sm)::events{}, sm);
  }
}
#else
int main() {}
#endif
