#include <boost/sml.hpp>
#include <cstdio>
#include <string>
#include <cassert>
#include <array>

namespace sml = boost::sml;

namespace {

struct char_t {
  char char_{};
  operator char() const { return char_; }
};

const auto is_digit = [](const auto& event) {
  return event >= '0' and event <= '9';
};

const auto is_letter = [](const auto& event) {
  return event >= 'a' and event <= 'z';
};

const auto is = [](auto c) { return [c](const auto& event) { return event == c; }; };

auto update_buffer = [n = 0]() mutable {
  return [&n](const auto& event, std::array<char, 32>& buffer) {
    buffer[n++] = event;
  };
};

struct Parser {
  auto operator()() {
    using namespace sml;
    // clang-format off
    return make_transition_table(
      * "Idle"_s + event<char_t> [ is('!') ]                          = "Name"_s,
        "Name"_s + event<char_t> [ is_letter ]     / update_buffer(),
        "Name"_s + event<char_t> [ is(' ') ]                          = "Year"_s,
        "Year"_s + event<char_t> [ is_digit ]      / update_buffer(),
        "Year"_s + event<char_t> [ is('!') ]                          = X
    );
    // clang-format on
  }
};

}

int main() {
  std::array<char, 32> buffer{};
  sml::sm<Parser> parser{buffer};

  parser.process_event(char_t{'!'});
  parser.process_event(char_t{'e'});
  parser.process_event(char_t{'m'});
  parser.process_event(char_t{'b'});
  parser.process_event(char_t{'o'});
  parser.process_event(char_t{' '});
  parser.process_event(char_t{'2'});
  parser.process_event(char_t{'0'});
  parser.process_event(char_t{'1'});
  parser.process_event(char_t{'8'});
  parser.process_event(char_t{'!'});

  assert(parser.is(sml::X));
  assert(std::string{"embo2018"} == buffer.data());
}
