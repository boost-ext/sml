// cppcheck-suppress missingIncludeSystem
#include <boost/sml.hpp>
// cppcheck-suppress missingIncludeSystem
#include <deque>

namespace sml = boost::sml;
using namespace sml;

struct e473_go {};
struct e473_a {};
struct e473_b {};
struct e473_mark {};

struct s473_wait {};
struct s473_s1 {};
struct s473_pass {};
struct s473_s2 {};
struct s473_done {};
struct s473_handled_in_pass {};
struct s473_r2_idle {};
struct s473_r2_active {};

test deferred_event_lost_when_replayed_before_anonymous_transition = [] {
  struct c {
    auto operator()() const noexcept {
      return make_transition_table(
         *state<s473_wait> + event<e473_go> = state<s473_s1>,
          state<s473_wait> + event<e473_a> / defer,
          state<s473_wait> + event<e473_b> / defer,
          state<s473_s1> + event<e473_a> = state<s473_pass>,
          state<s473_s1> + event<e473_b> / defer,
          state<s473_pass> = state<s473_s2>,
          state<s473_s2> + event<e473_b> = state<s473_done>
      );
    }
  };

  sm<c, defer_queue<std::deque>> m;
  m.process_event(e473_a{});
  m.process_event(e473_b{});
  m.process_event(e473_go{});
  expect(m.is(state<s473_done>));
};

test deferred_event_handled_in_state_whose_anonymous_guard_is_false = [] {
  struct c {
    auto operator()() const noexcept {
      return make_transition_table(
         *state<s473_wait> + event<e473_go> = state<s473_s1>,
          state<s473_wait> + event<e473_a> / defer,
          state<s473_wait> + event<e473_b> / defer,
          state<s473_s1> + event<e473_a> = state<s473_pass>,
          state<s473_s1> + event<e473_b> / defer,
          state<s473_pass> [([] { return false; })] = state<s473_s2>,
          state<s473_pass> + event<e473_b> = state<s473_handled_in_pass>,
          state<s473_s2> + event<e473_b> = state<s473_done>
      );
    }
  };

  sm<c, defer_queue<std::deque>> m;
  m.process_event(e473_a{});
  m.process_event(e473_b{});
  m.process_event(e473_go{});
  expect(m.is(state<s473_handled_in_pass>));
};

test deferred_event_lost_behind_anonymous_transition_with_orthogonal_region = [] {
  struct c {
    auto operator()() const noexcept {
      return make_transition_table(
         *state<s473_wait> + event<e473_go> = state<s473_s1>,
          state<s473_wait> + event<e473_a> / defer,
          state<s473_wait> + event<e473_b> / defer,
          state<s473_s1> + event<e473_a> = state<s473_pass>,
          state<s473_s1> + event<e473_b> / defer,
          state<s473_pass> = state<s473_s2>,
          state<s473_s2> + event<e473_b> = state<s473_done>,
         *state<s473_r2_idle> + event<e473_go> = state<s473_r2_active>
      );
    }
  };

  sm<c, defer_queue<std::deque>> m;
  m.process_event(e473_a{});
  m.process_event(e473_b{});
  m.process_event(e473_go{});
  expect(m.is(state<s473_done>, state<s473_r2_active>));
};

test deferred_event_lost_when_replay_enables_anonymous_guard_without_state_change = [] {
  struct c {
    auto operator()() const noexcept {
      auto marked = [](const bool &flag) { return flag; };
      auto set_mark = [](bool &flag) { flag = true; };
      return make_transition_table(
         *state<s473_wait> + event<e473_go> = state<s473_s1>,
          state<s473_wait> + event<e473_mark> / defer,
          state<s473_wait> + event<e473_b> / defer,
          state<s473_s1> + event<e473_mark> / set_mark,
          state<s473_s1> [marked] = state<s473_s2>,
          state<s473_s2> + event<e473_b> = state<s473_done>
      );
    }
  };

  bool flag = false;
  sm<c, defer_queue<std::deque>> m{flag};
  m.process_event(e473_mark{});
  m.process_event(e473_b{});
  m.process_event(e473_go{});
  expect(m.is(state<s473_done>));
};
