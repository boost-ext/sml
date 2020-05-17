//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#define FUSION_MAX_VECTOR_SIZE 50
#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_LIST_SIZE 50
#define BOOST_MPL_LIMIT_STRING_SIZE 64
#define MPLLIBS_LIMIT_STRING_SIZE BOOST_MPL_LIMIT_STRING_SIZE
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/euml2/euml2.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <cstdlib>
#include "benchmark.hpp"

namespace msm = boost::msm;

struct base_state {
  template <class Event, class Fsm>
  void on_entry(Event const&, Fsm&) {}
  template <class Event, class Fsm>
  void on_exit(Event const&, Fsm&) {}
};

struct player_ : msm::front::state_machine_def<player_, base_state> {
  using no_exception_thrown = int;
  using no_message_queue = int;
  using initial_state = BOOST_MSM_EUML2_STATE("idle", player_);

  // clang-format off
  EUML2_STT(
      player_,
      EUML2_STT_CFG(),
      EUML2_ROW("idle + e1  [ guard ] / action -> s1"),
      EUML2_ROW("s1   + e2  [ guard ] / action -> s2"),
      EUML2_ROW("s2   + e3  [ guard ] / action -> s3"),
      EUML2_ROW("s3   + e4  [ guard ] / action -> s4"),
      EUML2_ROW("s4   + e5  [ guard ] / action -> s5"),
      EUML2_ROW("s5   + e6  [ guard ] / action -> s6"),
      EUML2_ROW("s6   + e7  [ guard ] / action -> s7"),
      EUML2_ROW("s7   + e8  [ guard ] / action -> s8"),
      EUML2_ROW("s8   + e9  [ guard ] / action -> s9"),
      EUML2_ROW("s9   + e10 [ guard ] / action -> s10"),
      EUML2_ROW("s10  + e11 [ guard ] / action -> s11"),
      EUML2_ROW("s11  + e12 [ guard ] / action -> s12"),
      EUML2_ROW("s12  + e13 [ guard ] / action -> s13"),
      EUML2_ROW("s13  + e14 [ guard ] / action -> s14"),
      EUML2_ROW("s14  + e15 [ guard ] / action -> s15"),
      EUML2_ROW("s15  + e16 [ guard ] / action -> s16"),
      EUML2_ROW("s16  + e17 [ guard ] / action -> s17"),
      EUML2_ROW("s17  + e18 [ guard ] / action -> s18"),
      EUML2_ROW("s18  + e19 [ guard ] / action -> s19"),
      EUML2_ROW("s19  + e20 [ guard ] / action -> s20"),
      EUML2_ROW("s20  + e21 [ guard ] / action -> s21"),
      EUML2_ROW("s21  + e22 [ guard ] / action -> s22"),
      EUML2_ROW("s22  + e23 [ guard ] / action -> s23"),
      EUML2_ROW("s23  + e24 [ guard ] / action -> s24"),
      EUML2_ROW("s24  + e25 [ guard ] / action -> s25"),
      EUML2_ROW("s25  + e26 [ guard ] / action -> s26"),
      EUML2_ROW("s26  + e27 [ guard ] / action -> s27"),
      EUML2_ROW("s27  + e28 [ guard ] / action -> s28"),
      EUML2_ROW("s28  + e29 [ guard ] / action -> s29"),
      EUML2_ROW("s29  + e30 [ guard ] / action -> s30"),
      EUML2_ROW("s30  + e31 [ guard ] / action -> s31"),
      EUML2_ROW("s31  + e32 [ guard ] / action -> s32"),
      EUML2_ROW("s32  + e33 [ guard ] / action -> s33"),
      EUML2_ROW("s33  + e34 [ guard ] / action -> s34"),
      EUML2_ROW("s34  + e35 [ guard ] / action -> s35"),
      EUML2_ROW("s35  + e36 [ guard ] / action -> s36"),
      EUML2_ROW("s36  + e37 [ guard ] / action -> s37"),
      EUML2_ROW("s37  + e38 [ guard ] / action -> s38"),
      EUML2_ROW("s38  + e39 [ guard ] / action -> s39"),
      EUML2_ROW("s39  + e40 [ guard ] / action -> s40"),
      EUML2_ROW("s40  + e41 [ guard ] / action -> s41"),
      EUML2_ROW("s41  + e42 [ guard ] / action -> s42"),
      EUML2_ROW("s42  + e43 [ guard ] / action -> s43"),
      EUML2_ROW("s43  + e44 [ guard ] / action -> s44"),
      EUML2_ROW("s44  + e45 [ guard ] / action -> s45"),
      EUML2_ROW("s45  + e46 [ guard ] / action -> s46"),
      EUML2_ROW("s46  + e47 [ guard ] / action -> s47"),
      EUML2_ROW("s47  + e48 [ guard ] / action -> s48"),
      EUML2_ROW("s48  + e49 [ guard ] / action -> s49"),
      EUML2_ROW("s49  + e50 [ guard ] / action -> idle")
  )
  // clang-format on

  template <class FSM, class Event>
  void no_transition(Event const&, FSM&, int) {}
};

typedef msm::back::state_machine<player_> player;

template <>
template <class Event, class Fsm, class SourceState, class TargetState>
void BOOST_MSM_EUML2_ACTION_IMPL("action", player_)::operator()(Event const&, Fsm&, SourceState&, TargetState&) {}

template <>
template <class Event, class Fsm, class SourceState, class TargetState>
bool BOOST_MSM_EUML2_GUARD_IMPL("guard", player_)::operator()(Event const&, Fsm&, SourceState&, TargetState&) {
  return true;
}

int main() {
  player sm;
  sm.start();

  benchmark_execution_speed([&] {
    for (auto i = 0; i < 1'000'000; ++i) {
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e1", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e2", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e3", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e4", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e5", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e6", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e7", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e8", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e9", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e10", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e11", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e12", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e13", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e14", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e15", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e16", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e17", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e18", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e19", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e20", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e21", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e22", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e23", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e24", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e25", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e26", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e27", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e28", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e29", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e30", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e31", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e32", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e33", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e34", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e35", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e36", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e37", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e38", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e39", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e40", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e41", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e42", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e43", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e44", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e45", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e46", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e47", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e48", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e49", player_)());
      if (rand() % 2) sm.process_event(BOOST_MSM_EUML2_EVENT("e50", player_)());
    }
  });
  benchmark_memory_usage(sm);
}
