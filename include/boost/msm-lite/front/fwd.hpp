#ifndef FWD_FBZKCBEA
#define FWD_FBZKCBEA

namespace detail {

struct action_base {};

template <class...>
struct transition;
template <class, class>
struct transition_sg;
template <class, class>
struct transition_sa;
template <class, class>
struct transition_eg;
template <class, class>
struct transition_ea;

}  // detail

#endif
