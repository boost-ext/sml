Introduction
============

![Boost libraries](img/boost.png)
<a href="http://www.boost.org/LICENSE_1_0.txt" target="_blank">![Boost Licence](http://img.shields.io/badge/license-boost-blue.svg)</a>
<a href="https://github.com/krzysztof-jusiak/msm-lite" target="_blank">![Github](https://img.shields.io/badge/boost.msm-lite-github-blue.svg)</a>
<a href="https://github.com/krzysztof-jusiak/msm-lite/releases" target="_blank">![Version](https://badge.fury.io/gh/krzysztof-jusiak%2Fmsm-lite.svg)</a>
<a href="https://github.com/krzysztof-jusiak/msm-lite/releases/latest" target="_blank">![Github Release](http://img.shields.io/github/release/krzysztof-jusiak/msm-lite.svg)</a>
<a href="https://travis-ci.org/krzysztof-jusiak/msm-lite" target="_blank">![Build Status](https://img.shields.io/travis/krzysztof-jusiak/msm-lite/cpp14.svg?label=linux/osx)</a>
<a href="http://github.com/krzysztof-jusiak/msm-lite/issues" target="_blank">![Github Issues](https://img.shields.io/github/issues/krzysztof-jusiak/msm-lite.svg)</a>

---------------------------------------

msm-lite: C++14 Meta State Machine Library

> Your scalable C++14 header only eUML-like meta state machine library with no dependencies

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
    * No dependencies / one header (1k lines)
    * Functional programming support using lamda expressions

* Add a new functionality

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

* Get [msm.hpp](https://raw.githubusercontent.com/krzysztof-jusiak/msm-lite/master/include/msm/msm.hpp) header
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
