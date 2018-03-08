#include <boost/sml.hpp>

namespace sml = boost::sml;

struct connect { };

struct Connection {
  auto operator()() const {
    using namespace sml;
    return make_transition_table(
      * "Disconnected"_s + event<connect> = "Connecting"_s
    );
  }
};

int main() {
  sml::sm<Connection> sm;
  sm.process_event(connect{});

  static_assert(1 == sizeof(sm), "SM is too big!");
}
