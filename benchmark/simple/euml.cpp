// Copyright 2010 Christophe Henry
// henry UNDERSCORE christophe AT hotmail DOT com
// This is an extended version of the state machine available in the boost::mpl library
// Distributed under the same license as the original.
// Copyright for the original version:
// Copyright 2005 David Abrahams and Aleksey Gurtovoy. Distributed
// under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include "benchmark.hpp"
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/euml/euml.hpp>
#include <boost/msm/front/euml/stl.hpp>

namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace boost::msm::front::euml;

// events
BOOST_MSM_EUML_EVENT(play)
BOOST_MSM_EUML_EVENT(end_pause_)
BOOST_MSM_EUML_EVENT(stop)
BOOST_MSM_EUML_EVENT(pause_)
BOOST_MSM_EUML_EVENT(open_close)
BOOST_MSM_EUML_EVENT(cd_detected)

BOOST_MSM_EUML_ACTION(start_playback){
    template <class FSM, class EVT, class SourceState, class TargetState> void operator()(EVT const &, FSM &, SourceState &,
                                                                                          TargetState &){}};
BOOST_MSM_EUML_ACTION(open_drawer){template <class FSM, class EVT, class SourceState, class TargetState> void

                                   operator()(EVT const &, FSM &, SourceState &, TargetState &){}};
BOOST_MSM_EUML_ACTION(close_drawer){
    template <class FSM, class EVT, class SourceState, class TargetState> void operator()(EVT const &, FSM &, SourceState &,
                                                                                          TargetState &){}};
BOOST_MSM_EUML_ACTION(store_cd_info){
    template <class FSM, class EVT, class SourceState, class TargetState> void operator()(EVT const &, FSM &, SourceState &,
                                                                                          TargetState &){}};
BOOST_MSM_EUML_ACTION(stop_playback){
    template <class FSM, class EVT, class SourceState, class TargetState> void operator()(EVT const &, FSM &, SourceState &,
                                                                                          TargetState &){}};
BOOST_MSM_EUML_ACTION(pause__playback){
    template <class FSM, class EVT, class SourceState, class TargetState> void operator()(EVT const &, FSM &, SourceState &,
                                                                                          TargetState &){}};
BOOST_MSM_EUML_ACTION(resume_playback){
    template <class FSM, class EVT, class SourceState, class TargetState> void operator()(EVT const &, FSM &, SourceState &,
                                                                                          TargetState &){}};
BOOST_MSM_EUML_ACTION(stop_and_open){
    template <class FSM, class EVT, class SourceState, class TargetState> void operator()(EVT const &, FSM &, SourceState &,
                                                                                          TargetState &){}};
BOOST_MSM_EUML_ACTION(stopped_again){
    template <class FSM, class EVT, class SourceState, class TargetState> void operator()(EVT const &, FSM &, SourceState &,
                                                                                          TargetState &){}};

// The list of FSM states
BOOST_MSM_EUML_STATE((), Empty)
BOOST_MSM_EUML_STATE((), Open)
BOOST_MSM_EUML_STATE((), Stopped)
BOOST_MSM_EUML_STATE((), Playing)
BOOST_MSM_EUML_STATE((), pause_d)

// clang-format off
BOOST_MSM_EUML_TRANSITION_TABLE(
    (Playing == Stopped + play / start_playback,
     Playing == pause_d + end_pause_ / resume_playback,
     Empty == Open + open_close / close_drawer,
     Open == Empty + open_close / open_drawer,
     Open == pause_d + open_close / stop_and_open,
     Open == Stopped + open_close / open_drawer,
     Open == Playing + open_close / stop_and_open,
     pause_d == Playing + pause_ / pause__playback,
     Stopped == Playing + stop / stop_playback,
     Stopped == pause_d + stop / stop_playback,
     Stopped == Empty + cd_detected / store_cd_info,
     Stopped == Stopped + stop / stopped_again
     ),
    transition_table)
// clang-format on

BOOST_MSM_EUML_ACTION(Log_No_Transition){template <class FSM, class Event> void operator()(Event const &, FSM &, int){}};

// create a state machine "on the fly"
BOOST_MSM_EUML_DECLARE_STATE_MACHINE((transition_table,                            // STT
                                      init_ << Empty,                              // Init State
                                      no_action,                                   // Entry
                                      no_action,                                   // Exit
                                      attributes_ << no_attributes_,               // Attributes
                                      configure_ << no_exception << no_msg_queue,  // configuration
                                      Log_No_Transition                            // no_transition handler
                                      ),
                                     player_)  // fsm name

typedef msm::back::state_machine<player_> player;

int main() {
  player sm;
  sm.start();

  benchmark_execution_speed([&] {
    for (auto i = 0; i < 1000000; ++i) {
      sm.process_event(open_close);
      sm.process_event(open_close);
      sm.process_event(cd_detected);
      sm.process_event(play);
      sm.process_event(pause_);
      // go back to Playing
      sm.process_event(end_pause_);
      sm.process_event(pause_);
      sm.process_event(stop);
      // event leading to the same state
      sm.process_event(stop);
      sm.process_event(open_close);
      sm.process_event(open_close);
    }
  });
  benchmark_memory_usage(sm);
}
