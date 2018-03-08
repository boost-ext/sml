#include <boost/sml.hpp>
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
  bool valid(int id) const { return true; }
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

const auto establish = [](Sender& sender, Context& ctx) {
  sender.send(ctx.ip, "establish");
};

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
  Context ctx{"127.0.0.1"};
  Sender sender{};

  sml::sm<Connection> connection{ctx, sender};
  //sml::sm<Connection> connection{sender, ctx}; // okay
  //sml::sm<Connection> connection{ctx}; // missing_ctor<Sender>
  //sml::sm<Connection> connection{ctx}; // missing_ctor<Context> missing_ctor<Sender>

  connection.process_event(connect{});
  connection.process_event(established{});
  connection.process_event(ping{42});
  connection.process_event(disconnect{});
}
