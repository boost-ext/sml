// Copyright 2010 Christophe Henry
// henry UNDERSCORE christophe AT hotmail DOT com
// This is an extended version of the state machine available in the boost::mpl library
// Distributed under the same license as the original.
// Copyright for the original version:
// Copyright 2005 David Abrahams and Aleksey Gurtovoy. Distributed
// under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#define BOOST_MPL_LIMIT_STRING_SIZE 64
#define MPLLIBS_LIMIT_STRING_SIZE BOOST_MPL_LIMIT_STRING_SIZE
#include "benchmark.hpp"
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/euml2/euml2.hpp>

namespace msm = boost::msm;

struct base_state {
  template <class Event, class Fsm>
  void on_entry(Event const&, Fsm&) {}
  template <class Event, class Fsm>
  void on_exit(Event const&, Fsm&) {}
};

struct playing_ : msm::front::state_machine_def<playing_, base_state> {
  using no_exception_thrown = int;
  using no_message_queue = int;
  using initial_state = BOOST_MSM_EUML2_STATE("Song1", playing_);

  // clang-format off
  EUML2_STT(
      playing_,
      EUML2_STT_CFG(),
      EUML2_ROW("Song1 + next_song / start_next_song -> Song2"),
      EUML2_ROW("Song2 + previous_song / start_next_song -> Song1"),
      EUML2_ROW("Song2 + next_song / start_next_song -> Song3"),
      EUML2_ROW("Song3 + previous_song / start_prev_song -> Song2")
  )
  // clang-format on

  template <class FSM, class Event>
  void no_transition(Event const&, FSM&, int) {}
};

typedef msm::back::state_machine<playing_> Playing;

struct player_ : msm::front::state_machine_def<player_, base_state> {
  using no_exception_thrown = int;
  using no_message_queue = int;
  using initial_state = BOOST_MSM_EUML2_STATE("Empty", player_);

  // clang-format off
  EUML2_STT(player_,
    EUML2_STT_CFG(EUML2_STT_USE("Playing", Playing)),
    EUML2_ROW("Stopped + play / start_playback -> Playing"),
    EUML2_ROW("Pause + end_pause / resume_playback -> Playing"),
    EUML2_ROW("Open + open_close / close_drawer -> Empty"),
    EUML2_ROW("Empty + open_close / open_drawer -> Open"),
    EUML2_ROW("Pause + open_close / stop_and_open -> Open"),
    EUML2_ROW("Stopped + open_close / open_drawer -> Open"),
    EUML2_ROW("Playing + open_close / stop_and_open -> Open"),
    EUML2_ROW("Playing + pause / pause_playback -> Pause"),
    EUML2_ROW("Playing + stop / stop_playback -> Stopped"),
    EUML2_ROW("Pause + stop / stop_playback -> Stopped"),
    EUML2_ROW("Empty + cd_detected / store_cd_info -> Stopped"),
    EUML2_ROW("Stopped + stop / stopped_again -> Stopped")
  )
  // clang-format on

  template <class FSM, class Event>
  void no_transition(Event const&, FSM&, int) {}
};

typedef msm::back::state_machine<player_> player;

template <>
template <class Event, class Fsm, class SourceState, class TargetState>
void BOOST_MSM_EUML2_ACTION_IMPL("start_next_song", playing_)::operator()(Event const&, Fsm&, SourceState&, TargetState&) {}

template <>
template <class Event, class Fsm, class SourceState, class TargetState>
void BOOST_MSM_EUML2_ACTION_IMPL("start_prev_song", playing_)::operator()(Event const&, Fsm&, SourceState&, TargetState&) {}

template <>
template <class Event, class Fsm, class SourceState, class TargetState>
void BOOST_MSM_EUML2_ACTION_IMPL("start_playback", player_)::operator()(Event const&, Fsm&, SourceState&, TargetState&) {}

template <>
template <class Event, class Fsm, class SourceState, class TargetState>
void BOOST_MSM_EUML2_ACTION_IMPL("resume_playback", player_)::operator()(Event const&, Fsm&, SourceState&, TargetState&) {}

template <>
template <class Event, class Fsm, class SourceState, class TargetState>
void BOOST_MSM_EUML2_ACTION_IMPL("close_drawer", player_)::operator()(Event const&, Fsm&, SourceState&, TargetState&) {}

template <>
template <class Event, class Fsm, class SourceState, class TargetState>
void BOOST_MSM_EUML2_ACTION_IMPL("open_drawer", player_)::operator()(Event const&, Fsm&, SourceState&, TargetState&) {}

template <>
template <class Event, class Fsm, class SourceState, class TargetState>
void BOOST_MSM_EUML2_ACTION_IMPL("stop_and_open", player_)::operator()(Event const&, Fsm&, SourceState&, TargetState&) {}

template <>
template <class Event, class Fsm, class SourceState, class TargetState>
void BOOST_MSM_EUML2_ACTION_IMPL("stopped_again", player_)::operator()(Event const&, Fsm&, SourceState&, TargetState&) {}

int main() {
  player sm;
  sm.start();

  benchmark_execution_speed([&] {
    for (auto i = 0; i < 1'000; ++i) {
      sm.process_event(BOOST_MSM_EUML2_EVENT("open_close", player_)());
      sm.process_event(BOOST_MSM_EUML2_EVENT("open_close", player_)());
      sm.process_event(BOOST_MSM_EUML2_EVENT("cd_detected", player_)());
      sm.process_event(BOOST_MSM_EUML2_EVENT("play", player_)());
      for (auto j = 0; j < 1'000; ++j) {
        sm.process_event(BOOST_MSM_EUML2_EVENT("next_song", playing_)());
        sm.process_event(BOOST_MSM_EUML2_EVENT("next_song", playing_)());
        sm.process_event(BOOST_MSM_EUML2_EVENT("previous_song", playing_)());
        sm.process_event(BOOST_MSM_EUML2_EVENT("previous_song", playing_)());
      }

      sm.process_event(BOOST_MSM_EUML2_EVENT("pause", player_)());
      // go back to Playing
      sm.process_event(BOOST_MSM_EUML2_EVENT("end_pause", player_)());
      sm.process_event(BOOST_MSM_EUML2_EVENT("pause", player_)());
      sm.process_event(BOOST_MSM_EUML2_EVENT("stop", player_)());
      // event leading to the same state
      sm.process_event(BOOST_MSM_EUML2_EVENT("stop", player_)());
      sm.process_event(BOOST_MSM_EUML2_EVENT("open_close", player_)());
      sm.process_event(BOOST_MSM_EUML2_EVENT("open_close", player_)());
    }
  });
  benchmark_memory_usage(sm);
}
