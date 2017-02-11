#include <boost/sml.hpp>
#include <cstdio>

namespace sml = boost::sml;

struct connect {};
struct ping {};
struct established {};
struct timeout {};
struct disconnect {};

const auto establish = []{ std::puts("establish"); };
const auto close = []{ std::puts("close"); };
const auto is_valid = [](const ping&) { return true; };
const auto resetTimeout = [] { std::puts("resetTimeout"); };

struct ConnectionV4 {
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

int main() {
  sml::sm<ConnectionV4> connection;
  static_assert(1 == sizeof(connection), "");

  connection.process_event(connect{});
}
