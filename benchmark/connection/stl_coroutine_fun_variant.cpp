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

#include <experimental/coroutine>
#include <variant>

struct state {
  struct promise_type {
    state get_return_object() { return {std::experimental::coroutine_handle<promise_type>::from_promise(*this) }; }
    std::experimental::suspend_never initial_suspend() { return {}; }
    std::experimental::suspend_always final_suspend() { return {}; }
    template<class T>
    void return_value(T) {}
    void unhandled_exception() {}
  };

  state(std::experimental::coroutine_handle<promise_type> handle) noexcept
    : handle_{handle}
  { }

  state(state&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
  ~state() noexcept { if (handle_) { handle_.destroy(); } }

private:
  std::experimental::coroutine_handle<promise_type> handle_;
};

template<class... TEvents>
class state_machine {
 public:
  state_machine() = default;
  state_machine(const state_machine&) = delete;
  state_machine(state_machine&&) = delete;
  state_machine& operator=(const state_machine&) = delete;
  state_machine& operator=(state_machine&&) = delete;

  auto operator co_await() {
    struct {
      state_machine& sm;

      auto await_ready() const noexcept -> bool { return sm.event.index(); }
      auto await_suspend(std::experimental::coroutine_handle<> coroutine) noexcept {
        sm.coroutine = coroutine;
        return true;
      }

      auto await_resume() const noexcept {
        struct reset {
          std::variant<TEvents...>& event;
          ~reset() { event = {}; }
        } _{sm.event};
        return sm.event;
      }
    } awaiter{*this};

    return awaiter;
  }

  template<class TEvent>
  void process_event(const TEvent& event) {
    this->event = event;
    coroutine.resume();
  }

 private:
  std::variant<TEvents...> event{};
  std::experimental::coroutine_handle<> coroutine{};
};

class Connection {
  state connecting() {
    for (;;) {
      if (const auto event = co_await sm; std::get_if<established>(&event)) {
        co_return connected();
      }
    }
  }

  state disconnected() {
    for (;;) {
      if (const auto event = co_await sm; std::get_if<connect>(&event)) {
        establish();
        co_return connecting();
      }
    }
  }

  state connected() {
    for (;;) {
      const auto event = co_await sm;
      if (std::get_if<ping>(&event) and is_valid(std::get<ping>(event))) {
        reset_timeout();
      } else if (std::get_if<timeout>(&event)) {
        establish();
        co_return connecting();
      } else if (std::get_if<disconnect>(&event)) {
        close();
        co_return disconnected();
      }
    }
  }

 public:
  template<class TEvent>
  void process_event(const TEvent& event) {
    sm.process_event(event);
  }

 private:
  state_machine<connect, established, ping, timeout, disconnect> sm{};
  state initial{disconnected()};
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

  static void BM_stl_coroutine_fun(benchmark::State& state) {
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

  BENCHMARK(BM_stl_coroutine_fun);
  BENCHMARK_MAIN();
#endif
