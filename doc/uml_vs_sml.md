##Unified Modeling Language™ (UML®) Version 2.5

* [http://www.omg.org/spec/UML/2.5](http://www.omg.org/spec/UML/2.5)

---

###Initial Pseudostate

* An entry point where everything starts from

| UML | SML |
| - | - |
| ![](images/initial_state.png) | `* "idle"_s` |

---

###Terminate Pseudostate

* A state which terminates the state machine (there is no escape from)

| UML | SML |
| - | - |
| ![](images/terminate_state.png) | `sml::X`

---

###External transition

* Conditionally updates the current state

| UML | SML |
| - | - |
| ![](images/external_transition.png) | `"src_state"_s + event [ guard ] / action = "dst_state"_s`

---

###Anonymous transition

* transition without a trigger (event)

| UML | SML |
| - | - |
| ![](images/anonymous_transition.png) | `"src_state"_s = "dst_state"_s` |

---

<!--###Internal transition-->
<!--###Entry/Exit action-->
<!--###Self transition-->
<!--###Sub/Composite-->
<!--###Orthogonal regions-->
<!--###Deep History-->
<!--###Shallow History-->
<!--###Event defering-->
