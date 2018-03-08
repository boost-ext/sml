#include <boost/sml.hpp>
#include <cstdio>

namespace sml = boost::sml;

namespace {

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
  // C++17
  sml::sm<Connection, sml::dispatch<sml::back::policies::fold_expr>> connection{};

  connection.process_event(connect{});
  connection.process_event(established{});
  connection.process_event(ping{true});
  connection.process_event(disconnect{});
}
