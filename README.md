<a href="http://www.boost.org/LICENSE_1_0.txt" target="_blank">![Boost Licence](http://img.shields.io/badge/license-boost-blue.svg)</a>
<a href="https://github.com/krzysztof-jusiak/msm-lite/releases" target="_blank">![Version](https://badge.fury.io/gh/krzysztof-jusiak%2Fmsm-lite.svg)</a>
<a href="https://github.com/krzysztof-jusiak/msm-lite/releases/latest" target="_blank">![Github Release](http://img.shields.io/github/release/krzysztof-jusiak/msm-lite.svg)</a>
<a href="https://travis-ci.org/krzysztof-jusiak/msm-lite" target="_blank">![Build Status](https://img.shields.io/travis/krzysztof-jusiak/msm-lite/cpp14.svg?label=linux/osx)</a>
<a href="http://github.com/krzysztof-jusiak/msm-lite/issues" target="_blank">![Github Issues](https://img.shields.io/github/issues/krzysztof-jusiak/msm-lite.svg)</a>
<a href="https://gitter.im/krzysztof-jusiak/msm-lite" target="_blank">![Gitter Chat](https://img.shields.io/badge/gitter-join%20chat%20%E2%86%92-brightgreen.svg)</a>
<a href="http://waffle.io/krzysztof-jusiak/msm-lite" target="_blank">![Stories in Ready](https://badge.waffle.io/krzysztof-jusiak/msm-lite.svg?label=ready&title=Ready)</a>

msm-lite: C++14 Meta State Machine Library
===============================================

* Benchmarks

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

