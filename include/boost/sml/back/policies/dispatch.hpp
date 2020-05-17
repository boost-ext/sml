//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_BACK_POLICIES_DISPATCH_HPP
#define BOOST_SML_BACK_POLICIES_DISPATCH_HPP

#include "boost/sml/aux_/utility.hpp"
#include "boost/sml/back/mappings.hpp"

namespace back {
namespace policies {

struct dispatch_policy__ {};

template <class T>
struct dispatch : aux::pair<dispatch_policy__, T> {
  using type = T;
};

struct jump_table {
  template <int, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs>
  static bool dispatch(sm_impl &, State &, const TEvent &, TDeps &, TSubs &, const aux::type_list<> &) {
    return false;
  }

  template <int, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class... TStates>
  static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps, TSubs &subs,
                       const aux::type_list<TStates...> &) {
    using dispatch_table_t = bool (*)(const TEvent &, sm_impl &, TDeps &, TSubs &, State &);
    constexpr static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TStates))] = {
        &get_state_mapping_t<TStates, TMappings, typename sm_impl::has_unexpected_events>::template execute<TEvent, sm_impl,
                                                                                                            TDeps, TSubs>...};
    return dispatch_table[current_state](event, self, deps, subs, current_state);
  }
};

struct branch_stm {
  template <int, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs>
  static bool dispatch(sm_impl &, State &, const TEvent &, TDeps &, TSubs &, const aux::type_list<> &) {
    return false;
  }

  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class TState,
            class... TStates>
  static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps, TSubs &subs,
                       const aux::type_list<TState, TStates...> &) {
    return current_state == N
               ? get_state_mapping_t<TState, TMappings, typename sm_impl::has_unexpected_events>::template execute<
                     TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state)
               : dispatch<N + 1, TMappings>(self, current_state, event, deps, subs, aux::type_list<TStates...>{});
  }
};

struct switch_stm {
  template <int, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs>
  static bool dispatch(sm_impl &, State &, const TEvent &, TDeps &, TSubs &, const aux::type_list<> &) {
    return false;
  }

  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class TState,
            class... TStates>
  static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps, TSubs &subs,
                       const aux::type_list<TState, TStates...> &) {
    switch (current_state) {
      case N:
        return get_state_mapping_t<TState, TMappings, typename sm_impl::has_unexpected_events>::template execute<
            TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state);
      default:
        return dispatch<N + 1, TMappings>(self, current_state, event, deps, subs, aux::type_list<TStates...>{});
    }
  }
};

#if defined(__cpp_fold_expressions)  // __pph__
struct fold_expr {
  template <class TMappings, int... Ns, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class... TStates>
  static bool dispatch_impl(sm_impl &self, State &current_state, aux::index_sequence<Ns...>, const TEvent &event, TDeps &deps,
                            TSubs &subs, const aux::type_list<TStates...> &) {
    return ((current_state == Ns
                 ? get_state_mapping_t<TStates, TMappings, typename sm_impl::has_unexpected_events>::template execute<
                       TEvent, sm_impl, TDeps, TSubs>(event, self, deps, subs, current_state)
                 : false) ||
            ...);
  }

  template <int N, class TMappings, class sm_impl, class State, class TEvent, class TDeps, class TSubs, class... TStates>
  static bool dispatch(sm_impl &self, State &current_state, const TEvent &event, TDeps &deps, TSubs &subs,
                       const aux::type_list<TStates...> &states) {
    return dispatch_impl<TMappings>(self, current_state, aux::make_index_sequence<sizeof...(TStates)>{}, event, deps, subs,
                                    states);
  }
};
#endif  // __pph__

}  // namespace policies
}  // namespace back

#endif
