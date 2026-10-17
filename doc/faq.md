# FAQ

## Limitations

### `on_entry<_>` across translation units

When `on_entry<_>` (the wildcard entry handler) is defined in one translation unit
while the `initial`-event specialization is left undefined, the linker reports an
undefined reference. Make sure the entry handler is fully defined where it is used.

Dispatch priority: a specific `on_entry<e1>` handler takes priority over the
`on_entry<_>` wildcard. The wildcard fires only when no specific handler matches the
event used to enter the state.

```cpp
// on_entry<e1> fires when entering via e1 (specific handler wins)
// on_entry<_>  fires when entering via any other event (no specific handler)
```

### `operator,` with two raw member-function-pointer actions

Two raw member-function-pointer actions combined with the comma operator
(`&A::f, &A::g`) select the built-in comma operator, because both operands are
class types — so SML's action-sequencing is bypassed. Wrap the actions with
`wrap(...)` to restore sequencing:

```cpp
*"s1"_s + event<e> / (wrap(&A::f), wrap(&A::g)) = X
```

See `example/actions_guards.cpp` for a worked example.

## Frequently asked questions

### How do I get / inspect the current state(s)?

Use `sm.is(...)` to test the active state, or `sm.visit_current_states(...)` to
iterate them. `state.c_str()` gives the state name:

```cpp
assert(sm.is("idle"_s));
sm.visit_current_states([](auto state) { std::cout << state.c_str() << '\n'; });
```

### How do I check whether the machine is in one of several states?

`is(...)` accepts one state per orthogonal region, so all regions are tested in a
single call (no need to release a held mutex between checks):

```cpp
sm.is(s1, s2);  // region 0 == s1 AND region 1 == s2
```

### How do I know whether an event was handled? Can I pass an event instance?

`process_event` returns `bool` — `true` when the event was handled, `false`
otherwise (an unhandled event triggers `unexpected_event<E>`). You can pass either a
type or an instance:

```cpp
const bool handled = sm.process_event(my_event{42});
```

### How do I dispatch events from another thread or an async handler?

Configure the machine with a `process_queue` / `defer_queue` policy and call
`sm.flush_queue()` to drain events that were pushed after `process_event` returned
(for example from an asynchronous callback). `flush_queue()` is a no-op when empty.

### How do I discard deferred events?

Use the `sml::clear_defer` action in a transition to drop events that were deferred
earlier.

### Is SML part of Boost?

`[Boost].SML` is a standalone, single-header library with **no dependencies** —
neither STL nor Boost are required. It is part of the `boost-ext` project; it is not
(currently) an official Boost library, and you do not need Boost to use it.

### Is SML a Moore or a Mealy state machine?

Both. Transition actions run on transitions (Mealy), while `on_entry<...>` /
`on_exit<...>` provide per-state actions (Moore).

### Can a state hold data?

Yes. A state is a plain type, take it by reference in a guard or action to read or
change it. `example/data.cpp` keeps an `id` in the `Connected` and `Interrupted`
states and copies it between them:

```cpp
const auto set = [](const auto& event, Connected& state) { state.id = event.id; };
const auto update = [](Connected& src_state, Interrupted& dst_state) { dst_state.id = src_state.id; };
```

### Is a state machine thread safe? Can I call `process_event` from an action?

Not by default. Pass the `thread_safe` policy with a lock type. With
`std::recursive_mutex`, `process_event` can be called from inside an action:

```cpp
sml::sm<example, sml::thread_safe<std::recursive_mutex>> sm;
```

### How do I handle any event (wildcard)?

Use both `event<_>` and `unexpected_event<_>`. `"s"_s + event<_> / action` matches every event type that appears somewhere in the table, `"s"_s + unexpected_event<_> / action` the types that do not. A specific `event<e>` row on the same state has priority over the wildcard.

### Why doesn't `unexpected_event<_>` catch an event that is in the table?

`unexpected_event<_>` matches only event types that appear nowhere in the table. A table event with no transition in the current state is dropped and `process_event` returns `false`. To react to it add `unexpected_event<that_event>`, or check the returned `bool`.

### How do I handle an event in every state, with a default action?

Use `state<_>` as the source. It applies in every state with no transition of its own for the event, so a specific row such as `"s2"_s + event<e> / specific` wins there and the default does not run:

```cpp
"s2"_s + event<e> / specific,
state<_> + event<e> / default_action
```

### Why is my `"name"_e` event not handled?

`"name"_e` is the event token for tables. To send it, create an instance with parentheses: `sm.process_event("name"_e())`. Without them it compiles, nothing matches and `process_event` returns `false`.

### How do I check the state of a sub-state-machine?

Name the sub machine's state type in the first template argument of `is`:

```cpp
sm.is<decltype(sml::state<sub>)>(sml::state<sub_state>);
```

`sm.is(sml::state<sub>)` only says the parent is inside `sub`. For "any of these states" (what MSM calls a flag) combine `is` calls:

```cpp
template <class SM, class... S>
bool is_any(const SM& sm, S... s) { return (sm.is(s) || ...); }
```

### How do I enter a composite state in a specific sub-state?

There is no direct entry: `state<Composite::Sub2>` is treated as a state of the parent and skips the composite's `on_entry`. Transition to `state<Composite>` and give the composite an initial dispatch state with guarded anonymous transitions to the sub-states, driven by a dependency the parent sets first.

### How do I reuse a base transition table in several machines?

`make_transition_table` takes transitions, not other tables. Return the shared transitions in a `std::tuple` and unpack them with `std::apply` (C++17):

```cpp
struct base { static constexpr auto transitions() { return std::make_tuple(*"a"_s + event<e> = "b"_s); } };
struct derived { auto operator()() const { return std::apply([](auto... t) {
  return sml::make_transition_table(t..., "b"_s + event<f> = "c"_s); }, base::transitions()); } };
```

This is reuse, a base transition cannot be overridden.

### Can I build the transition table conditionally or in pieces?

Not at runtime and not by concatenating tables, the table is a type. Select a whole table with `if constexpr` on a template parameter, or use guards for runtime conditions. For the same state and event, transitions are tried in written order and the first one whose guard passes runs.

### Can I ask whether an event would be handled without processing it?

Not directly, there is no `can_process_event`. Processing a copy (`auto probe = sm; probe.process_event(e)`) answers it including guards, but actions run and dependencies passed by reference are shared with the copy. Use it only when actions have no outside side effects.

### How do I ignore events while an action is in progress?

Make the in-progress period a state with one way out: `idle + event<start> / begin_work = busy` and `busy + event<work_done> = idle`. Events with no transition in `busy` are dropped. Send `work_done` when the work completes, or queue it from the action with `process(work_done{})` and a `process_queue` policy.

### How do I process a new event built from the incoming one?

Use an action taking `back::process<E>` (needs a `process_queue` policy):

```cpp
event<raw> / [](const raw& e, back::process<converted> process) { process(converted{e.value * 2}); }
```

### How do errors reach the caller of `process_event`?

`process_event` returns only `bool`. With no `exception<>` transition in the table, exceptions propagate to the caller. With any `exception<>` transition, unmatched exceptions are swallowed. To report one, store it in a dependency:

```cpp
"s"_s + exception<_> / [](std::exception_ptr& e) { e = std::current_exception(); } = "idle"_s
```

Without exceptions, report an error from a sub-state by firing an event from the action with `process(request_failed{})` (needs a `process_queue` policy). Events the sub machine does not handle fall through to the parent.

### How do I read my context in a logger? Why is the initial substate not logged?

A logger is a dependency like any other: give it a reference to your context and pass both to the machine, `sml::sm<top, sml::logger<my_logger>> sm{logger, ctx}`. `log_state_change` gets only name holders (`c_str()`), not state objects, and runs on transitions only, before the destination's `on_entry`. The initial state of a sub machine is entered without a transition, log it from an `on_entry<_>` action on that state.

### How do I see the current state in a debugger?

The sm object holds template internals, not names. Track the name with a logger policy and watch the variable:

```cpp
template <class SM, class TSrc, class TDst>
void log_state_change(const TSrc&, const TDst& dst) { current_state_name = dst.c_str(); }
```

The logger must define all four `log_*` members and be passed as an lvalue to the `sm` constructor.

### Why does `wrap(&Class::method)` fail with "cannot bind non-const lvalue reference"?

A member-function pointer makes `Class` a dependency of the machine and dependencies are held by reference. Pass an lvalue of `Class` to the `sm` constructor, a temporary will not bind.

### Why can't I pass a temporary dependency to the `sm` constructor?

Dependencies are held by reference, so `sm{logger{}}` does not compile when an action or the logger takes `logger&`. Pass an lvalue that outlives the machine, or define `BOOST_SML_CREATE_DEFAULT_CONSTRUCTIBLE_DEPS` to make the machine keep its own copy of a default-constructible dependency.
