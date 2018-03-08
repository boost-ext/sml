#include <boost/sml.hpp>
#include <array>
#include <cstdio>
#include <string_view>
#include <variant>

namespace sml = boost::sml;

namespace {

class Sender {
public:
  template<class TAddress, class TData>
  auto send(const TAddress& ip, const TData& data) {
    std::puts(ip.data());
    std::puts(data);
    return true;
  }
};

class Context {
public:
  const char* const ip{};
  bool valid(int id) const { return id; }
  void log(...) {}
};

struct connect { int id{}; };
struct ping { int id{}; };
struct established {};
struct timeout { int id{}; };
struct disconnect {};

const auto close = []{ std::puts("close"); };
const auto resetTimeout = [] { std::puts("resetTimeout"); };

const auto is_valid = [](const Context& ctx, const auto& event) {
  return ctx.valid(event.id);
};

auto establish =
  [max_retries = 10](const auto& address, auto& data) mutable {
    return [&](Sender& sender, Context& ctx, const auto& event) {
      max_retries -= sender.send(address, "establish");

      if (!max_retries) {
        ctx.log("Can't send request!");
        max_retries = {};
      }

      data = event.id; // per state storage
    };
  };

struct Connection {
 public:
  explicit Connection(std::string_view address) : address_{address} {}

  auto operator()() {
    using namespace sml;
    return make_transition_table(
      * "Disconnected"_s + event<connect> / establish(address_, data_)  = "Connecting"_s,
        "Connecting"_s   + event<established>                           = "Connected"_s,
        "Connected"_s    + event<ping> [ is_valid ] / resetTimeout,
        "Connected"_s    + event<timeout> / establish(address_, data_)  = "Connecting"_s,
        "Connected"_s    + event<disconnect> / close                    = "Disconnected"_s
    );
  }

 private:
  std::string_view address_{};
  std::variant<int> data_{}; // Type safe union storage per state
};

}

int main() {
  Context ctx{};
  Sender sender{};

  std::array connections = {
    sml::sm<Connection>{Connection{"127.0.0.1"}, ctx, sender},
    sml::sm<Connection>{Connection{"127.0.0.2"}, ctx, sender},
    sml::sm<Connection>{Connection{"127.0.0.3"}, ctx, sender},
    sml::sm<Connection>{Connection{"127.0.0.4"}, ctx, sender}
  };

  connections[0].process_event(connect{42});
  connections[1].process_event(established{});
  connections[2].process_event(ping{42});
  connections[3].process_event(disconnect{});
}
