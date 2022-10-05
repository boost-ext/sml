msm="""
#define FUSION_MAX_VECTOR_SIZE 50
#define BOOST_MPL_LIMIT_VECTOR_SIZE 50
#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/euml/euml.hpp>
#include <boost/msm/front/euml/stl.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <cstdlib>

namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace boost::msm::front::euml;

// events
%events

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
%states

// clang-format off
BOOST_MSM_EUML_TRANSITION_TABLE(
    (
     %transitions
     ),
    transition_table)
// clang-format on

BOOST_MSM_EUML_ACTION(Log_No_Transition){template <class FSM, class Event> void operator()(Event const &, FSM &, int){}};

// create a state machine "on the fly"
BOOST_MSM_EUML_DECLARE_STATE_MACHINE((transition_table,                            // STT
                                      init_ << s0,                               // Init State
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
  %process_events
}
"""
sml_rs="""
#![feature(specialization)]
#![allow(incomplete_features)]

#[macro_use]
mod sml;
use sml::HList;
use sml::ProcessEvent;

%events

fn main() {
  fn guard<TEvent>(_:&TEvent) -> bool { true }
  fn action<TEvent>(_:&TEvent) { }

  let transitions = transition_table!{%transitions};

  let mut sm = sml::StateMachine::new(transitions);
  %process_events
}
"""

for i in range(0, 500):
    with open("sml.lite"+str(i) +".cpp", "w") as f:
        f.write("#include <sml>\n")
        for x in range(0, i):
            f.write("struct e" + str(x) + "{};\n")
        f.write("struct test {\n");
        f.write("auto operator()() {\n");
        f.write("auto guard = [] { return true; };\n")
        f.write("auto action = [] {};\n")
        f.write("using namespace boost::sml::dsl;\n");
        f.write("return transition_table{\n");
        if i > 0:
            f.write("*\"" + str(0) + "\"_s + event<e" + str(0) + "> [ guard ] / action = \"" + str(0+1) + "\"_s,\n")
        for x in range(1, i):
            f.write("\"" + str(x) + "\"_s + event<e" + str(x) + "> [ guard ] / action = \"" + str(x+1) + "\"_s,\n")
        f.write("};\n");
        f.write("}\n");
        f.write("};\n");
        f.write("int main() {\n");
        f.write("boost::sml::sm sm{test{}};\n");
        for x in range(0, i):
            f.write("sm.process_event(e" + str(x) + "{});\n")
        f.write("}\n");

    with open("sml"+str(i) +".cpp", "w") as f:
        f.write("#include <boost/sml.hpp>\n")
        for x in range(0, i):
            f.write("struct e" + str(x) + "{};\n")
        f.write("struct test {\n");
        f.write("auto operator()() {\n");
        f.write("auto guard = [] { return true; };\n")
        f.write("auto action = [] {};\n")
        f.write("using namespace boost::sml;\n");
        f.write("return make_transition_table(\n");
        if i > 0:
            f.write("*\"" + str(0) + "\"_s + event<e" + str(0) + "> [ guard ] / action = \"" + str(0+1) + "\"_s\n")
        for x in range(1, i):
            if x < i and i > 1: f.write(',')
            f.write("\"" + str(x) + "\"_s + event<e" + str(x) + "> [ guard ] / action = \"" + str(x+1) + "\"_s\n")
        f.write(");\n");
        f.write("}\n");
        f.write("};\n");
        f.write("int main() {\n");
        f.write("boost::sml::sm<test> sm{};\n");
        for x in range(0, i):
            f.write("sm.process_event(e" + str(x) + "{});\n")
        f.write("}\n");

    with open("msm"+str(i) +".cpp", "w") as f:
        states = ''
        events = ''
        transitions = ''
        process_events = ''
        for x in range(0, i+1):
            states += "BOOST_MSM_EUML_STATE((), s" + str(x) + ")\n"
        for x in range(0, i):
            events += "BOOST_MSM_EUML_EVENT(e" + str(x) + ")\n"
            transitions += "s" + str(x+1) + "  == s" + str(x) + "+ e" + str(x) + " ,\n"
            process_events += "sm.process_event(e" + str(x) + ");\n"
        f.write(msm.replace("%events", events).replace("%states", states).replace("%transitions", transitions[:-2]).replace("%process_events", process_events))

    with open("sml"+str(i) +".rs", "w") as f:
        states = ''
        events = ''
        transitions = ''
        process_events = ''
        ga = ''
        for x in range(0, i):
            events += "struct E" + str(x) + "{}\n"
            transitions += "S" + str(x) + " + E" + str(x) + " [guard] / (action) = S" + str(x+1) + ",\n"
            process_events += "sm.process_event(&E" + str(x) + "{});\n"
        f.write(sml_rs.replace("%events", events).replace("%transitions", transitions).replace("%process_events", process_events))
