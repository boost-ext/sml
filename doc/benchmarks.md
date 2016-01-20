*Benchmarks*

* Simple / 1'000'000 / clang++3.7 -O2 -s

|                  | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.467s   | 5.363s          | 1.550s           |
| Execution time   | 17ms     | 20ms            | 1838ms           |
| Memory usage     | 14b      | 32b             | 200b             |
| Executable size  | 11K      | 91K             | 59K              |

* Simple / 1'000'000 / g++-6.0 -O2 -s

|                  | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.534s   | 11.442s         | 2.819s           |
| Execution time   | 18ms     | 21ms            | 1431ms           |
| Memory usage     | 14b      | 32b             | 224b             |
| Executable size  | 11K      | 67K             | 67K              |

* Composite / 1'000'000 / clang++3.7 -O2 -s

|                  | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.566s   | 6.716s          | 1.999s           |
| Execution time   | 12ms     | 15ms            | 647ms            |
| Memory usage     | 32b      | 60b             | 200b             |
| Executable size  | 11K      | 111K            | 83K              |

* Composite / 1'000'000 / g++-6.0 -O2 -s

|                  | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.652s   | 14.376s         | 3.308s           |
| Execution time   | 9ms      | 14ms            | 612ms            |
| Memory usage     | 32b      | 60b             | 224b             |
| Executable size  | 11K      | 91K             | 83K              |

* Complex / 1'000'000 / clang++3.7 -O2 -s

|                  | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 2.483s   |                 |                  |
| Execution time   | 164ms    |                 |                  |
| Memory usage     | 202b     |                 |                  |
| Executable size  | 103K     |                 |                  |

* Complex / 1'000'000 / g++-6.0 -O2 -s

|                  | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 3.594s   |                 |                  |
| Execution time   | 159ms    |                 |                  |
| Memory usage     | 202b     |                 |                  |
| Executable size  | 99K      |                 |                  |

* Include / clang++3.7

|                  | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.067s   | 3.072s          | 0.847s           |

* Include / g++-6.0

|                  | msm-lite |  boost.msm-eUML | boost.statechart |
|------------------|----------|-----------------|------------------|
| Compilation time | 0.080s   | 4.817s          | 1.362s           |

