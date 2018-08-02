#include <cstdio>
#include <memory>
#include <tuple>

struct connect {};
struct ping {};
struct established {};
struct timeout {};
struct disconnect {};

const auto establish = []{ std::puts("establish"); };
const auto close = []{ std::puts("close"); };
const auto is_valid = [](const auto&) { return true; };
const auto resetTimeout = [] { std::puts("resetTimeout"); };

struct State {
  virtual ~State() noexcept = default;
  virtual void process_event(connect const&) { }
  virtual void process_event(ping const&) { }
  virtual void process_event(established const&) { }
  virtual void process_event(timeout const&) { }
  virtual void process_event(disconnect const&) { }
};

class Disconnected;
class Connecting;
class Connected;

class Connection {
 public:
  template<class TEvent>
  void process_event(TEvent const& event) {
    state->process_event(event);
  }

  template<class TState>
  void init_state() { change_state<TState>(); }

  template<class TState>
  void change_state() {
    state = std::make_unique<TState>(*this);
  }

 private:
  std::unique_ptr<State> state{};
};

class Disconnected : public State {
 public:
  explicit Disconnected(Connection& connection) : connection{connection} {}

  void process_event(connect const&) override final {
    establish();
    connection.change_state<Connecting>();
  }

 private:
  Connection& connection;
};

class Connecting : public State {
 public:
  explicit Connecting(Connection& connection) : connection{connection} {}

  void process_event(established const&) override final {
    connection.change_state<Connected>();
  }

 private:
  Connection& connection;
};

class Connected : public State {
 public:
  explicit Connected(Connection& connection) : connection{connection} {}

  void process_event(ping const& event) override final {
    if (is_valid(event)) {
      resetTimeout();
    }
  }

  void process_event(timeout const&) override final {
    establish();
    connection.change_state<Connecting>();
  }

  void process_event(disconnect const&) override final {
    close();
    connection.change_state<Disconnected>();
  }

 private:
  Connection& connection;
};

int main() {
  Connection connection;
  connection.init_state<Disconnected>();

  connection.process_event(connect{});
  connection.process_event(established{});
  connection.process_event(ping{});
  connection.process_event(disconnect{});
  connection.process_event(connect{});
  connection.process_event(established{});
  connection.process_event(ping{});
}
