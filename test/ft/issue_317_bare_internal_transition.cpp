// cppcheck-suppress missingIncludeSystem
#include <boost/sml.hpp>

namespace sml = boost::sml;

namespace {
struct ignored {};
struct leave {};
struct come_back {};

struct counters {
  int entries = 0;
  int exits = 0;
};
struct count_entry {
  void operator()(counters &c) const { ++c.entries; }
};
struct count_exit {
  void operator()(counters &c) const { ++c.exits; }
};

const auto idle = sml::state<class idle_317>;
const auto busy = sml::state<class busy_317>;
}  // namespace

test bare_row_on_initial_state_consumes_event_without_exit_or_entry = [] {
  struct c {
    auto operator()() const noexcept {
      using namespace sml;
      return make_transition_table(*idle + event<ignored>, idle + event<leave> = busy, busy + event<come_back> = idle,
                                   idle + sml::on_entry<_> / count_entry{}, idle + sml::on_exit<_> / count_exit{});
    }
  };
  counters calls{};
  sml::sm<c> sm{calls};
  expect(1 == calls.entries);

  expect(sm.process_event(ignored{}));
  expect(sm.is(idle));
  expect(1 == calls.entries);
  expect(0 == calls.exits);

  expect(!sm.process_event(come_back{}));
  expect(sm.is(idle));
};

test bare_row_on_non_initial_state_consumes_event_without_exit_or_entry = [] {
  struct c {
    auto operator()() const noexcept {
      using namespace sml;
      return make_transition_table(*idle + event<leave> = busy, busy + event<ignored>, busy + sml::on_entry<_> / count_entry{},
                                   busy + sml::on_exit<_> / count_exit{});
    }
  };
  counters calls{};
  sml::sm<c> sm{calls};
  expect(sm.process_event(leave{}));
  expect(sm.is(busy));
  expect(1 == calls.entries);

  expect(sm.process_event(ignored{}));
  expect(sm.is(busy));
  expect(1 == calls.entries);
  expect(0 == calls.exits);
};
