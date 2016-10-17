| Test | CXXFLAGS | Events | States | Transitions | Process Events |
| ---- | -------- | ------ | ------ | ----------- | -------------- |
|[Simple Test](https://github.com/boost-experimental/msm-lite/tree/master/test/pt/simple) | -O2 -s | 6 | 5 | 12 | 1'000'000 |

| Clang-3.7        | Boost.MSM-lite | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] |[Boost.Statechart] |
|------------------|----------------|------------------|--------------------|-------------------|
| Compilation time | 0.144s         | 3.855s           | 8.699s             | 1.028s            |
| Execution time   | 15ms           | 17ms             | 17ms               | 1232ms            |
| Memory usage     | 1b             | 32b              | 28b                | 200b              |
| Executable size  | 11K            | 91K              | 15K + boost_system | 59K               |

| GCC-5.2          | Boost.MSM-lite | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] |[Boost.Statechart] |
|------------------|----------------|------------------|--------------------|-------------------|
| Compilation time | 0.175s         | 7.879s           | 17.101s            | 1.790s            |
| Execution time   | 15ms           | 19ms             | 21ms               | 929ms             |
| Memory usage     | 1b             | 32b              | 28b                | 224b              |
| Executable size  | 11K            | 67K              | 15K + boost_system | 63K               |

---------------------------------------

| Test | CXXFLAGS | Events | States | Transitions | Process Events |
| ---- | -------- | ------ | ------ | ----------- | -------------- |
|[Composite Test](https://github.com/boost-experimental/msm-lite/tree/master/test/pt/composite) | -O2 -s | 8 | 5 + 3 | 12 + 4 | 1'000 * 1'000 |

| Clang-3.7        | Boost.MSM-lite | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] | [Boost.Statechart] |
|------------------|----------------|------------------|--------------------|--------------------|
| Compilation time | 0.184s         | 4.526s           | 10.599s            | 1.293s             |
| Execution time   | 10ms           | 14ms             | 10ms               | 491ms              |
| Memory usage     | 2b             | 60b              | 52b                | 200b               |
| Executable size  | 15K            | 111K             | 15K + boost_system | 83K                |

| GCC-5.2          | Boost.MSM-lite | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] | [Boost.Statechart] |
|------------------|----------------|------------------|--------------------|--------------------|
| Compilation time | 0.248s         | 9.363s           | 23.404s            | 2.037s             |
| Execution time   | 9ms            | 13ms             | 12ms               | 404ms              |
| Memory usage     | 2b             | 60b              | 52b                | 224b               |
| Executable size  | 12K            | 91K              | 15K + boost_system | 83K                |

---

| Test | CXXFLAGS | Events | States | Transitions | Process Events |
| ---- | -------- | ------ | ------ | ----------- | -------------- |
|[Complex Test](https://github.com/boost-experimental/msm-lite/tree/master/test/pt/complex) | -O2 -s | 50 | 50 | 50 | 1'000'000 |

| Clang-3.7        | Boost.MSM-lite | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] | [Boost.Statechart] |
|------------------|----------------|------------------|--------------------|--------------------|
| Compilation time | 0.582s         | 1m15.935s        | 43.341s            | 3.661s             |
| Execution time   | 69ms           | 81ms             | 78ms               | 6221ms             |
| Memory usage     | 1b             | 120b             | 72b                | 200b               |
| Executable size  | 35K            | 611K             | 31K + boost_system | 343K               |

| GCC-5.2          | Boost.MSM-lite | [Boost.MSM-eUML] | [Boost.MSM3-eUML2] | [Boost.Statechart] |
|------------------|----------------|------------------|--------------------|--------------------|
| Compilation time | 0.816s         | 52.238s          | 1m41.045s          | 4.997s             |
| Execution time   | 72ms           | 77ms             | 91ms               | 5520ms             |
| Memory usage     | 1b             | 120b             | 72b                | 224b               |
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
cd benchmark && make # make benchmark_simple benchmark_composite benchmark_complex
```


