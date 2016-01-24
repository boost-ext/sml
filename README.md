<a href="http://www.boost.org/LICENSE_1_0.txt" target="_blank">![Boost Licence](http://img.shields.io/badge/license-boost-blue.svg)</a>
<a href="https://github.com/boost-experimental/msm-lite/releases" target="_blank">![Version](https://badge.fury.io/gh/boost-experimental%2Fmsm-lite.svg)</a>
<a href="https://github.com/boost-experimental/msm-lite/releases/latest" target="_blank">![Github Release](http://img.shields.io/github/release/boost-experimental/msm-lite.svg)</a>
<a href="https://travis-ci.org/boost-experimental/msm-lite" target="_blank">![Build Status](https://img.shields.io/travis/boost-experimental/msm-lite/cpp14.svg?label=linux/osx)</a>
<a href="http://github.com/boost-experimental/msm-lite/issues" target="_blank">![Github Issues](https://img.shields.io/github/issues/boost-experimental/msm-lite.svg)</a>

---------------------------------------

#msm-lite: C++14 Meta State Machine Library

> Your scalable C++14 header only eUML-like meta state machine library with no dependencies (__[Try it online!](http://boost-experimental.github.io/msm-lite/examples/index.html#hello-world)__)

[](GENERATE_TOC_BEGIN)

* [Introduction](http://boost-experimental.github.io/msm-lite/index.html)
    * [UML State Machine](http://boost-experimental.github.io/msm-lite/index.html#uml-state-machine)
    * [Why msm-lite?](http://boost-experimental.github.io/msm-lite/index.html#why-msm-lite)
    * [Problems with Boost.MSM - eUML](http://boost-experimental.github.io/msm-lite/index.html#problems-with-boostmsm-euml)
    * [msm-lite design goals](http://boost-experimental.github.io/msm-lite/index.html#msm-lite-design-goals)
    * [What 'lite' implies?](http://boost-experimental.github.io/msm-lite/index.html#what-lite-implies)
    * [Supported UML features by msm-lite](http://boost-experimental.github.io/msm-lite/index.html#supported-uml-features-by-msm-lite)
    * [Additional features](http://boost-experimental.github.io/msm-lite/index.html#additional-features)
* [Overview](http://boost-experimental.github.io/msm-lite/overview/index.html)
    * [Quick Start](http://boost-experimental.github.io/msm-lite/overview/index.html#quick-start)
    * [Dependencies](http://boost-experimental.github.io/msm-lite/overview/index.html#dependencies)
    * [Supported/tested compilers](http://boost-experimental.github.io/msm-lite/overview/index.html#supportedtested-compilers)
    * [Configuration](http://boost-experimental.github.io/msm-lite/overview/index.html#configuration)
    * [Performance](http://boost-experimental.github.io/msm-lite/overview/index.html#performance)
    * [Error messages](http://boost-experimental.github.io/msm-lite/overview/index.html#error-messages)
* [Tutorial](http://boost-experimental.github.io/msm-lite/tutorial/index.html)
* [User Guide](http://boost-experimental.github.io/msm-lite/user_guide/index.html)
    * [make\_transition\_table](http://boost-experimental.github.io/msm-lite/user_guide/index.html#maketransitiontable)
    * [Concepts.Transitional](http://boost-experimental.github.io/msm-lite/user_guide/index.html#conceptstransitional)
    * [Concepts.Configurable](http://boost-experimental.github.io/msm-lite/user_guide/index.html#conceptsconfigurable)
    * [Concepts.Callable](http://boost-experimental.github.io/msm-lite/user_guide/index.html#conceptscallable)
    * [Concepts.Dispatchable](http://boost-experimental.github.io/msm-lite/user_guide/index.html#conceptsdispatchable)
* [Examples](http://boost-experimental.github.io/msm-lite/examples/index.html)
    * [Hello World](http://boost-experimental.github.io/msm-lite/examples/index.html#hello-world)
    * [Transitions](http://boost-experimental.github.io/msm-lite/examples/index.html#transitions)
    * [Action Guards](http://boost-experimental.github.io/msm-lite/examples/index.html#action-guards)
    * [States](http://boost-experimental.github.io/msm-lite/examples/index.html#states)
    * [Events](http://boost-experimental.github.io/msm-lite/examples/index.html#events)
    * [Orthogonal Regions](http://boost-experimental.github.io/msm-lite/examples/index.html#orthogonal-regions)
    * [Composite](http://boost-experimental.github.io/msm-lite/examples/index.html#composite)
    * [eUML Emulation](http://boost-experimental.github.io/msm-lite/examples/index.html#euml-emulation)
    * [Dependency Injection](http://boost-experimental.github.io/msm-lite/examples/index.html#dependency-injection)
    * [Runtime Dispatcher](http://boost-experimental.github.io/msm-lite/examples/index.html#runtime-dispatcher)
* [CHANGELOG](http://boost-experimental.github.io/msm-lite/CHANGELOG/index.html)
    * [[1.0.0] - TBD](http://boost-experimental.github.io/msm-lite/CHANGELOG/index.html#100-tbd)
    * [Added](http://boost-experimental.github.io/msm-lite/CHANGELOG/index.html#added)
* [TODO](http://boost-experimental.github.io/msm-lite/TODO/index.html)

[](GENERATE_TOC_END)

---

**Disclaimer** msm-lite is not an official Boost library.
