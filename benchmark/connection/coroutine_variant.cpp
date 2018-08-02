#include <cstdio>
#include <experimental/coroutine>
#include <variant>

struct state {
  struct promise_type {
    state get_return_object() { return {}; }
    std::experimental::suspend_never initial_suspend() { return {}; }
    std::experimental::suspend_never final_suspend() { return {}; }
    template<class T>
    void return_value(T) {}
    void unhandled_exception() {}
  };
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

struct connect {};
struct ping {};
struct established {};
struct timeout {};
struct disconnect {};

const auto establish = []{ std::puts("establish"); };
const auto close = []{ std::puts("close"); };
const auto is_valid = [](const auto&) { return true; };
const auto resetTimeout = [] { std::puts("resetTimeout"); };

//class Connection {
  //state disconnected();
  //state connecting();
  //state connected();

 //public:
  //template<class TEvent>
  //void process_event(const TEvent& event) {
    //sm.process_event(event);
  //}

 //private:
  //state initial{disconnected()};
  //state_machine<connect, established, ping, timeout, disconnect> sm{};
//};

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
        resetTimeout();
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
