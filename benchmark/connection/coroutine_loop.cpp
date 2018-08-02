#include <cstdio>
#include <experimental/coroutine>
#include <iostream>

constexpr auto connect = 1;
constexpr auto ping = 2;
constexpr auto established =3;
constexpr auto timeout = 4;
constexpr auto disconnect = 5;

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

      auto await_ready() const noexcept -> bool { return {}; }
      auto await_suspend(std::experimental::coroutine_handle<> coroutine) noexcept {
        sm.coroutine = coroutine;
      }

      auto await_resume() const noexcept {
        switch(rand() % 4) {
          default: break;
          case 0: return std::pair{connect, connect}; break;
          case 1: return std::pair{established, established}; break;
          case 2: return std::pair{ping, ping}; break;
          case 3: return std::pair{disconnect, disconnect}; break;
        }
        //struct reset {
          //TEvent& event;
          //~reset() { event = {}; }
        //} _{sm.event};
        //return std::pair{sm.event, sm.event};
        return std::pair{0, 0};
      }
    } awaiter{*this};

    return awaiter;
  }

  void process() {
    coroutine.resume();
  }

 private:
  //TEvent event{};
  std::experimental::coroutine_handle<> coroutine{};
};

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
  void process() {
    sm.process();
  }

 private:
  state_machine<int> sm{};
  state initial{disconnected()};
};

int main() {
  Connection connection{};
  while (true) {
    connection.process();
  }
}
