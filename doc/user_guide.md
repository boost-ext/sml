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

[▶ See the compile error on Compiler Explorer](https://godbolt.org/z/3aWdcTanT)

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

[▶ See the compile error on Compiler Explorer](https://godbolt.org/z/zehzezMoe)

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

[▶ See the compile error on Compiler Explorer](https://godbolt.org/z/fb8K6q4WK)

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

[▶ See the compile error on Compiler Explorer](https://godbolt.org/z/984KoaP6x)
[▶ Run on Compiler Explorer](https://godbolt.org/z/8heY58eE6)

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

[▶ Run on Compiler Explorer](https://godbolt.org/z/c9asGjYTr)
[▶ Run on Compiler Explorer](https://godbolt.org/z/5sYb9eGbe)
[▶ Run on Compiler Explorer](https://godbolt.org/z/vqGbsrqM3)

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

[▶ Run on Compiler Explorer](https://godbolt.org/z/nehrdofPx)
[▶ Run on Compiler Explorer](https://godbolt.org/z/cGd7TnM1G)

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

[▶ Run on Compiler Explorer](https://godbolt.org/z/qr8qs78Ts)

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

      void flush_queue(); // drain events still pending in the process queue

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
| `flush_queue` | requires a `process_queue` / `defer_queue` policy | drain events still pending in the process queue (e.g. pushed from an async handler after `process_event` returned); no-op when empty | - |
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

[▶ Run on Compiler Explorer](https://godbolt.org/z/dq85hW4ab)
[▶ Run on Compiler Explorer](https://godbolt.org/z/1Pv6EaG7n)
[▶ Run on Compiler Explorer](https://godbolt.org/z/n93Y5Mner)

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
    deps<Ts...>

| Expression | Requirement | Description | Example |
| ---------- | ----------- | ----------- | ------- |
| `Lockable` | `lock/unlock` | Lockable type | `std::mutex`, `std::recursive_mutex` |
| `Loggable` | `log_process_event/log_state_change/log_action/log_guard` | Loggable type | - |
| `deps<Ts...>` | - | force `Ts...` into the dependency pool even when no action/guard signature names them (needed when a dependency is only reached via a generic lambda using `sml::aux::get<Dep&>(deps)`) | `sml::sm<example, sml::deps<MyDep>> sm{dep};` |

***Example***

    sml::sm<example, sml::thread_safe<std::recursive_mutex>> sm; // thread safe policy
    sml::sm<example, sml::logger<my_logger>> sm; // logger policy
    sml::sm<example, sml::thread_safe<std::recursive_mutex>, sml::logger<my_logger>> sm; // thread safe and logger policy
    sml::sm<example, sml::logger<my_logger>, sml::thread_safe<std::recursive_mutex>> sm; // thread safe and logger policy
    sml::sm<example, sml::deps<MyDep>> sm{dep}; // explicit pool dependency for generic-lambda actions/guards

[▶ Run on Compiler Explorer](https://godbolt.org/z/Tec7jz36r)

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

[▶ Run on Compiler Explorer](https://godbolt.org/z/oc73h5sY8)

&nbsp;

---

###make_action [utility]

***Header***

    #include <boost/sml.hpp>

***Description***

Wraps a template/generic/constrained callable as an action or guard, naming its
dependency (and event) types explicitly via `<Deps...>`. SML normally deduces the
parameters a callable wants by inspecting its `operator()`; a generic lambda
(`[](auto&){}`) or a concept-constrained one hides that information, so the wrong
type may be deduced. `make_action<Deps...>(f)` supplies the concrete signature
`f` should be called with.

***Synopsis***

    template <class... Deps, class F>
    constexpr auto make_action(F f) noexcept;

***Semantics***

    make_action<Dep&>(generic_callable);
    make_action<const Event&, Dep&>(generic_callable);

***Example***

    struct trigger {};
    struct Counter { int n = 0; };

    auto table = [] {
      using namespace sml;
      return make_transition_table(
        // generic lambda: name the dependency explicitly so the right type is passed
        *"idle"_s + event<trigger> / make_action<Counter&>([](auto& c) { c.n++; }) = X
      );
    };

Also works for guards and for callables taking the event plus dependencies:

    *"idle"_s + event<trigger>[make_action<Counter&>([](auto& c) { return c.n == 0; })] = X
    *"idle"_s + event<trigger> / make_action<const trigger&, Counter&>(
                                   [](const auto& e, auto& c) { /* ... */ }) = X

[▶ Run on Compiler Explorer](https://godbolt.org/z/Ge84qGM5z)

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

[▶ Run on Compiler Explorer](https://godbolt.org/z/j5bMnobY9)
[▶ Run on Compiler Explorer](https://godbolt.org/z/8heY58eE6)

&nbsp;

---
