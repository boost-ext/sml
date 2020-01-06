//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <algorithm>
#include <boost/sml.hpp>
#include <iomanip>
#include <sstream>
#include <vector>

namespace sml = boost::sml;

void remove_spaces(std::string& str) { str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end()); }

#if !defined(_MSC_VER)
struct my_logger {
  template <class SM, class TEvent>
  void log_process_event(const TEvent& evt) {
    std::stringstream sstr;
    sstr << "[" << sml::aux::get_type_name<SM>() << "] " << evt.c_str();
    messages_out.push_back(sstr.str());
  }

  template <class SM, class TGuard, class TEvent>
  void log_guard(const TGuard&, const TEvent&, bool result) {
    std::stringstream sstr;
    sstr << "[" << sml::aux::get_type_name<SM>() << "] " << sml::aux::get_type_name<TEvent>() << "["
         << sml::aux::get_type_name<TGuard>() << "]: " << std::boolalpha << result;
    messages_out.push_back(sstr.str());
  }

  template <class SM, class TAction, class TEvent>
  void log_action(const TAction&, const TEvent&) {
    std::stringstream sstr;
    sstr << "[" << sml::aux::get_type_name<SM>() << "] "
         << "/ " << sml::aux::get_type_name<TAction>();
    messages_out.push_back(sstr.str());
  }

  template <class SM, class TSrcState, class TDstState>
  void log_state_change(const TSrcState& src, const TDstState& dst) {
    std::stringstream sstr;
    std::string src_state = src.c_str();
    remove_spaces(src_state);
    std::string dst_state = dst.c_str();
    remove_spaces(dst_state);
    sstr << "[" << sml::aux::get_type_name<SM>() << "] " << src_state << " -> " << dst_state;
    messages_out.push_back(sstr.str());
  }

  std::vector<std::string> messages_out;
};

struct e1 {
  static auto c_str() { return "e1"; }
};
struct e2 {
  static auto c_str() { return "e2"; }
};
struct e3 {
  static auto c_str() { return "e3"; }
};
struct s1_label {
  static auto c_str() { return "s1_label"; }
};
auto s1 = sml::state<s1_label>;

struct guard {
  bool operator()() const { return true; }
};
struct action {
  void operator()() const {}
};

struct c_log_sm {
  auto operator()() {
    using namespace sml;
    // clang-format off
    return make_transition_table(
       *"idle"_s + "e1"_e = s1
      , s1 + event<e2> = X
    );
    // clang-format on
  }
};

test log_sm = [] {
  // clang-format off
  std::vector<std::string> messages_expected = {
     "[c_log_sm] e1"
   , "[c_log_sm] idle -> s1_label"
   , "[c_log_sm] e2"
   , "[c_log_sm] s1_label -> terminate"
  };
  // clang-format on

  my_logger logger;
  sml::sm<c_log_sm, sml::logger<my_logger>> sm{logger};

  using namespace sml;
  sm.process_event("e1"_e());
  sm.process_event(e2{});

  expect(logger.messages_out.size() == messages_expected.size());
  expect(std::equal(logger.messages_out.begin(), logger.messages_out.end(), messages_expected.begin()));
};

struct c_log_entry_exit {
  auto operator()() {
    using namespace sml;
    // clang-format off
    return make_transition_table(
       *"idle"_s + "e1"_e = s1
      , s1 + sml::on_entry<_> / action{}
      , s1 + sml::on_exit<_> / action{}
      , s1 + event<e2> = X
    );
    // clang-format on
  }
};

test log_sm_entry_exit = [] {
  // clang-format off
  std::vector<std::string> messages_expected = {
     "[c_log_entry_exit] on_entry"
   , "[c_log_entry_exit] e1"
   , "[c_log_entry_exit] on_exit"
   , "[c_log_entry_exit] idle -> s1_label"
   , "[c_log_entry_exit] on_entry"
   , "[c_log_entry_exit] / action"
   , "[c_log_entry_exit] e2"
   , "[c_log_entry_exit] on_exit"
   , "[c_log_entry_exit] / action"
   , "[c_log_entry_exit] s1_label -> terminate"
   , "[c_log_entry_exit] on_entry"
  };
  // clang-format on

  my_logger logger;
  sml::sm<c_log_entry_exit, sml::logger<my_logger>> sm{logger};

  using namespace sml;
  sm.process_event("e1"_e());
  sm.process_event(e2{});

  expect(logger.messages_out.size() == messages_expected.size());
  expect(std::equal(logger.messages_out.begin(), logger.messages_out.end(), messages_expected.begin()));
};

struct sub {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
      *"idle"_s + event<e2> = X
    );
    // clang-format on
  }
};

class a;
class b;

struct c_log_sub_sm {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
      *state<a>.sm<sub>() + event<e1> = state<b>.sm<sub>(),
       state<b>.sm<sub>() + event<e3> [ (guard{}) ] / action{} = X
    );
    // clang-format on
  }
};

test log_sub_sm = [] {
  // clang-format off
  std::vector<std::string> messages_expected = {
     "[c_log_sub_sm] e1"
   , "[c_log_sub_sm] sub(a) -> sub(b)"
   , "[c_log_sub_sm] e2"
   , "[sub] e2"
   , "[sub] idle -> terminate"
   , "[c_log_sub_sm] e3"
   , "[c_log_sub_sm] e3[guard]: true"
   , "[c_log_sub_sm] sub(b) -> terminate"
   , "[c_log_sub_sm] / action"
  };
  // clang-format on

  my_logger logger;
  sml::sm<c_log_sub_sm, sml::logger<my_logger>> sm{logger};

  sm.process_event(e1{});
  sm.process_event(e2{});
  sm.process_event(e3{});

  expect(logger.messages_out.size() == messages_expected.size());
  expect(std::equal(logger.messages_out.begin(), logger.messages_out.end(), messages_expected.begin()));
};

struct c_log_sub_sm_mix {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
      *state<a>.sm<sub>() + event<e1> = state<sub>,
       state<sub> + event<e3> [ (guard{}) ] / action{} = X
    );
    // clang-format on
  }
};

test log_sub_sm_mix = [] {
  // clang-format off
  std::vector<std::string> messages_expected = {
     "[c_log_sub_sm_mix] e1"
   , "[c_log_sub_sm_mix] sub(a) -> sub"
   , "[c_log_sub_sm_mix] e2"
   , "[sub] e2"
   , "[sub] idle -> terminate"
   , "[c_log_sub_sm_mix] e3"
   , "[c_log_sub_sm_mix] e3[guard]: true"
   , "[c_log_sub_sm_mix] sub -> terminate"
   , "[c_log_sub_sm_mix] / action"
  };
  // clang-format on

  my_logger logger;
  sml::sm<c_log_sub_sm_mix, sml::logger<my_logger>> sm{logger};

  sm.process_event(e1{});
  sm.process_event(e2{});
  sm.process_event(e3{});

  expect(logger.messages_out.size() == messages_expected.size());
  expect(std::equal(logger.messages_out.begin(), logger.messages_out.end(), messages_expected.begin()));
};

#endif
