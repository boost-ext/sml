#include <cassert>

void establish() {}
bool is_valid(const void* message) { return message; }

void handle(int id, const void* message) {
  static bool is_connected = false;
  static bool is_connecting = false;

  if (id == 0 && !is_connected && !is_connecting) {
    establish();
    is_connecting = true;
  } else if (id == 1 && is_connecting && !is_connected && is_valid(message)) {
    is_connecting = false;
    is_connected = true;
  } /* else if...*/
}

class Connection {
  enum {
    CONNECT, ESTABLISHED, PING, TIMEOUT, DISCONNECT
  };

public:
  void handle(int id, const void* message) {
    switch (state) {
      default: assert(false); break;
      case Disconnected:
        switch (id) {
          case CONNECT: establish(); state = Connecting; break;
          default: break;
        }
      case Connecting:
        switch (id) {
          case ESTABLISHED: state = Connected; break;
          default: break;
        }
      /*case...*/
    }
  }

private:
  bool is_valid(const void* message) const { return message; }
  void establish() { }
  void resetTimeout() { }
  void close() { }

  enum {
    Disconnected,
    Connecting,
    Connected
  } state = Disconnected;
};

struct connect { };
struct established { };
struct ping { };
struct timeout { };
struct disconnect { };

int main() {
  Connection connection;
  connect message{};
  connection.handle(0, (void*)&message);
}
