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
    reset_timeout();
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

#if defined(TEST_ASM)
  int main() {
    Connection connection{};
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

    for (auto i = 0; i < 100'000'000; ++i) {
      connection.process_event(connect{});
      connection.process_event(established{});
      connection.process_event(ping{});
      connection.process_event(disconnect{});
    }
  }
#elif defined(TEST_GBENCH)
  #include <benchmark/benchmark.h>

  static void BM_naive_state_pattern_v2(benchmark::State& state) {
    constexpr auto size = 1'000'000;

    int dispatch[size]{};
    for (auto i = 0; i < size; ++i) {
      dispatch[i] = rand() % 4;
    }

    Connection connection;

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

  BENCHMARK(BM_naive_state_pattern_v2);
  BENCHMARK_MAIN();
#endif
