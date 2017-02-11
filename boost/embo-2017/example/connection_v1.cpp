#include <cstdio>

struct Ping {};
void resetTimeout() { std::puts("resetTimeout"); }
void establish() { std::puts("establish"); }
void close() { std::puts("close"); }
bool is_valid(const Ping&) { return true; }

class ConnectionV1 {
    bool disconnected = true, connected = false, connecting = false;

public:
    void connect() {
        if (disconnected || connected) {
            establish();
            disconnected = false;
            connecting = true;
        }
    }

    void disconnect() {
        if (connecting || connected) {
            close();
            connected = false;
            disconnected = true;
        }
    }

    void established() {
        connecting = false;
        connected = true;
    }

    void ping(const Ping& event) {
        if (connected && is_valid(event)) {
            resetTimeout();
        }
    }

    void timeout() { connect(); }
};

int main() {
  ConnectionV1 connection;
  static_assert(3 == sizeof(connection), "");

  connection.connect();
}
