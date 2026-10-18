# UML vs SML

Reference: [UML 2.5 specification](https://www.omg.org/spec/UML/2.5).

## Initial pseudostate

An entry point where everything starts from.

| UML | SML |
| --- | --- |
| ![Initial state](images/initial_state.png) | `* "idle"_s` |

## Terminate pseudostate

A state which terminates the state machine.

| UML | SML |
| --- | --- |
| ![Terminate state](images/terminate_state.png) | `sml::X` |

## External transition

Conditionally updates the current state.

| UML | SML |
| --- | --- |
| ![External transition](images/external_transition.png) | `"src_state"_s + event [ guard ] / action = "dst_state"_s` |

## Anonymous transition

A transition without a trigger (event).

| UML | SML |
| --- | --- |
| ![Anonymous transition](images/anonymous_transition.png) | `"src_state"_s = "dst_state"_s` |
