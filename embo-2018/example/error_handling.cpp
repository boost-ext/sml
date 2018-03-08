#include <boost/sml.hpp>
#include <cstdio>
#include <iostream>
#include <cassert>

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
      * "Disconnected"_s + event<connect>
            / (establish, [] { throw std::runtime_error{"err"}; })   = "Connecting"_s,
        "Disconnected"_s + unexpected_event<ping>                    = X,
        "Connecting"_s   + exception<std::runtime_error>
            / [] { std::puts("cleanup"); }                           = X,
        "Connecting"_s   + exception<_>                              = X,
        "Connecting"_s   + event<established>                        = "Connected"_s,
        "Connected"_s    + event<ping> [ is_valid ] / resetTimeout,
        "Connected"_s    + event<timeout> / establish                = "Connecting"_s,
        "Connected"_s    + event<disconnect> / close                 = "Disconnected"_s
    );
  }
};

}

int main() {
  {
  sml::sm<Connection> connection{};
  connection.process_event(connect{}); /// throws runtime_error
  assert(connection.is(sml::X));       /// terminated
  }

  {
  sml::sm<Connection> connection{};
  connection.process_event(ping{});   /// unexpected
  assert(connection.is(sml::X));      /// terminated
  }
}
