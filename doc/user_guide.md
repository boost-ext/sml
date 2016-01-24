###make_transition_table

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

![CPP_TEST](https://raw.githubusercontent.com/boost-experimental/msm-lite/master/example/hello_world.cpp)

###Concepts.Transitional

###Concepts.Configurable

###Concepts.Callable

