#include <cstdio>
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

struct Connection : sc::state_machine<Connection, Disconnected> {
  template<class TEvent>
  void establish(TEvent const&) { std::puts("establish"); }
  void resetTimeout(ping const&) { std::puts("resetTimeout"); }
  void close(disconnect const&) { std::puts("close"); }
  bool is_valid(ping const&) { return true; }
};

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
      context<Connection>().resetTimeout(event);
    }
    return discard_event();
  }
};

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
