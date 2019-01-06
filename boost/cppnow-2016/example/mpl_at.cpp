//
// Copyright (c) 2016-2019 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#define BOOST_MPL_LIMIT_MAP_SIZE 128
#include <boost/preprocessor/iterate.hpp>
#include <boost/mpl/map/map50.hpp>

namespace boost {
namespace mpl {
// clang-format off
#define BOOST_PP_ITERATION_PARAMS_1                                            \
  (3, (51, BOOST_MPL_LIMIT_MAP_SIZE, <boost/mpl/map/aux_/numbered.hpp>))
#include BOOST_PP_ITERATE()
// clang-format on
} // namespace mpl
} // namespace boost

#define BOOST_MPL_PREPROCESSING_MODE
#include <boost/mpl/map.hpp>
#undef BOOST_MPL_PREPROCESSING_MODE

#include <boost/mpl/map.hpp>
#include <boost/mpl/at.hpp>

namespace mpl = boost::mpl;

using m = mpl::map<mpl::pair<mpl::int_<1>, mpl::int_<1>>,
                   mpl::pair<mpl::int_<2>, mpl::int_<2>>,
                   mpl::pair<mpl::int_<3>, mpl::int_<3>>,
                   mpl::pair<mpl::int_<4>, mpl::int_<4>>,
                   mpl::pair<mpl::int_<5>, mpl::int_<5>>,
                   mpl::pair<mpl::int_<6>, mpl::int_<6>>,
                   mpl::pair<mpl::int_<7>, mpl::int_<7>>,
                   mpl::pair<mpl::int_<8>, mpl::int_<8>>,
                   mpl::pair<mpl::int_<9>, mpl::int_<9>>,
                   mpl::pair<mpl::int_<10>, mpl::int_<10>>,
                   mpl::pair<mpl::int_<11>, mpl::int_<11>>,
                   mpl::pair<mpl::int_<12>, mpl::int_<12>>,
                   mpl::pair<mpl::int_<13>, mpl::int_<13>>,
                   mpl::pair<mpl::int_<14>, mpl::int_<14>>,
                   mpl::pair<mpl::int_<15>, mpl::int_<15>>,
                   mpl::pair<mpl::int_<16>, mpl::int_<16>>,
                   mpl::pair<mpl::int_<17>, mpl::int_<17>>,
                   mpl::pair<mpl::int_<18>, mpl::int_<18>>,
                   mpl::pair<mpl::int_<19>, mpl::int_<19>>,
                   mpl::pair<mpl::int_<20>, mpl::int_<20>>,
                   mpl::pair<mpl::int_<21>, mpl::int_<21>>,
                   mpl::pair<mpl::int_<22>, mpl::int_<22>>,
                   mpl::pair<mpl::int_<23>, mpl::int_<23>>,
                   mpl::pair<mpl::int_<24>, mpl::int_<24>>,
                   mpl::pair<mpl::int_<25>, mpl::int_<25>>,
                   mpl::pair<mpl::int_<26>, mpl::int_<26>>,
                   mpl::pair<mpl::int_<27>, mpl::int_<27>>,
                   mpl::pair<mpl::int_<28>, mpl::int_<28>>,
                   mpl::pair<mpl::int_<29>, mpl::int_<29>>,
                   mpl::pair<mpl::int_<30>, mpl::int_<30>>,
                   mpl::pair<mpl::int_<31>, mpl::int_<31>>,
                   mpl::pair<mpl::int_<32>, mpl::int_<32>>,
                   mpl::pair<mpl::int_<33>, mpl::int_<33>>,
                   mpl::pair<mpl::int_<34>, mpl::int_<34>>,
                   mpl::pair<mpl::int_<35>, mpl::int_<35>>,
                   mpl::pair<mpl::int_<36>, mpl::int_<36>>,
                   mpl::pair<mpl::int_<37>, mpl::int_<37>>,
                   mpl::pair<mpl::int_<38>, mpl::int_<38>>,
                   mpl::pair<mpl::int_<39>, mpl::int_<39>>,
                   mpl::pair<mpl::int_<40>, mpl::int_<40>>,
                   mpl::pair<mpl::int_<41>, mpl::int_<41>>,
                   mpl::pair<mpl::int_<42>, mpl::int_<42>>,
                   mpl::pair<mpl::int_<43>, mpl::int_<43>>,
                   mpl::pair<mpl::int_<44>, mpl::int_<44>>,
                   mpl::pair<mpl::int_<45>, mpl::int_<45>>,
                   mpl::pair<mpl::int_<46>, mpl::int_<46>>,
                   mpl::pair<mpl::int_<47>, mpl::int_<47>>,
                   mpl::pair<mpl::int_<48>, mpl::int_<48>>,
                   mpl::pair<mpl::int_<49>, mpl::int_<49>>,
                   mpl::pair<mpl::int_<50>, mpl::int_<50>>,
                   mpl::pair<mpl::int_<51>, mpl::int_<51>>,
                   mpl::pair<mpl::int_<52>, mpl::int_<52>>,
                   mpl::pair<mpl::int_<53>, mpl::int_<53>>,
                   mpl::pair<mpl::int_<54>, mpl::int_<54>>,
                   mpl::pair<mpl::int_<55>, mpl::int_<55>>,
                   mpl::pair<mpl::int_<56>, mpl::int_<56>>,
                   mpl::pair<mpl::int_<57>, mpl::int_<57>>,
                   mpl::pair<mpl::int_<58>, mpl::int_<58>>,
                   mpl::pair<mpl::int_<59>, mpl::int_<59>>,
                   mpl::pair<mpl::int_<60>, mpl::int_<60>>,
                   mpl::pair<mpl::int_<61>, mpl::int_<61>>,
                   mpl::pair<mpl::int_<62>, mpl::int_<62>>,
                   mpl::pair<mpl::int_<63>, mpl::int_<63>>,
                   mpl::pair<mpl::int_<64>, mpl::int_<64>>,
                   mpl::pair<mpl::int_<65>, mpl::int_<65>>,
                   mpl::pair<mpl::int_<66>, mpl::int_<66>>,
                   mpl::pair<mpl::int_<67>, mpl::int_<67>>,
                   mpl::pair<mpl::int_<68>, mpl::int_<68>>,
                   mpl::pair<mpl::int_<69>, mpl::int_<69>>,
                   mpl::pair<mpl::int_<70>, mpl::int_<70>>,
                   mpl::pair<mpl::int_<71>, mpl::int_<71>>,
                   mpl::pair<mpl::int_<72>, mpl::int_<72>>,
                   mpl::pair<mpl::int_<73>, mpl::int_<73>>,
                   mpl::pair<mpl::int_<74>, mpl::int_<74>>,
                   mpl::pair<mpl::int_<75>, mpl::int_<75>>,
                   mpl::pair<mpl::int_<76>, mpl::int_<76>>,
                   mpl::pair<mpl::int_<77>, mpl::int_<77>>,
                   mpl::pair<mpl::int_<78>, mpl::int_<78>>,
                   mpl::pair<mpl::int_<79>, mpl::int_<79>>,
                   mpl::pair<mpl::int_<80>, mpl::int_<80>>,
                   mpl::pair<mpl::int_<81>, mpl::int_<81>>,
                   mpl::pair<mpl::int_<82>, mpl::int_<82>>,
                   mpl::pair<mpl::int_<83>, mpl::int_<83>>,
                   mpl::pair<mpl::int_<84>, mpl::int_<84>>,
                   mpl::pair<mpl::int_<85>, mpl::int_<85>>,
                   mpl::pair<mpl::int_<86>, mpl::int_<86>>,
                   mpl::pair<mpl::int_<87>, mpl::int_<87>>,
                   mpl::pair<mpl::int_<88>, mpl::int_<88>>,
                   mpl::pair<mpl::int_<89>, mpl::int_<89>>,
                   mpl::pair<mpl::int_<90>, mpl::int_<90>>,
                   mpl::pair<mpl::int_<91>, mpl::int_<91>>,
                   mpl::pair<mpl::int_<92>, mpl::int_<92>>,
                   mpl::pair<mpl::int_<93>, mpl::int_<93>>,
                   mpl::pair<mpl::int_<94>, mpl::int_<94>>,
                   mpl::pair<mpl::int_<95>, mpl::int_<95>>,
                   mpl::pair<mpl::int_<96>, mpl::int_<96>>,
                   mpl::pair<mpl::int_<97>, mpl::int_<97>>,
                   mpl::pair<mpl::int_<98>, mpl::int_<98>>,
                   mpl::pair<mpl::int_<99>, mpl::int_<99>>,
                   mpl::pair<mpl::int_<100>, mpl::int_<100>>,
                   mpl::pair<mpl::int_<101>, mpl::int_<101>>,
                   mpl::pair<mpl::int_<102>, mpl::int_<102>>,
                   mpl::pair<mpl::int_<103>, mpl::int_<103>>,
                   mpl::pair<mpl::int_<104>, mpl::int_<104>>,
                   mpl::pair<mpl::int_<105>, mpl::int_<105>>,
                   mpl::pair<mpl::int_<106>, mpl::int_<106>>,
                   mpl::pair<mpl::int_<107>, mpl::int_<107>>,
                   mpl::pair<mpl::int_<108>, mpl::int_<108>>,
                   mpl::pair<mpl::int_<109>, mpl::int_<109>>,
                   mpl::pair<mpl::int_<110>, mpl::int_<110>>,
                   mpl::pair<mpl::int_<111>, mpl::int_<111>>,
                   mpl::pair<mpl::int_<112>, mpl::int_<112>>,
                   mpl::pair<mpl::int_<113>, mpl::int_<113>>,
                   mpl::pair<mpl::int_<114>, mpl::int_<114>>,
                   mpl::pair<mpl::int_<115>, mpl::int_<115>>,
                   mpl::pair<mpl::int_<116>, mpl::int_<116>>,
                   mpl::pair<mpl::int_<117>, mpl::int_<117>>,
                   mpl::pair<mpl::int_<118>, mpl::int_<118>>,
                   mpl::pair<mpl::int_<119>, mpl::int_<119>>,
                   mpl::pair<mpl::int_<120>, mpl::int_<120>>,
                   mpl::pair<mpl::int_<121>, mpl::int_<121>>,
                   mpl::pair<mpl::int_<122>, mpl::int_<122>>,
                   mpl::pair<mpl::int_<123>, mpl::int_<123>>,
                   mpl::pair<mpl::int_<124>, mpl::int_<124>>,
                   mpl::pair<mpl::int_<125>, mpl::int_<125>>,
                   mpl::pair<mpl::int_<126>, mpl::int_<126>>,
                   mpl::pair<mpl::int_<127>, mpl::int_<127>>,
                   mpl::pair<mpl::int_<128>, mpl::int_<128>>>;

template <class> struct q;
int main() {
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<1>>::type,
                               mpl::int_<1>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<2>>::type,
                               mpl::int_<2>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<3>>::type,
                               mpl::int_<3>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<4>>::type,
                               mpl::int_<4>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<5>>::type,
                               mpl::int_<5>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<6>>::type,
                               mpl::int_<6>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<7>>::type,
                               mpl::int_<7>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<8>>::type,
                               mpl::int_<8>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<9>>::type,
                               mpl::int_<9>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<10>>::type,
                               mpl::int_<10>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<11>>::type,
                               mpl::int_<11>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<12>>::type,
                               mpl::int_<12>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<13>>::type,
                               mpl::int_<13>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<14>>::type,
                               mpl::int_<14>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<15>>::type,
                               mpl::int_<15>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<16>>::type,
                               mpl::int_<16>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<17>>::type,
                               mpl::int_<17>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<18>>::type,
                               mpl::int_<18>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<19>>::type,
                               mpl::int_<19>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<20>>::type,
                               mpl::int_<20>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<21>>::type,
                               mpl::int_<21>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<22>>::type,
                               mpl::int_<22>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<23>>::type,
                               mpl::int_<23>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<24>>::type,
                               mpl::int_<24>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<25>>::type,
                               mpl::int_<25>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<26>>::type,
                               mpl::int_<26>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<27>>::type,
                               mpl::int_<27>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<28>>::type,
                               mpl::int_<28>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<29>>::type,
                               mpl::int_<29>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<30>>::type,
                               mpl::int_<30>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<31>>::type,
                               mpl::int_<31>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<32>>::type,
                               mpl::int_<32>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<33>>::type,
                               mpl::int_<33>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<34>>::type,
                               mpl::int_<34>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<35>>::type,
                               mpl::int_<35>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<36>>::type,
                               mpl::int_<36>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<37>>::type,
                               mpl::int_<37>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<38>>::type,
                               mpl::int_<38>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<39>>::type,
                               mpl::int_<39>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<40>>::type,
                               mpl::int_<40>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<41>>::type,
                               mpl::int_<41>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<42>>::type,
                               mpl::int_<42>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<43>>::type,
                               mpl::int_<43>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<44>>::type,
                               mpl::int_<44>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<45>>::type,
                               mpl::int_<45>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<46>>::type,
                               mpl::int_<46>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<47>>::type,
                               mpl::int_<47>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<48>>::type,
                               mpl::int_<48>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<49>>::type,
                               mpl::int_<49>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<50>>::type,
                               mpl::int_<50>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<51>>::type,
                               mpl::int_<51>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<52>>::type,
                               mpl::int_<52>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<53>>::type,
                               mpl::int_<53>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<54>>::type,
                               mpl::int_<54>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<55>>::type,
                               mpl::int_<55>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<56>>::type,
                               mpl::int_<56>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<57>>::type,
                               mpl::int_<57>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<58>>::type,
                               mpl::int_<58>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<59>>::type,
                               mpl::int_<59>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<60>>::type,
                               mpl::int_<60>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<61>>::type,
                               mpl::int_<61>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<62>>::type,
                               mpl::int_<62>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<63>>::type,
                               mpl::int_<63>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<64>>::type,
                               mpl::int_<64>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<65>>::type,
                               mpl::int_<65>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<66>>::type,
                               mpl::int_<66>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<67>>::type,
                               mpl::int_<67>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<68>>::type,
                               mpl::int_<68>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<69>>::type,
                               mpl::int_<69>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<70>>::type,
                               mpl::int_<70>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<71>>::type,
                               mpl::int_<71>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<72>>::type,
                               mpl::int_<72>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<73>>::type,
                               mpl::int_<73>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<74>>::type,
                               mpl::int_<74>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<75>>::type,
                               mpl::int_<75>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<76>>::type,
                               mpl::int_<76>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<77>>::type,
                               mpl::int_<77>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<78>>::type,
                               mpl::int_<78>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<79>>::type,
                               mpl::int_<79>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<80>>::type,
                               mpl::int_<80>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<81>>::type,
                               mpl::int_<81>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<82>>::type,
                               mpl::int_<82>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<83>>::type,
                               mpl::int_<83>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<84>>::type,
                               mpl::int_<84>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<85>>::type,
                               mpl::int_<85>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<86>>::type,
                               mpl::int_<86>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<87>>::type,
                               mpl::int_<87>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<88>>::type,
                               mpl::int_<88>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<89>>::type,
                               mpl::int_<89>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<90>>::type,
                               mpl::int_<90>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<91>>::type,
                               mpl::int_<91>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<92>>::type,
                               mpl::int_<92>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<93>>::type,
                               mpl::int_<93>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<94>>::type,
                               mpl::int_<94>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<95>>::type,
                               mpl::int_<95>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<96>>::type,
                               mpl::int_<96>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<97>>::type,
                               mpl::int_<97>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<98>>::type,
                               mpl::int_<98>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<99>>::type,
                               mpl::int_<99>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<100>>::type,
                               mpl::int_<100>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<101>>::type,
                               mpl::int_<101>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<102>>::type,
                               mpl::int_<102>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<103>>::type,
                               mpl::int_<103>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<104>>::type,
                               mpl::int_<104>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<105>>::type,
                               mpl::int_<105>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<106>>::type,
                               mpl::int_<106>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<107>>::type,
                               mpl::int_<107>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<108>>::type,
                               mpl::int_<108>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<109>>::type,
                               mpl::int_<109>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<110>>::type,
                               mpl::int_<110>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<111>>::type,
                               mpl::int_<111>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<112>>::type,
                               mpl::int_<112>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<113>>::type,
                               mpl::int_<113>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<114>>::type,
                               mpl::int_<114>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<115>>::type,
                               mpl::int_<115>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<116>>::type,
                               mpl::int_<116>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<117>>::type,
                               mpl::int_<117>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<118>>::type,
                               mpl::int_<118>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<119>>::type,
                               mpl::int_<119>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<120>>::type,
                               mpl::int_<120>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<121>>::type,
                               mpl::int_<121>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<122>>::type,
                               mpl::int_<122>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<123>>::type,
                               mpl::int_<123>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<124>>::type,
                               mpl::int_<124>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<125>>::type,
                               mpl::int_<125>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<126>>::type,
                               mpl::int_<126>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<127>>::type,
                               mpl::int_<127>>::value,
                "");
  static_assert(boost::is_same<typename mpl::at<m, mpl::int_<128>>::type,
                               mpl::int_<128>>::value,
                "");
}
