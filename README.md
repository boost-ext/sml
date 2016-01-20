<a href="http://www.boost.org/LICENSE_1_0.txt" target="_blank">![Boost Licence](http://img.shields.io/badge/license-boost-blue.svg)</a>
<a href="https://github.com/krzysztof-jusiak/msm-lite/releases" target="_blank">![Version](https://badge.fury.io/gh/krzysztof-jusiak%2Fmsm-lite.svg)</a>
<a href="https://github.com/krzysztof-jusiak/msm-lite/releases/latest" target="_blank">![Github Release](http://img.shields.io/github/release/krzysztof-jusiak/msm-lite.svg)</a>
<a href="https://travis-ci.org/krzysztof-jusiak/msm-lite" target="_blank">![Build Status](https://img.shields.io/travis/krzysztof-jusiak/msm-lite/cpp14.svg?label=linux/osx)</a>
<a href="http://github.com/krzysztof-jusiak/msm-lite/issues" target="_blank">![Github Issues](https://img.shields.io/github/issues/krzysztof-jusiak/msm-lite.svg)</a>
<a href="https://gitter.im/krzysztof-jusiak/msm-lite" target="_blank">![Gitter Chat](https://img.shields.io/badge/gitter-join%20chat%20%E2%86%92-brightgreen.svg)</a>
<a href="http://waffle.io/krzysztof-jusiak/msm-lite" target="_blank">![Stories in Ready](https://badge.waffle.io/krzysztof-jusiak/msm-lite.svg?label=ready&title=Ready)</a>

msm-lite: C++14 Meta State Machine Library
===============================================
Yours scalable C++14 header only eUML-like meta state machine library with no dependencies

> **Hello world example**

```cpp
#include "msm.hpp"

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};

auto guard1 = [] { return true; };
auto guard2 = [](int i, auto event) {
    return i == 42 && is_same<decltype(event), e1>::value;
};
auto action1 = [] { };
auto action2 = [](const auto& event, int i) {
    cout << i << " " << typeid(event).name() << endl;
};

class hello_world {
public:
  auto configure() const noexcept {
    using namespace msm; // bring operators support
    return make_transition_table(
        "idle"_s(initial) == "s1"_s + event<e1> [ guard2 ] / action1
      , "s1"_s == "s2"_s + event<e2> / [] { cout << "in place action" << endl; }
      , "s2"_s == "s3"_s + event<e3> / [ guard1 && !guard2 ] / (action1, action2)
      , "s2"_s == "game_over"_s + event<e4>
      ,
    );
  }
};

int main() {
  msm::sm<hello_world> sm{42/*int dependency to be injected into guards/actions*/}
  assert(sm.process_event(e1{}));
  assert(sm.process_event(e2{}));
  assert(sm.process_event(e3{}));
  assert(sm.process_event(e4{}));
  sm.visit_current_states([](auto state){
    static_assert(is_same<decltype(state), "game_over"_s>::value);}
  );
}
```

