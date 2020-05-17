//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <memory>

namespace sml = boost::sml;

struct fwd_sm;

class c {
 public:
  c();
  void update();
  bool is_done() const;

 private:
  std::unique_ptr<sml::sm<fwd_sm>> sm;
};
