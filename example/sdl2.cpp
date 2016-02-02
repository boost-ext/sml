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

// clang-format off
#if __has_include(<SDL2/SDL_events.h>)
#include <SDL2/SDL_events.h>
// clang-format on
#else
enum { SDLK_SPACE = ' ' };
enum SDL_EventType { SDL_FIRSTEVENT = 0, SDL_QUIT, SDL_KEYUP, SDL_MOUSEBUTTONUP, SDL_LASTEVENT };
struct SDL_KeyboardEvent {
  SDL_EventType type;
  struct {
    int sym;
  } keysym;
};
struct SDL_MouseButtonEvent {
  SDL_EventType type;
  int button;
};
struct SDL_QuitEvent {
  SDL_EventType type;
};
union SDL_Event {
  SDL_EventType type;
  SDL_KeyboardEvent key;
  SDL_MouseButtonEvent button;
  SDL_QuitEvent quit;
};
#endif

template <SDL_EventType Id>
struct sdl_event_impl {
  static constexpr auto id = Id;
  explicit sdl_event_impl(const SDL_Event& data) noexcept : data(data) {}
  SDL_Event data;
};

template <SDL_EventType Id>
decltype(msm::event<sdl_event_impl<Id>>) sdl_event{};

auto is_key = [](auto key) { return [=](auto event) { return event.data.key.keysym.sym == key; }; };

struct sdl2 {
  auto configure() const noexcept {
    using namespace msm;
    // clang-format off
    return make_transition_table(
      //------------------------------------------------------------------------------//
        "idle"_s(initial) == "wait_for_user_input"_s
          / [] { std::cout << "initialization" << std::endl; }

      , "wait_for_user_input"_s == "key_pressed"_s + sdl_event<SDL_KEYUP> [ is_key(SDLK_SPACE) ]
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
  auto dispatch_event = msm::make_dispatch_table<SDL_Event, SDL_FIRSTEVENT, SDL_LASTEVENT>(sm);

  SDL_Event event;

  // while (SDL_PollEvent(&event)) {
  //   dispatch_event(event, event.type)
  // };

  {
    SDL_KeyboardEvent keyboard_event;
    keyboard_event.type = SDL_KEYUP;
    keyboard_event.keysym.sym = SDLK_SPACE;
    event.key = keyboard_event;
    assert(dispatch_event(event, event.type));
  }

  {
    SDL_MouseButtonEvent mousebutton_event;
    mousebutton_event.type = SDL_MOUSEBUTTONUP;
    mousebutton_event.button = 1;
    event.button = mousebutton_event;
    assert(dispatch_event(event, event.type));
  }

  {
    SDL_QuitEvent quit_event;
    quit_event.type = SDL_QUIT;
    event.quit = quit_event;
    assert(dispatch_event(event, event.type));
  }

  assert(sm.is(msm::terminate, msm::terminate));
}
