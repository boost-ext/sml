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

struct Parser {
  auto operator()() {
    using namespace sml;
    // clang-format off
    return make_transition_table(
      * "Idle"_s + event<char_t> = X // FIXME
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
