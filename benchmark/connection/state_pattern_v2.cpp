#include <cstdio>
#include <memory>

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
  virtual std::unique_ptr<State> process_event(connect const&) { return {}; }
  virtual std::unique_ptr<State> process_event(ping const&) { return {}; }
  virtual std::unique_ptr<State> process_event(established const&) { return {}; }
  virtual std::unique_ptr<State> process_event(timeout const&) { return {}; }
  virtual std::unique_ptr<State> process_event(disconnect const&) { return {}; }
};

struct Disconnected : State {
  std::unique_ptr<State> process_event(connect const&) override final;
};

struct Connecting : State {
  std::unique_ptr<State> process_event(established const&) override final;
};

struct Connected : State {
  std::unique_ptr<State> process_event(ping const& event) override final;
  std::unique_ptr<State> process_event(timeout const&) override final;
  std::unique_ptr<State> process_event(disconnect const&) override final;
};

std::unique_ptr<State> Disconnected::process_event(connect const&) {
  establish();
  return std::make_unique<Connecting>();
}

std::unique_ptr<State> Connecting::process_event(established const&) {
  return std::make_unique<Connected>();
}

std::unique_ptr<State> Connected::process_event(ping const& event) {
  if (is_valid(event)) {
    resetTimeout();
  }
  return {};
}

std::unique_ptr<State> Connected::process_event(timeout const&) {
  establish();
  return std::make_unique<Connecting>();
}

std::unique_ptr<State> Connected::process_event(disconnect const&) {
  close();
  return std::make_unique<Disconnected>();
}

struct Connection {
  template<class TEvent>
  void process_event(TEvent const& event) {
    if (auto new_state = state->process_event(event); new_state) {
      state = std::move(new_state);
    }
  }

  std::unique_ptr<State> state = std::make_unique<Disconnected>();
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
