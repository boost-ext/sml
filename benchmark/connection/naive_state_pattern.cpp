struct connect {};
struct ping {};
struct established {};
struct timeout {};
struct disconnect {};

#if defined(TEST_PERF) or defined(TEST_GBENCH)
  static void clobber() { asm volatile("" : : : "memory"); }
  const auto establish = []{ clobber(); };
  const auto close = []{ clobber(); };
  const auto is_valid = [](auto const&) { clobber(); return true; };
  const auto reset_timeout = [] { clobber(); };
#else
  #include <cstdio>
  constexpr auto establish = []{ std::puts("establish"); };
  constexpr auto close = []{ std::puts("close"); };
  constexpr auto is_valid = [](auto const&) { return true; };
  constexpr auto reset_timeout = []{ std::puts("reset_timeout"); };
#endif

#include <memory>

struct State {
  virtual ~State() noexcept = default;
  virtual void process_event(connect const&) { }
  virtual void process_event(ping const&) { }
  virtual void process_event(established const&) { }
  virtual void process_event(timeout const&) { }
  virtual void process_event(disconnect const&) { }
};

class Connecting;
class Connected;
class Disconnected;

class Connection {
 public:
  template<class TState>
  void init_state() { change_state<TState>(); }

  template<class TEvent>
  void process_event(TEvent const& event) {
    state->process_event(event);
  }

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
      reset_timeout();
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

#if defined(TEST_ASM)
  int main() {
    Connection connection{};
    connection.init_state<Disconnected>();

    connection.process_event(connect{});
    connection.process_event(established{});
    connection.process_event(ping{});
    connection.process_event(disconnect{});
    connection.process_event(connect{});
    connection.process_event(established{});
    connection.process_event(ping{});
  }
#elif defined(TEST_ASM_GENERIC)
  #include <cstdlib>

  int main() {
    Connection connection{};
    connection.init_state<Disconnected>();

    while(true) {
      switch(rand() % 5) {
        default: break;
        case 0: connection.process_event(connect{}); break;
        case 1: connection.process_event(established{}); break;
        case 2: connection.process_event(ping{}); break;
        case 3: connection.process_event(disconnect{}); break;
        case 4: connection.process_event(timeout{}); break;
      }
    }
  }
#elif defined(TEST_PERF)
  int main() {
    Connection connection{};
    connection.init_state<Disconnected>();

    for (auto i = 0; i < 100'000'000; ++i) {
      connection.process_event(connect{});
      connection.process_event(established{});
      connection.process_event(ping{});
      connection.process_event(disconnect{});
    }
  }
#elif defined(TEST_GBENCH)
  #include <benchmark/benchmark.h>

  static void BM_naive_state_pattern_v1(benchmark::State& state) {
    constexpr auto size = 1'000'000;

    int dispatch[size]{};
    for (auto i = 0; i < size; ++i) {
      dispatch[i] = rand() % 4;
    }

    Connection connection;
    connection.init_state<Disconnected>();

    auto i = 0;
    for (auto _ : state) {
      switch(dispatch[i++]) {
        default: break;
        case 0: connection.process_event(connect{}); break;
        case 1: connection.process_event(established{}); break;
        case 2: connection.process_event(ping{}); break;
        case 3: connection.process_event(disconnect{}); break;
        case 4: connection.process_event(timeout{}); break;
      }
      if (i++ >= size) i = {};
    }
  }

  BENCHMARK(BM_naive_state_pattern_v1);
  BENCHMARK_MAIN();
#endif
