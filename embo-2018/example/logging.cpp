#include <boost/sml.hpp>
#include <cstdio>

namespace sml = boost::sml;

namespace {

struct my_logger {
  template<class T>
  auto name() { return sml::aux::get_type_name<T>(); }

  template <class SM, class TEvent>
  void log_process_event(const TEvent&) {
    printf("[%s][process_event] %s\n", name<SM>(), name<TEvent>());
  }
  template <class SM, class TGuard, class TEvent>
  void log_guard(const TGuard&, const TEvent&, bool result) {
    printf("[%s][guard] %s %s %s\n", name<SM>(), name<TGuard>(), name<TEvent>(),
      (result ? "[OK]" : "[Reject]"));
  }
  template <class SM, class TAction, class TEvent>
  void log_action(const TAction&, const TEvent&) {
    printf("[%s][action] %s %s\n", name<SM>(), name<TAction>(), name<TEvent>());
  }
  template <class SM, class TSrcState, class TDstState>
  void log_state_change(const TSrcState& src, const TDstState& dst) {
    printf("[%s][transition] %s -> %s\n", name<SM>(), src.c_str(), dst.c_str());
  }
};

struct connect {};
struct ping { bool valid = false; };
struct established {};
struct timeout {};
struct disconnect {};

const auto establish = []{ std::puts("establish"); };
const auto close = []{ std::puts("close"); };
const auto is_valid = [](const auto& event) { return event.valid; };
const auto resetTimeout = [] { std::puts("resetTimeout"); };

struct Connection {
  auto operator()() const {
    using namespace sml;
    return make_transition_table(
      * "Disconnected"_s + event<connect> / establish                = "Connecting"_s,
        "Connecting"_s   + event<established>                        = "Connected"_s,
        "Connected"_s    + event<ping> [ is_valid ] / resetTimeout,
        "Connected"_s    + event<timeout> / establish                = "Connecting"_s,
        "Connected"_s    + event<disconnect> / close                 = "Disconnected"_s
    );
  }
};

}

int main() {
  my_logger logger{};
  sml::sm<Connection, sml::logger<my_logger>> connection{logger};

  connection.process_event(connect{});
  connection.process_event(established{});
  connection.process_event(ping{true});
  connection.process_event(disconnect{});
}
