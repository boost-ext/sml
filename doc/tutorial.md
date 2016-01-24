* [Boost.MSM - UML Short Guide](http://www.boost.org/doc/libs/1_60_0/libs/msm/doc/HTML/ch02.html)
* [Boost.MSM - eUML Documentation](http://www.boost.org/doc/libs/1_60_0/libs/msm/doc/HTML/ch03s04.html)

* Transition Table DSL

    | Expression | Description |
    |------------|-------------|
    | src\_state == dst\_state + event<e> [ guard && (![]{return true;} && guard2) ] / (action, action2, []{}) | transition from src\_state to dst\_state on event e with guard and action |
    | src\_state == dst\_state + event<e> [ guard ] / action | transition from src\_state to dst\_state on event e with guard and action |
    | src\_state == dst\_state / [] {} | anonymous transition with action |
    | src\_state == dst\_state + event<e> | transition on event e without guard or action |
    | state + event<e> [ guard ] | internal transition on event e when guard |

* Action/Guards data dependencies
```cpp
                             /---- event
                            |
auto guard = [](double d, auto event) { return true; }
                   |
                   \-------\
                           |
auto action = [](int i){}  |
                 |         |
                 |         |
            /---/  /------/
           |      /
sm<...> s{42, 87.0};
```

