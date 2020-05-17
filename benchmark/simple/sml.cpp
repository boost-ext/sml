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

auto start_playback = [] {};
auto resume_playback = [] {};
auto close_drawer = [] {};
auto open_drawer = [] {};
auto stop_and_open = [] {};
auto stopped_again = [] {};
auto store_cd_info = [] {};
auto pause_playback = [] {};
auto stop_playback = [] {};

struct player {
  auto operator()() const noexcept {
    using namespace sml;
    auto Empty = state<class Empty>;
    auto Open = state<class Open>;
    auto Stopped = state<class Stopped>;
    auto Playing = state<class Playing>;
    auto Pause = state<class Pause>;

    // clang-format off
    return make_transition_table(
        Playing <= Stopped + event<play> / start_playback,
        Playing <= Pause + event<end_pause> / resume_playback,
        Empty <= Open + event<open_close> / close_drawer,
        Open <= *Empty + event<open_close> / open_drawer,
        Open <= Pause + event<open_close> / stop_and_open,
        Open <= Stopped + event<open_close> / open_drawer,
        Open <= Playing + event<open_close> / stop_and_open,
        Pause <= Playing + event<pause> / pause_playback,
        Stopped <= Playing + event<stop> / stop_playback,
        Stopped <= Pause + event<stop> / stop_playback,
        Stopped <= Empty + event<cd_detected> / store_cd_info,
        Stopped <= Stopped + event<stop> / stopped_again
    );
    // clang-format on
  }
};

int main() {
  sml::sm<player> sm;

  benchmark_execution_speed([&] {
    for (auto i = 0; i < 1'000'000; ++i) {
      sm.process_event(open_close{});
      sm.process_event(open_close{});
      sm.process_event(cd_detected{});
      sm.process_event(play{});
      sm.process_event(pause{});
      // go back to Playing
      sm.process_event(end_pause{});
      sm.process_event(pause{});
      sm.process_event(stop{});
      // event leading to the same state
      sm.process_event(stop{});
      sm.process_event(open_close{});
      sm.process_event(open_close{});
    }
  });
  benchmark_memory_usage(sm);
}
