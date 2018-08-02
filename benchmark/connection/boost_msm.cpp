#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/euml/euml.hpp>

namespace msm = boost::msm;
using namespace boost::msm::front::euml;

BOOST_MSM_EUML_EVENT(connect)
BOOST_MSM_EUML_EVENT(ping)
BOOST_MSM_EUML_EVENT(established)
BOOST_MSM_EUML_EVENT(timeout)
BOOST_MSM_EUML_EVENT(disconnect)

BOOST_MSM_EUML_ACTION(establish){
  template <class FSM, class EVT, class SourceState, class TargetState>
  void operator()(EVT const &, FSM &, SourceState &, TargetState &) {
    std::puts("establish");
  }
};

BOOST_MSM_EUML_ACTION(resetTimeout){
  template <class FSM, class EVT, class SourceState, class TargetState>
  void operator()(EVT const &, FSM &, SourceState &, TargetState &) {
    std::puts("resetTimeout");
  }
};

BOOST_MSM_EUML_ACTION(close_) {
  template <class FSM, class EVT, class SourceState, class TargetState>
  void operator()(EVT const &, FSM &, SourceState &, TargetState &) {
    std::puts("close");
  }
};

BOOST_MSM_EUML_ACTION(is_valid) {
  template <class FSM, class EVT, class SourceState, class TargetState>
  bool operator()(EVT const &, FSM &, SourceState &, TargetState &) {
    return true;
  }
};

BOOST_MSM_EUML_STATE((), Disconnected)
BOOST_MSM_EUML_STATE((), Connecting)
BOOST_MSM_EUML_STATE((), Connected)

// clang-format off
BOOST_MSM_EUML_TRANSITION_TABLE((
  Connecting   == Disconnected + connect / establish,
  Connected    == Connecting   + established,
  Connected                    + ping [ is_valid ] / resetTimeout,
  Connecting   == Connected    + timeout / establish,
  Disconnected == Connected    + disconnect / close_
), transition_table)
// clang-format on

BOOST_MSM_EUML_ACTION(Log_No_Transition) {
  template <class FSM, class Event>
  void operator()(Event const &, FSM &, int) { }
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

int main() {
  Connection connection;
  connection.start();

  connection.process_event(connect);
  connection.process_event(established);
  connection.process_event(ping);
  connection.process_event(disconnect);
  connection.process_event(connect);
  connection.process_event(established);
  connection.process_event(ping);
}
