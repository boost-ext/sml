#define BOOST_SML_CREATE_DEFAULT_CONSTRUCTIBLE_DEPS
// cppcheck-suppress missingIncludeSystem
#include <boost/sml.hpp>
// cppcheck-suppress missingIncludeSystem
#include <cstddef>
// cppcheck-suppress missingIncludeSystem
#include <vector>
#if defined(__has_include)
#if __has_include(<version>)
// cppcheck-suppress missingIncludeSystem
#include <version>
#endif
#endif
#if defined(__cpp_lib_expected)
// cppcheck-suppress missingIncludeSystem
#include <expected>
#endif

namespace sml = boost::sml;

struct e718 {};

struct greedy718 {
  int value{};
  greedy718() = default;
  template <class U>
  greedy718(U&&) : value{-1} {}
};

struct greedy_first_member718 {
  greedy718 member;
};

test default_dep_with_greedy_first_member_is_value_initialized_and_shared = [] {
  static int value_on_entry = -2;
  static int value_on_event = -2;

  struct c {
    auto operator()() const noexcept {
      using namespace sml;
      return make_transition_table(
          *state<greedy_first_member718> + event<e718> / [](greedy_first_member718& s) { value_on_event = s.member.value; } = X,
          state<greedy_first_member718> + on_entry<_> /
                                              [](greedy_first_member718& s) {
                                                value_on_entry = s.member.value;
                                                s.member.value = 7;
                                              });
    }
  };

  sml::sm<c> sm;
  expect(value_on_entry == 0);
  sm.process_event(e718{});
  expect(value_on_event == 7);
  expect(sm.is(sml::X));
};

test default_dep_with_initializer_list_ctor_is_empty = [] {
  static std::size_t size_on_event = 1;

  struct c {
    auto operator()() const noexcept {
      using namespace sml;
      return make_transition_table(*state<class idle718> + event<e718> / [](std::vector<int>& v) { size_on_event = v.size(); } =
                                       X);
    }
  };

  sml::sm<c> sm;
  sm.process_event(e718{});
  expect(size_on_event == 0u);
  expect(sm.is(sml::X));
};

#if defined(__cpp_lib_expected)
struct expected_first_member718 {
  std::expected<int, int> member;
};

test default_dep_with_std_expected_first_member_is_shared = [] {
  static int value_on_event = 0;

  struct c {
    auto operator()() const noexcept {
      using namespace sml;
      return make_transition_table(
          *state<expected_first_member718> + event<e718> / [](expected_first_member718& s) { value_on_event = *s.member; } = X,
          state<expected_first_member718> + on_entry<_> / [](expected_first_member718& s) { s.member = 1; });
    }
  };

  sml::sm<c> sm;
  sm.process_event(e718{});
  expect(value_on_event == 1);
  expect(sm.is(sml::X));
};
#endif
