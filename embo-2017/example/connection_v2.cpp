#include <cstdio>

struct Ping {};
void resetTimeout() { std::puts("resetTimeout"); }
void establish() { std::puts("establish"); }
void close() { std::puts("close"); }
bool is_valid(const Ping&) { return true; }

class ConnectionV2 {
    enum class State : unsigned char { DISCONNECTED, CONNECTING, CONNECTED } state;

public:
    void connect() {
        switch(state) {
            default: break;
            case State::DISCONNECTED:
            case State::CONNECTED: establish(); state = State::CONNECTING; break;
        }
    }

    void disconnect() {
        switch(state) {
            default: break;
            case State::CONNECTING:
            case State::CONNECTED: close(); state = State::DISCONNECTED; break;
        }
    }

    void established() {
        state = State::CONNECTED;
    }

    void ping(const Ping& event) {
        if (state == State::CONNECTED && is_valid(event)) {
            resetTimeout();
        }
    }

    void timeout() { connect(); }
};

int main() {
  ConnectionV2 connection;
  static_assert(1 == sizeof(connection), "");

  connection.connect();
}
