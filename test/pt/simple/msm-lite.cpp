//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "benchmark.hpp"
#include "boost/msm.hpp"

namespace msm = boost::msm;

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
  auto configure() const noexcept {
    using namespace msm;
    state<class Empty> Empty;
    state<class Open> Open;
    state<class Stopped> Stopped;
    state<class Playing> Playing;
    state<class Pause> Pause;

    // clang-format off
    return make_transition_table(
        Stopped == Playing + event<play> / start_playback,
        Pause == Playing + event<end_pause> / resume_playback,
        Open == Empty + event<open_close> / close_drawer,
        Empty(initial) == Open + event<open_close> / open_drawer,
        Pause == Open + event<open_close> / stop_and_open,
        Stopped == Open + event<open_close> / open_drawer,
        Playing == Open + event<open_close> / stop_and_open,
        Playing == Pause + event<pause> / pause_playback,
        Playing == Stopped + event<stop> / stop_playback,
        Pause == Stopped + event<stop> / stop_playback,
        Empty == Stopped + event<cd_detected> / store_cd_info,
        Stopped == Stopped + event<stop> / stopped_again
    );
    // clang-format on
  }
};

int main() {
  msm::sm<player> sm;

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
