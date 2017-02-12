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

struct Ping {};
void resetTimeout() { std::puts("resetTimeout"); }
void establish() { std::puts("establish"); }
void close() { std::puts("close"); }
bool is_valid(const Ping&) { return true; }

class ConnectionV3 {
    struct Disconnected { };
    struct Connecting { };
    struct Connected { };

    std::variant<Disconnected, Connecting, Connected> state;

public:
    void connect() {
        std::visit(overload{
            [&](Disconnected) { establish(); state = Connecting{}; }, [&](Connected) { establish(); state = Connecting{}; }, [](auto) { }
        }, state);
    }

    void disconnect() {
        std::visit(overload{
            [&](Connecting) { close(); state = Disconnected{}; },
            [&](Connected) { close(); state = Disconnected{}; },
            [](auto) { }
        }, state);
    }

    void established() {
        state = Connected{};
    }

    void ping(const Ping& event) {
        if (std::get_if<Connected>(&state) && is_valid(event)) {
            resetTimeout();
        }
    }

    void timeout() { connect(); }
};

int main() {
  ConnectionV3 connection;
  #if defined(__clang__)
  static_assert(8 == sizeof(connection), "");
  #elif defined(__GNUC__)
  static_assert(2 == sizeof(connection), "");
  #endif

  connection.connect();
}
