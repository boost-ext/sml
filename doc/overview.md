###Quick Start

* Get [boost/msm.hpp](https://raw.githubusercontent.com/boost-experimental/msm-lite/master/include/boost/msm.hpp) header
```sh
    wget https://raw.githubusercontent.com/boost-experimental/msm-lite/master/include/boost/msm.hpp
```

* Include the header
```cpp
    #include "boost/msm.hpp"
```

* Compile with C++14 support
```sh
    $CXX -std=c++14 ...
```

###Dependencies

* No external dependencies are required (niether STL nor Boost)

###Supported/tested compilers

* Clang-3.4+ (100% coverage, clang-tidy clean, valgrind clean)
* GCC-5.2+ (100% coverage, clang-tidy clean, valgrind clean)

###Configuration
| Macro                                   | Description |
| ----------------------------------------|------------ |
| BOOST\_MSM\_VERSION                     | Current versoin of msm-lite (ex. 1'0'0) |
| BOOST\_MSM\_LOG                         | Enables logging system  (see [Logging](examples.md#logging)) |
| BOOST\_MSM\_DSL\_DST\_STATE\_FIRST      | dst\_state == src\_state (see [eUML emulation](examples.md#euml-emulation)) |

###Performance

| Test | CXXFLAGS | Events | States | Transitions | Process Events |
| ---- | -------- | ------ | ------ | ----------- | -------------- |
|[Simple Test](https://github.com/boost-experimental/msm-lite/tree/master/test/pt/simple) | -O2 -s | 6 | 5 | 12 | 1'000'000 |

| clang++-3.7      | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.307s   | 3.855s          | 1.061s           |
| Execution time   | 15ms     | 17ms            | 1232ms           |
| Memory usage     | 14b      | 32b             | 200b             |
| Executable size  | 11K      | 91K             | 59K              |

| g++-6.0          | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.386s   | 7.879s          | 1.790s           |
| Execution time   | 15ms     | 20ms            | 929ms            |
| Memory usage     | 14b      | 32b             | 224b             |
| Executable size  | 11K      | 67K             | 63K              |

---------------------------------------

| Test | CXXFLAGS | Events | States | Transitions | Process Events |
| ---- | -------- | ------ | ------ | ----------- | -------------- |
|[Composite Test](https://github.com/boost-experimental/msm-lite/tree/master/test/pt/composite) | -O2 -s | 8 | 5 + 3 | 12 + 4 | 1'000 * 1'000 |

| clang++-3.7      | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.354s   | 4.526s          | 1.293s           |
| Execution time   | 10ms     | 14ms            | 491ms            |
| Memory usage     | 20b      | 60b             | 200b             |
| Executable size  | 15K      | 111K            | 83K              |

| g++-6.0          | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.435s   | 9.363s          | 2.037s           |
| Execution time   | 9ms      | 13ms            | 404ms            |
| Memory usage     | 20b      | 60b             | 224b             |
| Executable size  | 12K      | 91K             | 83K              |

---------------------------------------

| Test | CXXFLAGS | Events | States | Transitions | Process Events |
| ---- | -------- | ------ | ------ | ----------- | -------------- |
|[Complex Test](https://github.com/boost-experimental/msm-lite/tree/master/test/pt/complex) | -O2 -s | 50 | 50 | 50 | 1'000'000 |

| clang++-3.7      | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.765s   | 1m15.935s       | 3.661s           |
| Execution time   | 70ms     | 81ms            | 6221ms           |
| Memory usage     | 102b     | 120b            | 200b             |
| Executable size  | 35K      | 611K            | 343K             |

| g++-6.0          | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.996s   | 52.238s         | 4.997s           |
| Execution time   | 72ms     | 77ms            | 5520ms           |
| Memory usage     | 102b     | 120b            | 224b             |
| Executable size  | 35K      | 271K            | 215K             |

---------------------------------------

| Test |
| ---- |
|[Header Test](https://github.com/boost-experimental/msm-lite/tree/master/test/pt/header) |

| clang++-3.7      | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.060s   | 2.072s          | 0.552s           |

| g++-6.0          | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.073s   | 3.197s          | 0.704s           |


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
