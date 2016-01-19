Introduction
============

msm-lite: C++14 Meta State Machine Library

> Yours scalable C++14 header only eUML-like meta state machine library with no dependencies

![CPP](https://raw.githubusercontent.com/krzysztof-jusiak/msm-lite/master/example/action_guards.cpp)
```cpp
    return make_transition_table(
        idle(initial) == s1 + event<e1>
      , s1 == s2 + event<e2> [ guard1 ] / action1
      , s2 == s3 + event<e3> [ guard1 && ![] { return false;} ] / (action1, action2{})
      , s3 == s4 + event<e4> [ !guard1 || guard2 ] / (action1, [] { std::cout << "action3" << std::endl; })
      , s3 == terminate + event<e4> [ guard1 ] / ([] { std::cout << "action4" << std::endl; })
    );
```

