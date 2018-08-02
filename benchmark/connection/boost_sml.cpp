#include <cstdio>
#include <boost/sml.hpp>

namespace sml = boost::sml;

struct connect {};
struct ping {};
struct established {};
struct timeout {};
struct disconnect {};

constexpr auto establish = []{ std::puts("establish"); };
constexpr auto close = []{ std::puts("close"); };
constexpr auto is_valid = [](const auto&) { return true; };
constexpr auto resetTimeout = [] { std::puts("resetTimeout"); };

int main() {
  sml::sm connection([]{
    using namespace sml;
    return transition_table{
      * "Disconnected"_s + event<connect> / establish                = "Connecting"_s,
        "Connecting"_s   + event<established>                        = "Connected"_s,
        "Connected"_s    + event<ping> [ is_valid ] / resetTimeout,
        "Connected"_s    + event<timeout> / establish                = "Connecting"_s,
        "Connected"_s    + event<disconnect> / close                 = "Disconnected"_s
    };
  });

  //static_assert(1 == sizeof(connection), "");
  connection.process_event(connect{});
  connection.process_event(established{});
  connection.process_event(ping{});
  connection.process_event(disconnect{});
  connection.process_event(connect{});
  connection.process_event(established{});
  connection.process_event(ping{});
}
