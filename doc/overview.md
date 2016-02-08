###Quick Start

* Get [boost/msm-lite.hpp](https://raw.githubusercontent.com/boost-experimental/msm-lite/master/include/boost/msm-lite.hpp) header
```sh
wget https://raw.githubusercontent.com/boost-experimental/msm-lite/master/include/boost/msm-lite.hpp
```

* Include the header and define `msm` namespace alias
```cpp
#include "boost/msm-lite.hpp"
namespace msm = boost::msm::lite;
```

* Compile with C++14 support
```sh
$CXX -std=c++14 ...
```

* To run tests
```sh
git clone https://github.com/boost-experimental/msm-lite && cd msm-lite && make test
```

###Dependencies

* No external dependencies are required (neither STL nor Boost)

###Supported/tested compilers

* [Clang-3.4+](https://travis-ci.org/boost-experimental/msm-lite)
* [GCC-5.2+](https://travis-ci.org/boost-experimental/msm-lite)

###Configuration
| Macro                                                         | Description                                                  |
| --------------------------------------------------------------|--------------------------------------------------------------|
| `BOOST_MSM_LITE_VERSION`                                      | Current version of Boost.MSM-lite (ex. 1'0'0)                |
| [`BOOST_MSM_LITE_LOG`](user_guide.md#boost_msm_log-debugging) | Enables logging system  (see [Logging](examples.md#logging)) |

###Exception Safety

Boost.MSM-lite is not using exceptions internally and therefore might be compiled with `-fno-exceptions`.
Check [User Guide](user_guide.md) to verify which API's are marked `noexcept`.

* When guard/action throws an exception [State Machine](user_guide.md##sm-state-machine) will stay in a defined state.

    | Expression | Description |
    | ---------- | ----------- |
    | If `guard` throws an exception | Current state is NOT changed, process event is canceled |
    | If `action` throws an exception | Current state IS changed to the new one, process event is canceled |

###Thread Safety

Boost.MSM-lite is thread safe.

###Performance

| Test | CXXFLAGS | Events | States | Transitions | Process Events |
| ---- | -------- | ------ | ------ | ----------- | -------------- |
|[Simple Test](https://github.com/boost-experimental/msm-lite/tree/master/test/pt/simple) | -O2 -s | 6 | 5 | 12 | 1'000'000 |

| Clang-3.7        | Boost.MSM-lite | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] |[Boost.Statechart] |
|------------------|----------------|------------------|--------------------|-------------------|
| Compilation time | 0.144s         | 3.855s           | 8.699s             | 1.028s            |
| Execution time   | 15ms           | 17ms             | 17ms               | 1232ms            |
| Memory usage     | 14b            | 32b              | 28b                | 200b              |
| Executable size  | 11K            | 91K              | 15K + boost_system | 59K               |

| GCC-5.2          | Boost.MSM-lite | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] |[Boost.Statechart] |
|------------------|----------------|------------------|--------------------|-------------------|
| Compilation time | 0.175s         | 7.879s           | 17.101s            | 1.790s            |
| Execution time   | 15ms           | 19ms             | 21ms               | 929ms             |
| Memory usage     | 14b            | 32b              | 28b                | 224b              |
| Executable size  | 11K            | 67K              | 15K + boost_system | 63K               |

---------------------------------------

| Test | CXXFLAGS | Events | States | Transitions | Process Events |
| ---- | -------- | ------ | ------ | ----------- | -------------- |
|[Composite Test](https://github.com/boost-experimental/msm-lite/tree/master/test/pt/composite) | -O2 -s | 8 | 5 + 3 | 12 + 4 | 1'000 * 1'000 |

| Clang-3.7        | Boost.MSM-lite | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] | [Boost.Statechart] |
|------------------|----------------|------------------|--------------------|--------------------|
| Compilation time | 0.184s         | 4.526s           | 10.599s            | 1.293s             |
| Execution time   | 10ms           | 14ms             | 10ms               | 491ms              |
| Memory usage     | 20b            | 60b              | 52b                | 200b               |
| Executable size  | 15K            | 111K             | 15K + boost_system | 83K                |

| GCC-5.2          | Boost.MSM-lite | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] | [Boost.Statechart] |
|------------------|----------------|------------------|--------------------|--------------------|
| Compilation time | 0.248s         | 9.363s           | 23.404s            | 2.037s             |
| Execution time   | 9ms            | 13ms             | 12ms               | 404ms              |
| Memory usage     | 20b            | 60b              | 52b                | 224b               |
| Executable size  | 12K            | 91K              | 15K + boost_system | 83K                |

---

| Test | CXXFLAGS | Events | States | Transitions | Process Events |
| ---- | -------- | ------ | ------ | ----------- | -------------- |
|[Complex Test](https://github.com/boost-experimental/msm-lite/tree/master/test/pt/complex) | -O2 -s | 50 | 50 | 50 | 1'000'000 |

| Clang-3.7        | Boost.MSM-lite | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] | [Boost.Statechart] |
|------------------|----------------|------------------|--------------------|--------------------|
| Compilation time | 0.582s         | 1m15.935s        | 43.341s            | 3.661s             |
| Execution time   | 69ms           | 81ms             | 78ms               | 6221ms             |
| Memory usage     | 102b           | 120b             | 72b                | 200b               |
| Executable size  | 35K            | 611K             | 31K + boost_system | 343K               |

| GCC-5.2          | Boost.MSM-lite | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] | [Boost.Statechart] |
|------------------|----------------|------------------|--------------------|--------------------|
| Compilation time | 0.816s         | 52.238s          | 1m41.045s          | 4.997s             |
| Execution time   | 72ms           | 77ms             | 91ms               | 5520ms             |
| Memory usage     | 102b           | 120b             | 72b                | 224b               |
| Executable size  | 35K            | 271K             | 47K + boost_system | 215K               |

---

| Test |
| ---- |
|[Header Test](https://github.com/boost-experimental/msm-lite/tree/master/test/pt/header) |

| Clang-3.7        | Boost.MSM-lite | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] | [Boost.Statechart] |
|------------------|----------------|------------------|--------------------|--------------------|
| Compilation time | 0.060s         | 2.072s           | 2.504s             | 0.552s             |

| GCC-5.2          | Boost.MSM-lite | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] | [Boost.Statechart] |
|------------------|----------------|------------------|--------------------|--------------------|
| Compilation time | 0.073s         | 3.197s           | 3.986s             | 0.704s             |


* To run benchmarks

```sh
make pt # make pt_simple pt_composite pt_complex
```

###Error messages

***Not configurable***
![CPP](https://raw.githubusercontent.com/boost-experimental/msm-lite/master/example/errors/not_configurable.cpp)

***Not callable***
![CPP](https://raw.githubusercontent.com/boost-experimental/msm-lite/master/example/errors/not_callable.cpp)

***Not transitional***
![CPP](https://raw.githubusercontent.com/boost-experimental/msm-lite/master/example/errors/not_transitional.cpp)

***Not dispatchable***
![CPP](https://raw.githubusercontent.com/boost-experimental/msm-lite/master/example/errors/not_dispatchable.cpp)

[Boost.MSM-eUML]: http://www.boost.org/doc/libs/1_60_0/libs/msm/doc/HTML/ch03s04.html
[Boost.MSM3-eUML2]: https://htmlpreview.github.io/?https://raw.githubusercontent.com/boostorg/msm/msm3/doc/HTML/ch03s05.html
[Boost.Statechart]: http://www.boost.org/doc/libs/1_60_0/libs/statechart/doc/tutorial.html
