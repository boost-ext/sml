//
// Copyright (c) 2016-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#define BOOST_MPL_LIMIT_VECTOR_SIZE 130

#include <boost/preprocessor/iterate.hpp>
#include <boost/mpl/vector/vector50.hpp>

namespace boost {
namespace mpl {
// clang-format off
#define BOOST_PP_ITERATION_PARAMS_1                                            \
  (3, (51, BOOST_MPL_LIMIT_VECTOR_SIZE,                                        \
       <boost/mpl/vector/aux_/numbered.hpp>))
#include BOOST_PP_ITERATE()
// clang-format on
} // namespace mpl
} // namespace boost

#define BOOST_MPL_PREPROCESSING_MODE
#include <boost/mpl/vector.hpp>
#undef BOOST_MPL_PREPROCESSING_MODE
#include <boost/mpl/unique.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/sort.hpp>

namespace mpl = boost::mpl;

int main() {
  using v = mpl::vector<
      mpl::int_<1>, mpl::int_<2>, mpl::int_<3>, mpl::int_<4>, mpl::int_<5>,
      mpl::int_<6>, mpl::int_<7>, mpl::int_<8>, mpl::int_<9>, mpl::int_<10>,
      mpl::int_<11>, mpl::int_<12>, mpl::int_<13>, mpl::int_<14>, mpl::int_<15>,
      mpl::int_<16>, mpl::int_<17>, mpl::int_<18>, mpl::int_<19>, mpl::int_<20>,
      mpl::int_<21>, mpl::int_<22>, mpl::int_<23>, mpl::int_<24>, mpl::int_<25>,
      mpl::int_<26>, mpl::int_<27>, mpl::int_<28>, mpl::int_<29>, mpl::int_<30>,
      mpl::int_<31>, mpl::int_<32>, mpl::int_<33>, mpl::int_<34>, mpl::int_<35>,
      mpl::int_<36>, mpl::int_<37>, mpl::int_<38>, mpl::int_<39>, mpl::int_<40>,
      mpl::int_<41>, mpl::int_<42>, mpl::int_<43>, mpl::int_<44>, mpl::int_<45>,
      mpl::int_<46>, mpl::int_<47>, mpl::int_<48>, mpl::int_<49>, mpl::int_<50>,
      mpl::int_<51>, mpl::int_<52>, mpl::int_<53>, mpl::int_<54>, mpl::int_<55>,
      mpl::int_<56>, mpl::int_<57>, mpl::int_<58>, mpl::int_<59>, mpl::int_<60>,
      mpl::int_<61>, mpl::int_<62>, mpl::int_<63>, mpl::int_<64>, mpl::int_<1>,
      mpl::int_<65>, mpl::int_<66>, mpl::int_<67>, mpl::int_<68>, mpl::int_<69>,
      mpl::int_<70>, mpl::int_<71>, mpl::int_<72>, mpl::int_<73>, mpl::int_<74>,
      mpl::int_<75>, mpl::int_<76>, mpl::int_<77>, mpl::int_<78>, mpl::int_<79>,
      mpl::int_<80>, mpl::int_<81>, mpl::int_<82>, mpl::int_<83>, mpl::int_<84>,
      mpl::int_<85>, mpl::int_<86>, mpl::int_<87>, mpl::int_<88>, mpl::int_<89>,
      mpl::int_<90>, mpl::int_<91>, mpl::int_<92>, mpl::int_<93>, mpl::int_<94>,
      mpl::int_<95>, mpl::int_<96>, mpl::int_<97>, mpl::int_<98>, mpl::int_<99>,
      mpl::int_<100>, mpl::int_<101>, mpl::int_<102>, mpl::int_<103>,
      mpl::int_<104>, mpl::int_<105>, mpl::int_<106>, mpl::int_<107>,
      mpl::int_<108>, mpl::int_<109>, mpl::int_<110>, mpl::int_<111>,
      mpl::int_<112>, mpl::int_<113>, mpl::int_<114>, mpl::int_<115>,
      mpl::int_<116>, mpl::int_<117>, mpl::int_<118>, mpl::int_<119>,
      mpl::int_<120>, mpl::int_<121>, mpl::int_<122>, mpl::int_<123>,
      mpl::int_<124>, mpl::int_<125>, mpl::int_<126>, mpl::int_<127>,
      mpl::int_<128>, mpl::int_<56>>;

  using u = typename mpl::unique<typename mpl::sort<v>::type,
                                 boost::is_same<mpl::_1, mpl::_2>>::type;

  static_assert(
      mpl::equal<mpl::vector<
                     mpl::int_<1>, mpl::int_<2>, mpl::int_<3>, mpl::int_<4>,
                     mpl::int_<5>, mpl::int_<6>, mpl::int_<7>, mpl::int_<8>,
                     mpl::int_<9>, mpl::int_<10>, mpl::int_<11>, mpl::int_<12>,
                     mpl::int_<13>, mpl::int_<14>, mpl::int_<15>, mpl::int_<16>,
                     mpl::int_<17>, mpl::int_<18>, mpl::int_<19>, mpl::int_<20>,
                     mpl::int_<21>, mpl::int_<22>, mpl::int_<23>, mpl::int_<24>,
                     mpl::int_<25>, mpl::int_<26>, mpl::int_<27>, mpl::int_<28>,
                     mpl::int_<29>, mpl::int_<30>, mpl::int_<31>, mpl::int_<32>,
                     mpl::int_<33>, mpl::int_<34>, mpl::int_<35>, mpl::int_<36>,
                     mpl::int_<37>, mpl::int_<38>, mpl::int_<39>, mpl::int_<40>,
                     mpl::int_<41>, mpl::int_<42>, mpl::int_<43>, mpl::int_<44>,
                     mpl::int_<45>, mpl::int_<46>, mpl::int_<47>, mpl::int_<48>,
                     mpl::int_<49>, mpl::int_<50>, mpl::int_<51>, mpl::int_<52>,
                     mpl::int_<53>, mpl::int_<54>, mpl::int_<55>, mpl::int_<56>,
                     mpl::int_<57>, mpl::int_<58>, mpl::int_<59>, mpl::int_<60>,
                     mpl::int_<61>, mpl::int_<62>, mpl::int_<63>, mpl::int_<64>,
                     mpl::int_<65>, mpl::int_<66>, mpl::int_<67>, mpl::int_<68>,
                     mpl::int_<69>, mpl::int_<70>, mpl::int_<71>, mpl::int_<72>,
                     mpl::int_<73>, mpl::int_<74>, mpl::int_<75>, mpl::int_<76>,
                     mpl::int_<77>, mpl::int_<78>, mpl::int_<79>, mpl::int_<80>,
                     mpl::int_<81>, mpl::int_<82>, mpl::int_<83>, mpl::int_<84>,
                     mpl::int_<85>, mpl::int_<86>, mpl::int_<87>, mpl::int_<88>,
                     mpl::int_<89>, mpl::int_<90>, mpl::int_<91>, mpl::int_<92>,
                     mpl::int_<93>, mpl::int_<94>, mpl::int_<95>, mpl::int_<96>,
                     mpl::int_<97>, mpl::int_<98>, mpl::int_<99>,
                     mpl::int_<100>, mpl::int_<101>, mpl::int_<102>,
                     mpl::int_<103>, mpl::int_<104>, mpl::int_<105>,
                     mpl::int_<106>, mpl::int_<107>, mpl::int_<108>,
                     mpl::int_<109>, mpl::int_<110>, mpl::int_<111>,
                     mpl::int_<112>, mpl::int_<113>, mpl::int_<114>,
                     mpl::int_<115>, mpl::int_<116>, mpl::int_<117>,
                     mpl::int_<118>, mpl::int_<119>, mpl::int_<120>,
                     mpl::int_<121>, mpl::int_<122>, mpl::int_<123>,
                     mpl::int_<124>, mpl::int_<125>, mpl::int_<126>,
                     mpl::int_<127>, mpl::int_<128>>,
                 u>::type::value,
      "");
}
