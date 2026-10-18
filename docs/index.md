# Introduction

**[Boost::ext].SML (State Machine Language)** is a scalable C++14 single-header
state machine library with no dependencies.

[Try it online](examples.md#hello-world) |
[Download the latest header](https://github.com/boost-ext/sml/releases) |
[Tutorial](tutorial.md) |
[Examples](examples.md)

## UML state machine

Background reading:

* [UML state machine](https://en.wikipedia.org/wiki/UML_state_machine)
* [UML 2.5 specification](https://www.omg.org/spec/UML/2.5)
* [Boost.MSM - eUML](https://www.boost.org/doc/libs/1_60_0/libs/msm/doc/HTML/ch03s04.html)

## Do I need a state machine?

The state machine pattern keeps conditional logic explicit instead of letting it
spread into nested `if`/`else` chains:

```cpp
void some_function() {
  // ...
  if ((is_running && !is_jumping) || just_started) {
    // ...
  } else if (is_boss_level && extra_feature_enabled && !ab_test) {
    // ...
  } else {
    // ...
  }
}
```

If code like this looks familiar, SML may suit your project.

## Why SML?

[Boost.MSM - eUML](https://www.boost.org/doc/libs/1_60_0/libs/msm/doc/HTML/ch03s04.html)
is expressive but has practical limitations on large projects:

* long compilation times and large binaries (see [Overview](overview.md))
* macro-heavy interface and long error messages
* boilerplate-heavy actions and guards (fsm, event, source state, target state)
* hard to share state data and to achieve loosely coupled designs
* many Boost dependencies

### Design goals

Keep the good parts of Boost.MSM - eUML:

* performance and low memory usage (see [Overview](overview.md))
* eUML-like DSL: `src_state + event [ guard ] / action = dst_state`
* UML compliant as much as possible

Eliminate the pain points:

* fast compilation times and small binaries
* no macros, short and informative error messages
* minimal boilerplate (see [Hello World](examples.md#hello-world))
* all actions visible on the transition table (see [States](examples.md#states))
* loosely coupled designs (see [Dependency Injection](examples.md#dependency-injection))
* functional programming with lambdas (see [Actions and Guards](examples.md#actions-and-guards))
* no dependencies, single header (~2k LOC)

Lightweight means: fast to compile, fast at runtime, no dependencies, and easy
to use.

## Supported UML features

* Transitions, anonymous transitions, internal transitions, self transitions
  (see [Transitions](examples.md#transitions) and [Events](examples.md#events))
* Actions and guards (see [Actions and Guards](examples.md#actions-and-guards))
* State entry and exit actions (see [States](examples.md#states))
* Orthogonal regions (see [Orthogonal Regions](examples.md#orthogonal-regions))
* Sub and composite state machines (see [Composite](examples.md#composite))
* History (see [History](examples.md#history))
* Defer and process (see [Defer and Process](examples.md#defer-and-process))

## Additional features

* Logging (see [Logging](examples.md#logging))
* Testing (see [Testing](examples.md#testing))
* Runtime dispatcher (see [Runtime Dispatcher](examples.md#runtime-dispatcher))
* Dependency injection (see [Dependency Injection](examples.md#dependency-injection))

## Acknowledgements

* Thanks to [Christophe Henry](https://github.com/henry-ch) for
  [Boost.MSM - eUML](https://www.boost.org/doc/libs/1_60_0/libs/msm/doc/HTML/ch03s04.html).
* Thanks to [Deniz Bahadir](https://github.com/Bagira80) for contributions and support.
* Thanks to [Takatoshi Kondo](https://github.com/redboltz) for testing and improvements.
* Thanks to [Jean Davy](https://github.com/splitsen) for CMake support.
* Thanks to [Pavel Guzenfeld](https://github.com/PavelGuzenfeld) for the v1.2.0 release and bug fixes.
* Thanks to all [contributors](https://github.com/boost-ext/sml/graphs/contributors).
