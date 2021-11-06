###Quick Start

* Get [boost/sml.hpp](https://raw.githubusercontent.com/boost-ext/sml/master/include/boost/sml.hpp) header
```sh
wget https://raw.githubusercontent.com/boost-ext/sml/master/include/boost/sml.hpp
```

* Include the header and define `sml` namespace alias
```cpp
#include "boost/sml.hpp"
namespace sml = boost::sml;
```

* Compile with C++14 support
```sh
$CXX -std=c++14 ... | cl /std:c++14 ...
```

* To run tests
```sh
git clone https://github.com/boost-ext/sml && cd sml && make test
```

###Dependencies

* No external dependencies are required (neither STL nor Boost)

###Supported/Tested compilers

* [Clang-3.4+](https://travis-ci.org/boost-ext/sml)
* [GCC-5.2+](https://travis-ci.org/boost-ext/sml)
* [MSVC-2015](https://ci.appveyor.com/project/krzysztof-jusiak/sml)
    * Known limitations

```cpp
  "src_state"_s + event<e> = "dst_state"_s                                // Error on MSVC-2015, Ok on GCC-5+, Clang-3.4+
  state<class src_state> + event<e> = state<class dst_state>              // Ok on all supported compilers
```

```cpp
  const auto guard1 = [] { return true; }
  state<class a> + event<e> [ guard1 ] / [](const auto& event) {}          // Error on MSVC-2015, Ok on GCC-5+, Clang-3.4+

  const auto guard2 = [] -> bool { return true; }
  state<class a> + event<e> [ guard2 ] / [](const auto& event) -> void {}  // Ok on all supported compilers
```

###Configuration
| Macro                                                         | Description                                                  |
| --------------------------------------------------------------|--------------------------------------------------------------|
| `BOOST_SML_VERSION`                                           | Current version of [Boost].SML (ex. 1'0'0)                |


###Exception Safety

* [Boost].SML doesn't use exceptions internally and therefore might be compiled with `-fno-exceptions`.
* If guard throws an exception [State Machine](user_guide.md##sm-state-machine) will stay in a current state.
* If action throws an exception [State Machine](user_guide.md##sm-state-machine) will be in the new state
* Exceptions might be caught using transition table via `exception` event. See [Error handling](tutorial.md#8-error-handling).

###Thread Safety

* [Boost].SML is not thread safe by default.
  * Thread Safety might be enabled by defining a thread_safe policy when creating a State Machine. Lock type has to be provided.

```cpp
sml::sm<example, sml::thread_safe<std::recursive_mutex>> sm;
sm.process_event(event{}); // thread safe call
```

* See [Thread Safe Policy](user_guide.md#policies)

###Design

[![Design](images/design.png)](images/design.png)

| Component    | Description |
| ------------ | ----------- |
| [Front-End]  | Transition Table Domain Specific Language |
| [Back-End]   | [State Machine](user_guide.md##sm-state-machine) implementation details |

###Error messages

***Not configurable***
![CPP](https://raw.githubusercontent.com/boost-ext/sml/master/test/ft/errors/not_configurable.cpp)

***Not callable***
![CPP](https://raw.githubusercontent.com/boost-ext/sml/master/test/ft/errors/not_callable.cpp)

***Not transitional***
![CPP](https://raw.githubusercontent.com/boost-ext/sml/master/test/ft/errors/not_transitional.cpp)

***Not dispatchable***
![CPP](https://raw.githubusercontent.com/boost-ext/sml/master/test/ft/errors/not_dispatchable.cpp)

[Boost.MSM-eUML]: http://www.boost.org/doc/libs/1_60_0/libs/msm/doc/HTML/ch03s04.html
[Boost.MSM3-eUML2]: https://htmlpreview.github.io/?https://raw.githubusercontent.com/boostorg/msm/msm3/doc/HTML/ch03s05.html
[Boost.Statechart]: http://www.boost.org/doc/libs/1_60_0/libs/statechart/doc/tutorial.html
