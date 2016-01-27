###transitional [concept]

*Description*

Requirements for transition.

*Synopsis*

    template <class T>
    concept bool transitional() {
      return requires(T transition) {
      {
        typename T::src_state;
        typename T::dst_state;
        typename T::event;
        typename T::deps;
        T::has_initial;
        { transition.execute() } -> bool;
      }
    }

*Semantics*

    transitional<T>

*Header*

    #include <boost/msm.hpp>

*Example*

    using namespace msm;
    auto transition = ("idle"_s == terminate);
    static_assert(transitional<decltype(transition)>);

---

###configurable [concept]

*Description*

Requirements for the state machine.

*Synopsis*

    template <class SM>
    concept bool configurable() {
      return requires(SM sm) {
        { sm.configure() };
      }
    }

*Semantics*

    configurable<SM>

*Header*

    #include <boost/msm.hpp>

*Example*

    class example {
      auto configure() const noexcept {
        return make_transition_table();
      }
    };

    static_assert(configurable<example>);

---

###callable [concept]

*Description*

Requirements for action and guards.

*Synopsis*

    template <class TResult, class T>
    concept bool callable() {
      return requires(T object) {
        { object(...) } -> TResult;
      }
    }

*Semantics*

    callable<SM>

*Header*

    #include <boost/msm.hpp>

*Example*

    auto guard = [] { return true; };
    auto action = [] { };

    static_assert(callable<bool, decltype(guard)>);
    static_assert(callable<void, decltype(action)>);

---

###dispatchable [concept]

*Description*

Requirements for the dispatch table.

*Synopsis*

    template <class TDynamicEvent, TEvent>
    concept bool dispatchable() {
      return requires(T) {
        typename TEvent::id;
        { TEvent(declval<TDynamicEvent>()) };
      }
    }

*Semantics*

    dispatchable<SM>

*Header*

    #include <boost/msm.hpp>

*Example*

    struct runtime_event { };

    struct event {
      static constexpr auto id = 42;
      event(runtime_event) {}
    };

    static_assert(dispatchable<runtime_event, event>);

---

###state [core]

*Description*

Represents a state machine state.

*Synopsis*

    template<class TState> // no requirements, TState may be a state machine
    class state {
    public:
      template <class T> // no requirements
      auto operator==(const T &) const noexcept;

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
    state<unspecified> terminate;
    state<unspecified> initial;

*Semantics*

    state<T>{}

*Header*

    #include <boost/msm.hpp>

*Example*

    state<class idle> idle;
    auto idle = state<class idle>{};
    auto idle = "idle"_s;

    auto initial_state = idle(initial);

    auto last_state = terminate;

---

###event [core]

*Description*

Represents a state machine event.

*Synopsis*

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

*Semantics*

    event<T>

*Header*

    #include <boost/msm.hpp>

*Example*

    auto my_int_event = event<int>;

---

###make_transition_table [state machine]

*Description*

Creates a transition table.

*Synopsis*

    template <class... Ts> requires transitional<Ts>...
    auto make_transition_table(Ts...) noexcept;

*Semantics*

    make_transition_table(transitions...);

*Header*

    #include <boost/msm.hpp>

*Example*

    auto transition_table = make_transition_table(
      "idle_s"(initial) == terminate + event<int> / [] {}
    );

    class example {
    public:
      auto configure() const noexcept {
        return make_transition_table();
      }
    };

![CPP(TEST)](https://raw.githubusercontent.com/boost-experimental/msm-lite/master/example/hello_world.cpp)

---

###sm [state machine]

*Description*

Creates a state machine.

*Synopsis*

    template<class T> requires configurable<T>
    class sm {
    public:
      using states = unspecified;
      using events = unspecified;

      sm(sm &&) = default;
      sm(const sm &) = delete;
      sm &operator=(const sm &) = delete;

      template <class... TDeps> requires dependable<TDeps>...
      sm(TDeps&&...) noexcept;

      template<class TEvent> // no requirements
      bool process_event(const TEvent&) noexcept;

      template <class TVisitor> requires callable<void, TVisitor>
      void visit_current_states(const TVisitor &) const noexcept(noexcept(visitor(state{})));

      template <class TState>
      bool is(const state<TState> &) const noexcept;

      template <class... TStates> requires sizeof...(TStates) == number_of_initial_states
      bool is(const state<TStates> &...) const noexcept;
    };

| Function  | Requirement | Description | Returns |
| --------- | ----------- | ----------- | ------- |
| `process_event<TEvent>` | - | process event `TEvent` | returns true when handled, false otherwise |
| `visit_current_states<TVisitor>` | callable | visit current states | - |
| `is<TState>` | - | verify whether any of current states equals `TState` | true when any current state matches `TState`, false otherwise |
| `is<TStates...>` | size of TStates... equals number of initial states | verify whether all current states match `TStates...` | true when all states match `TState...`, false otherwise |

*Semantics*

    sm<T>{...};
    sm.process_event(TEvent{});
    sm.visit_current_states([](auto state){});
    sm.is(terminate);
    sm.is(s1, s2);

*Header*

    #include <boost/msm.hpp>

*Example*

    struct my_event {};

    class example {
    public:
      auto configure() const noexcept {
      using namespace msm;
        return make_transition_table(
          "idle"_s(initial) == terminate + event<my_event> / [](int i) { std::cout << i << std::endl; }
        );
      }
    };

    sm<example> sm{42};
    assert(sm.is("idle"_s));
    assert(!sm.process_event(int{})); // no handled
    assert(sm.process_event(my_event{})); // handled
    assert(sm.is(terminate));

    sm.visit_current_states([](auto state) { std::cout << state.c_str() << std::endl; });

![CPP(TEST)](https://raw.githubusercontent.com/boost-experimental/msm-lite/master/example/hello_world.cpp)

---

###make_dispatch_table [extension]

*Description*

Creates a dispatch table to handle runtime events.

*Synopsis*

    template<class TEvent, int EventRangeBegin, int EventRangeBegin, class SM> requires dispatchable<TEvent, typename SM::events>
    callable<bool, (TEvent, int)> make_dispatch_table(sm<SM>&) noexcept;

*Semantics*

    make_dispatch_table<T, 0, 10>(sm);

*Header*

    #include <boost/msm.hpp>

*Example*

    struct runtime_event {
      int id = 0;
    };
    struct event1 {
      static constexpr auto id = 1;
      event1(const runtime_event &) {}
    };

    auto dispatch_event = msm::make_dispatch_table<runtime_event, 1 /*min*/, 5 /*max*/>(sm);
    assert(dispatch_event(event, event.id));

![CPP(TEST)](https://raw.githubusercontent.com/boost-experimental/msm-lite/master/example/hello_world.cpp)

---
