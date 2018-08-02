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
    bool disconnected = true, connected = false, connecting = false;

public:
    void process_event(const connect& event) {
      if (disconnected or connected) {
        establish();
        connected = disconnected = false;
        connecting = true;
      }
    }

    void process_event(const disconnect& event) {
      if (connecting or connected) {
        close();
        connected = connecting = false;
        disconnected = true;
      }
    }

    void process_event(const established& event) {
      if (connecting) {
        connecting = disconnected = false;
        connected = true;
      }
    }

    void process_event(const ping& event) {
      if (connected and is_valid(event)) {
        resetTimeout();
      }
    }

    void process_event(const timeout& event) {
      if (connected) {
        establish();
        connecting = true;
        connected = disconnected = false;
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
