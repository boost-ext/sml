//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_SML_BACK_POLICIES_TESTING_HPP
#define BOOST_SML_BACK_POLICIES_TESTING_HPP

#include "boost/sml/aux_/utility.hpp"

namespace back {
namespace policies {

struct testing_policy__ {};
struct testing : aux::pair<testing_policy__, testing> {};

}  // namespace policies
}  // namespace back

#endif
