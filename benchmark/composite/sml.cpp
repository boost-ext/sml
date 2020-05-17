//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "benchmark.hpp"
#include <boost/sml.hpp>

namespace sml = boost::sml;

struct play {};
struct end_pause {};
struct stop {};
struct pause {};
struct open_close {};
struct cd_detected {};
struct NextSong {};
struct PreviousSong {};

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

struct playing {
  auto operator()() const noexcept {
    using namespace sml;
    auto Song1 = state<class Song1>;
    auto Song2 = state<class Song2>;
    auto Song3 = state<class Song3>;

    // clang-format off
    return make_transition_table(
      Song2 <= *Song1 + event<NextSong> / start_next_song,
      Song1 <= Song2 + event<PreviousSong> / start_prev_song,
      Song3 <= Song2 + event<NextSong> / start_next_song,
      Song2 <= Song3 + event<PreviousSong> / start_prev_song
    );
    // clang-format on
  }
};

struct player {
  auto operator()() const noexcept {
    using namespace sml;
    auto Empty = state<class Empty>;
    auto Open = state<class Open>;
    auto Stopped = state<class Stopped>;
    auto Paused = state<class Paused>;
    auto Playing = state<playing>;

    // clang-format off
    return make_transition_table(
      Playing <= Stopped + event<play> / start_playback,
      Open <= Stopped + event<open_close> / open_drawer,
      Stopped <= Stopped + event<stop> / stopped_again,
      Empty <= Open + event<open_close> / close_drawer,
      Open <= *Empty + event<open_close> / open_drawer,
      Stopped <= Empty + event<cd_detected> / store_cd_info,
      Stopped <= Playing + event<stop> / stop_playback,
      Paused <= Playing + event<pause> / pause_playback,
      Open <= Playing + event<open_close> / stop_and_open,
      Playing <= Paused + event<end_pause> / resume_playback,
      Stopped <= Paused + event<stop> / stop_playback,
      Open <= Paused + event<open_close> / stop_and_open
    );
    // clang-format on
  }
};

int main() {
  sml::sm<player> sm;

  benchmark_execution_speed([&] {
    for (auto i = 0; i < 1'000; ++i) {
      sm.process_event(open_close());
      sm.process_event(open_close());
      sm.process_event(cd_detected());
      sm.process_event(play());

      for (auto j = 0; j < 1'000; ++j) {
        sm.process_event(NextSong());
        sm.process_event(NextSong());
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
  benchmark_memory_usage(sm);
}
