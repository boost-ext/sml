#include <boost/sml.hpp>
#include <boost/sml/utility/dispatch_table.hpp>
#include <cstdio>

namespace sml = boost::sml;

namespace {

struct connect : sml::utility::id<0> {};
struct ping : sml::utility::id<1> {
  explicit ping(const void* msg)
     : valid{msg}
  { }

  bool valid{};
};
struct established : sml::utility::id<2> {};
struct timeout : sml::utility::id<3> {};
struct disconnect : sml::utility::id<4> {};

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
  sml::sm<Connection> connection{};

  auto dispatch = sml::utility::make_dispatch_table<void*, 0 /*min*/, 4 /*max*/>(connection);
  dispatch(nullptr, 0);      // connect
  dispatch(nullptr, 2);      // established
  int data{};
  dispatch((void*)&data, 1); // ping
  dispatch(nullptr, 4);      // disconnect
}
