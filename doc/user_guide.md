###Concepts

* Transitional
* Configurable
* Callable
* Dispatchable

###State

###Event
* on\_entry/on\_exit

###Transition Table

***Description***

Creates transition table.

***Synopsis***

    template <class... Ts>
    auto make_transition_table(Ts...) noexcept;


***Semantics***

    make_transition_table(transitions...);

***Requirements***

| Parameter | Requirement | Description | Returns |
| --------- | ----------- | ----------- | ------- |
| Ts        | transitional| transitions | list of transitions |

***Header***

    #include <boost/msm/msm.hpp>


***Example***

![CPP(TEST)](https://raw.githubusercontent.com/boost-experimental/msm-lite/master/example/hello_world.cpp)


###State Machine

###Dispatch Table
