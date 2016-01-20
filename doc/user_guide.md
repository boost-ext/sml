Documentation
============

* [Boost.MSM - eUML Documentation](http://www.boost.org/doc/libs/1_60_0/libs/msm/doc/HTML/ch03s04.html)

* Boost.MSM - eUML vs msm-lite
```cpp
// Boost.MSM - eUML

#include <boost/msm/back/state_machine.hpp>
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/euml/euml.hpp>

namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace boost::msm::front::euml;

BOOST_MSM_EUML_EVENT(open_close)

BOOST_MSM_EUML_ACTION(open_drawer){
    template <class FSM, class EVT, class SourceState, class TargetState>
    void operator()(EVT const &, FSM &, SourceState &, TargetState &){}
};

BOOST_MSM_EUML_ACTION(close_drawer){
    template <class FSM, class EVT, class SourceState, class TargetState>
    void operator()(EVT const &, FSM &, SourceState &, TargetState &){}
};

BOOST_MSM_EUML_STATE((), Empty)
BOOST_MSM_EUML_STATE((), Open)

BOOST_MSM_EUML_TRANSITION_TABLE(
    (
     Open == Empty + open_close / open_drawer,
     Empty == Open + open_close / close_drawer
    ), transition_table
)

BOOST_MSM_EUML_ACTION(Log_No_Transition){
    template <class FSM, class Event> void operator()(Event const &, FSM &, int state){}};

BOOST_MSM_EUML_DECLARE_STATE_MACHINE((transition_table,                            // STT
                                      init_ << Empty,                              // Init State
                                      no_action,                                   // Entry
                                      no_action,                                   // Exit
                                      attributes_ << no_attributes_,               // Attributes
                                      configure_ << no_exception << no_msg_queue,  // configuration
                                      Log_No_Transition                            // no_transition handler
                                      ),
                                     player_)  // fsm name

int main() {
    msm::back::state_machine<player_> sm;
    sm.process_event(open_close);
    sm.process_event(open_close);
}
```

```cpp
// msm-lite

#include "msm/msm.hpp"

struct open_close {};

auto open_drawer = [] {};
auto close_drawer = [] {};

struct player {
  auto configure() const noexcept {
    using namespace msm;
    return make_transition_table(
        "Empty"_s(initial) == "Open"_s + event<open_close> / open_drawer,
        "Open"_s == "Empty"_s + event<open_close> / close_drawer
    );
  }
};

int main() {
  msm::sm<player> player;
  player.process_event(open_close{});
  player.process_event(open_close{});
}
```

* msm-lite DSL

    | Expression | Description |
    |------------|-------------|
    | src\_state == dst\_state + event<e> [ guard && (![]{return true;} && guard2) ] / (action, action2, []{}) | transition from src\_state to dst\_state on event e with guard and action |
    | src\_state == dst\_state + event<e> [ guard ] / action | transition from src\_state to dst\_state on event e with guard and action |
    | src\_state == dst\_state / [] {} | anonymous transition with action |
    | src\_state == dst\_state + event<e> | transition on event e without guard or action |
    | state + event<e> [ guard ] | internal transition on event e when guard |

* msm-lite data dependencies

```cpp
                             /---- event
                            |
auto guard = [](double d, auto event) { return true; }
                   |
                   \-------\
                           |
auto action = [](int i){}  |
                 |         |
                 |         |
            /---/  /------/
           |      /
sm<...> s{42, 87.0};
```

<a id="configuration"></a>
* Configuration

    Macro                                   | Description
    ----------------------------------------|-----------------------------------------
    MSM\_POLICY\_STATES\_DST\_SRC           | dst\_state == src\_state (same as in eUML)
