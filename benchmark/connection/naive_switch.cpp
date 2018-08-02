#include <cstdio>

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
  enum class State : char/*std::byte*/ { DISCONNECTED, CONNECTING, CONNECTED } state;

public:
  void process_event(const connect& event) {
    switch(state) {
      default: break;
      case State::DISCONNECTED:
      case State::CONNECTED: establish(); state = State::CONNECTING; break;
    }
  }

  void process_event(const disconnect& event) {
    switch(state) {
      default: break;
      case State::CONNECTING:
      case State::CONNECTED: close(); state = State::DISCONNECTED; break;
    }
  }

  void process_event(const established& event) {
    if (state == State::CONNECTING) {
      state = State::CONNECTED;
    }
  }

  void process_event(const ping& event) {
    if (state == State::CONNECTED and is_valid(event)) {
      resetTimeout();
    }
  }

  void process_event(const timeout& event) {
    if (state == State::CONNECTED) {
      establish();
      state = State::CONNECTING;
    }
  }
};

int main() {
  Connection connection;
  connection.process_event(connect{});
  connection.process_event(established{});
  connection.process_event(ping{});
  connection.process_event(disconnect{});
  connection.process_event(connect{});
  connection.process_event(established{});
  connection.process_event(ping{});
}
