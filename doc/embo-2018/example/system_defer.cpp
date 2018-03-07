#include <boost/sml.hpp>
#include <cstdio>
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
struct connect {};
struct ping {
  bool valid = false;
};
struct established {};
struct timeout {};
struct disconnect {};
struct power_up {};
struct suspend {};
struct resume {};
struct tick {};

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
       "Suspended"_s     + event<ping> / defer
    );
    // clang-format on
  }
};

}

int main() {
  using namespace sml;
  printf_logger log{};
  sm<System, logger<printf_logger>, defer_queue<std::queue>> system{log};

  // clang-format off
  /// start
  system.process_event(power_up{});
  system.process_event(connect{});      /// handled by Connection
  system.process_event(established{});  /// handled by Connection

  /// defer
  system.process_event(suspend{});      /// handled by System
  system.process_event(ping{true});     /// deferred
  system.process_event(resume{});       /// ping in connected
  // clang-format on
}
