![Boost libraries](img/boost.png)
<a href="http://www.boost.org/LICENSE_1_0.txt" target="_blank">![Boost Licence](http://img.shields.io/badge/license-boost-blue.svg)</a>
<a href="https://github.com/krzysztof-jusiak/msm-lite" target="_blank">![Github](https://img.shields.io/badge/msm_lite-github-blue.svg)</a>
<a href="https://github.com/krzysztof-jusiak/msm-lite/releases" target="_blank">![Version](https://badge.fury.io/gh/krzysztof-jusiak%2Fmsm-lite.svg)</a>
<a href="https://github.com/krzysztof-jusiak/msm-lite/releases/latest" target="_blank">![Github Release](http://img.shields.io/github/release/krzysztof-jusiak/msm-lite.svg)</a>
<a href="https://travis-ci.org/krzysztof-jusiak/msm-lite" target="_blank">![Build Status](https://img.shields.io/travis/krzysztof-jusiak/msm-lite/cpp14.svg?label=linux/osx)</a>
<a href="http://github.com/krzysztof-jusiak/msm-lite/issues" target="_blank">![Github Issues](https://img.shields.io/github/issues/krzysztof-jusiak/msm-lite.svg)</a>

---------------------------------------

Introduction
============

msm-lite: C++14 Meta State Machine Library

> Your scalable C++14 header only eUML-like meta state machine library with no dependencies


###UML State Machine

* [State Machine](https://en.wikipedia.org/wiki/UML_state_machine)
* [UML2 Specification](http://www.omg.org/spec/UML/2.5)
* [Boost.MSM - eUML](http://www.boost.org/doc/libs/1_60_0/libs/msm/doc/HTML/ch03s04.html)

###Why msm-lite?

* [Boost.MSM - eUML](http://www.boost.org/doc/libs/1_60_0/libs/msm/doc/HTML/ch03s04.html) is awesome, however it has a few huge limitations which stop it from being used it on a larger scale;
  msm-lite, therefore, is trying to address those issues.

###Problems with Boost.MSM - eUML

* Horrible compilation times (see [Performance](overview.md#performance))
* Produces huge binaries (see [Performance](overview.md#performance))
* Based on too many macros
* Horrible and long error messages (see [Error Messages](overview.md#error-messages))
* Sometimes hard to follow as not all actions might be seen on transition table (ex. initial states, on\_entry, on\_exit)
* A lot of boilerplate code with actions/guards (requires fsm, event, source state, target state)
* Data in states makes it harder share/encapsulate (UML compliant though)
* Functional programming emulation
* Huge complexity may overwhelm in the beginning
* A lot of Boost dependencies

###msm-lite design goals

* Keep the Boost.MSM - eUML goodies
    * Performance (see [Performance](overview.md#performance))
    * Memory usage (see [Performance](overview.md#performance))
    * eUML DSL (s1 == s2 + event [ guard ] / action)
    * UML standard compliant (As much as possible)

* Eliminate Boost.MSM - eUML problems
    * Compilation times (see [Performance](overview.md#performance))
    * Binary size (see [Performance](overview.md#performance))
    * Reduce complexity by eliminating less used features
    * Short and informative error messages (see [Error Messages](overview.md#error-messages))
    * Less boilerplate / no macros
    * Improve visibility by having all actions on transition table
    * No dependencies / one header (1k lines)
    * Functional programming support using lamda expressions

###What 'lite' implies?

* Minimal learning curve
* Only crucial features
* Guaranteed performance and quick compilation times
* No dependencies

###Supported UML features by msm-lite

* Transitions / Internal transitions / Anonymous transitions / No transition (see [Transitions](examples.md#transitions))
* Actions / Guards (see [Action/Guards](examples.md#action-guards))
* State entry / exit actions (see [States](examples.md#states))
* Orthogonal regions (see [Orthogonal Regions](examples.md#orthogonal-regions))
* Sub / Composite state machines (see [Composite](examples.md#composite))

###Additional features

* Runtime Dispatcher (see [Runtime Dispatcher](examples.md#runtime-dispatcher))
* Dependency Injection integration (see [DI](examples.md#dependency-injection))

