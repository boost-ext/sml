#ifndef SM_VEYB5F0H
#define SM_VEYB5F0H

#include "boost/msm-lite.hpp"

BOOST_MSM_LITE_NAMESPACE_BEGIN

namespace testing {
template <class TSM>
class state_machine : public detail::sm<TSM> {
  using states_ids_t = aux::apply_t<aux::type_id, typename detail::sm<TSM>::states>;

 public:
  using detail::sm<TSM>::sm;

  template <class... TStates>
  void set_current_states(const detail::state<TStates> &...) {
    auto &sm = static_cast<aux::pool_type<detail::sm_impl<TSM>> &>(detail::sm<TSM>::sub_sms_).value;
    auto region = 0;
    int _[]{0, (sm.current_state_[region++] = aux::get_id<states_ids_t, 0, TStates>(), 0)...};
    (void)_;
  }
};

template <class T, class... TPolicies> /*, BOOST_MSM_LITE_REQUIRES(concepts::configurable<T>::value)*/
using sm = testing::state_machine<detail::sm_policy<T, TPolicies...>>;
}  // testing

BOOST_MSM_LITE_NAMESPACE_END

#endif
