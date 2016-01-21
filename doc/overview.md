###Quick Start

* Get [msm.hpp](https://raw.githubusercontent.com/krzysztof-jusiak/msm-lite/master/include/msm/msm.hpp) header
```sh
    wget https://raw.githubusercontent.com/krzysztof-jusiak/msm-lite/master/include/msm/msm.hpp
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

###Supported/tested compilers

* Clang-3.4+
* GCC-5.2+

###Design

###Performance

* Simple / 1'000'000 / clang++3.7 -O2 -s

|                  | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.308s   | 3.855s          | 1.061s           |
| Execution time   | 15ms     | 17ms            | 1232ms           |
| Memory usage     | 14b      | 32b             | 200b             |
| Executable size  | 11K      | 91K             | 59K              |

* Simple / 1'000'000 / g++-6.0 -O2 -s

|                  | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.407s   | 7.879s          | 1.790s           |
| Execution time   | 16ms     | 20ms            | 929ms            |
| Memory usage     | 14b      | 32b             | 224b             |
| Executable size  | 11K      | 67K             | 63K              |

* Composite / 1'000'000 / clang++3.7 -O2 -s

|                  | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.360s   | 4.526s          | 1.293s           |
| Execution time   | 12ms     | 14ms            | 491ms            |
| Memory usage     | 20b      | 60b             | 200b             |
| Executable size  | 15K      | 111K            | 83K              |

* Composite / 1'000'000 / g++-6.0 -O2 -s

|                  | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.475s   | 9.363s          | 2.037s           |
| Execution time   | 9ms      | 13ms            | 404ms            |
| Memory usage     | 20b      | 60b             | 224b             |
| Executable size  | 12K      | 91K             | 83K              |

* Complex / 1'000'000 / clang++3.7 -O2 -s

|                  | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.813s   | 1m15.935s       | 3.661s           |
| Execution time   | 71ms     | 81ms            | 6221ms           |
| Memory usage     | 102b     | 120b            | 200b             |
| Executable size  | 35K      | 611K            | 343K             |

* Complex / 1'000'000 / g++-6.0 -O2 -s

|                  | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 1.025s   | 52.238s         | 4.997s           |
| Execution time   | 73ms     | 77ms            | 5520ms           |
| Memory usage     | 102b     | 120b            | 224b             |
| Executable size  | 35K      | 271K            | 215K             |

* Include / clang++3.7

|                  | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.047s   | 2.072s          | 0.552s           |

* Include / g++-6.0

|                  | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.058s   | 3.197s          | 0.704s           |

