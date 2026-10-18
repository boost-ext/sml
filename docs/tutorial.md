# Tutorial

All code examples include `boost/sml.hpp` and declare a convenient `sml`
namespace alias:

```cpp
#include <boost/sml.hpp>
namespace sml = boost::sml;
```

## Read the Boost.MSM - eUML documentation

Background reading:

* [Boost.MSM - UML short guide](https://www.boost.org/doc/libs/1_60_0/libs/msm/doc/HTML/ch02.html)
* [Boost.MSM - eUML documentation](https://www.boost.org/doc/libs/1_60_0/libs/msm/doc/HTML/ch03s04.html)

## Create events and states

A state machine is composed of a finite number of states and transitions which
are triggered by events.

An event is a unique type processed by the state machine:

```cpp
struct my_event { /* ... */ };
```

An event instance can be created to simplify the transition table notation:

```cpp
auto event = sml::event<my_event>;
```

With Clang/GCC, an event can be created on the fly:

```cpp
using namespace sml;
auto event = "event"_e;
```

Such an event does not store any data.

A state can have entry/exit behavior executed when the machine enters or leaves
the state, and represents the current location of the state machine flow:

```cpp
auto idle = sml::state<class idle>;
```

With Clang/GCC, a state can be created on the fly:

```cpp
using namespace sml;
auto state = "idle"_s;
```

Note that the string-literal form is a non-standard extension for Clang/GCC.

SML states are plain types, so a state can hold data. Guards and actions take
the state by reference to read or change it (see the
[Data](examples.md#data) example). Other data is injected directly into
guards and actions.

A state machine can itself be a state:

```cpp
sml::state<state_machine> composite;
```

SML supports a `terminate` state which stops event processing. It is written
as `X`:

```cpp
"state"_s = X;
```

States are printable:

```cpp
assert(std::string("idle") == "idle"_s.c_str());
```

* [Run on Compiler Explorer](https://godbolt.org/z/s816cG5j8)
* [Run on Compiler Explorer](https://godbolt.org/z/c9asGjYTr)
* [Run on Compiler Explorer](https://godbolt.org/z/5sYb9eGbe)

## Create guards and actions

Guards and actions are callables executed by the state machine to decide whether
a transition (and its action) should run.

A guard must return a boolean:

```cpp
auto guard1 = [] {
  return true;
};

auto guard2 = [](int, double) { // guard with dependencies
  return true;
};

auto guard3 = [](int, auto event, double) { // guard with an event and dependencies
  return true;
};

struct guard4 {
  bool operator()() const noexcept {
    return true;
  }
};
```

An action must not return a value:

```cpp
auto action1 = [] { };
auto action2 = [](int, double) { }; // action with dependencies
auto action3 = [](int, auto event, double) { }; // action with an event and dependencies
struct action4 {
  void operator()() noexcept { }
};
```

SML deduces the dependencies and event a guard or action wants from its
`operator()`. A generic lambda (`[](auto&){}`) or a concept-constrained one
hides that information, so wrap it with
[`make_action<Deps...>`](user_guide.md#make_action-utility) to name the types
explicitly:

```cpp
struct Counter { int n = 0; };
auto inc = sml::make_action<Counter&>([](auto& c) { c.n++; }); // generic lambda -> Counter&
```

* [Run on Compiler Explorer](https://godbolt.org/z/nehrdofPx)

## Create a transition table

With states and events available, a transition table describes the transitions.
SML uses an eUML-like DSL close to UML design.

Postfix notation:

| Expression | Description |
| ---------- | ----------- |
| `state + event<e> [ guard ]` | internal transition on event `e` when `guard` |
| `src_state / [] {} = dst_state` | anonymous transition with action |
| `src_state / [] {} = src_state` | self transition (calls `on_exit`/`on_entry`) |
| `src_state + event<e> = dst_state` | external transition on event `e` without guard or action |
| `src_state + event<e> [ guard ] / action = dst_state` | transition from `src_state` to `dst_state` on event `e` with guard and action |
| `src_state + event<e> [ guard && (![]{return true;} && guard2) ] / (action, action2, []{}) = dst_state` | transition with combined guards and actions |

Prefix notation:

| Expression | Description |
| ---------- | ----------- |
| `state + event<e> [ guard ]` | internal transition on event `e` when `guard` |
| `dst_state <= src_state / [] {}` | anonymous transition with action |
| `src_state <= src_state / [] {}` | self transition (calls `on_exit`/`on_entry`) |
| `dst_state <= src_state + event<e>` | external transition on event `e` without guard or action |
| `dst_state <= src_state + event<e> [ guard ] / action` | transition from `src_state` to `dst_state` on event `e` with guard and action |

Transition flow:

```text
src_state + event [ guard ] / action = dst_state
                                     ^
                                     |
                                    1. src_state on_exit
                                    2. dst_state on_entry
```

Use [`make_transition_table`](user_guide.md#make_transition_table-state-machine)
to create a transition table:

```cpp
using namespace sml; // Postfix notation

make_transition_table(
  *"src_state"_s + event<my_event> [ guard ] / action = "dst_state"_s
, "dst_state"_s + "other_event"_e = X
);
```

or:

```cpp
using namespace sml; // Prefix notation

make_transition_table(
   "dst_state"_s <= *"src_state"_s + event<my_event> [ guard ] / action
, X              <= "dst_state"_s  + "other_event"_e
);
```

* [Run on Compiler Explorer](https://godbolt.org/z/qr8qs78Ts)
* [Run on Compiler Explorer](https://godbolt.org/z/n93Y5Mner)

## Set initial states

The initial state tells the state machine where to start. Prefix a state with
`*`:

```cpp
using namespace sml;
make_transition_table(
  *"src_state"_s + event<my_event> [ guard ] / action = "dst_state"_s,
   "dst_state"_s + event<game_over> = X
);
```

The last active state can be remembered so that re-entering a state reactivates
it. Replace `*` with the postfixed `(H)` to enable history:

```cpp
using namespace sml;
make_transition_table(
  "src_state"_s(H) + event<my_event> [ guard ] / action = "dst_state"_s,
  "dst_state"_s    + event<game_over>                   = X
);
```

A machine can have more than one initial state. All initial states run in a
pseudo-parallel way. Such states are called orthogonal regions:

```cpp
using namespace sml;
make_transition_table(
  *"region_1"_s   + event<my_event1> [ guard ] / action = "dst_state1"_s,
   "dst_state1"_s + event<game_over> = X,

  *"region_2"_s   + event<my_event2> [ guard ] / action = "dst_state2"_s,
   "dst_state2"_s + event<game_over> = X
);
```

* [Run on Compiler Explorer](https://godbolt.org/z/vqGbsrqM3)
* [Run on Compiler Explorer](https://godbolt.org/z/Pvz6Y7vd4)

## Create a state machine

A state machine holds the transition table, the current states, and the event
processing logic. First define a type with a transition table:

```cpp
class example {
 public:
  auto operator()() {
    using namespace sml;
    return make_transition_table(
      *"src_state"_s + event<my_event> [ guard ] / action = "dst_state"_s,
       "dst_state"_s + event<game_over> = X
    );
  }
};
```

Then create the state machine:

```cpp
sml::sm<example> sm;
```

The state machine constructor provides the dependencies required by actions and
guards:

```cpp
                            // event (injected from process_event)
                            // |
auto guard = [](double d, auto event) { return true; };
//                  |
//                  +--------+
//                           |
auto action = [](int i) {}   |
//                 |         |
//                 +---+ +---+
//                     | |
sml::sm<example> s{42, 87.0};

sml::sm<example> s{87.0, 42}; // parameter order does not matter
```

When many dependencies are involved, a dependency injection library such as
[Boost.DI](https://github.com/boost-ext/di) can provide them automatically:

```cpp
auto injector = di::make_injector(
    di::bind<>.to(42)
  , di::bind<interface>.to<implementation>()
);

auto sm = injector.create<sm<example>>();
sm.process_event(e1{});
```

* [Run on Compiler Explorer](https://godbolt.org/z/dq85hW4ab)
* [Run on Compiler Explorer](https://godbolt.org/z/1Pv6EaG7n)

## Process events

The main purpose of a state machine is to process events with `process_event`:

```cpp
sml::sm<example> sm;

sm.process_event(my_event{}); // handled
sm.process_event(int{});      // not handled -> unexpected_event<int>
```

Events can also be triggered from within the transition table:

```cpp
using namespace sml;
return make_transition_table(
  *"s1"_s + event<my_event> / process(other_event{}) = "s2"_s,
   "s2"_s + event<other_event> = X
);
```

Events can be deferred and re-evaluated later with `defer`. A transition can
discard everything currently deferred with `clear_defer` (a no-op when nothing
is deferred). This is handy when leaving a composite state so stale deferred
events do not survive re-entry. Both require a `defer_queue` policy:

```cpp
using namespace sml;
return make_transition_table(
  *"s1"_s + event<my_event>  / defer,               // postpone my_event
   state<sub> + event<leave> / clear_defer = "s1"_s // drop anything deferred on the way out
);
```

When events are queued asynchronously - for example an action stores a
`back::process<>` handle that fires after `process_event` has returned - call
`flush_queue()` to drain whatever is still pending (a no-op when the queue is
empty). This requires a `process_queue` policy:

```cpp
sml::sm<example, sml::process_queue<std::queue>> sm{/* ... */};
sm.process_event(e1{}); // action stores a back::process<e2> handle
handle.push(e2{});      // async callback pushes e2 after process_event returned
sm.flush_queue();       // drain: e2 is processed now
```

SML can also dispatch dynamically created (runtime) events:

```cpp
struct game_over {
  static constexpr auto id = SDL_QUIT;
  // explicit game_over(const SDL_Event&) noexcept; // optional
};
enum SDL_EventType { SDL_FIRSTEVENT = 0, SDL_QUIT, SDL_KEYUP, SDL_MOUSEBUTTONUP, SDL_LASTEVENT };
// create dispatcher from state machine and range of events
auto dispatch_event = sml::utility::make_dispatch_table<SDL_Event, SDL_FIRSTEVENT, SDL_LASTEVENT>(sm);
SDL_Event event{SDL_QUIT};
dispatch_event(event, event.type); // will call sm.process(game_over{});
```

* [Run on Compiler Explorer](https://godbolt.org/z/dq85hW4ab)
* [Run on Compiler Explorer](https://godbolt.org/z/j5bMnobY9)
* [Run on Compiler Explorer](https://godbolt.org/z/8heY58eE6)

## Handle errors

When a state machine cannot handle an event, an `unexpected_event` is fired:

```cpp
make_transition_table(
  *"src_state"_s + event<my_event> [ guard ] / action = "dst_state"_s
, "src_state"_s + unexpected_event<some_event> = X
);
```

Any unexpected event can be handled with `unexpected_event<_>`:

```cpp
make_transition_table(
  *"src_state"_s + event<my_event> [ guard ] / action = "dst_state"_s
, "src_state"_s + unexpected_event<some_event> / [] { std::cout << "unexpected 'some_event'\n"; }
, "src_state"_s + unexpected_event<_> = X // any event
);
```

For example:

```cpp
sm.process_event(some_event{}); // prints "unexpected 'some_event'"
sm.process_event(int{});        // terminate
assert(sm.is(X));
```

An orthogonal region is a convenient way to handle unexpected events regardless
of the active state:

```cpp
make_transition_table(
  *"idle"_s + event<my_event> [ guard ] / action = "s1"_s
, "s1"_s + event<other_event> [ guard ] / action = "s2"_s
, "s2"_s + event<yet_another_event> [ guard ] / action = X
// terminate (= X) the machine or reset to another state
,*"error_handler"_s + unexpected_event<some_event> = X
);
```

Check whether the machine is terminated with:

```cpp
assert(sm.is(sml::X)); // independent of the number of regions
```

When exceptions are enabled (the project is not compiled with
`-fno-exceptions`), they can be caught with the `exception<name>` syntax.
Exception handlers run in definition order, and `exception<>` catches anything
(equivalent to `catch (...)`). Without an exception handler in the transition
table, the exception is not handled by the state machine:

```cpp
make_transition_table(
  *"idle"_s + event<event> / [] { throw std::runtime_error{"error"}; }
,*"error_handler"_s + exception<std::runtime_error> = X
, "error_handler"_s + exception<std::logic_error> = X
, "error_handler"_s + exception<> / [] { /* cleanup... */ } = X // any exception
);
```

* [Run on Compiler Explorer](https://godbolt.org/z/cGd7TnM1G)

## Test it

Use `is` or `visit_current_states` to verify the active state, for example to
check for termination:

```cpp
sml::sm<example> sm;
sm.process_event(my_event{});
assert(sm.is(X)); // is(X, s1, ...) with orthogonal regions

// or

sm.visit_current_states([](auto state) { std::cout << state.c_str() << std::endl; });
```

SML also provides testing facilities. `set_current_states` from `testing::sm`
puts the machine into a requested state:

```cpp
sml::sm<example, sml::testing> sm{/* fake data... */};
sm.set_current_states("s3"_s); // set_current_states("s3"_s, "s1"_s, ...) for orthogonal regions
sm.process_event(event{});
assert(sm.is(X));
```

* [Run on Compiler Explorer](https://godbolt.org/z/oc73h5sY8)

## Debug it

SML provides logging capabilities to inspect the state machine flow. Enable
logging with the [logger policy](user_guide.md#policies-state-machine):

```cpp
struct my_logger {
  template <class SM, class TEvent>
  void log_process_event(const TEvent&) {
    printf("[%s][process_event] %s\n", sml::aux::get_type_name<SM>(), sml::aux::get_type_name<TEvent>());
  }

  template <class SM, class TGuard, class TEvent>
  void log_guard(const TGuard&, const TEvent&, bool result) {
    printf("[%s][guard] %s %s %s\n", sml::aux::get_type_name<SM>(), sml::aux::get_type_name<TGuard>(),
           sml::aux::get_type_name<TEvent>(), (result ? "[OK]" : "[Reject]"));
  }

  template <class SM, class TAction, class TEvent>
  void log_action(const TAction&, const TEvent&) {
    printf("[%s][action] %s %s\n", sml::aux::get_type_name<SM>(), sml::aux::get_type_name<TAction>(),
           sml::aux::get_type_name<TEvent>());
  }

  template <class SM, class TSrcState, class TDstState>
  void log_state_change(const TSrcState& src, const TDstState& dst) {
    printf("[%s][transition] %s -> %s\n", sml::aux::get_type_name<SM>(), src.c_str(), dst.c_str());
  }
};

my_logger logger;
sml::sm<logging, sml::logger<my_logger>> sm{logger};
sm.process_event(my_event{}); // will call the logger appropriately
```

* [Run on Compiler Explorer](https://godbolt.org/z/Tec7jz36r)
* [Run on Compiler Explorer](https://godbolt.org/z/8eT6aWsc9)
