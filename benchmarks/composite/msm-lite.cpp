//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "benchmark.hpp"
#include <typeinfo>
#include <cassert>
#include "msm/msm.hpp"

struct play {};
struct end_pause {};
struct stop {};
struct pause {};
struct open_close {};
struct cd_detected {};
struct NextSong {};
struct PreviousSong {};

// void start_playback() {}
// void open_drawer() {}
// void close_drawer() {}
// void store_cd_info() {}
// void stop_playback() {}
// void pause_playback() {}
// void resume_playback() {}
// void stop_and_open() {}
// void stopped_again() {}
// void start_next_song() {}
// void start_prev_song() {}

auto start_playback = [] {};
auto open_drawer = [] {};
auto close_drawer = [] {};
auto store_cd_info = [] {};
auto stop_playback = [] {};
auto pause_playback = [] {};
auto resume_playback = [] {};
auto stop_and_open = [] {};
auto stopped_again = [] {};
auto start_next_song = [] {};
auto start_prev_song = [] {};

struct player {
  struct playing {
    auto configure() const noexcept {
      using namespace msm;
      state Song1, Song2, Song3;

      // clang-format off
        return make_transition_table(
          Song1(initial) == Song2 + event<NextSong> / start_next_song,
          Song2 == Song1 + event<PreviousSong> / start_prev_song,
          Song2 == Song3 + event<NextSong> / start_next_song,
          Song3 == Song2 + event<PreviousSong> / start_prev_song
        );
      // clang-format on
    }
  };

  auto configure() const noexcept {
    using namespace msm;
    state Empty, Open, Stopped, Paused;
    sm<playing> Playing;

    // clang-format off
    return make_transition_table(
      Stopped == Playing + event<play> / start_playback,
      Stopped == Stopped + event<stop> / stopped_again,
      Open == Empty + event<open_close> / close_drawer,
      Empty(initial) == Open + event<open_close> / open_drawer,
      Empty == Stopped + event<cd_detected> / store_cd_info,
      Playing == Stopped + event<stop> / stop_playback,
      Playing == Paused + event<pause> / pause_playback,
      Playing == Open + event<open_close> / stop_and_open,
      Paused == Playing + event<end_pause> / resume_playback,
      Paused == Stopped + event<stop> / stop_playback,
      Paused == Open + event<open_close> / stop_and_open
    );
    // clang-format on
  }
};

int main() {
  msm::sm<player> sm;

  benchmark([&] {
    for (auto i = 0; i < 1'000; ++i) {
      sm.process_event(open_close());
      sm.process_event(open_close());
      sm.process_event(cd_detected());
      sm.process_event(play());

      for (auto j = 0; j < 1'000; ++j) {
        assert(sm.process_event(NextSong()));
        assert(sm.process_event(NextSong()));
        assert(sm.process_event(PreviousSong()));
        sm.process_event(PreviousSong());
        sm.process_event(PreviousSong());
      }

      sm.process_event(pause());
      // go back to Playing
      sm.process_event(end_pause());
      sm.process_event(pause());
      sm.process_event(stop());
      // event leading to the same state
      sm.process_event(stop());
      sm.process_event(open_close());
      sm.process_event(open_close());
    }
  });
}
