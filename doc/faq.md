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
