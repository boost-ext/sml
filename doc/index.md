<a href="http://www.boost.org/LICENSE_1_0.txt" target="_blank">![Boost Licence](http://img.shields.io/badge/license-boost-blue.svg)</a>
<a href="https://github.com/boost-experimental/msm-lite/releases" target="_blank">![Version](https://badge.fury.io/gh/boost-experimental%2Fmsm-lite.svg)</a>
<a href="https://travis-ci.org/boost-experimental/msm-lite" target="_blank">![Build Status](https://img.shields.io/travis/boost-experimental/msm-lite/master.svg?label=linux/osx)</a>
<a href="https://ci.appveyor.com/project/boost-experimental/msm-lite" target="_blank">![Build Status](https://img.shields.io/appveyor/ci/boost-experimental/di/master.svg?label=windows)</a>
<a href="https://coveralls.io/r/boost-experimental/msm-lite?branch=master" target="_blank">![Coveralls](http://img.shields.io/coveralls/boost-experimental/msm-lite/master.svg)</a>
<a href="http://github.com/boost-experimental/msm-lite/issues" target="_blank">![Github Issues](https://img.shields.io/github/issues/boost-experimental/msm-lite.svg)</a>

---------------------------------------

Introduction
============

| **Experimental Boost.MSM-lite** | |
| - | - |
| Your scalable C++14 header only eUML-like meta state machine library with no dependencies ([__Try it online!__](http://boost-experimental.github.io/msm-lite/examples/index.html#hello-world)) | <a class="github-button" href="https://github.com/boost-experimental/msm-lite" data-style="mega" data-count-href="/boost-experimental/msm-lite/stargazers" data-count-api="/repos/boost-experimental/msm-lite#stargazers_count" data-count-aria-label="# stargazers on GitHub" aria-label="Star boost-experimental/msm-lite on GitHub">GitHub</a> |

###UML State Machine

* [State Machine](https://en.wikipedia.org/wiki/UML_state_machine)
* [UML2 Specification](http://www.omg.org/spec/UML/2.5)
* [Boost.MSM - eUML](http://www.boost.org/doc/libs/1_60_0/libs/msm/doc/HTML/ch03s04.html)

###Do I need a State Machine?
State Machine design pattern allows you to avoid maintaining following spaghetti code.

```cpp
void some_function() {
    ...
    if ((is_running && !is_jumping) || just_started) {
        ...
    } else if (is_boss_level && extra_feature_enabled && !ab_test) {
        ...
    } else {
        ...
    }
}
```

If above code looks somewhat similar to your code base or if you like
to avoid it `Boost.MSM-lite` may suits you!

###Real Life examples?

![CPP(BTN)](Run_SDL2_Integration_Example|https://raw.githubusercontent.com/boost-experimental/msm-lite/master/example/sdl2.cpp)
![CPP(BTN)](Run_Plant_UML_Example|https://raw.githubusercontent.com/boost-experimental/msm-lite/master/example/plant_uml.cpp)
![CPP(BTN)](Run_Logging_Example|https://raw.githubusercontent.com/boost-experimental/msm-lite/master/example/logging.cpp)
![CPP(BTN)](Run_Testing_Example|https://raw.githubusercontent.com/boost-experimental/msm-lite/master/example/testing.cpp)

&nbsp;

###Why Boost.MSM-lite?

* [Boost.MSM - eUML](http://www.boost.org/doc/libs/1_60_0/libs/msm/doc/HTML/ch03s04.html) is awesome, however it has a few huge limitations which stop it from being used it on a larger scale;
  Boost.MSM-lite, therefore, is trying to address those issues.

###Problems with Boost.MSM - eUML

* Horrible compilation times (see [Performance](overview.md#performance))
* Produces huge binaries (see [Performance](overview.md#performance))
* Based on too many macros
* Horrible and long error messages (see [Error Messages](overview.md#error-messages))
* Sometimes hard to follow as not all actions might be seen on transition table (ex. initial states, state entry/exit actions)
* A lot of boilerplate code with actions/guards (requires fsm, event, source state, target state)
* Data in states makes it harder to share/encapsulate (UML compliant though)
* Loosley coupled design is hard to achieve
* Functional programming emulation
* Huge complexity may overwhelm in the beginning
* A lot of Boost dependencies

###Boost.MSM-lite design goals

* Keep the Boost.MSM - eUML 'goodies'
    * Performance (see [Performance](overview.md#performance))
    * Memory usage (see [Performance](overview.md#performance))
    * eUML DSL (s1 == s2 + event [ guard ] / action)
    * UML standard compliant (As much as possible)

>

* Eliminate Boost.MSM - eUML problems
    * Compilation times (see [Performance](overview.md#performance))
    * Binary size (see [Performance](overview.md#performance))
    * Reduce complexity by eliminating less used features
    * Short and informative error messages (see [Error Messages](overview.md#error-messages))
    * Less boilerplate / no macros (see [Hello World](examples.md#hello-world))
    * Improve visibility by having all actions on transition table (see [States](examples.md#states))
	* Allows loosely coupled design (see [Dependency Injection](examples.md#dependency-injection))
    * Functional programming support using lamda expressions (see [Action/Guards](examples.md#action-guards))
    * No dependencies / one header (1k lines)

###What 'lite' implies?

* Minimal learning curve
* Maximized performance
* Guaranteed quick compilation times
* No dependencies

###*Supported* UML features

* Transitions / Internal transitions / Anonymous transitions / No transition (see [Transitions](examples.md#transitions), [Events](examples.md#events))
* Actions / Guards (see [Action/Guards](examples.md#action-guards))
* State entry / exit actions (see [States](examples.md#states))
* Orthogonal regions (see [Orthogonal Regions](examples.md#orthogonal-regions))
* Sub / Composite state machines (see [Composite](examples.md#composite))
* History (see [History](examples.md#history))

###*Additional* features

* Logging (see [Logging](examples.md#logging))
* Testing (see [Testing](examples.md#testing))
* Runtime Dispatcher (see [Runtime Dispatcher](examples.md#runtime-dispatcher))
* Dependency Injection integration (see [Dependency Injection](examples.md#dependency-injection))

###Acknowledgements
* Thanks to Christophe Henry for a great [Boost.MSM - eUML](http://www.boost.org/doc/libs/1_60_0/libs/msm/doc/HTML/ch03s04.html) library
