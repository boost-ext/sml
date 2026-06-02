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
