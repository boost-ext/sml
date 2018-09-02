#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/euml/euml.hpp>

namespace msm = boost::msm;
using namespace boost::msm::front::euml;

BOOST_MSM_EUML_EVENT(connect)
BOOST_MSM_EUML_EVENT(ping)
BOOST_MSM_EUML_EVENT(established)
BOOST_MSM_EUML_EVENT(timeout)
BOOST_MSM_EUML_EVENT(disconnect)

#if defined(TEST_PERF) or defined(TEST_GBENCH)
  static void clobber() { asm volatile("" : : : "memory"); }
  BOOST_MSM_EUML_ACTION(establish){
    template <class FSM, class EVT, class SourceState, class TargetState>
    void operator()(EVT const&, FSM&, SourceState&, TargetState&) {
      clobber();
    }
  };

  BOOST_MSM_EUML_ACTION(reset_timeout){
    template <class FSM, class EVT, class SourceState, class TargetState>
    void operator()(EVT const&, FSM&, SourceState&, TargetState&) {
      clobber();
    }
  };

  BOOST_MSM_EUML_ACTION(close_) {
    template <class FSM, class EVT, class SourceState, class TargetState>
    void operator()(EVT const&, FSM&, SourceState&, TargetState&) {
      clobber();
    }
  };

  BOOST_MSM_EUML_ACTION(is_valid) {
    template <class FSM, class EVT, class SourceState, class TargetState>
    auto operator()(EVT const&, FSM&, SourceState&, TargetState&) {
      clobber();
      return true;
    }
  };
#else
  #include <cstdio>

  BOOST_MSM_EUML_ACTION(establish){
    template <class FSM, class EVT, class SourceState, class TargetState>
    void operator()(EVT const&, FSM&, SourceState&, TargetState&) {
      std::puts("establish");
    }
  };

  BOOST_MSM_EUML_ACTION(reset_timeout){
    template <class FSM, class EVT, class SourceState, class TargetState>
    void operator()(EVT const&, FSM&, SourceState&, TargetState&) {
      std::puts("reset_timeout");
    }
  };

  BOOST_MSM_EUML_ACTION(close_) {
    template <class FSM, class EVT, class SourceState, class TargetState>
    void operator()(EVT const&, FSM&, SourceState&, TargetState&) {
      std::puts("close");
    }
  };

  BOOST_MSM_EUML_ACTION(is_valid) {
    template <class FSM, class EVT, class SourceState, class TargetState>
    auto operator()(EVT const&, FSM&, SourceState&, TargetState&) {
      return true;
    }
  };
#endif

BOOST_MSM_EUML_STATE((), Disconnected)
BOOST_MSM_EUML_STATE((), Connecting)
BOOST_MSM_EUML_STATE((), Connected)

// clang-format off
BOOST_MSM_EUML_TRANSITION_TABLE((
  Connecting   == Disconnected + connect / establish,
  Connected    == Connecting   + established,
  Connected                    + ping [ is_valid ] / reset_timeout,
  Connecting   == Connected    + timeout / establish,
  Disconnected == Connected    + disconnect / close_
), transition_table)
// clang-format on

BOOST_MSM_EUML_ACTION(Log_No_Transition) {
  template <class FSM, class Event>
  void operator()(Event const&, FSM&, int) { }
};

BOOST_MSM_EUML_DECLARE_STATE_MACHINE((
  transition_table,
  init_ << Disconnected,
  no_action,
  no_action,
  attributes_ << no_attributes_,
  configure_ << no_exception << no_msg_queue,
  Log_No_Transition
), ConnectionImpl)

using Connection = msm::back::state_machine<ConnectionImpl>;

#if defined(TEST_ASM)
  int main() {
    Connection connection{};
    connection.start();

    connection.process_event(connect);
    connection.process_event(established);
    connection.process_event(ping);
    connection.process_event(disconnect);
    connection.process_event(connect);
    connection.process_event(established);
    connection.process_event(ping);
  }
#elif defined(TEST_ASM_GENERIC)
  #include <cstdlib>

  int main() {
    Connection connection{};
    connection.start();

    while(true) {
      switch(rand() % 5) {
        default: break;
        case 0: connection.process_event(connect); break;
        case 1: connection.process_event(established); break;
        case 2: connection.process_event(ping); break;
        case 3: connection.process_event(disconnect); break;
        case 4: connection.process_event(timeout); break;
      }
    }
  }
#elif defined(TEST_PERF)
  int main() {
    Connection connection{};
    connection.start();

    for (auto i = 0; i < 100'000'000; ++i) {
      connection.process_event(connect);
      connection.process_event(established);
      connection.process_event(ping);
      connection.process_event(disconnect);
    }
  }
#elif defined(TEST_GBENCH)
  #include <benchmark/benchmark.h>

  static void BM_boost_msm(benchmark::State& state) {
    constexpr auto size = 1'000'000;

    int dispatch[size]{};
    for (auto i = 0; i < size; ++i) {
      dispatch[i] = rand() % 4;
    }

    Connection connection;
    connection.start();

    auto i = 0;
    for (auto _ : state) {
      switch(dispatch[i++]) {
        default: break;
        case 0: connection.process_event(connect); break;
        case 1: connection.process_event(established); break;
        case 2: connection.process_event(ping); break;
        case 3: connection.process_event(disconnect); break;
        case 4: connection.process_event(timeout); break;
      }
      if (i++ >= size) i = {};
    }
  }

  BENCHMARK(BM_boost_msm);
  BENCHMARK_MAIN();
#endif
