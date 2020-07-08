**[Boost].SML vs Boost.MSM-eUML vs Boost.Statechart**

----

##Features

*Overview*

| Library     | [Boost].SML   | Boost.MSM-eUML  | Boost.Statechart |
| --------    | ----------- | --------------- | ---------------- |
| Standard    | C++14       | C++98/03        | C++98/03         |
| Version     | 1.0.1       | 1.61            | 1.61             |
| License     | Boost 1.0   | Boost 1.0       | Boost 1.0        |
| Linkage     | header only | header only     | header only      |

*Details*

| Library            | [Boost].SML   | Boost.MSM-eUML  | Boost.Statechart |
| ------------------ | ----------- | --------------- | ---------------- |
| UML                | 2.0         | 2.0             | 1.5              |
| RTTI               | -           | -               | ✓                |
| Exceptions         | -           | -               | ✓                |
| Memory Allocations | -           | -               | ✓                |

*UML features*

| Library              | [Boost].SML | Boost.MSM-eUML | Boost.Statechart |
| -------------------- | --------- | -------------- | ---------------- |
| Transition           | ✓         | ✓              | ✓                |
| Anonymous transition | ✓         | ✓              | ✓                |
| Internal transition  | ✓         | ✓              | ✓                |
| Local transitions    | -         | -              | -                |
| State entry/exit     | ✓         | ✓              | ✓                |
| Guard                | ✓         | ✓              | ✓                |
| Action               | ✓         | ✓              | ✓                |
| Event defering       | ~         | ✓              | ✓                |
| Error handling       | ✓         | ✓              | ✓                |
| Initial state        | ✓         | ✓              | ✓                |
| Terminate State      | ✓         | ✓              | ✓                |
| Explicit entry       | ✓         | ✓              | ✓                |
| Explicit exit        | -         | ✓              | ✓                |
| Fork                 | -         | ✓              | -                |
| Orthogonal regions   | ✓         | ✓              | ✓                |
| Sub / Composite      | ✓         | ✓              | ✓                |
| Shallow History      | ✓         | ✓              | ✓                |
| Deep History         | ~         | ~              | ✓                |

*Non-UML features*

| Library              | [Boost].SML | Boost.MSM-eUML | Boost.Statechart |
| -------------------- | --------- | -------------- | ---------------- |
| Any event            | -         | ✓              | -                |
| Flags                | -         | ✓              | -                |
| Interrupt state      | -         | ✓              | -                |
| State Visitor        | ✓         | ✓              | ✓                |
| Serialization        | -         | ✓              | -                |
| Dispatcher           | ✓         | -              | -                |
| Asynchronous SM      | -         | -              | ✓                |

----

##Benchmarks

| Test | CXXFLAGS | Events | States | Transitions | Process Events |
| ---- | -------- | ------ | ------ | ----------- | -------------- |
|[Simple Test](https://github.com/boost-ext/sml/tree/master/benchmark/simple) | -O2 -s, /Ox | 6 | 5 | 12 | 1'000'000 |

| Clang-3.7        | [Boost].SML      | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] |[Boost.Statechart] |
|------------------|----------------|------------------|--------------------|-------------------|
| Compilation time | 0.144s         | 3.855s           | 8.699s             | 1.028s            |
| Execution time   | 15ms           | 17ms             | 17ms               | 1232ms            |
| Memory usage     | 1b             | 32b              | 28b                | 200b              |
| Executable size  | 11K            | 91K              | 15K + boost_system | 59K               |

| GCC-5.2          | [Boost].SML      | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] |[Boost.Statechart] |
|------------------|----------------|------------------|--------------------|-------------------|
| Compilation time | 0.175s         | 7.879s           | 17.101s            | 1.790s            |
| Execution time   | 15ms           | 19ms             | 21ms               | 929ms             |
| Memory usage     | 1b             | 32b              | 28b                | 224b              |
| Executable size  | 11K            | 67K              | 15K + boost_system | 63K               |

| MSVC-2015        | [Boost].SML      | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] | [Boost.Statechart] |
|------------------|----------------|------------------|--------------------|--------------------|
| Compilation time | 0.450s         |                  |                    |                    |
| Execution time   | 27ms           |                  |                    |                    |
| Memory usage     | 16b            |                  |                    |                    |
| Executable size  | 206K           |                  |                    |                    |

---------------------------------------

| Test | CXXFLAGS | Events | States | Transitions | Process Events |
| ---- | -------- | ------ | ------ | ----------- | -------------- |
|[Composite Test](https://github.com/boost-ext/sml/tree/master/benchmark/composite) | -O2 -s, /Ox | 8 | 5 + 3 | 12 + 4 | 1'000 * 1'000 |

| Clang-3.7        | [Boost].SML      | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] | [Boost.Statechart] |
|------------------|----------------|------------------|--------------------|--------------------|
| Compilation time | 0.184s         | 4.526s           | 10.599s            | 1.293s             |
| Execution time   | 10ms           | 14ms             | 10ms               | 491ms              |
| Memory usage     | 2b             | 60b              | 52b                | 200b               |
| Executable size  | 15K            | 111K             | 15K + boost_system | 83K                |

| GCC-5.2          | [Boost].SML      | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] | [Boost.Statechart] |
|------------------|----------------|------------------|--------------------|--------------------|
| Compilation time | 0.248s         | 9.363s           | 23.404s            | 2.037s             |
| Execution time   | 9ms            | 13ms             | 12ms               | 404ms              |
| Memory usage     | 2b             | 60b              | 52b                | 224b               |
| Executable size  | 12K            | 91K              | 15K + boost_system | 83K                |

| MSVC-2015        | [Boost].SML      | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] | [Boost.Statechart] |
|------------------|----------------|------------------|--------------------|--------------------|
| Compilation time | 0.599s         |                  |                    |                    |
| Execution time   | 21ms           |                  |                    |                    |
| Memory usage     | 23b            |                  |                    |                    |
| Executable size  | 224K           |                  |                    |                    |

---

| Test | CXXFLAGS | Events | States | Transitions | Process Events |
| ---- | -------- | ------ | ------ | ----------- | -------------- |
|[Complex Test](https://github.com/boost-ext/sml/tree/master/benchmark/complex) | -O2 -s, /Ox | 50 | 50 | 50 | 1'000'000 |

| Clang-3.7        | [Boost].SML      | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] | [Boost.Statechart] |
|------------------|----------------|------------------|--------------------|--------------------|
| Compilation time | 0.582s         | 1m15.935s        | 43.341s            | 3.661s             |
| Execution time   | 69ms           | 81ms             | 78ms               | 6221ms             |
| Memory usage     | 1b             | 120b             | 72b                | 200b               |
| Executable size  | 35K            | 611K             | 31K + boost_system | 343K               |

| GCC-5.2          | [Boost].SML      | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] | [Boost.Statechart] |
|------------------|----------------|------------------|--------------------|--------------------|
| Compilation time | 0.816s         | 52.238s          | 1m41.045s          | 4.997s             |
| Execution time   | 72ms           | 77ms             | 91ms               | 5520ms             |
| Memory usage     | 1b             | 120b             | 72b                | 224b               |
| Executable size  | 35K            | 271K             | 47K + boost_system | 215K               |

| MSVC-2015        | [Boost].SML      | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] | [Boost.Statechart] |
|------------------|----------------|------------------|--------------------|--------------------|
| Compilation time | 1.891s         |                  |                    |                    |
| Execution time   | 166ms          |                  |                    |                    |
| Memory usage     | 104b           |                  |                    |                    |
| Executable size  | 224K           |                  |                    |                    |

---

| Test |
| ---- |
|[Header Test](https://github.com/boost-ext/sml/tree/master/benchmark/header) |

| Clang-3.7        | [Boost].SML      | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] | [Boost.Statechart] |
|------------------|----------------|------------------|--------------------|--------------------|
| Compilation time | 0.060s         | 2.072s           | 2.504s             | 0.552s             |

| GCC-5.2          | [Boost].SML      | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] | [Boost.Statechart] |
|------------------|----------------|------------------|--------------------|--------------------|
| Compilation time | 0.073s         | 3.197s           | 3.986s             | 0.704s             |

| MSVC-2015        | [Boost].SML      | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] | [Boost.Statechart] |
|------------------|----------------|------------------|--------------------|--------------------|
| Compilation time | 0.182s         |                  |                    |                    |


* To run benchmarks

```sh
cd benchmark && make # make benchmark_simple benchmark_composite benchmark_complex
```
