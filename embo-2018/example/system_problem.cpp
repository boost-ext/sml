#include <boost/sml.hpp>
#include <boost/sml/utility/dispatch_table.hpp>
#include <cassert>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <queue>

namespace sml = boost::sml;

namespace {

/// logger
struct printf_logger {
  template <class SM, class TEvent>
  void log_process_event(const TEvent&) {
    printf("[%s][process_event] %s\n", sml::aux::get_type_name<SM>(), sml::aux::get_type_name<TEvent>());
  }

  template <class SM, class TGuard, class TEvent>
  void log_guard(const TGuard&, const TEvent&, bool result) {
    printf("[%s][guard] %s %s %s\n", sml::aux::get_type_name<SM>(), sml::aux::get_type_name<TEvent>(),
           sml::aux::get_type_name<TGuard>(), (result ? "[OK]" : "[Reject]"));
  }

  template <class SM, class TAction, class TEvent>
  void log_action(const TAction&, const TEvent&) {
    printf("[%s][action] %s %s\n", sml::aux::get_type_name<SM>(), sml::aux::get_type_name<TEvent>(),
           sml::aux::get_type_name<TAction>());
  }

  template <class SM, class TSrcState, class TDstState>
  void log_state_change(const TSrcState& src, const TDstState& dst) {
    printf("[%s][transition] %s -> %s\n", sml::aux::get_type_name<SM>(), src.c_str(), dst.c_str());
  }
};

/// events
struct connect : sml::utility::id<__COUNTER__> {};
struct ping : sml::utility::id<__COUNTER__> {
  explicit ping(const void* msg)
     : valid{msg}
  { }

  bool valid{};
};
struct established : sml::utility::id<__COUNTER__> {};
struct timeout : sml::utility::id<__COUNTER__> {};
struct disconnect : sml::utility::id<__COUNTER__> {};
struct power_up : sml::utility::id<__COUNTER__> {};
struct suspend : sml::utility::id<__COUNTER__> {};
struct resume : sml::utility::id<__COUNTER__> {};
struct tick : sml::utility::id<__COUNTER__> {};

/// guards
const auto is_valid = [](const auto& event) {
  std::puts("is_valid");
  return event.valid;
};
const auto is_healthy = [] {
  std::puts("is_healthy");
  return true;
};
const auto has_battery = [] {
  std::puts("has_battery");
  return true;
};

/// actions
const auto establish = [] { std::puts("establish"); };
const auto close = [] { std::puts("close"); };
const auto resetTimeout = [] { std::puts("resetTimeout"); };
const auto setup = [] { std::puts("setup"); };

class System {
  struct Connection {
    auto operator()() const {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        "Disconnected"_s(H) + event<connect> / establish                = "Connecting"_s,
        "Connecting"_s      + event<established>                        = "Connected"_s,
        "Connected"_s       + event<ping> [ is_valid ] / resetTimeout,
        "Connected"_s       + event<timeout> / establish                = "Connecting"_s,
        "Connected"_s       + event<disconnect> / close                 = "Disconnected"_s
      );
      // clang-format on
    }
  };

 public:
  auto operator()() const {
    using namespace sml;
    // clang-format off
    return make_transition_table(
     * "Idle"_s          + event<power_up> [ has_battery and
                                             is_healthy ] / setup       = state<Connection>,
       state<Connection> + event<suspend>                               = "Suspended"_s,
       "Suspended"_s     + event<resume>                                = state<Connection>,
       "Suspended"_s     + event<ping> / defer,
     // --------------------------------------------------------------------------------- //
     * "Watchdog"_s      + event<tick> / resetTimeout,
       "Watchdog"_s      + event<timeout>                               = X
    );
    // clang-format on
  }
};

}

int main(int argc, char** argv) {
  using namespace sml;
  printf_logger log{};
  sm<System, logger<printf_logger>, defer_queue<std::queue>> system{log};

  for (auto i = 1; i < argc; ++i) {
    (void)argc;
    (void)argv;
    assert(false); // FIXME
  }

  // ./a.out 5 0 2 1
  // cat out
  //   setup
  //   establish
}
