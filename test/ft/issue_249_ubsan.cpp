// Issue #249: UBSan "Insufficient object size" with lambda guards/actions at -O2+.
// zero_wrapper_impl::operator() used reinterpret_cast<const TExpr&>(*this) in C++17
// where *this is zero_wrapper_impl (not TExpr), violating strict aliasing.
// Fix: guard the zero_wrapper_impl specialisation to C++20 (where TExpr{} is valid);
// C++17 falls back to the primary template which inherits from TExpr via EBO.
// cppcheck-suppress missingIncludeSystem
#include <boost/sml.hpp>

namespace sml = boost::sml;

struct release {};
struct ack {};
struct fin {};
struct timeout {};

struct c249 {
  auto operator()() const noexcept {
    using namespace sml;
    const auto is_ack = [](const ack &) { return true; };
    const auto is_fin = [](const fin &) { return true; };
    const auto do_fin = [] {};
    const auto do_ack = [] {};
    // clang-format off
    return make_transition_table(
       *state<class established> + event<release>          / do_fin   = state<class fin_wait_1>,
        state<class fin_wait_1>  + event<ack> [ is_ack ]              = state<class fin_wait_2>,
        state<class fin_wait_2>  + event<fin> [ is_fin ] / do_ack     = state<class timed_wait>,
        state<class timed_wait>  + event<timeout>         / do_ack    = X
    );
    // clang-format on
  }
};

test issue_249_ubsan_lambda_guards_and_actions = [] {
  sml::sm<c249> sm;
  sm.process_event(release{});
  sm.process_event(ack{});
  sm.process_event(fin{});
  sm.process_event(timeout{});
  expect(sm.is(sml::X));
};
