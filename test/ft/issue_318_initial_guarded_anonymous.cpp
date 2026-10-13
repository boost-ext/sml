// cppcheck-suppress missingIncludeSystem
#include <boost/sml.hpp>
// cppcheck-suppress missingIncludeSystem
#include <type_traits>

namespace sml = boost::sml;

namespace {
struct guard_passes {
  bool operator()() const { return true; }
};
struct guard_blocks {
  bool operator()() const { return false; }
};
struct count_call {
  void operator()(int &calls) const { ++calls; }
};

const auto from = sml::state<class from_318>;
const auto to = sml::state<class to_318>;

static_assert(std::is_same<decltype(*from[guard_passes{}]), decltype((*from)[guard_passes{}])>::value, "");
static_assert(
    std::is_same<decltype(*from[guard_passes{}] / count_call{}), decltype((*from)[guard_passes{}] / count_call{})>::value, "");
}  // namespace

test initial_state_marked_on_guarded_row_takes_anonymous_transition_when_guard_passes = [] {
  struct c {
    auto operator()() const noexcept {
      using namespace sml;
      return make_transition_table(*from[guard_passes{}] = to);
    }
  };
  sml::sm<c> sm{};
  expect(sm.is(to));
};

test initial_state_marked_on_guarded_row_stays_when_guard_blocks = [] {
  struct c {
    auto operator()() const noexcept {
      using namespace sml;
      return make_transition_table(*from[guard_blocks{}] = to);
    }
  };
  sml::sm<c> sm{};
  expect(sm.is(from));
};

test initial_state_marked_on_guarded_row_with_action_runs_action_once = [] {
  struct c {
    auto operator()() const noexcept {
      using namespace sml;
      return make_transition_table(*from[guard_passes{}] / count_call{} = to);
    }
  };
  int calls = 0;
  sml::sm<c> sm{calls};
  expect(sm.is(to));
  expect(1 == calls);
};
