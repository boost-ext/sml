#include <boost/sml.hpp>
#include <cstdio>
#include <cstdlib>

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
  sml::sm<Connection> connection{};

  connection.process_event(connect{});
  connection.process_event(established{});
  connection.process_event(ping{true});
  connection.process_event(disconnect{});
}

/*
int main(int argc, char**) {
  sml::sm<Connection> connection{};
  switch(argc) {
    default: break;
    // the only valid case
    case 0: connection.process_event(connect{}); break;

    // optimized out
    case 1: connection.process_event(established{}); break;
    case 2: connection.process_event(ping{true}); break;
    case 3: connection.process_event(disconnect{}); break;
  }
}*/

/*
int main() {
  sml::sm<Connection> connection{};
  while(true) {
    switch(rand()) {
      default: break;
      case 0: connection.process_event(connect{}); break;
      case 1: connection.process_event(established{}); break;
      case 2: connection.process_event(ping{true}); break;
      case 3: connection.process_event(disconnect{}); break;
    }
  }
}*/

/*
 Add -fno-exceptions
*/

/*
int main() {
  sml::sm<Connection, sml::dispatch<sml::back::policies::jump_table>> connection{};
  //sml::sm<Connection, sml::dispatch<sml::back::policies::switch_stm>> connection{};
  //sml::sm<Connection, sml::dispatch<sml::back::policies::branch_stm>> connection{};
  //sml::sm<Connection, sml::dispatch<sml::back::policies::fold_expr>> connection{};

  while(true) {
    switch(rand()) {
      default: break;
      case 0: connection.process_event(connect{}); break;
      case 1: connection.process_event(established{}); break;
      case 2: connection.process_event(ping{true}); break;
      case 3: connection.process_event(disconnect{}); break;
    }
  }
}*/
