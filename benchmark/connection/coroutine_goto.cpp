#include <cstdio>
#include <experimental/coroutine>
#include <iostream>

struct task {
  struct promise_type {
    task get_return_object() { return {}; }
    std::experimental::suspend_never initial_suspend() { return {}; }
    std::experimental::suspend_never final_suspend() { return {}; }
    void return_void() {}
    void unhandled_exception() {}
  };
};


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

template<class TEvent>
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

      auto await_ready() const noexcept -> bool { return sm.event; }
      auto await_suspend(std::experimental::coroutine_handle<> coroutine) noexcept {
        sm.coroutine = coroutine;
        return true;
      }

      auto await_resume() const noexcept {
        struct reset {
          TEvent& event;
          ~reset() { event = {}; }
        } _{sm.event};
        return std::pair{sm.event, sm.event};
      }
    } awaiter{*this};

    return awaiter;
  }

  void process_event(const TEvent& event) {
    this->event = event;
    coroutine.resume();
  }

 private:
  TEvent event{};
  std::experimental::coroutine_handle<> coroutine{};
};

constexpr auto connect = 1;
constexpr auto ping = 2;
constexpr auto established =3;
constexpr auto timeout = 4;
constexpr auto disconnect = 5;

const auto establish = []{ std::puts("establish"); };
const auto close = []{ std::puts("close"); };
const auto is_valid = [](const auto&) { return true; };
const auto resetTimeout = [] { std::puts("resetTimeout"); };

class Connection {
  state connecting() {
    for (;;) {
      if (auto [event, data] = co_await sm; event == established) {
        co_return connected();
      }
    }
  }

  state disconnected() {
    for (;;) {
      if (auto [event, data] = co_await sm; event == connect) {
        establish();
        co_return connecting();
      }
    }
  }

  state connected() {
    for (;;) {
      switch (auto [event, data] = co_await sm; event) {
        case ping: if (is_valid(data)) resetTimeout(); break;
        case timeout: establish(); co_return connecting(); break;
        case disconnect: close(); co_return disconnected(); break;
      }
    }
  }

 public:
  template<class TEvent>
  void process_event(const TEvent& event) {
    sm.process_event(event);
  }

 private:
  state_machine<int> sm{};
  state initial{disconnected()};
};

//auto connection = [](auto& sm) -> state {
  //for (;;) {
    //disconnected:
    //if (auto [event, data] = co_await sm; event == connect) {
      //establish();
      //connecting:
      //if (auto [event, data] = co_await sm; event == established) {
        //connected:
          //switch (auto [event, data] = co_await sm; event) {
            //case ping: if (is_valid(data)) resetTimeout(); goto connected;
            //case timeout: establish(); goto connecting;
            //case disconnect: close(); goto disconnected;
          //}
      //}
    //}
  //}
//};

int main() {
  Connection connection{};
  connection.process_event(connect);
  connection.process_event(established);
  connection.process_event(ping);
  connection.process_event(disconnect);
  connection.process_event(connect);
  connection.process_event(established);
  connection.process_event(ping);
}
