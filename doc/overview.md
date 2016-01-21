###Quick Start

* Get [boost/msm/msm.hpp](https://raw.githubusercontent.com/boost-experimental/msm-lite/master/include/boost/msm/msm.hpp) header
```sh
    wget https://raw.githubusercontent.com/boost-experimental/msm-lite/master/include/boost/msm/msm.hpp
```

* Include the header
```cpp
    #include "boost/msm/msm.hpp"
```

* Compile with C++14 support
```sh
    $CXX -std=c++14 ...
```

###Dependencies

* msm-lite has no external dependencies (no STL, no Boost)

###Supported/tested compilers

* Clang-3.4+
* GCC-5.2+

###Performance

| Test | CXXFLAGS | States | Submachines | Events | Transitions | Process Events |
| ---- | -------- | ------ | ----------- | ------ | ----------- | -------------- |
|[Simple Test](https://raw.githubusercontent.com/boost-experimental/msm-lite/master/test/pt/simple) | -O2 -s | 5 | 0 | 6 | 12 | 1'000'000 |

| clang++-3.7      | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.308s   | 3.855s          | 1.061s           |
| Execution time   | 15ms     | 17ms            | 1232ms           |
| Memory usage     | 14b      | 32b             | 200b             |
| Executable size  | 11K      | 91K             | 59K              |

| g++-6.0          | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.407s   | 7.879s          | 1.790s           |
| Execution time   | 16ms     | 20ms            | 929ms            |
| Memory usage     | 14b      | 32b             | 224b             |
| Executable size  | 11K      | 67K             | 63K              |

---------------------------------------

| Test | CXXFLAGS | States | Submachines | Events | Transitions | Process Events |
| ---- | -------- | ------ | ----------- | ------ | ----------- | -------------- |
|[Composite Test](https://raw.githubusercontent.com/boost-experimental/msm-lite/master/test/pt/simple) | -O2 -s | 5 | 0 | 6 | 12 | 1'000'000 |

| clang++-3.7      | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.360s   | 4.526s          | 1.293s           |
| Execution time   | 12ms     | 14ms            | 491ms            |
| Memory usage     | 20b      | 60b             | 200b             |
| Executable size  | 15K      | 111K            | 83K              |

| g++-.6.0         | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.475s   | 9.363s          | 2.037s           |
| Execution time   | 9ms      | 13ms            | 404ms            |
| Memory usage     | 20b      | 60b             | 224b             |
| Executable size  | 12K      | 91K             | 83K              |

---------------------------------------

| Test | CXXFLAGS | States | Submachines | Events | Transitions | Process Events |
| ---- | -------- | ------ | ----------- | ------ | ----------- | -------------- |
|[Complex Test](https://raw.githubusercontent.com/boost-experimental/msm-lite/master/test/pt/simple) | -O2 -s | 5 | 0 | 6 | 12 | 1'000'000 |

| clang++-3.7      | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.813s   | 1m15.935s       | 3.661s           |
| Execution time   | 71ms     | 81ms            | 6221ms           |
| Memory usage     | 102b     | 120b            | 200b             |
| Executable size  | 35K      | 611K            | 343K             |

| g++-6.0          | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 1.025s   | 52.238s         | 4.997s           |
| Execution time   | 73ms     | 77ms            | 5520ms           |
| Memory usage     | 102b     | 120b            | 224b             |
| Executable size  | 35K      | 271K            | 215K             |

---------------------------------------

| Test | CXXFLAGS | States | Submachines | Events | Transitions | Process Events |
| ---- | -------- | ------ | ----------- | ------ | ----------- | -------------- |
|[Header Test](https://raw.githubusercontent.com/boost-experimental/msm-lite/master/test/pt/simple) | -O2 -s | 5 | 0 | 6 | 12 | 1'000'000 |

| clang++-3.7      | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.047s   | 2.072s          | 0.552s           |

| g++-6.0          | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.058s   | 3.197s          | 0.704s           |

