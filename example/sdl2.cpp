//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/msm-lite.hpp"
#include <cassert>
#include <iostream>

namespace msm = boost::msm::lite;

// Fake SDL2
enum { SDLK_SPACE = ' ' };
enum SDL_EventType { SDL_FIRST_EVENT = 0, SDL_QUIT, SDL_KEYUP, SDL_MOUSEBUTTONUP, SDL_LAST_EVENT };
struct SDL_Event {
  SDL_EventType type;
  int key = 0;
  bool button = false;
};
//

template <SDL_EventType Id>
struct sdl_event_impl {
  static constexpr auto id = Id;
  explicit sdl_event_impl(const SDL_Event& data) noexcept : data(data) {}
  SDL_Event data;
};

template <SDL_EventType Id>
decltype(msm::event<sdl_event_impl<Id>>) sdl_event{};

template <int Key>
auto is_key = [](auto event) { return event.data.key == Key; };

struct sdl2 {
  auto configure() const noexcept {
    using namespace msm;
    // clang-format off
    return make_transition_table(
      //------------------------------------------------------------------------------//
        "idle"_s(initial) == "wait_for_user_input"_s
          / [] { std::cout << "initialization" << std::endl; }

      , "wait_for_user_input"_s == "key_pressed"_s + sdl_event<SDL_KEYUP> [ is_key<SDLK_SPACE> ]
          / [] { std::cout << "space pressed" << std::endl; }

      , "key_pressed"_s == terminate + sdl_event<SDL_MOUSEBUTTONUP>
          / [] { std::cout << "mouse button pressed" << std::endl; }
      //------------------------------------------------------------------------------//
      , "waiting_for_quit"_s(initial) == terminate + sdl_event<SDL_QUIT>
          / [] { std::cout << "quit" << std::endl; }
      //------------------------------------------------------------------------------//
    );
    // clang-format on
  }
};

int main() {
  msm::sm<sdl2> sm;
  auto dispatch_event = msm::make_dispatch_table<SDL_Event, SDL_FIRST_EVENT, SDL_LAST_EVENT>(sm);

  {
    SDL_Event event{SDL_KEYUP};
    event.key = SDLK_SPACE;
    assert(dispatch_event(event, event.type));
  }

  {
    SDL_Event event{SDL_MOUSEBUTTONUP};
    event.button = true;
    assert(dispatch_event(event, event.type));
  }

  {
    SDL_Event event{SDL_QUIT};
    assert(dispatch_event(event, event.type));
  }

  assert(sm.is(msm::terminate, msm::terminate));
}
