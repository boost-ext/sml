#include <boost/sml.hpp>
#include <cstdio>
#include <array>

#include <iostream>

namespace sml = boost::sml;

namespace {

using Id_t = std::array<char, 16>;
using Num_t = int;

struct c {
  char ch;
};

const auto is_digit = [](const auto& event) {
  return event.ch >= '0' and event.ch <= '9';
};

const auto is_letter = [](const auto& event) {
  return event.ch >= 'a' and event.ch <= 'z';
};

const auto is_separator = [](const auto& event) { return event.ch == ' '; };

auto next_id = [n = 0](const auto& event, Id_t& id) mutable {
  static auto i = 0;
  //std::clog << id.data() << i++ << std::endl;
  id[i++] = event.ch;
};

auto next_num = [](const auto& event, Num_t& id) {
  id *= 10;
  id += (event.ch - '0');
};

struct Parser {
  auto operator()() {
    using namespace sml;
    return make_transition_table(
      * "idle"_s + event<c> [ is_digit ] / next_num = "number"_s,
        "idle"_s + event<c> [ is_letter ] / next_id = "id"_s,
        "number"_s + event<c> [ is_digit ] / next_num,
        "number"_s + event<c> [ is_separator ] = "id"_s,
        "id"_s + event<c> [ is_letter or is_digit ] / next_id,
        "id"_s + event<c> [ is_separator ] = "number"_s
    );
  }
};

}

int main() {
  Num_t num{};
  Id_t id{};
  sml::sm<Parser> parser{id, num};

  parser.process_event(c{'e'});
  parser.process_event(c{'m'});
  parser.process_event(c{'b'});
  parser.process_event(c{'o'});

  parser.process_event(c{','});

  parser.process_event(c{'2'});
  parser.process_event(c{'0'});
  parser.process_event(c{'1'});
  parser.process_event(c{'8'});

  std::cout << id.data() << " " << num << std::endl;
}

