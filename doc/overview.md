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

###Supported/Tested compilers

* [Clang-3.4+](https://travis-ci.org/boost-experimental/msm-lite)
* [GCC-5.2+](https://travis-ci.org/boost-experimental/msm-lite)

###Configuration
| Macro                                                         | Description                                                  |
| --------------------------------------------------------------|--------------------------------------------------------------|
| `BOOST_MSM_LITE_VERSION`                                      | Current version of Boost.MSM-lite (ex. 1'0'0)                |
| [`BOOST_MSM_LITE_LOG`](user_guide.md#boost_msm_log-debugging) | Enables logging system  (see [Logging](examples.md#logging)) |

###Exception Safety

* Boost.MSM-lite doesn't use exceptions internally and therefore might be compiled with `-fno-exceptions`.
* Check [User Guide](user_guide.md) to verify which API's are marked `noexcept`.
* If guard/action throws an exception [State Machine](user_guide.md##sm-state-machine) will stay in a current state.
* Exceptions might be caught using transition table via `exception` event. See [Error handling](tutorial.md#8-error-handling).

###Thread Safety

* Boost.MSM-lite is thread safe.

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
