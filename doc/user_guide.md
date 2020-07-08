###transitional [concept]

***Header***

    #include <boost/sml.hpp>

***Description***

Requirements for transition.

***Synopsis***

    template <class T>
    concept bool transitional() {
      return requires(T transition) {
        typename T::src_state;
        typename T::dst_state;
        typename T::event;
        typename T::deps;
        T::initial;
        T::history;
        { transition.execute() } -> bool;
      }
    };

***Semantics***

    transitional<T>

***Example***

    using namespace sml;

    {
    auto transition = ("idle"_s = X); // Postfix Notation
    static_assert(transitional<decltype(transition)>::value);
    }

    {
    auto transition = (X <= "idle"_s); // Prefix Notation
    static_assert(transitional<decltype(transition)>::value);
    }

![CPP(BTN)](Run_Transitional_Example|https://raw.githubusercontent.com/boost-ext/sml/master/example/errors/not_transitional.cpp)

&nbsp;

---

###configurable [concept]

***Header***

    #include <boost/sml.hpp>

***Description***

Requirements for the state machine.

***Synopsis***

    template <class SM>
    concept bool configurable() {
      return requires(SM sm) {
        { sm.operator()() };
      }
    };

***Semantics***

    configurable<SM>

***Example***

    class example {
      auto operator()() const noexcept {
        return make_transition_table();
      }
    };

    static_assert(configurable<example>::value);

![CPP(BTN)](Run_Configurable_Example|https://raw.githubusercontent.com/boost-ext/sml/master/example/errors/not_configurable.cpp)

&nbsp;

---

###callable [concept]

***Header***

    #include <boost/sml.hpp>

***Description***

Requirements for action and guards.

***Synopsis***

    template <class TResult, class T>
    concept bool callable() {
      return requires(T object) {
        { object(...) } -> TResult;
      }
    }

***Semantics***

    callable<SM>

***Example***

    auto guard = [] { return true; };
    auto action = [] { };

    static_assert(callable<bool, decltype(guard)>::value);
    static_assert(callable<void, decltype(action)>::value);

![CPP(BTN)](Run_Callable_Example|https://raw.githubusercontent.com/boost-ext/sml/master/example/errors/not_callable.cpp)

&nbsp;

---

###dispatchable [concept]

***Header***

    #include <boost/sml.hpp>

***Description***

Requirements for the dispatch table.

***Synopsis***

    template <class TDynamicEvent, TEvent>
    concept bool dispatchable() {
      return requires(T) {
        typename TEvent::id;
        { TEvent(declval<TDynamicEvent>()) };
      }
    };

***Semantics***

    dispatchable<SM>

***Example***

    struct runtime_event { };

    struct event1 {
      static constexpr auto id = 1;
    };

    struct event2 {
      static constexpr auto id = 2;
      explicit event2(const runtime_event&) {}
    };

    static_assert(dispatchable<runtime_event, event1>::value);
    static_assert(dispatchable<runtime_event, event2>::value);

![CPP(BTN)](Run_Dispatchable_Example|https://raw.githubusercontent.com/boost-ext/sml/master/example/errors/not_dispatchable.cpp)
![CPP(BTN)](Run_SDL2_Integration_Example|https://raw.githubusercontent.com/boost-ext/sml/master/example/sdl2.cpp)

&nbsp;

---

###state [core]

***Header***

    #include <boost/sml.hpp>

***Description***

Represents a state machine state.

***Synopsis***

    template<class TState> // no requirements, TState may be a state machine
    class state {
    public:
      initial operator*() const noexcept; // no requirements

      template <class T> // no requirements
      auto operator<=(const T &) const noexcept;

      template <class T> // no requirements
      auto operator=(const T &) const noexcept;

      template <class T> // no requirements
      auto operator+(const T &) const noexcept;

      template <class T> requires callable<bool, T>
      auto operator[](const T) const noexcept;

      template <class T> requires callable<void, T>
      auto operator/(const T &t) const noexcept;

      const char* c_str() noexcept;
    };

    template <class T, T... Chrs>
    state<unspecified> operator""_s() noexcept;

    // predefined states
    state<unspecified> X;

***Requirements***

* [callable](#callable-concept)

***Semantics***

    state<T>{}

***Example***

    auto idle = state<class idle>;
    auto idle = "idle"_s;

    auto initial_state = *idle;
    auto history_state = idle(H);
    auto terminate_state = X;

![CPP(BTN)](Run_States_Example|https://raw.githubusercontent.com/boost-ext/sml/master/example/states.cpp)
![CPP(BTN)](Run_Composite_Example|https://raw.githubusercontent.com/boost-ext/sml/master/example/composite.cpp)
![CPP(BTN)](Run_Orthogonal_Regions_Example|https://raw.githubusercontent.com/boost-ext/sml/master/example/orthogonal_regions.cpp)

&nbsp;

---

###event [core]

***Header***

    #include <boost/sml.hpp>

***Description***

Represents a state machine event.

***Synopsis***

    template<TEvent> // no requirements
    class event {
    public:
      template <class T> requires callable<bool, T>
      auto operator[](const T &) const noexcept;

      template <class T> requires callable<void, T>
      auto operator/(const T &t) const noexcept;
    };

    template<class TEvent>
    event<TEvent> event{};

    // predefined events
    auto on_entry = event<unspecified>;
    auto on_exit = event<unspecified>;

    template<class TEvent> unexpected_event{};
    template<class T> exception{};

***Requirements***

* [callable](#callable-concept)

***Semantics***

    event<T>

***Example***

    auto my_int_event = event<int>;

![CPP(BTN)](Run_Events_Example|https://raw.githubusercontent.com/boost-ext/sml/master/example/action_guards.cpp)
![CPP(BTN)](Run_Error_Handling_Example|https://raw.githubusercontent.com/boost-ext/sml/master/example/error_handling.cpp)

&nbsp;

---

###make_transition_table [state machine]

***Header***

    #include <boost/sml.hpp>

***Description***

Creates a transition table.

***Synopsis***

    template <class... Ts> requires transitional<Ts>...
    auto make_transition_table(Ts...) noexcept;

***Requirements***

* [transitional](#transitional-concept)

***Semantics***

    make_transition_table(transitions...);

***Example***

    auto transition_table_postfix_notation = make_transition_table(
      *"idle_s" + event<int> / [] {} = X
    );

    auto transition_table_prefix_notation = make_transition_table(
      X <= *"idle_s" + event<int> / [] {}
    );

    class example {
    public:
      auto operator()() const noexcept {
        return make_transition_table();
      }
    };

![CPP(BTN)](Run_Transition_Table_Example|https://raw.githubusercontent.com/boost-ext/sml/master/example/transitions.cpp)

&nbsp;

---

###sm [state machine]

***Header***

    #include <boost/sml.hpp>

***Description***

Creates a State Machine.

***Synopsis***

    template<class T> requires configurable<T>
    class sm {
    public:
      using states = unspecified; // unique list of states
      using events = unspecified; // unique list of events which can be handled by the State Machine
      using transitions = unspecified; // list of transitions

      sm(sm &&) = default;
      sm(const sm &) = delete;
      sm &operator=(const sm &) = delete;

      template <class... TDeps> requires is_base_of<TDeps, dependencies>...
      sm(TDeps&&...) noexcept;

      template<class TEvent> // no requirements
      bool process_event(const TEvent&)

      template <class TVisitor> requires callable<void, TVisitor>
      void visit_current_states(const TVisitor &) const noexcept(noexcept(visitor(state{})));

      template <class TState>
      bool is(const state<TState> &) const noexcept;

      template <class... TStates> requires sizeof...(TStates) == number_of_initial_states
      bool is(const state<TStates> &...) const noexcept;
    };

| Expression | Requirement | Description | Returns |
| ---------- | ----------- | ----------- | ------- |
| `TDeps...` | is_base_of dependencies | constructor | |
| `process_event<TEvent>` | - | process event `TEvent` | returns true when handled, false otherwise |
| `visit_current_states<TVisitor>` | [callable](#callable-concept) | visit current states | - |
| `is<TState>` | - | verify whether any of current states equals `TState` | true when any current state matches `TState`, false otherwise |
| `is<TStates...>` | size of TStates... equals number of initial states | verify whether all current states match `TStates...` | true when all states match `TState...`, false otherwise |

***Semantics***

    sml::sm<T>{...};
    sm.process_event(TEvent{});
    sm.visit_current_states([](auto state){});
    sm.is(X);
    sm.is(s1, s2);

***Example***

    struct my_event {};

    class example {
    public:
      auto operator()() const noexcept {
        using namespace sml;
        return make_transition_table(
          *"idle"_s + event<my_event> / [](int i) { std::cout << i << std::endl; } = X
        );
      }
    };

    sml::sm<example> sm{42};
    assert(sm.is("idle"_s));
    sm.process_event(int{}); // no handled, will call unexpected_event<int>
    sm.process_event(my_event{}); // handled
    assert(sm.is(X));

    sm.visit_current_states([](auto state) { std::cout << state.c_str() << std::endl; });

![CPP(BTN)](Run_Hello_World_Example|https://raw.githubusercontent.com/boost-ext/sml/master/example/hello_world.cpp)
![CPP(BTN)](Run_Dependency_Injection_Example|https://raw.githubusercontent.com/boost-ext/sml/master/example/dependency_injection.cpp)
![CPP(BTN)](Run_eUML_Emulation_Example|https://raw.githubusercontent.com/boost-ext/sml/master/example/euml_emulation.cpp)

&nbsp;

---

###policies [state machine]

***Header***

    #include <boost/sml.hpp>

***Description***

Additional State Machine configurations.

***Synopsis***

    thread_safe<Lockable>
    logger<Loggable>

| Expression | Requirement | Description | Example |
| ---------- | ----------- | ----------- | ------- |
| `Lockable` | `lock/unlock` | Lockable type | `std::mutex`, `std::recursive_mutex` |
| `Loggable` | `log_process_event/log_state_change/log_action/log_guard` | Loggable type | - |

***Example***

    sml::sm<example, sml::thread_safe<std::recursive_mutex>> sm; // thread safe policy
    sml::sm<example, sml::logger<my_logger>> sm; // logger policy
    sml::sm<example, sml::thread_safe<std::recursive_mutex>, sml::logger<my_logger>> sm; // thread safe and logger policy
    sml::sm<example, sml::logger<my_logger>, sml::thread_safe<std::recursive_mutex>> sm; // thread safe and logger policy

![CPP(BTN)](Run_Logging_Example|https://raw.githubusercontent.com/boost-ext/sml/master/example/logging.cpp)

&nbsp;

---

###testing::sm [testing]

***Header***

    #include <boost/sml/testing/state_machine.hpp>

***Description***

Creates a state machine with testing capabilities.

***Synopsis***

    namespace testing {
      template <class T>
      class sm : public sml::sm<T> {
       public:
        using sml::sm<T>::sm;

        template <class... TStates>
        void set_current_states(const detail::state<TStates> &...) noexcept;
      };
    }

| Expression | Requirement | Description | Returns |
| ---------- | ----------- | ----------- | ------- |
| `set_current_states<TStates...>` | - | set current states | |

***Semantics***

    sml::testing::sm<T>{...};
    sm.set_current_states("s1"_s);

***Example***

    sml::testing::sm<T>{inject_fake_data...};
    sm.set_current_states("s1"_s);
    sm.process_event(TEvent{});
    sm.is(X);

![CPP(BTN)](Run_Testing_Example|https://raw.githubusercontent.com/boost-ext/sml/master/example/testing.cpp)

&nbsp;

---

###make_dispatch_table [utility]

***Header***

    #include <boost/sml/utility/dispatch_table.hpp>

***Description***

Creates a dispatch table to handle runtime events.

***Synopsis***

    namespace utility {
      template<class TEvent, int EventRangeBegin, int EventRangeBegin, class SM> requires dispatchable<TEvent, typename SM::events>
      callable<bool, (TEvent, int)> make_dispatch_table(sm<SM>&) noexcept;
    }

***Requirements***

* [dispatchable](#dispatchable-concept)

***Semantics***

    sml::utility::make_dispatch_table<T, 0, 10>(sm);

***Example***

    struct runtime_event {
      int id = 0;
    };
    struct event1 {
      static constexpr auto id = 1;
      event1(const runtime_event &) {}
    };

    auto dispatch_event = sml::utility::make_dispatch_table<runtime_event, 1 /*min*/, 5 /*max*/>(sm);
    dispatch_event(event, event.id);

![CPP(BTN)](Run_Dispatch_Table_Example|https://raw.githubusercontent.com/boost-ext/sml/master/example/dispatch_table.cpp)
![CPP(BTN)](Run_SDL2_Integration_Example|https://raw.githubusercontent.com/boost-ext/sml/master/example/sdl2.cpp)

&nbsp;

---
