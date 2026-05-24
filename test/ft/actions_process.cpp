//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <queue>
#include <string>
#include <utility>
#include <vector>

#include "static_queue.h"

namespace sml = boost::sml;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};

const auto idle = sml::state<class idle>;
const auto s1 = sml::state<class s1>;
const auto s2 = sml::state<class s2>;
const auto s3 = sml::state<class s3>;

// Regression fixture for issue #465.
// Defined at file scope so sm<c465> can be named without circular instantiation:
// the action lambda calls do_reentrant (a plain function pointer set in the test),
// which in turn calls sm.process_event — without any sm<c465> reference in operator().
struct c465 {
  bool (*do_reentrant)(void*) = nullptr;  // filled in by the test after sm construction
  void* sm_ptr = nullptr;
  int e2_action_count = 0;
  bool first_dispatch = true;

  auto operator()() noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
        *state<class s1_465> + event<e1> / process(e2{}) = state<class s2_465>
      , state<class s2_465> + event<e2> / [this] {
            ++e2_action_count;
            if (first_dispatch) {
              first_dispatch = false;
              if (do_reentrant) do_reentrant(sm_ptr);  // re-entrant sm.process_event(e3)
            }
          } = sml::X
    );
    // clang-format on
  }
};

test process_event = [] {
  struct c {
    auto operator()() {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = s1
        , s1 + event<e2> / process(e3{})
        , s1 + event<e3> / [this] { a_called++; } = X
      );
      // clang-format on
    }

    int a_called = 0;
  };

  sml::sm<c, sml::process_queue<std::queue>> sm{};
  c& c_ = sm;
  expect(sm.is(idle));
  expect(!sm.process_event(e4{}));
  expect(sm.process_event(e1{}));
  expect(sm.is(s1));
  expect(!c_.a_called);
  expect(sm.process_event(e2{}));  // + process(e3{})
  expect(1 == c_.a_called);
  expect(sm.is(sml::X));
};

test process_event_from_substate = [] {
  struct sub {
    auto operator()() const noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *s1 + event<e1> = s2
        , s2 + event<e2> / process(e3{})
        , s2 + event<e3> / process(e2{}) = s3
        , s3 + event<e3> = X
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = state<sub>
        , state<sub> + event<e4> / [this] { a_called++; }
      );
      // clang-format on
    }

    int a_called = 0;
  };

  sml::sm<c, sml::process_queue<std::queue>> sm;
  const c& c_ = sm;
  expect(sm.is(idle));
  expect(sm.is<decltype(sml::state<sub>)>(s1));

  expect(sm.process_event(e1{}));
  expect(sm.is(sml::state<sub>));
  expect(sm.is<decltype(sml::state<sub>)>(s1));
  expect(!c_.a_called);

  expect(sm.process_event(e1{}));
  expect(sm.is(sml::state<sub>));
  expect(sm.is<decltype(sml::state<sub>)>(s2));
  expect(!c_.a_called);

  expect(!sm.process_event(e2{}));  // + process(e3{}) + process(e2{})
  expect(sm.is(sml::state<sub>));
  expect(sm.is<decltype(sml::state<sub>)>(s3));
  expect(!c_.a_called);

  expect(sm.process_event(e3{}));
  expect(sm.is(sml::state<sub>));
  expect(sm.is<decltype(sml::state<sub>)>(sml::X));

  expect(sm.process_event(e4{}));
  expect(1 == c_.a_called);
};

test queue_process_events = [] {
  struct c {
    std::vector<int> calls;

    auto operator()() {
      using namespace sml;

      const auto a0 = [this] { calls.push_back(0); };
      const auto a1 = [this] { calls.push_back(1); };
      const auto a2 = [this] { calls.push_back(2); };

      // clang-format off
      return make_transition_table(
        * idle + on_entry<sml::initial> / (process(e1()), process(e2()), a0)
        , idle + event<e1> / a1
        , idle + event<e2> / a2 = X
      );
      // clang-format on
    }
  };

  sml::sm<c, sml::process_queue<std::queue>> sm{};
  expect(sm.is(sml::X));

  const c& c_ = sm;
  expect(3u == c_.calls.size());
  expect(0 == c_.calls[0]);
  expect(1 == c_.calls[1]);
  expect(2 == c_.calls[2]);
};

test queue_and_internal_process_events = [] {
  struct c {
    auto operator()() {
      using namespace sml;

      // clang-format off
      return make_transition_table(
        * idle + on_entry<sml::initial> / process(e1())
        , s1 <= idle + event<e1>
        , s2 <= s1 / process(e1())  // This process is unhandled, but the state machine still transitions to s2
        , s2 + on_entry<_> / process(e2())
        , s3 <= s2 + event<e2>
        , X <= s3
      );
      // clang-format on
    }
  };

  sml::sm<c, sml::process_queue<std::queue>> sm{};
  expect(sm.is(sml::X));
};

test queue_process_reaction = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        * s1 + on_entry<sml::initial> / [this] { calls += "s1e|"; }
        , s1 + event<e1> / [this](sml::back::process<e2, e3> processEvent) -> void {
            calls += "a1.begin|";
            processEvent(e2{});
            processEvent(e3{});
            calls += "a1.end|";
          }
         , s1 + event<e2> / [this] { calls += "a2|"; }
         , s2 + event<e3> / [this] { calls += "a3|"; }
         , s1 + unexpected_event<e3> / [this] { calls += "err|"; } = X
      );
      // clang-format on
    }

    std::string calls{};
  };

  sml::sm<c, sml::process_queue<std::queue>> sm{};

  expect(!sm.process_event(e1()));  // e3 was unexpected so it returns false
  expect(sm.is(sml::X));

  const c& c_ = sm;
  expect("s1e|a1.begin|a1.end|a2|err|" == c_.calls);
};

test mix_process_and_internal = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        * s1 + event<e1> / process(e2{})
        , s1 + event<e2> = s2
        , s2 = X
      );
      // clang-format on
    }
  };

  sml::sm<c, sml::process_queue<std::queue>> sm{};

  expect(sm.process_event(e1{}));
  expect(sm.is(sml::X));
};

test process_event_sent_from_substate = [] {
  struct sub {
    auto operator()() const noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *s1 + event<e1> / process(e2{}) = X
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = state<sub>
        , state<sub> + event<e2> = X
        ,*s2 + event<e2> = X
      );
      // clang-format on
    }
  };

  sml::sm<c, sml::process_queue<std::queue>> sm;
  expect(sm.is(idle, s2));
  expect(sm.is<decltype(sml::state<sub>)>(s1));

  expect(sm.process_event(e1{}));
  expect(sm.is(sml::state<sub>, s2));
  expect(sm.is<decltype(sml::state<sub>)>(s1));

  expect(sm.process_event(e1{}));  // + process(e2{})
  expect(sm.is(sml::X, sml::X));
  expect(sm.is<decltype(sml::state<sub>)>(sml::X));
};

test process_event_of_substate = [] {
  struct sub {
    auto operator()() const noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *s1 + event<e3> = X
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *idle + event<e1> = state<sub>
        , state<sub> + event<e2> / process(e3{})
      );
      // clang-format on
    }
  };

  sml::sm<c, sml::process_queue<std::queue>> sm;
  expect(sm.is(idle));
  expect(sm.is<decltype(sml::state<sub>)>(s1));

  expect(sm.process_event(e1{}));
  expect(sm.is(sml::state<sub>));

  expect(sm.process_event(e2{}));  // + process(e2{})
  expect(sm.is(sml::state<sub>));
  expect(sm.is<decltype(sml::state<sub>)>(sml::X));
};

test process_between_substates = [] {
  struct sub1 {
    auto operator()() const noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *s1 + event<e1> / process(e2{}) = X
      );
      // clang-format on
    }
  };

  struct sub2 {
    auto operator()() const noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *s2 + event<e2> / process(e3{}) = X
      );
      // clang-format on
    }
  };

  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
         *state<sub1> + event<e3> / process(e4{}) = X,
         *state<sub2> + event<e4> = X
      );
      // clang-format on
    }
  };

  sml::sm<c, sml::process_queue<std::queue>> sm;
  expect(sm.is(sml::state<sub1>, sml::state<sub2>));
  expect(sm.is<decltype(sml::state<sub1>)>(s1));
  expect(sm.is<decltype(sml::state<sub2>)>(s2));

  expect(sm.process_event(e1{}));
  expect(sm.is(sml::X, sml::X));
  expect(sm.is<decltype(sml::state<sub1>)>(sml::X));
  expect(sm.is<decltype(sml::state<sub2>)>(sml::X));
};

template <typename T>
using MinimalStaticQueue10 = MinimalStaticQueue<T, 10>;

test queue_process_events_static_queue = [] {
  struct c {
    std::vector<int> calls;

    auto operator()() {
      using namespace sml;

      const auto a0 = [this] { calls.push_back(0); };
      const auto a1 = [this] { calls.push_back(1); };
      const auto a2 = [this] { calls.push_back(2); };

      // clang-format off
      return make_transition_table(
        * idle + on_entry<sml::initial> / (process(e1()), process(e2()), a0)
        , idle + event<e1> / a1
        , idle + event<e2> / a2 = X
      );
      // clang-format on
    }
  };

  sml::sm<c, sml::process_queue<MinimalStaticQueue10>> sm{};
  expect(sm.is(sml::X));

  const c& c_ = sm;
  expect(3u == c_.calls.size());
  expect(0 == c_.calls[0]);
  expect(1 == c_.calls[1]);
  expect(2 == c_.calls[2]);
};

// Issue #465: process_queued_events dispatched the front event before popping it.
// A re-entrant sm.process_event() call inside the action saw the same event still at
// the front, dispatched it again (double action call), popped it, then the outer loop
// called pop() on an empty queue — undefined behaviour / crash.
// c465 is defined at file scope (above) to avoid circular sm<c465> instantiation.
test process_queue_no_double_pop_on_reentrant_process_event = [] {
  using sm465_t = sml::sm<c465, sml::process_queue<std::queue>>;

  sm465_t sm{};
  c465& c_ = sm;
  c_.sm_ptr = &sm;
  // Captureless lambda → function pointer: calls sm.process_event(e3) re-entrantly.
  // e3 has no handler; its sole job is triggering a re-entrant process_queued_events.
  c_.do_reentrant = +[](void* ptr) -> bool {
    return static_cast<sm465_t*>(ptr)->process_event(e3{});
  };

  sm.process_event(e1{});  // queues e2; process_queued_events then dispatches it

  // With the bug: e2 action fires twice (count==2) and outer pop() on empty queue is UB.
  // With the fix: e2 action fires exactly once.
  expect(1 == c_.e2_action_count);
  expect(sm.is(sml::X));
};

// Issue #542: process_queued_events drained the entire process queue in one pass,
// giving anonymous (guard-only) transitions no opportunity to fire between dispatches.
// Events were therefore dispatched in the wrong state, causing guard failures and
// silent drops.  Fix: dispatch one queued event per process_queued_events call so the
// outer loop runs anonymous transitions before picking up the next queued event.
//
// c542 is defined at file scope because the fixture uses a dedicated counted event
// type (e542) and four cycling states connected by anonymous guard-only transitions.
struct e542 {
  int count;
};

struct c542 {
  int n = 0;
  int fires = 0;

  auto operator()() noexcept {
    using namespace sml;
    auto g = [this] { return n > 0; };
    auto on_ev = [this](const e542 &ev, sml::back::process<e542> proc) {
      ++fires;
      ++n;
      if (ev.count > 0) proc(e542{ev.count - 1});
    };
    auto reset = [this] { n = 0; };
    // clang-format off
    return make_transition_table(
        *sml::state<class sa542> + event<e542>[!g] / on_ev
       , sml::state<class sa542> + on_entry<_> / reset
       , sml::state<class sa542> [g] = sml::state<class sb542>

       , sml::state<class sb542> + event<e542>[!g] / on_ev
       , sml::state<class sb542> + on_entry<_> / reset
       , sml::state<class sb542> [g] = sml::state<class sc542>

       , sml::state<class sc542> + event<e542>[!g] / on_ev
       , sml::state<class sc542> + on_entry<_> / reset
       , sml::state<class sc542> [g] = sml::state<class sd542>

       , sml::state<class sd542> + event<e542>[!g] / on_ev
       , sml::state<class sd542> + on_entry<_> / reset
       , sml::state<class sd542> [g] = sml::state<class sa542>
    );
    // clang-format on
  }
};

test process_queue_anonymous_transitions_between_queued_events = [] {
  sml::sm<c542, sml::process_queue<std::queue>> sm{};
  c542 &c_ = sm;

  sm.process_event(e542{5});

  // With the bug: only 2 events fire (all queued events dispatched in the second
  // state before the anonymous transition runs, so subsequent events see n>0
  // and the guard !g fails — events silently dropped).
  // With the fix: all 6 events fire (initial e542{5} + 5 recursive).
  expect(6 == c_.fires);
};

// Issue #441: using process_queue<std::queue> failed to compile on GCC 9/10 and
// Clang 10/11 because push_impl passed a const lvalue to std::queue::push, causing
// overload resolution to select push(const value_type&) instead of push(value_type&&).
// Since queue_event's copy constructor is deleted this produced a hard compile error.
// Fix: explicitly construct the value_type rvalue so push(value_type&&) is always chosen.
test process_queue_std_queue_compiles_and_works = [] {
  struct c441 {
    int processed = 0;

    auto operator()() noexcept {
      using namespace sml;
      auto enqueue = [this](back::process<e2> proc) {
        proc(e2{});
        ++processed;
      };
      auto finish = [this] { ++processed; };
      // clang-format off
      return make_transition_table(
          *idle + event<e1> / enqueue = s1
        ,  s1   + event<e2> / finish  = X
      );
      // clang-format on
    }
  };

  sml::sm<c441, sml::process_queue<std::queue>> sm{};
  c441 &c_ = sm;

  sm.process_event(e1{});   // enqueues e2, transitions idle→s1
  sm.process_event(e2{});   // drains the queue: fires finish, transitions s1→X

  expect(2 == c_.processed);
  expect(sm.is(sml::X));
};
