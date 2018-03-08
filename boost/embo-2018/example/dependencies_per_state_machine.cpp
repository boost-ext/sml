#include <boost/sml.hpp>
#include <array>
#include <cstdio>

namespace sml = boost::sml;

namespace {

class Sender {
public:
  template<class TAddress, class TData>
  void send(const TAddress& ip, const TData& data) {
      std::puts(ip);
      std::puts(data);
  }
};

class Context {
public:
  const char* const ip{};
  bool valid(int id) const { return id; }
};

struct connect {};
struct ping { int id{}; };
struct established {};
struct timeout {};
struct disconnect {};

const auto close = []{ std::puts("close"); };
const auto resetTimeout = [] { std::puts("resetTimeout"); };

const auto is_valid = [](const Context& ctx, const auto& event) {
  return ctx.valid(event.id);
};

const auto establish = [](const auto& address) {
  return [&address](Sender& sender) {
    sender.send(address, "establish");
  };
};

struct Connection {
  const char* const address{}; // local dependency injected

  auto operator()() const {
    using namespace sml;
    return make_transition_table(
      * "Disconnected"_s + event<connect> / establish(address)       = "Connecting"_s,
        "Connecting"_s   + event<established>                        = "Connected"_s,
        "Connected"_s    + event<ping> [ is_valid ] / resetTimeout,
        "Connected"_s    + event<timeout> / establish(address)       = "Connecting"_s,
        "Connected"_s    + event<disconnect> / close                 = "Disconnected"_s
    );
  }
};

}

int main() {
  Context ctx{};
  Sender sender{};

  std::array<sml::sm<Connection>, 4> connections = {
    sml::sm<Connection>{Connection{"127.0.0.1"}, ctx, sender},
    sml::sm<Connection>{Connection{"127.0.0.2"}, ctx, sender},
    sml::sm<Connection>{Connection{"127.0.0.3"}, ctx, sender},
    sml::sm<Connection>{Connection{"127.0.0.4"}, ctx, sender}
  };

  connections[0].process_event(connect{});
  connections[1].process_event(established{});
  connections[2].process_event(ping{42});
  connections[3].process_event(disconnect{});
}

