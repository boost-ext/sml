#ifndef PROCESS_Q3QVBZJG
#define PROCESS_Q3QVBZJG

#include "boost/sml/front/fwd.hpp"

namespace detail {

struct process {
  template <class TEvent>
  class process_impl : public action_base {
   public:
    explicit process_impl(const TEvent &event) : event(event) {}

    template <class TSelf, class T>
    void operator()(TSelf &self, const T &) {
      self.me_.process_event(event, self.deps_, self.sub_sms_);
    }

   private:
    TEvent event;
  };

  template <class TEvent>
  auto operator()(const TEvent &event) {
    return process_impl<TEvent>{event};
  }
};

}  // detail

#endif
