struct connect{};
struct established{};
struct ping{};
struct disconnect{};
struct timeout{};

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

#include <variant>
#include <type_traits>

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

class Connection {
  struct Disconnected { };
  struct Connecting { };
  struct Connected { };

  std::variant<Disconnected, Connecting, Connected> state
    = Disconnected{};

public:
  void process_event(connect const&) {
    state = std::visit(overload{
      [&](Disconnected&&) { establish(); return Connecting{}; },
      [&](Connected&&)    { establish(); return Connecting{}; },
      [](auto&& state)    { return state; }
    }, std::move(state));
  }

  void process_event(disconnect const&) {
    state = std::visit(overload{
      [&](Connecting&&) { close(); return Disconnected{}; },
      [&](Connected&&)  { close(); return Disconnected{}; },
      [](auto&& state)  { return state; }
    }, std::move(state));
  }

  void process_event(established const&) {
    if (std::get_if<Connecting>(&state)) {
      state = Connected{};
    }
  }

  void process_event(ping const& event) {
    if (std::get_if<Connected>(&state) and is_valid(event)) {
      reset_timeout();
    }
  }

  void process_event(timeout const&) {
    if (std::get_if<Connected>(&state)) {
      establish();
      state = Connecting{};
    }
  }
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

  static void BM_stl_variant_v2(benchmark::State& state) {
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

  BENCHMARK(BM_stl_variant_v2);
  BENCHMARK_MAIN();
#endif
