#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>

namespace sc = boost::statechart;
namespace mpl = boost::mpl;

struct connect : sc::event<connect> {};
struct ping : sc::event<ping> {};
struct established : sc::event<established> {};
struct timeout : sc::event<timeout> {};
struct disconnect : sc::event<disconnect> {};

struct Disconnected;
struct Connecting;
struct Connected;

#if defined(TEST_PERF) or defined(TEST_GBENCH)
  static void clobber() { asm volatile("" : : : "memory"); }

  struct Connection : sc::state_machine<Connection, Disconnected> {
    template<class TEvent>
    void establish(TEvent const&) { clobber(); }
    void reset_timeout(ping const&) { clobber(); }
    void close(disconnect const&) { clobber(); }
    bool is_valid(ping const&) { clobber(); return true; }
  };
#else
  #include <cstdio>
  struct Connection : sc::state_machine<Connection, Disconnected> {
    template<class TEvent>
    void establish(TEvent const&) { std::puts("establish"); }
    void reset_timeout(ping const&) { std::puts("reset_timeout"); }
    void close(disconnect const&) { std::puts("close"); }
    bool is_valid(ping const&) { return true; }
  };
#endif

struct Disconnected : sc::simple_state<Disconnected, Connection> {
  using reactions = mpl::list<sc::transition<connect, Connecting, Connection, &Connection::establish>>;
};

struct Connecting : sc::simple_state<Connecting, Connection> {
  using reactions = mpl::list<sc::transition<established, Connected>>;
};

struct Connected : sc::simple_state<Connected, Connection> {
  using reactions = mpl::list<
    sc::transition<timeout, Connecting, Connection, &Connection::establish>,
    sc::transition<disconnect, Disconnected, Connection, &Connection::close>,
    sc::custom_reaction<ping>
  >;

  sc::result react(ping const & event) {
    if (context<Connection>().is_valid(event)) {
      context<Connection>().reset_timeout(event);
    }
    return discard_event();
  }
};

#if defined(TEST_ASM)
  int main() {
    Connection connection{};
    connection.initiate();

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
    connection.initiate();

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
    connection.initiate();

    for (auto i = 0; i < 100'000'000; ++i) {
      connection.process_event(connect{});
      connection.process_event(established{});
      connection.process_event(ping{});
      connection.process_event(disconnect{});
    }
  }
#elif defined(TEST_GBENCH)
  #include <benchmark/benchmark.h>

  static void BM_boost_statechart(benchmark::State& state) {
    constexpr auto size = 1'000'000;

    int dispatch[size]{};
    for (auto i = 0; i < size; ++i) {
      dispatch[i] = rand() % 4;
    }

    Connection connection;
    connection.initiate();

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

  BENCHMARK(BM_boost_statechart);
  BENCHMARK_MAIN();
#endif
