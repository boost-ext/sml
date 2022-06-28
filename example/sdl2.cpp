//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <cassert>
#include <iostream>

#include "boost/sml/utility/dispatch_table.hpp"

namespace sml = boost::sml;

// clang-format off
#if __has_include(<SDL2/SDL_events.h>)
#include <SDL2/SDL_events.h>
// clang-format on

namespace {

#else
namespace {

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
decltype(sml::event<sdl_event_impl<Id>>) sdl_event{};

struct IsKey {
  auto operator()(int key) {
    return [=](auto event) { return event.data.key.keysym.sym == key; };
  }
} is_key;

struct sdl2 {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
      //------------------------------------------------------------------------------//
        "wait_for_user_input"_s <= *"idle"_s
          / [] { std::cout << "initialization" << std::endl; }

      , "key_pressed"_s <= "wait_for_user_input"_s + sdl_event<SDL_KEYUP> [ is_key(SDLK_SPACE) ]
          / [] { std::cout << "space pressed" << std::endl; }

      , X <= "key_pressed"_s + sdl_event<SDL_MOUSEBUTTONUP>
          / [] { std::cout << "mouse button pressed" << std::endl; }
      //------------------------------------------------------------------------------//
      , X <= *"waiting_for_quit"_s + sdl_event<SDL_QUIT>
          / [] { std::cout << "quit" << std::endl; }
      //------------------------------------------------------------------------------//
    );
    // clang-format on
  }
};
}

int main() {
  sml::sm<sdl2> sm;
  auto dispatch_event = sml::utility::make_dispatch_table<SDL_Event, SDL_FIRSTEVENT, SDL_LASTEVENT>(sm);

  SDL_Event event;

  // while (SDL_PollEvent(&event)) {
  //   dispatch_event(event, event.type)
  // };

  {
    SDL_KeyboardEvent keyboard_event;
    keyboard_event.type = SDL_KEYUP;
    keyboard_event.keysym.sym = SDLK_SPACE;
    event.key = keyboard_event;
    dispatch_event(event, event.type);
  }

  {
    SDL_MouseButtonEvent mousebutton_event;
    mousebutton_event.type = SDL_MOUSEBUTTONUP;
    mousebutton_event.button = 1;
    event.button = mousebutton_event;
    dispatch_event(event, event.type);
  }

  {
    SDL_QuitEvent quit_event;
    quit_event.type = SDL_QUIT;
    event.quit = quit_event;
    dispatch_event(event, event.type);
  }

  assert(sm.is(sml::X, sml::X));
}
