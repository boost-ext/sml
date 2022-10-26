//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#if __cplusplus >= 201703L
#include <algorithm>
#include <boost/sml.hpp>
#include <iostream>
#include <ostream>
#include <string>
#include <tuple>
#include <typeinfo>
#include <utility>
#include <vector>

namespace sml = boost::sml;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};

struct guard {
  bool operator()() const { return true; }
} guard;

struct guard2 {
  bool operator()() const { return true; }
} guard2;

struct action {
  void operator()() {}
} action;

struct action2 {
  void operator()() {}
} action2;

void on_s1_entry_f() {}
void on_s2_exit_f() {}

struct on_s1_entry {
  auto operator()() { on_s1_entry_f(); }
} on_s1_entry;

struct on_s2_exit {
  auto operator()() { on_s2_exit_f(); }
} on_s2_exit;

struct sub_machine {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
       *"orth1"_s = X
      ,*"orth2"_s = X
    );
    // clang-format on
  }
};

struct plant_uml {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
       *"idle"_s + event<e1> = "s1"_s
      , "s1"_s + event<e2> [ !guard && guard2 ] / action = "s2"_s
      , "s1"_s + sml::on_entry<_> / on_s1_entry
      , "s2"_s + event<e3> [ guard || guard2 ] = "s1"_s
      , "s2"_s + sml::on_exit<_> / on_s2_exit
      , "s2"_s + event<e4> / action = state<sub_machine>
      , state<sub_machine> + event<e5> / (action, action2) = X
    );
    // clang-format on
  }
};

inline void do_indent(std::ostream& out, unsigned int indent) { out << std::string(indent, ' '); }

// use this to track initialization for orthogonal states
bool state_initialized = false;  // NOLINT(misc-definitions-in-headers)
// use this to track which submachines have already been dumped so they don't get dumped twice
std::vector<std::string> completed_submachines;  // NOLINT(misc-definitions-in-headers)

/** allows for checking if the type is sml::front::seq_
 * This type is used by sml when there are lists of actions.
 */
template <class... Ts>
struct is_seq_ : sml::aux::false_type {};  // NOLINT(readability-identifier-naming)
template <class... Ts>
struct is_seq_<sml::front::seq_<Ts...>> : sml::aux::true_type {};  // NOLINT(readability-identifier-naming)

/** allows for checking if the type is sml::front::not_
 * This type is used by sml inside of guards, when the guard value is negated with !
 *
 * The partial specialization matches if the type passed in is sml::front::not_, causing the struct to
 * inherit from sml::aux::true_type, which gives it a member called "value" that is set to true.
 * If the type passed doesn't match sml::front::not_, it'll match the generic is_not_ which inherits
 * from sml::aux::false_type, giving it a member called "value" that is set to false.
 */
template <class... Ts>
struct is_not_ : sml::aux::false_type {};  // NOLINT(readability-identifier-naming)
template <class... Ts>
struct is_not_<sml::front::not_<Ts...>> : sml::aux::true_type {};  // NOLINT(readability-identifier-naming)

/** provides access to the template parameter type of an sml::front::not_<T>
 */
template <class T>
struct strip_not_ {
  using type = T;
};  // NOLINT(readability-identifier-naming)
template <class T>
struct strip_not_<sml::front::not_<T>> {
  using type = T;
};  // NOLINT(readability-identifier-naming)

/** allows for checking if the type is sml::front::and_
 * This type is used by sml inside of guards when two guard functions are combined with &&
 */
template <class... Ts>
struct is_and_ : sml::aux::false_type {};  // NOLINT(readability-identifier-naming)
template <class... Ts>
struct is_and_<sml::front::and_<Ts...>> : sml::aux::true_type {};  // NOLINT(readability-identifier-naming)

/** allows for checking if the type is sml::front::or_
 * This type is used by sml inside of guards when two guard functions are combined with ||
 */
template <class... Ts>
struct is_or_ : sml::aux::false_type {};  // NOLINT(readability-identifier-naming)
template <class... Ts>
struct is_or_<sml::front::or_<Ts...>> : sml::aux::true_type {};  // NOLINT(readability-identifier-naming)

/** uses std::tuple_element and std::tuple to access the Nth type in a parameter pack
 */
template <int N, class... Ts>
using NthTypeOf = typename std::tuple_element<N, std::tuple<Ts...>>::type;

/** gets the size of a parameter pack
 * this isn't really necessary, sizeof...(Ts) can be used directly instead
 */
template <class... Ts>
struct count {                                     // NOLINT(readability-identifier-naming)
  static const std::size_t value = sizeof...(Ts);  // NOLINT(readability-identifier-naming)
};

/** allows for checking if the type is sml::aux::zero_wrapper
 * sml puts this around types inside of guards and event sequences
 */
template <class T>
struct is_zero_wrapper : sml::aux::false_type {};  // NOLINT(readability-identifier-naming)
template <class T>
struct is_zero_wrapper<sml::aux::zero_wrapper<T>> : sml::aux::true_type {};  // NOLINT(readability-identifier-naming)

/** if T is a zero wrapper, ::type will be the inner type. if not, it will be T.
 */
template <class T>
struct strip_zero_wrapper {
  using type = T;
};  // NOLINT(readability-identifier-naming)
template <class T>
struct strip_zero_wrapper<sml::aux::zero_wrapper<T>> {
  using type = T;
};  // NOLINT(readability-identifier-naming)

/** accesses the type of a state-machine, sml::back::sm
 */
template <class T>
struct submachine_type {
  using type = T;
};  // NOLINT(readability-identifier-naming)
template <class T>
struct submachine_type<sml::back::sm<T>> {
  using type = typename T::sm;
};  // NOLINT(readability-identifier-naming)

/** print the types inside a sml::front::seq_
 * These types came from a list of actions.
 */
template <class... Ts>
struct print_seq_types {  // NOLINT(readability-identifier-naming)
  template <int I>
  static void func(std::ostream& out) {
    constexpr auto param_pack_empty = (sizeof...(Ts) == I);
    if constexpr (!param_pack_empty) {  // NOLINT(readability-braces-around-statements,bugprone-suspicious-semicolon)
      using current_type = NthTypeOf<I, Ts...>;
      if constexpr (is_seq_<typename current_type::type>::value) {  // NOLINT(readability-braces-around-statements)
        // handle nested seq_ types, these happen when there are 3 or more actions
        print_seq_types<typename current_type::type>::template func<0>(out);
      } else {  // NOLINT(readability-misleading-indentation)
        // print this param directly
        out << sml::aux::string<typename strip_zero_wrapper<current_type>::type>{}.c_str();
      }
      if constexpr (I + 1 < sizeof...(Ts)) {  // NOLINT(readability-braces-around-statements,bugprone-suspicious-semicolon)
        out << ",\\n ";
      }
      print_seq_types<Ts...>::template func<I + 1>(out);
    }
  }
};
template <class... Ts>
struct print_seq_types<sml::front::seq_<Ts...>> {  // NOLINT(readability-identifier-naming)
  template <int I>
  static void func(std::ostream& out) {
    print_seq_types<Ts...>::template func<0>(out);
  }
};

/** print the types inside a guard
 * These can be a functor, an sml::front::not_, an sml::front::and_, or an sml::front::or_ which makes
 * this one more complicated. They also involve the zero_wrapper.
 * The various partial specializations handle all of the possible types.
 */
template <class... Ts>
struct print_guard {  // NOLINT(readability-identifier-naming)
  template <int I>
  static void func(std::ostream& out, const std::string& sep = "") {
    constexpr auto param_pack_empty = (sizeof...(Ts) == I);
    if constexpr (!param_pack_empty) {  // NOLINT(readability-braces-around-statements,bugprone-suspicious-semicolon)
      using current_type = NthTypeOf<I, Ts...>;
      if constexpr (is_zero_wrapper<
                        current_type>::value) {  // NOLINT(readability-braces-around-statements,bugprone-suspicious-semicolon)
        // unwrap the zero_wrapper and put it back into the recursion, it could be anything
        print_guard<typename strip_zero_wrapper<current_type>::type>::template func<0>(out);
      } else {  // NOLINT(readability-misleading-indentation)
        // it's just a functor, print it
        out << sml::aux::string<current_type>{}.c_str();
      }

      // if we're not at the end, print the separator
      if constexpr (I + 1 < sizeof...(Ts)) {  // NOLINT(readability-braces-around-statements,bugprone-suspicious-semicolon)
        if (!sep.empty()) {
          out << sep;
        }
      }

      // keep the recursion going, call for the next type in the parameter pack
      print_guard<Ts...>::template func<I + 1>(out, sep);
    }
  }
};
template <class T>
struct print_guard<sml::front::not_<T>> {  // NOLINT(readability-identifier-naming)
  template <int I>
  static void func(std::ostream& out, const std::string& /*sep*/ = "") {
    out << "!" << sml::aux::string<typename strip_zero_wrapper<T>::type>{}.c_str();
  }
};
template <class... Ts>
struct print_guard<sml::front::and_<Ts...>> {  // NOLINT(readability-identifier-naming)
  template <int I>
  static void func(std::ostream& out, const std::string& /*sep*/ = "") {
    constexpr auto param_pack_empty = (sizeof...(Ts) == I);
    if constexpr (!param_pack_empty) {
      print_guard<Ts...>::template func<I>(out, " &&\\n ");
    }
  }
};
template <class... Ts>
struct print_guard<sml::front::or_<Ts...>> {  // NOLINT(readability-identifier-naming)
  template <int I>
  static void func(std::ostream& out, const std::string& /*sep*/ = "") {
    constexpr auto param_pack_empty = (sizeof...(Ts) == I);
    if constexpr (!param_pack_empty) {
      print_guard<Ts...>::template func<I>(out, " ||\\n ");
    }
  }
};

// forward declaration of dump_transitions
template <typename...>
struct dump_transitions;

template <int N, class T>
void dump_transition(std::ostream& out) noexcept {
  constexpr auto src_is_sub_sm =
      !sml::aux::is_same<sml::aux::type_list<>, sml::back::get_sub_sms<typename T::src_state>>::value;
  constexpr auto dst_is_sub_sm =
      !sml::aux::is_same<sml::aux::type_list<>, sml::back::get_sub_sms<typename T::dst_state>>::value;

  std::string src_state, dst_state;
  // NOLINTNEXTLINE(readability-braces-around-statements)
  if constexpr (src_is_sub_sm) {
    src_state = std::string{sml::aux::string<typename submachine_type<typename T::src_state>::type>{}.c_str()};
  } else {  // NOLINT(readability-misleading-indentation)
    src_state = std::string{sml::aux::string<typename T::src_state>{}.c_str()};
  }

  // NOLINTNEXTLINE(readability-braces-around-statements)
  if constexpr (dst_is_sub_sm) {
    dst_state = std::string{sml::aux::string<typename submachine_type<typename T::dst_state>::type>{}.c_str()};
  } else {  // NOLINT(readability-misleading-indentation)
    dst_state = std::string{sml::aux::string<typename T::dst_state>{}.c_str()};
  }

  const auto dst_internal = sml::aux::is_same<typename T::dst_state, sml::front::internal>::value;

  const auto has_event = !sml::aux::is_same<typename T::event, sml::anonymous>::value;
  const auto has_guard = !sml::aux::is_same<typename T::guard, sml::front::always>::value;
  const auto has_action = !sml::aux::is_same<typename T::action, sml::front::none>::value;

  if (has_event && has_action && sml::aux::is_same<typename T::action::type, sml::front::actions::defer>::value) {
    do_indent(out, N);
    out << src_state << " : " << boost::sml::aux::get_type_name<typename T::event>() << " / defer\n";
    return;
  }

  if (dst_state == "terminate") {
    dst_state = "[*]";
  }

  if (T::initial) {
    if (state_initialized) {  // create an orthogonal section
      do_indent(out, N);
      out << "--\n";
    }

    state_initialized = true;
    do_indent(out, N);
    out << "[*] --> " << src_state << "\n";
  }

  // NOLINTNEXTLINE(readability-braces-around-statements, bugprone-suspicious-semicolon)
  if constexpr (src_is_sub_sm) {
    auto already_in =
        std::find(completed_submachines.begin(), completed_submachines.end(), src_state) != completed_submachines.end();
    if (!already_in) {
      completed_submachines.push_back(src_state);
      constexpr int indent = N + 2;
      do_indent(out, N);
      out << "state " << src_state << " {\n";
      bool prev_state = state_initialized;
      state_initialized = false;
      dump_transitions<typename T::src_state::transitions>::template func<indent>(out);
      do_indent(out, N);
      out << "}\n";
      state_initialized = prev_state;
    }
  }

  do_indent(out, N);
  out << src_state;
  if (!dst_internal) {
    out << " --> " << dst_state;
  }

  if (has_event || has_guard || has_action) {
    out << " :";
  }

  if (has_event) {
    out << " " << std::string{sml::aux::string<typename T::event>{}.c_str()};
  }

  if (has_guard) {
    out << "\\n [";
    print_guard<typename T::guard::type>::template func<0>(out);
    out << "]";
  }

  if (has_action) {
    out << " /\\n ";

    if constexpr (is_seq_<typename T::action::type>::value) {  // NOLINT(readability-braces-around-statements)
      out << "(";
      print_seq_types<typename T::action::type>::template func<0>(out);
      out << ")";
    } else {  // NOLINT(readability-misleading-indentation)
      out << sml::aux::string<typename T::action::type>{}.c_str();
    }
  }

  out << "\n";

  // NOLINTNEXTLINE(readability-braces-around-statements, bugprone-suspicious-semicolon)
  if constexpr (dst_is_sub_sm) {
    auto already_in =
        std::find(completed_submachines.begin(), completed_submachines.end(), dst_state) != completed_submachines.end();
    if (!already_in) {
      completed_submachines.push_back(dst_state);
      constexpr int indent = N + 2;
      do_indent(out, N);
      out << "state " << dst_state << " {\n";
      bool prev_state = state_initialized;
      state_initialized = false;
      dump_transitions<typename T::dst_state::transitions>::template func<indent>(out);
      do_indent(out, N);
      out << "}\n";
      state_initialized = prev_state;
    }
  }
}

// this template allows iterating through the types in the parameter pack Ts...
// I is the counter
// INDENT is the current indentation level (for the state machine or sub-state machine)
template <int INDENT, int I, class... Ts>
void apply_dump_transition(std::ostream& out) {
  // iteration is finished when I == the size of the parameter pack
  constexpr auto param_pack_empty = (sizeof...(Ts) == I);
  if constexpr (!param_pack_empty) {  // NOLINT(readability-braces-around-statements,bugprone-suspicious-semicolon)
    // run the dump_transition function to print this sml::front::transition type
    dump_transition<INDENT, NthTypeOf<I, Ts...>>(out);
    // iteration isn't finished, keep going
    apply_dump_transition<INDENT, I + 1, Ts...>(out);
  }
}

// SFINAE type
template <typename...>
struct dump_transitions {  // NOLINT(readability-identifier-naming)
  template <int INDENT>
  static void func(std::ostream&) {}
};

// Partial specialization for sml::aux::type_list<Ts...>. This grants access to the
// types inside the type list, which are sml::front::transition types, so they can
// be passed to apply_dump_transition.
template <typename... Ts>
struct dump_transitions<typename sml::aux::type_list<Ts...>> {  // NOLINT(readability-identifier-naming)
  template <int INDENT>
  static void func(std::ostream& out) {
    apply_dump_transition<INDENT, 0, Ts...>(out);
  }
};

template <class T>
void dump(std::ostream& out) noexcept {
  out << "@startuml\n\n";
  dump_transitions<typename sml::sm<T>::transitions>::template func<0>(out);
  out << "\n@enduml\n";
}

int main() { dump<plant_uml>(std::cout); }

#elif __cplusplus == 201402L

#include <boost/sml.hpp>
#include <cassert>
#include <iostream>
#include <string>
#include <typeinfo>

namespace sml = boost::sml;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};

struct guard {
  bool operator()() const { return true; }
} guard;

struct action {
  void operator()() {}
} action;

void on_s1_entry() {}
void on_s2_exit() {}

struct plant_uml {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
       *"idle"_s + event<e1> = "s1"_s
      , "s1"_s + event<e2> [ guard ] / action = "s2"_s
      , "s1"_s + sml::on_entry<_> / [] { on_s1_entry(); }
      , "s2"_s + event<e3> [ guard ] = "s1"_s
      , "s2"_s + sml::on_exit<_> / [] { on_s2_exit(); }
      , "s2"_s + event<e4> / action = X
    );
    // clang-format on
  }
};

template <class T>
void dump_transition(std::ostream& out) noexcept {
  auto src_state = std::string{sml::aux::string<typename T::src_state>{}.c_str()};
  auto dst_state = std::string{sml::aux::string<typename T::dst_state>{}.c_str()};
  if (dst_state == "X") {
    dst_state = "[*]";
  }

  if (T::initial) {
    out << "[*] --> " << src_state << "\n";
  }

  const auto has_event = !sml::aux::is_same<typename T::event, sml::anonymous>::value;
  const auto has_guard = !sml::aux::is_same<typename T::guard, sml::front::always>::value;
  const auto has_action = !sml::aux::is_same<typename T::action, sml::front::none>::value;

  const auto is_entry = sml::aux::is_same<typename T::event, sml::back::on_entry<sml::_, sml::_>>::value;
  const auto is_exit = sml::aux::is_same<typename T::event, sml::back::on_exit<sml::_, sml::_>>::value;

  // entry / exit entry
  if (is_entry || is_exit) {
    out << src_state;
  } else {  // state to state transition
    out << src_state << " --> " << dst_state;
  }

  if (has_event || has_guard || has_action) {
    out << " :";
  }

  if (has_event) {
    // handle 'on_entry' and 'on_exit' per plant-uml syntax
    auto event = std::string(boost::sml::aux::get_type_name<typename T::event>());
    if (is_entry) {
      event = "entry";
    } else if (is_exit) {
      event = "exit";
    }
    out << " " << event;
  }

  if (has_guard) {
    out << " [" << boost::sml::aux::get_type_name<typename T::guard::type>() << "]";
  }

  if (has_action) {
    out << " / " << boost::sml::aux::get_type_name<typename T::action::type>();
  }

  out << "\n";
}

template <template <class...> class T, class... Ts>
void dump_transitions(const T<Ts...>&, std::ostream& out) noexcept {
  int _[]{0, (dump_transition<Ts>(out), 0)...};
  (void)_;
}

template <class SM>
void dump(const SM&, std::ostream& out) noexcept {
  out << "@startuml\n\n";
  dump_transitions(typename SM::transitions{}, out);
  out << "\n@enduml\n";
}

int main() {
  sml::sm<plant_uml> sm;
  dump(sm, std::cout);
}

#endif
