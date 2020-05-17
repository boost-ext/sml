//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#define FUSION_MAX_VECTOR_SIZE 50
#define BOOST_MPL_LIMIT_VECTOR_SIZE 50
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/euml/euml.hpp>
#include <boost/msm/front/euml/stl.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <cstdlib>
#include "benchmark.hpp"

namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace boost::msm::front::euml;

// events
BOOST_MSM_EUML_EVENT(e1)
BOOST_MSM_EUML_EVENT(e2)
BOOST_MSM_EUML_EVENT(e3)
BOOST_MSM_EUML_EVENT(e4)
BOOST_MSM_EUML_EVENT(e5)
BOOST_MSM_EUML_EVENT(e6)
BOOST_MSM_EUML_EVENT(e7)
BOOST_MSM_EUML_EVENT(e8)
BOOST_MSM_EUML_EVENT(e9)
BOOST_MSM_EUML_EVENT(e10)
BOOST_MSM_EUML_EVENT(e11)
BOOST_MSM_EUML_EVENT(e12)
BOOST_MSM_EUML_EVENT(e13)
BOOST_MSM_EUML_EVENT(e14)
BOOST_MSM_EUML_EVENT(e15)
BOOST_MSM_EUML_EVENT(e16)
BOOST_MSM_EUML_EVENT(e17)
BOOST_MSM_EUML_EVENT(e18)
BOOST_MSM_EUML_EVENT(e19)
BOOST_MSM_EUML_EVENT(e20)
BOOST_MSM_EUML_EVENT(e21)
BOOST_MSM_EUML_EVENT(e22)
BOOST_MSM_EUML_EVENT(e23)
BOOST_MSM_EUML_EVENT(e24)
BOOST_MSM_EUML_EVENT(e25)
BOOST_MSM_EUML_EVENT(e26)
BOOST_MSM_EUML_EVENT(e27)
BOOST_MSM_EUML_EVENT(e28)
BOOST_MSM_EUML_EVENT(e29)
BOOST_MSM_EUML_EVENT(e30)
BOOST_MSM_EUML_EVENT(e31)
BOOST_MSM_EUML_EVENT(e32)
BOOST_MSM_EUML_EVENT(e33)
BOOST_MSM_EUML_EVENT(e34)
BOOST_MSM_EUML_EVENT(e35)
BOOST_MSM_EUML_EVENT(e36)
BOOST_MSM_EUML_EVENT(e37)
BOOST_MSM_EUML_EVENT(e38)
BOOST_MSM_EUML_EVENT(e39)
BOOST_MSM_EUML_EVENT(e40)
BOOST_MSM_EUML_EVENT(e41)
BOOST_MSM_EUML_EVENT(e42)
BOOST_MSM_EUML_EVENT(e43)
BOOST_MSM_EUML_EVENT(e44)
BOOST_MSM_EUML_EVENT(e45)
BOOST_MSM_EUML_EVENT(e46)
BOOST_MSM_EUML_EVENT(e47)
BOOST_MSM_EUML_EVENT(e48)
BOOST_MSM_EUML_EVENT(e49)
BOOST_MSM_EUML_EVENT(e50)

BOOST_MSM_EUML_ACTION(guard){
    template <class FSM, class EVT, class SourceState, class TargetState> bool operator()(EVT const &, FSM &, SourceState &,
                                                                                          TargetState &){return true;
}
}
;

BOOST_MSM_EUML_ACTION(action){
    template <class FSM, class EVT, class SourceState, class TargetState> void operator()(EVT const &, FSM &, SourceState &,
                                                                                          TargetState &){}};

// The list of FSM states
BOOST_MSM_EUML_STATE((), idle)
BOOST_MSM_EUML_STATE((), s1)
BOOST_MSM_EUML_STATE((), s2)
BOOST_MSM_EUML_STATE((), s3)
BOOST_MSM_EUML_STATE((), s4)
BOOST_MSM_EUML_STATE((), s5)
BOOST_MSM_EUML_STATE((), s6)
BOOST_MSM_EUML_STATE((), s7)
BOOST_MSM_EUML_STATE((), s8)
BOOST_MSM_EUML_STATE((), s9)
BOOST_MSM_EUML_STATE((), s10)
BOOST_MSM_EUML_STATE((), s11)
BOOST_MSM_EUML_STATE((), s12)
BOOST_MSM_EUML_STATE((), s13)
BOOST_MSM_EUML_STATE((), s14)
BOOST_MSM_EUML_STATE((), s15)
BOOST_MSM_EUML_STATE((), s16)
BOOST_MSM_EUML_STATE((), s17)
BOOST_MSM_EUML_STATE((), s18)
BOOST_MSM_EUML_STATE((), s19)
BOOST_MSM_EUML_STATE((), s20)
BOOST_MSM_EUML_STATE((), s21)
BOOST_MSM_EUML_STATE((), s22)
BOOST_MSM_EUML_STATE((), s23)
BOOST_MSM_EUML_STATE((), s24)
BOOST_MSM_EUML_STATE((), s25)
BOOST_MSM_EUML_STATE((), s26)
BOOST_MSM_EUML_STATE((), s27)
BOOST_MSM_EUML_STATE((), s28)
BOOST_MSM_EUML_STATE((), s29)
BOOST_MSM_EUML_STATE((), s30)
BOOST_MSM_EUML_STATE((), s31)
BOOST_MSM_EUML_STATE((), s32)
BOOST_MSM_EUML_STATE((), s33)
BOOST_MSM_EUML_STATE((), s34)
BOOST_MSM_EUML_STATE((), s35)
BOOST_MSM_EUML_STATE((), s36)
BOOST_MSM_EUML_STATE((), s37)
BOOST_MSM_EUML_STATE((), s38)
BOOST_MSM_EUML_STATE((), s39)
BOOST_MSM_EUML_STATE((), s40)
BOOST_MSM_EUML_STATE((), s41)
BOOST_MSM_EUML_STATE((), s42)
BOOST_MSM_EUML_STATE((), s43)
BOOST_MSM_EUML_STATE((), s44)
BOOST_MSM_EUML_STATE((), s45)
BOOST_MSM_EUML_STATE((), s46)
BOOST_MSM_EUML_STATE((), s47)
BOOST_MSM_EUML_STATE((), s48)
BOOST_MSM_EUML_STATE((), s49)

// clang-format off
BOOST_MSM_EUML_TRANSITION_TABLE(
    (
     s1  == idle + e1 [guard] / action
   , s2  == s1 + e2 [guard] / action
   , s3  == s2 + e3 [guard] / action
   , s4  == s3 + e4 [guard] / action
   , s5  == s4 + e5 [guard] / action
   , s6  == s5 + e6 [guard] / action
   , s7  == s6 + e7 [guard] / action
   , s8  == s7 + e8 [guard] / action
   , s9  == s8 + e9 [guard] / action
   , s10 == s9 + e10 [guard] / action
   , s11 == s10 + e11 [guard] / action
   , s12 == s11 + e12 [guard] / action
   , s13 == s12 + e13 [guard] / action
   , s14 == s13 + e14 [guard] / action
   , s15 == s14 + e15 [guard] / action
   , s16 == s15 + e16 [guard] / action
   , s17 == s16 + e17 [guard] / action
   , s18 == s17 + e18 [guard] / action
   , s19 == s18 + e19 [guard] / action
   , s20 == s19 + e20 [guard] / action
   , s21 == s20 + e21 [guard] / action
   , s22 == s21 + e22 [guard] / action
   , s23 == s22 + e23 [guard] / action
   , s24 == s23 + e24 [guard] / action
   , s25 == s24 + e25 [guard] / action
   , s26 == s25 + e26 [guard] / action
   , s27 == s26 + e27 [guard] / action
   , s28 == s27 + e28 [guard] / action
   , s29 == s28 + e29 [guard] / action
   , s30 == s29 + e30 [guard] / action
   , s31 == s30 + e31 [guard] / action
   , s32 == s31 + e32 [guard] / action
   , s33 == s32 + e33 [guard] / action
   , s34 == s33 + e34 [guard] / action
   , s35 == s34 + e35 [guard] / action
   , s36 == s35 + e36 [guard] / action
   , s37 == s36 + e37 [guard] / action
   , s38 == s37 + e38 [guard] / action
   , s39 == s38 + e39 [guard] / action
   , s40 == s39 + e40 [guard] / action
   , s41 == s40 + e41 [guard] / action
   , s42 == s41 + e42 [guard] / action
   , s43 == s42 + e43 [guard] / action
   , s44 == s43 + e44 [guard] / action
   , s45 == s44 + e45 [guard] / action
   , s46 == s45 + e46 [guard] / action
   , s47 == s46 + e47 [guard] / action
   , s48 == s47 + e48 [guard] / action
   , s49 == s48 + e49 [guard] / action
   , idle == s49 + e50 [guard] / action
     ),
    transition_table)
// clang-format on

BOOST_MSM_EUML_ACTION(Log_No_Transition){template <class FSM, class Event> void operator()(Event const &, FSM &, int){}};

// create a state machine "on the fly"
BOOST_MSM_EUML_DECLARE_STATE_MACHINE((transition_table,                            // STT
                                      init_ << idle,                               // Init State
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
      if (rand() % 2) sm.process_event(e1);
      if (rand() % 2) sm.process_event(e2);
      if (rand() % 2) sm.process_event(e3);
      if (rand() % 2) sm.process_event(e4);
      if (rand() % 2) sm.process_event(e5);
      if (rand() % 2) sm.process_event(e6);
      if (rand() % 2) sm.process_event(e7);
      if (rand() % 2) sm.process_event(e8);
      if (rand() % 2) sm.process_event(e9);
      if (rand() % 2) sm.process_event(e10);
      if (rand() % 2) sm.process_event(e11);
      if (rand() % 2) sm.process_event(e12);
      if (rand() % 2) sm.process_event(e13);
      if (rand() % 2) sm.process_event(e14);
      if (rand() % 2) sm.process_event(e15);
      if (rand() % 2) sm.process_event(e16);
      if (rand() % 2) sm.process_event(e17);
      if (rand() % 2) sm.process_event(e18);
      if (rand() % 2) sm.process_event(e19);
      if (rand() % 2) sm.process_event(e20);
      if (rand() % 2) sm.process_event(e21);
      if (rand() % 2) sm.process_event(e22);
      if (rand() % 2) sm.process_event(e23);
      if (rand() % 2) sm.process_event(e24);
      if (rand() % 2) sm.process_event(e25);
      if (rand() % 2) sm.process_event(e26);
      if (rand() % 2) sm.process_event(e27);
      if (rand() % 2) sm.process_event(e28);
      if (rand() % 2) sm.process_event(e29);
      if (rand() % 2) sm.process_event(e30);
      if (rand() % 2) sm.process_event(e31);
      if (rand() % 2) sm.process_event(e32);
      if (rand() % 2) sm.process_event(e33);
      if (rand() % 2) sm.process_event(e34);
      if (rand() % 2) sm.process_event(e35);
      if (rand() % 2) sm.process_event(e36);
      if (rand() % 2) sm.process_event(e37);
      if (rand() % 2) sm.process_event(e38);
      if (rand() % 2) sm.process_event(e39);
      if (rand() % 2) sm.process_event(e40);
      if (rand() % 2) sm.process_event(e41);
      if (rand() % 2) sm.process_event(e42);
      if (rand() % 2) sm.process_event(e43);
      if (rand() % 2) sm.process_event(e44);
      if (rand() % 2) sm.process_event(e45);
      if (rand() % 2) sm.process_event(e46);
      if (rand() % 2) sm.process_event(e47);
      if (rand() % 2) sm.process_event(e48);
      if (rand() % 2) sm.process_event(e49);
      if (rand() % 2) sm.process_event(e50);
    }
  });
  benchmark_memory_usage(sm);
}
