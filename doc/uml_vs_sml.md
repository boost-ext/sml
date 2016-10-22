##Unified Modeling Language™ (UML®) Version 2.5

* [http://www.omg.org/spec/UML/2.5](http://www.omg.org/spec/UML/2.5)

---

###Initial Pseudostate

UML ![](images/initial_state.png)

SML

```cpp
*("idle")
```

---

###Terminate Pseudostate
UML ![](images/initial_state.png)

SML

```cpp
X
```

---

###External transition
UML ![](images/external_transition.png)

SML

```cpp
"src_state"_s + event [ guard ] / action = "dst_state"_s
```

> ***where***

```cpp
struct event {};
const auto guard = [] { return true; };
const auto action = [] {};
```

---

###Anonymous transition

UML ![](images/anonymous_transition.png)

SML

```cpp
idle [ guard ] / action = connecting
```

---

###Internal transition

---

###Entry/Exit action

---

###Self transition

---

###Sub/Composite

---

###Orthogonal regions

---

###Deep History

---

###Shallow History

---

###Event defering

---
