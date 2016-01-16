<a href="http://www.boost.org/LICENSE_1_0.txt" target="_blank">![Boost Licence](http://img.shields.io/badge/license-boost-blue.svg)</a>
<a href="https://github.com/krzysztof-jusiak/msm-lite/releases" target="_blank">![Version](https://badge.fury.io/gh/krzysztof-jusiak%2Fmsm-lite.svg)</a>
<a href="https://github.com/krzysztof-jusiak/msm-lite/releases/latest" target="_blank">![Github Release](http://img.shields.io/github/release/krzysztof-jusiak/msm-lite.svg)</a>
<a href="https://travis-ci.org/krzysztof-jusiak/msm-lite" target="_blank">![Build Status](https://img.shields.io/travis/krzysztof-jusiak/msm-lite/cpp14.svg?label=linux/osx)</a>
<a href="http://github.com/krzysztof-jusiak/msm-lite/issues" target="_blank">![Github Issues](https://img.shields.io/github/issues/krzysztof-jusiak/msm-lite.svg)</a>
<a href="https://gitter.im/krzysztof-jusiak/msm-lite" target="_blank">![Gitter Chat](https://img.shields.io/badge/gitter-join%20chat%20%E2%86%92-brightgreen.svg)</a>
<a href="http://waffle.io/krzysztof-jusiak/msm-lite" target="_blank">![Stories in Ready](https://badge.waffle.io/krzysztof-jusiak/msm-lite.svg?label=ready&title=Ready)</a>

msm-lite: C++14 Meta State Machine Library
===============================================
Yours scalable C++14 one header(1k lines) eUML-like meta state machine library with no dependencies

> **Introduction**

* [UML](http://www.uml.org)
* [UML2 Specification - State Machines](http://www.omg.org/spec/UML/2.5)
* [Boost.MSM - UML Short Guide](http://www.boost.org/doc/libs/1_60_0/libs/msm/doc/HTML/ch02.html)
* [Boost.MSM - eUML](http://www.boost.org/doc/libs/1_60_0/libs/msm/doc/HTML/ch03s04.html)

> **Why msm-lite?**

* Boost.MSM - eUML is awesome, however it has a few huge limitations which stop it from being used it on a larger scale;
  msm-lite, therefore, is trying to address those issues.

> **Problems with Boost.MSM - eUML**
    
* Horrible compilation times (see Benchmarks)
* Produces huge binaries (see Benchmarks)
* Based on too many macros
* Horrible and long error messages
* Sometimes hard to follow as not all actions might be seen on transition table (ex. initial states, on\_entry, on\_exit)
* A lot of boilerplate code with actions/guards (requires fsm, event, source state, target state)
* Data in states makes it harder share/encapsulate (UML compliant though)
* Functional programming emulation (introduced before lambda expressions)
* Huge complexity may overwhelm in the beginning
* A lot of Boost dependencies

> **msm-lite design goals**

* Keep the Boost.MSM - eUML goodies

    * Performance (see Benchmarks)
    * Memory usage (see Benchmarks)
    * eUML DSL (s1 == s2 + event [ guard ] / action)
    * UML standard compliant (As much as possible)
    
* Eliminate Boost.MSM - eUML problems

    * Compilation times (see Benchmarls)
    * Binary size (see Benchmarks)
    * Reduce complexity by eliminating less used features
    * Short and informative error messages (see Error Messages)
    * Less boilerplate / no macros
    * Improve visibility by having all actions on transition table
    * No dependencies / one header
    * Functional programming support using lamda expressions

* Add new functionality
 
   * Dependency injection support for guards/actions (see DI)
   * Logging support (TBD)
   * Testing support (TBD)

> **What 'lite' implies?**

* Minimal learning curve
* Only crucial features
* Guaranteed performance and quick compilation times
* No dependencies

> **Supported features by msm-lite**

* Transitions / internal transitions / anonymous transitions / no transition (see Example)
* Guards / actions (see Example)
* State entry / exit actions (see Example)
* Orthogonal regions (see Example)
* Sub/Composite state machines (see Example)
* Custom flags (see Example)
* Dispatcher (see Example)
* Visit current states (see Example)
* Proposed boost.di integration (see Example)
 
> **How to start**

* Get msm.hpp header
```sh
    wget https://raw.githubusercontent.com/krzysztof-jusiak/msm-lite/master/include/msm/msm.hpp
```

* Include the header
```cpp
    #include "msm.hpp"
```

* Compile with C++14 support
```sh
    $CXX -std=c++14 ...
```

> **Supported/tested compilers**

* Clang-3.4+
* GCC-5.2+

> **Hello world example**

```cpp
#include "msm.hpp"

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};

auto guard1 = [] { return true; };
auto guard2 = [](int i, auto event) {
    return i == 42 && is_same<decltype(event), e1>::value;
};
auto action1 = [] { };
auto action2 = [](const auto& event, int i) { 
    cout << i << " " << typeid(event).name() << endl;
};

class hello_world {
public:
  auto configure() const noexcept {
    using namespace msm; // bring operators support
    return make_transition_table(
        "idle"_s(initial) == "s1"_s + event<e1> [ guard2 ] / action1
      , "s1"_s == "s2"_s + event<e2> / [] { cout << "in place action" << endl; }
      , "s2"_s == "s3"_s + event<e3> / [ guard1 && !guard2 ] / (action1, action2)
      , "s2"_s == "game_over"_s + event<e4>
      , 
    );
  }
};

int main() {
  msm::sm<hello_world> sm{42/*int dependency to be injected into guards/actions*/}
  assert(sm.process_event(e1{}));
  assert(sm.process_event(e2{}));
  assert(sm.process_event(e3{}));
  assert(sm.process_event(e4{}));
  sm.visit_current_states([](auto state){
    static_assert(is_same<decltype(state), "game_over"_s>::value);}
  );
}
```
> **User Guide**

* [Boost.MSM - eUML Documentation](http://www.boost.org/doc/libs/1_60_0/libs/msm/doc/HTML/ch03s04.html)

* Boost.MSM - eUML vs msm-lite

| Events           |          |
|------------------|----------|
| Boost.MSM - eUML | msm-lite |
|------------------|----------|

* DSL introduction

| Expression | Description |
|------------|-------------|
| src\_state == dst\_state + event<e> [ guard && (![]{return true;} && guard2) ] / (action, action2, []{}) | transition from src\_state to dst\_state on event e with guard and action |
| src\_state == dst\_state + event<e> [ guard ] / action | transition from src\_state to dst\_state on event e with guard and action |
| src\_state == dst\_state / [] {} | anonymous transition with action |
| src\_state == dst\_state + event<e> | transition on event e without guard or action |
| state + event<e> [ guard ] | internal transition on event e when guard |

* Data dependencies introduction

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

* Error messages

```cpp
```

* API reference


<a id="configuration"></a>
* Configuration

Macro                                   | Description
----------------------------------------|-----------------------------------------
MSM\_POLICY\_STATES\_DST\_SRC           | dst\_state == src\_state (same as in eUML)

> **Boost.MSM - eUML emulation**

```cpp
```

> **Benchmarks**

* Header

* Simple / 1'000'000 / clang++3.7 -O2 -s

|                  | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.467s   | 5.363s          | 1.550s           |
| Execution time   | 17ms     | 20ms            | 1838ms           |
| Memory usage     | 14b      | 32b             | 200b             |
| Executable size  | 11K      | 91K             | 59K              |

* Simple / 1'000'000 / gcc-6.0 -O2 -s

|                  | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.534s   | 11.442s         | 2.819s           |
| Execution time   | 18ms     | 21ms            | 1431ms           |
| Memory usage     | 14b      | 32b             | 224b             |
| Executable size  | 11K      | 67K             | 67K              |

* Composite / 1'000'000 / clang++3.7 -O2 -s

|                  | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.467s   | 5.363s          | 1.550s           |
| Execution time   | 17ms     | 20ms            | 1838ms           |
| Memory usage     | 14b      | 32b             | 200b             |
| Executable size  | 11K      | 91K             | 59K              |

* Composite / 1'000'000 / gcc-6.0 -O2 -s

|                  | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.534s   | 11.442s         | 2.819s           |
| Execution time   | 18ms     | 21ms            | 1431ms           |
| Memory usage     | 14b      | 32b             | 224b             |
| Executable size  | 11K      | 67K             | 67K              |

* Complex / 1'000'000 / clang++3.7 -O2 -s

|                  | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 2.483s   |                 |                  |
| Execution time   | 164ms    |                 |                  |
| Memory usage     | 202b     |                 |                  |
| Executable size  | 103K     |                 |                  |

* Complex / 1'000'000 / gcc-6.0 -O2 -s

|                  | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 3.594s   |                 |                  |
| Execution time   | 159ms    |                 |                  |
| Memory usage     | 202b     |                 |                  |
| Executable size  | 99K      |                 |                  |

> **TODO**

* Policies

    * Are state truly orthogonal
    * Are all states reachable from initial state

* Logging
* Testing
