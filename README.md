<a href="http://www.boost.org/LICENSE_1_0.txt" target="_blank">![Boost Licence](http://img.shields.io/badge/license-boost-blue.svg)</a>
<a href="https://github.com/boost-experimental/sml/releases" target="_blank">![Version](https://badge.fury.io/gh/boost-experimental%2Fsml.svg)</a>
<a href="https://travis-ci.org/boost-experimental/sml" target="_blank">![Build Status](https://img.shields.io/travis/boost-experimental/sml/master.svg?label=linux/osx)</a>
<a href="https://ci.appveyor.com/project/krzysztof-jusiak/sml" target="_blank">![Build Status](https://img.shields.io/appveyor/ci/krzysztof-jusiak/sml/master.svg?label=windows)</a>
<a href="https://codecov.io/gh/boost-experimental/sml" target="_blank">![Codecov](https://codecov.io/gh/boost-experimental/sml/branch/master/graph/badge.svg)</a>
<a href="http://github.com/boost-experimental/sml/issues" target="_blank">![Github Issues](https://img.shields.io/github/issues/boost-experimental/sml.svg)</a>

---------------------------------------

# [Boost].SML (State Machine Language)

> Your scalable C++14 **one header only** State Machine Library with no dependencies ([__Try it online!__](http://boost-experimental.github.io/sml/examples/index.html#hello-world))

<p align="center">
  <br />
  <b>Let's release a TCP connection!</b>
  <br />
</p>

<p align="center"><img src="doc/images/tcp_release.png" alt="tcp release"/></p>

### Quick start

#### Download
> [Boost].SML requires only one file. Get the latest header [here!](https://raw.githubusercontent.com/boost-experimental/sml/master/include/boost/sml.hpp)

#### Include
```cpp
#include <boost/sml.hpp>
namespace sml = boost::sml;
```

#### Dependencies
```cpp
struct sender {
  template<class TMsg>
  constexpr void send(const TMsg& msg) { std::printf("send: %d\n", msg.id); }
};
```

#### Events
```cpp
struct ack { bool valid{}; };
struct fin { int id{}; bool valid{}; };
struct release {};
struct timeout {};
```

#### Guards
```cpp
constexpr auto is_valid = [](const auto& event) { return event.valid; };
```

#### Actions
```cpp
constexpr auto send_fin = [](sender& s) { s.send(fin{0}); };
constexpr auto send_ack = [](const auto& event, sender& s) { s.send(event); };
```

#### State Machine
```cpp
struct tcp_release final {
  auto operator()() const {
    using namespace sml;
    /**
     * Initial state: *initial_state
     * Transition DSL: src_state + event [ guard ] / action = dst_state
     */
    return make_transition_table(
      *"established"_s + event<release>          / send_fin  = "fin wait 1"_s,
       "fin wait 1"_s  + event<ack> [ is_valid ]             = "fin wait 2"_s,
       "fin wait 2"_s  + event<fin> [ is_valid ] / send_ack  = "timed wait"_s,
       "timed wait"_s  + event<timeout>                      = X
    );
  }
};
```

#### Usage
```cpp
int main() {
  using namespace sml;

  sender s{};
  sm<tcp_release> sm{s}; // pass dependencies via ctor
  assert(sm.is("established"_s));

  sm.process_event(release{}); // complexity O(1)
  assert(sm.is("fin wait 1"_s));

  sm.process_event(ack{true}); // prints 'send: 0'
  assert(sm.is("fin wait 2"_s));

  sm.process_event(fin{42, true}); // prints 'send: 42'
  assert(sm.is("timed wait"_s));

  sm.process_event(timeout{});
  assert(sm.is(X));  // terminated
}
```

> MSVC-2015 ([Example](http://boost-experimental.github.io/sml/examples/index.html#hello-world))

  * use `state<class state_name>` instead of `"state_name"_s`
  * expliclty state a lambda's result type `auto action = [] -> void {}`

#### Compile
* **GCC/Clang**
  ```sh
  $CXX -std=c++14 -O2 -fno-exceptions -Wall -Wextra -Werror -pedantic -pedantic-errors tcp_release.cpp
  ```
* **MSVC**
  ```sh
  cl /std:c++14 /Ox /W3 tcp_release.cpp
  ```

<p align="center">
<table>
  <tr>
    <th>tcp_release.cpp</th>
    <th>Clang-3.8</th>
    <th>GCC-6.3</th>
    <th>MSVC-2015</th>
  </tr>

  <tr>
    <td>Compilation Time</td>
    <td>0.102s</td>
    <td>0.118s</td>
    <td>0.296s</td>
  </tr>

  <tr>
    <td>Binary size (stripped)</td>
    <td>6.2kb</td>
    <td>6.2kb</td>
    <td>105kb</td>
  </tr>

  <tr>
    <td>ASM x86-64 - <br/>https://godbolt.org/z/y99L50</td>
    <td colspan="3">
      <pre><code>
main: # @main
  pushq %rax
  movl $.L.str, %edi
  xorl %esi, %esi
  xorl %eax, %eax
  callq printf
  movl $.L.str, %edi
  movl $42, %esi
  xorl %eax, %eax
  callq printf
  xorl %eax, %eax
  popq %rcx
  retq
.L.str:
  .asciz "send: %d\n"
      </code></pre>
    </td>
  </tr>
</table>
</p>

#### Run
> Output (https://wandbox.org/permlink/WbvV9HsIyiPkCFw7)
```sh
send: 0
send: 42
```

### Benchmark

> [Complex Test](https://github.com/boost-experimental/sml/tree/master/benchmark/complex)

|                  |[Enum/Switch](https://github.com/boost-experimental/sml/blob/master/benchmark/complex/switch.cpp) | [Variant](https://github.com/boost-experimental/sml/blob/master/benchmark/complex/variant.cpp) | [[Boost].SML - 1.1.0](https://github.com/boost-experimental/sml/blob/master/benchmark/complex/sml.cpp)    | [Boost-1.65.MSM-eUML](https://github.com/boost-experimental/sml/blob/master/benchmark/complex/euml.cpp)   | [Boost-1.65.Statechart](https://github.com/boost-experimental/sml/blob/master/benchmark/complex/sc.cpp)   |
|------------------|------------|---------|--------------|------------------|--------------------|
| **Compilation time** |  0.132s    | 15.321s |       0.582s |        1m15.935s |             5.671s |
| **Execution time**   |   679ms    | 827ms   |        622ms |        664ms     |             2282ms |
| **Memory usage**     |      1b    | 2b/8b   |           1b |        120b      |             224b   |
| **Executable size**  |     15K    | 187K    |          34K |        611K      |             211K   |

---------------------------------------

### Documentation

[](GENERATE_TOC_BEGIN)

* [Introduction](http://boost-experimental.github.io/sml/index.html)
    * [UML State Machine](http://boost-experimental.github.io/sml/index.html#uml-state-machine)
    * [Do I need a State Machine?](http://boost-experimental.github.io/sml/index.html#do-i-need-a-state-machine)
    * [Real Life examples?](http://boost-experimental.github.io/sml/index.html#real-life-examples)
    * [Why [Boost].SML?](http://boost-experimental.github.io/sml/index.html#why-boostsml)
    * [Problems with Boost.MSM - eUML](http://boost-experimental.github.io/sml/index.html#problems-with-boostmsm-euml)
    * [[Boost].SML design goals](http://boost-experimental.github.io/sml/index.html#boostsml-design-goals)
    * [What 'lite' implies?](http://boost-experimental.github.io/sml/index.html#what-lite-implies)
    * [*Supported* UML features](http://boost-experimental.github.io/sml/index.html#supported-uml-features)
    * [*Additional* features](http://boost-experimental.github.io/sml/index.html#additional-features)
    * [Related materials](http://boost-experimental.github.io/sml/index.html#related-materials)
    * [Acknowledgements](http://boost-experimental.github.io/sml/index.html#acknowledgements)
* [Overview](http://boost-experimental.github.io/sml/overview/index.html)
    * [Quick Start](http://boost-experimental.github.io/sml/overview/index.html#quick-start)
    * [Dependencies](http://boost-experimental.github.io/sml/overview/index.html#dependencies)
    * [Supported/Tested compilers](http://boost-experimental.github.io/sml/overview/index.html#supportedtested-compilers)
    * [Configuration](http://boost-experimental.github.io/sml/overview/index.html#configuration)
    * [Exception Safety](http://boost-experimental.github.io/sml/overview/index.html#exception-safety)
    * [Thread Safety](http://boost-experimental.github.io/sml/overview/index.html#thread-safety)
    * [Design](http://boost-experimental.github.io/sml/overview/index.html#design)
    * [Error messages](http://boost-experimental.github.io/sml/overview/index.html#error-messages)
* [Features/Benchmarks](http://boost-experimental.github.io/sml/benchmarks/index.html)
    * [Features](http://boost-experimental.github.io/sml/benchmarks/index.html#features)
    * [Benchmarks](http://boost-experimental.github.io/sml/benchmarks/index.html#benchmarks)
* [Tutorial/Workshop](http://boost-experimental.github.io/sml/tutorial/index.html)
    * [0. Read Boost.MSM - eUML documentation](http://boost-experimental.github.io/sml/tutorial/index.html#0-read-boostmsm-euml-documentation)
    * [1. Create events and states](http://boost-experimental.github.io/sml/tutorial/index.html#1-create-events-and-states)
    * [2. Create guards and actions](http://boost-experimental.github.io/sml/tutorial/index.html#2-create-guards-and-actions)
    * [3. Create a transition table](http://boost-experimental.github.io/sml/tutorial/index.html#3-create-a-transition-table)
    * [4. Set initial states](http://boost-experimental.github.io/sml/tutorial/index.html#4-set-initial-states)
    * [5. Create a state machine](http://boost-experimental.github.io/sml/tutorial/index.html#5-create-a-state-machine)
    * [6. Process events](http://boost-experimental.github.io/sml/tutorial/index.html#6-process-events)
    * [8. Handle errors](http://boost-experimental.github.io/sml/tutorial/index.html#8-handle-errors)
    * [9. Test it](http://boost-experimental.github.io/sml/tutorial/index.html#9-test-it)
    * [10. Debug it](http://boost-experimental.github.io/sml/tutorial/index.html#10-debug-it)
    * [ Workshop](http://boost-experimental.github.io/sml/tutorial/index.html#-workshop)
* [UML vs SML](http://boost-experimental.github.io/sml/uml_vs_sml/index.html)
    * [Unified Modeling Language™ (UML®) Version 2.5](http://boost-experimental.github.io/sml/uml_vs_sml/index.html#unified-modeling-language-uml-version-25)
    * [Initial Pseudostate](http://boost-experimental.github.io/sml/uml_vs_sml/index.html#initial-pseudostate)
    * [Terminate Pseudostate](http://boost-experimental.github.io/sml/uml_vs_sml/index.html#terminate-pseudostate)
    * [External transition](http://boost-experimental.github.io/sml/uml_vs_sml/index.html#external-transition)
    * [Anonymous transition](http://boost-experimental.github.io/sml/uml_vs_sml/index.html#anonymous-transition)
* [User Guide](http://boost-experimental.github.io/sml/user_guide/index.html)
    * [transitional [concept]](http://boost-experimental.github.io/sml/user_guide/index.html#transitional-concept)
    * [configurable [concept]](http://boost-experimental.github.io/sml/user_guide/index.html#configurable-concept)
    * [callable [concept]](http://boost-experimental.github.io/sml/user_guide/index.html#callable-concept)
    * [dispatchable [concept]](http://boost-experimental.github.io/sml/user_guide/index.html#dispatchable-concept)
    * [state [core]](http://boost-experimental.github.io/sml/user_guide/index.html#state-core)
    * [event [core]](http://boost-experimental.github.io/sml/user_guide/index.html#event-core)
    * [make_transition_table [state machine]](http://boost-experimental.github.io/sml/user_guide/index.html#make_transition_table-state-machine)
    * [sm [state machine]](http://boost-experimental.github.io/sml/user_guide/index.html#sm-state-machine)
    * [policies [state machine]](http://boost-experimental.github.io/sml/user_guide/index.html#policies-state-machine)
    * [testing::sm [testing]](http://boost-experimental.github.io/sml/user_guide/index.html#testingsm-testing)
    * [make_dispatch_table [utility]](http://boost-experimental.github.io/sml/user_guide/index.html#make_dispatch_table-utility)
* [Examples](http://boost-experimental.github.io/sml/examples/index.html)
    * [Hello World](http://boost-experimental.github.io/sml/examples/index.html#hello-world)
    * [Events](http://boost-experimental.github.io/sml/examples/index.html#events)
    * [States](http://boost-experimental.github.io/sml/examples/index.html#states)
    * [Actions Guards](http://boost-experimental.github.io/sml/examples/index.html#actions-guards)
    * [Transitions](http://boost-experimental.github.io/sml/examples/index.html#transitions)
    * [Defer/Process](http://boost-experimental.github.io/sml/examples/index.html#deferprocess)
    * [Orthogonal Regions](http://boost-experimental.github.io/sml/examples/index.html#orthogonal-regions)
    * [Composite](http://boost-experimental.github.io/sml/examples/index.html#composite)
    * [History](http://boost-experimental.github.io/sml/examples/index.html#history)
    * [Error handling](http://boost-experimental.github.io/sml/examples/index.html#error-handling)
    * [Logging](http://boost-experimental.github.io/sml/examples/index.html#logging)
    * [Nested](http://boost-experimental.github.io/sml/examples/index.html#nested)
    * [Testing](http://boost-experimental.github.io/sml/examples/index.html#testing)
    * [Runtime Dispatcher](http://boost-experimental.github.io/sml/examples/index.html#runtime-dispatcher)
    * [eUML Emulation](http://boost-experimental.github.io/sml/examples/index.html#euml-emulation)
    * [Dependencies](http://boost-experimental.github.io/sml/examples/index.html#dependencies)
    * [Data](http://boost-experimental.github.io/sml/examples/index.html#data)
    * [Dependency Injection](http://boost-experimental.github.io/sml/examples/index.html#dependency-injection)
    * [SDL2 Integration](http://boost-experimental.github.io/sml/examples/index.html#sdl2-integration)
    * [Plant UML Integration](http://boost-experimental.github.io/sml/examples/index.html#plant-uml-integration)
* [FAQ](http://boost-experimental.github.io/sml/faq/index.html)
* [CHANGELOG](http://boost-experimental.github.io/sml/CHANGELOG/index.html)
    * [ [1.1.0] - 2019-01-08](http://boost-experimental.github.io/sml/CHANGELOG/index.html#-110-2019-01-08)
    * [ [1.0.1] - 2016-05-06](http://boost-experimental.github.io/sml/CHANGELOG/index.html#-101-2016-05-06)
    * [[1.0.0] - 2016-01-28](http://boost-experimental.github.io/sml/CHANGELOG/index.html#100-2016-01-28)

[](GENERATE_TOC_END)

---

**Disclaimer** `Boost.SML` is not an official Boost library.
