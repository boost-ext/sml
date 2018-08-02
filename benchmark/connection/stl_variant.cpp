#include <cstdio>
#include <variant>
#include <type_traits>

template<class ... Ts>
struct overload : Ts... {
  using Ts::operator()...;
  template<class ... TArgs>
  overload(TArgs && ... args) : TArgs(std::forward<TArgs>(args))... { }
};

template<class ... Ts>
overload(Ts...) -> overload<std::decay_t<Ts>...>;

struct connect{};
struct established{};
struct ping{};
struct disconnect{};
struct timeout{};

const auto establish = []{ std::puts("establish"); };
const auto close = []{ std::puts("close"); };
const auto is_valid = [](const auto&) { return true; };
const auto resetTimeout = [] { std::puts("resetTimeout"); };

class Connection {
  struct Disconnected { };
  struct Connecting { };
  struct Connected { };

  std::variant<Disconnected, Connecting, Connected> state;

public:
  void process_event(const connect& event) {
    std::visit(overload{
      [&](Disconnected) { establish(); state = Connecting{}; },
      [&](Connected) { establish(); state = Connecting{}; },
      [](auto) { }
    }, state);
  }

  void process_event(const disconnect& event) {
    std::visit(overload{
      [&](Connecting) { close(); state = Disconnected{}; },
      [&](Connected) { close(); state = Disconnected{}; },
      [](auto) { }
    }, state);
  }

  void process_event(const established& event) {
    if (std::get_if<Connecting>(&state)) {
      state = Connected{};
    }
  }

  void process_event(const ping& event) {
    if (std::get_if<Connected>(&state) and is_valid(event)) {
      resetTimeout();
    }
  }

  void process_event(const timeout& event) {
    if (std::get_if<Connected>(&state)) {
      establish();
      state = Connecting{};
    }
  }
};

int main() {
  Connection connection;
  //#if defined(__clang__)
  //static_assert(8 == sizeof(connection), "");
  //#elif defined(__GNUC__)
  //static_assert(2 == sizeof(connection), "");
  //#endif

  connection.process_event(connect{});
  connection.process_event(established{});
  connection.process_event(ping{});
  connection.process_event(disconnect{});
  connection.process_event(connect{});
  connection.process_event(established{});
  connection.process_event(ping{});
}
