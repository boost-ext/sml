//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>

namespace sml = boost::sml;

#if !defined(_MSC_VER)
test transition_sizeof = [] {
  using namespace sml;
  constexpr auto i = 0;

  {
    auto t = "state"_s + "event"_e[([] {})];
    static_expect(0 == sizeof(t));
  }

  {
    auto t = "state"_s + "event"_e / [] {};
    static_expect(0 == sizeof(t));
  }

  {
    auto t = "state"_s + "event"_e[([] {})] / [] {};
    static_expect(0 == sizeof(t));
  }

  {
    auto t = "state"_s + "event"_e[([](int) {})] / [] {};
    static_expect(0 == sizeof(t));
  }

  {
    auto t = "state"_s + "event"_e[([] {})] / [](int) {};
    static_expect(0 == sizeof(t));
  }

  {
    auto t = "state"_s + "event"_e[([](int) {})] / [](int) {};
    static_expect(0 == sizeof(t));
  }

  {
    auto t = "state"_s + "event"_e[([](int, float) {})] / [](double, const int&) {};
    static_expect(0 == sizeof(t));
  }

  {
    auto t = "state"_s + "event"_e[([i] { (void)i; })] / [] {};
    static_expect(sizeof(i) == sizeof(t));
  }

  {
    auto t = "state"_s + "event"_e[([] {})] / [i] { (void)i; };
    static_expect(sizeof(i) == sizeof(t));
  }

  {
    auto t = "state"_s + "event"_e[([] {})] / [&i] { (void)i; };
    static_expect(sizeof(&i) == sizeof(t));
  }
};

test sm_sizeof_minimal = [] {
  struct minimal {
    auto operator()() noexcept {
      using namespace sml;
      return make_transition_table(*"idle"_s + "event1"_e = X);
    }
  };

  static_expect(1 /*current_state=1*/ == sizeof(sml::sm<minimal>{}));
};

test sm_sizeof_default_guard_action = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        *"idle"_s + "event1"_e = "s1"_s,
        "s1"_s + "event2"_e = X,
        "s2"_s + "event2"_e [([]{return true;})] = X,
        "s3"_s + "event3"_e / []{} = X,
        "s4"_s + "event4"_e [([]{return true;})] / []{} = X,
        "s5"_s + "event5"_e = X
      );
      // clang-format on
    }
  };

  static_expect(1 /*current_state=1*/ == sizeof(sml::sm<c>{}));
};

test sm_sizeof_no_capture = [] {
  struct no_capture_transition {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        *"idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{},
         "idle"_s + "event"_e [([]{return true;})] / []{}
      );
      // clang-format on
    }
  };
  static_expect(1 /*current_state=1*/ == sizeof(sml::sm<no_capture_transition>));
};

test sm_sizeof_more_than_256_transitions = [] {
  struct c {
    auto operator()() noexcept {
      using namespace sml;
      // clang-format off
      return make_transition_table(
        *"s1"_s + "event1"_e [([]{return true;})] / []{} = "s2"_s,
        "s2"_s + "event2"_e [([]{return true;})] / []{} = "s3"_s,
        "s3"_s + "event3"_e [([]{return true;})] / []{} = "s4"_s,
        "s4"_s + "event4"_e [([]{return true;})] / []{} = "s5"_s,
        "s5"_s + "event5"_e [([]{return true;})] / []{} = "s6"_s,
        "s6"_s + "event6"_e [([]{return true;})] / []{} = "s7"_s,
        "s7"_s + "event7"_e [([]{return true;})] / []{} = "s8"_s,
        "s8"_s + "event8"_e [([]{return true;})] / []{} = "s9"_s,
        "s9"_s + "event9"_e [([]{return true;})] / []{} = "s10"_s,
        "s10"_s + "event10"_e [([]{return true;})] / []{} = "s11"_s,
        "s11"_s + "event11"_e [([]{return true;})] / []{} = "s12"_s,
        "s12"_s + "event12"_e [([]{return true;})] / []{} = "s13"_s,
        "s13"_s + "event13"_e [([]{return true;})] / []{} = "s14"_s,
        "s14"_s + "event14"_e [([]{return true;})] / []{} = "s15"_s,
        "s15"_s + "event15"_e [([]{return true;})] / []{} = "s16"_s,
        "s16"_s + "event16"_e [([]{return true;})] / []{} = "s17"_s,
        "s17"_s + "event17"_e [([]{return true;})] / []{} = "s18"_s,
        "s18"_s + "event18"_e [([]{return true;})] / []{} = "s19"_s,
        "s19"_s + "event19"_e [([]{return true;})] / []{} = "s20"_s,
        "s20"_s + "event20"_e [([]{return true;})] / []{} = "s21"_s,
        "s21"_s + "event21"_e [([]{return true;})] / []{} = "s22"_s,
        "s22"_s + "event22"_e [([]{return true;})] / []{} = "s23"_s,
        "s23"_s + "event23"_e [([]{return true;})] / []{} = "s24"_s,
        "s24"_s + "event24"_e [([]{return true;})] / []{} = "s25"_s,
        "s25"_s + "event25"_e [([]{return true;})] / []{} = "s26"_s,
        "s26"_s + "event26"_e [([]{return true;})] / []{} = "s27"_s,
        "s27"_s + "event27"_e [([]{return true;})] / []{} = "s28"_s,
        "s28"_s + "event28"_e [([]{return true;})] / []{} = "s29"_s,
        "s29"_s + "event29"_e [([]{return true;})] / []{} = "s30"_s,
        "s30"_s + "event30"_e [([]{return true;})] / []{} = "s31"_s,
        "s31"_s + "event31"_e [([]{return true;})] / []{} = "s32"_s,
        "s32"_s + "event32"_e [([]{return true;})] / []{} = "s33"_s,
        "s33"_s + "event33"_e [([]{return true;})] / []{} = "s34"_s,
        "s34"_s + "event34"_e [([]{return true;})] / []{} = "s35"_s,
        "s35"_s + "event35"_e [([]{return true;})] / []{} = "s36"_s,
        "s36"_s + "event36"_e [([]{return true;})] / []{} = "s37"_s,
        "s37"_s + "event37"_e [([]{return true;})] / []{} = "s38"_s,
        "s38"_s + "event38"_e [([]{return true;})] / []{} = "s39"_s,
        "s39"_s + "event39"_e [([]{return true;})] / []{} = "s40"_s,
        "s40"_s + "event40"_e [([]{return true;})] / []{} = "s41"_s,
        "s41"_s + "event41"_e [([]{return true;})] / []{} = "s42"_s,
        "s42"_s + "event42"_e [([]{return true;})] / []{} = "s43"_s,
        "s43"_s + "event43"_e [([]{return true;})] / []{} = "s44"_s,
        "s44"_s + "event44"_e [([]{return true;})] / []{} = "s45"_s,
        "s45"_s + "event45"_e [([]{return true;})] / []{} = "s46"_s,
        "s46"_s + "event46"_e [([]{return true;})] / []{} = "s47"_s,
        "s47"_s + "event47"_e [([]{return true;})] / []{} = "s48"_s,
        "s48"_s + "event48"_e [([]{return true;})] / []{} = "s49"_s,
        "s49"_s + "event49"_e [([]{return true;})] / []{} = "s50"_s,
        "s50"_s + "event50"_e [([]{return true;})] / []{} = "s51"_s,
        "s51"_s + "event51"_e [([]{return true;})] / []{} = "s52"_s,
        "s52"_s + "event52"_e [([]{return true;})] / []{} = "s53"_s,
        "s53"_s + "event53"_e [([]{return true;})] / []{} = "s54"_s,
        "s54"_s + "event54"_e [([]{return true;})] / []{} = "s55"_s,
        "s55"_s + "event55"_e [([]{return true;})] / []{} = "s56"_s,
        "s56"_s + "event56"_e [([]{return true;})] / []{} = "s57"_s,
        "s57"_s + "event57"_e [([]{return true;})] / []{} = "s58"_s,
        "s58"_s + "event58"_e [([]{return true;})] / []{} = "s59"_s,
        "s59"_s + "event59"_e [([]{return true;})] / []{} = "s60"_s,
        "s60"_s + "event60"_e [([]{return true;})] / []{} = "s61"_s,
        "s61"_s + "event61"_e [([]{return true;})] / []{} = "s62"_s,
        "s62"_s + "event62"_e [([]{return true;})] / []{} = "s63"_s,
        "s63"_s + "event63"_e [([]{return true;})] / []{} = "s64"_s,
        "s64"_s + "event64"_e [([]{return true;})] / []{} = "s65"_s,
        "s65"_s + "event65"_e [([]{return true;})] / []{} = "s66"_s,
        "s66"_s + "event66"_e [([]{return true;})] / []{} = "s67"_s,
        "s67"_s + "event67"_e [([]{return true;})] / []{} = "s68"_s,
        "s68"_s + "event68"_e [([]{return true;})] / []{} = "s69"_s,
        "s69"_s + "event69"_e [([]{return true;})] / []{} = "s70"_s,
        "s70"_s + "event70"_e [([]{return true;})] / []{} = "s71"_s,
        "s71"_s + "event71"_e [([]{return true;})] / []{} = "s72"_s,
        "s72"_s + "event72"_e [([]{return true;})] / []{} = "s73"_s,
        "s73"_s + "event73"_e [([]{return true;})] / []{} = "s74"_s,
        "s74"_s + "event74"_e [([]{return true;})] / []{} = "s75"_s,
        "s75"_s + "event75"_e [([]{return true;})] / []{} = "s76"_s,
        "s76"_s + "event76"_e [([]{return true;})] / []{} = "s77"_s,
        "s77"_s + "event77"_e [([]{return true;})] / []{} = "s78"_s,
        "s78"_s + "event78"_e [([]{return true;})] / []{} = "s79"_s,
        "s79"_s + "event79"_e [([]{return true;})] / []{} = "s80"_s,
        "s80"_s + "event80"_e [([]{return true;})] / []{} = "s81"_s,
        "s81"_s + "event81"_e [([]{return true;})] / []{} = "s82"_s,
        "s82"_s + "event82"_e [([]{return true;})] / []{} = "s83"_s,
        "s83"_s + "event83"_e [([]{return true;})] / []{} = "s84"_s,
        "s84"_s + "event84"_e [([]{return true;})] / []{} = "s85"_s,
        "s85"_s + "event85"_e [([]{return true;})] / []{} = "s86"_s,
        "s86"_s + "event86"_e [([]{return true;})] / []{} = "s87"_s,
        "s87"_s + "event87"_e [([]{return true;})] / []{} = "s88"_s,
        "s88"_s + "event88"_e [([]{return true;})] / []{} = "s89"_s,
        "s89"_s + "event89"_e [([]{return true;})] / []{} = "s90"_s,
        "s90"_s + "event90"_e [([]{return true;})] / []{} = "s91"_s,
        "s91"_s + "event91"_e [([]{return true;})] / []{} = "s92"_s,
        "s92"_s + "event92"_e [([]{return true;})] / []{} = "s93"_s,
        "s93"_s + "event93"_e [([]{return true;})] / []{} = "s94"_s,
        "s94"_s + "event94"_e [([]{return true;})] / []{} = "s95"_s,
        "s95"_s + "event95"_e [([]{return true;})] / []{} = "s96"_s,
        "s96"_s + "event96"_e [([]{return true;})] / []{} = "s97"_s,
        "s97"_s + "event97"_e [([]{return true;})] / []{} = "s98"_s,
        "s98"_s + "event98"_e [([]{return true;})] / []{} = "s99"_s,
        "s99"_s + "event99"_e [([]{return true;})] / []{} = "s100"_s,
        "s100"_s + "event100"_e [([]{return true;})] / []{} = "s101"_s,
        "s101"_s + "event101"_e [([]{return true;})] / []{} = "s102"_s,
        "s102"_s + "event102"_e [([]{return true;})] / []{} = "s103"_s,
        "s103"_s + "event103"_e [([]{return true;})] / []{} = "s104"_s,
        "s104"_s + "event104"_e [([]{return true;})] / []{} = "s105"_s,
        "s105"_s + "event105"_e [([]{return true;})] / []{} = "s106"_s,
        "s106"_s + "event106"_e [([]{return true;})] / []{} = "s107"_s,
        "s107"_s + "event107"_e [([]{return true;})] / []{} = "s108"_s,
        "s108"_s + "event108"_e [([]{return true;})] / []{} = "s109"_s,
        "s109"_s + "event109"_e [([]{return true;})] / []{} = "s110"_s,
        "s110"_s + "event110"_e [([]{return true;})] / []{} = "s111"_s,
        "s111"_s + "event111"_e [([]{return true;})] / []{} = "s112"_s,
        "s112"_s + "event112"_e [([]{return true;})] / []{} = "s113"_s,
        "s113"_s + "event113"_e [([]{return true;})] / []{} = "s114"_s,
        "s114"_s + "event114"_e [([]{return true;})] / []{} = "s115"_s,
        "s115"_s + "event115"_e [([]{return true;})] / []{} = "s116"_s,
        "s116"_s + "event116"_e [([]{return true;})] / []{} = "s117"_s,
        "s117"_s + "event117"_e [([]{return true;})] / []{} = "s118"_s,
        "s118"_s + "event118"_e [([]{return true;})] / []{} = "s119"_s,
        "s119"_s + "event119"_e [([]{return true;})] / []{} = "s120"_s,
        "s120"_s + "event120"_e [([]{return true;})] / []{} = "s121"_s,
        "s121"_s + "event121"_e [([]{return true;})] / []{} = "s122"_s,
        "s122"_s + "event122"_e [([]{return true;})] / []{} = "s123"_s,
        "s123"_s + "event123"_e [([]{return true;})] / []{} = "s124"_s,
        "s124"_s + "event124"_e [([]{return true;})] / []{} = "s125"_s,
        "s125"_s + "event125"_e [([]{return true;})] / []{} = "s126"_s,
        "s126"_s + "event126"_e [([]{return true;})] / []{} = "s127"_s,
        "s127"_s + "event127"_e [([]{return true;})] / []{} = "s128"_s,
        "s128"_s + "event128"_e [([]{return true;})] / []{} = "s129"_s,
        "s129"_s + "event129"_e [([]{return true;})] / []{} = "s130"_s,
        "s130"_s + "event130"_e [([]{return true;})] / []{} = "s131"_s,
        "s131"_s + "event131"_e [([]{return true;})] / []{} = "s132"_s,
        "s132"_s + "event132"_e [([]{return true;})] / []{} = "s133"_s,
        "s133"_s + "event133"_e [([]{return true;})] / []{} = "s134"_s,
        "s134"_s + "event134"_e [([]{return true;})] / []{} = "s135"_s,
        "s135"_s + "event135"_e [([]{return true;})] / []{} = "s136"_s,
        "s136"_s + "event136"_e [([]{return true;})] / []{} = "s137"_s,
        "s137"_s + "event137"_e [([]{return true;})] / []{} = "s138"_s,
        "s138"_s + "event138"_e [([]{return true;})] / []{} = "s139"_s,
        "s139"_s + "event139"_e [([]{return true;})] / []{} = "s140"_s,
        "s140"_s + "event140"_e [([]{return true;})] / []{} = "s141"_s,
        "s141"_s + "event141"_e [([]{return true;})] / []{} = "s142"_s,
        "s142"_s + "event142"_e [([]{return true;})] / []{} = "s143"_s,
        "s143"_s + "event143"_e [([]{return true;})] / []{} = "s144"_s,
        "s144"_s + "event144"_e [([]{return true;})] / []{} = "s145"_s,
        "s145"_s + "event145"_e [([]{return true;})] / []{} = "s146"_s,
        "s146"_s + "event146"_e [([]{return true;})] / []{} = "s147"_s,
        "s147"_s + "event147"_e [([]{return true;})] / []{} = "s148"_s,
        "s148"_s + "event148"_e [([]{return true;})] / []{} = "s149"_s,
        "s149"_s + "event149"_e [([]{return true;})] / []{} = "s150"_s,
        "s150"_s + "event150"_e [([]{return true;})] / []{} = "s151"_s,
        "s151"_s + "event151"_e [([]{return true;})] / []{} = "s152"_s,
        "s152"_s + "event152"_e [([]{return true;})] / []{} = "s153"_s,
        "s153"_s + "event153"_e [([]{return true;})] / []{} = "s154"_s,
        "s154"_s + "event154"_e [([]{return true;})] / []{} = "s155"_s,
        "s155"_s + "event155"_e [([]{return true;})] / []{} = "s156"_s,
        "s156"_s + "event156"_e [([]{return true;})] / []{} = "s157"_s,
        "s157"_s + "event157"_e [([]{return true;})] / []{} = "s158"_s,
        "s158"_s + "event158"_e [([]{return true;})] / []{} = "s159"_s,
        "s159"_s + "event159"_e [([]{return true;})] / []{} = "s160"_s,
        "s160"_s + "event160"_e [([]{return true;})] / []{} = "s161"_s,
        "s161"_s + "event161"_e [([]{return true;})] / []{} = "s162"_s,
        "s162"_s + "event162"_e [([]{return true;})] / []{} = "s163"_s,
        "s163"_s + "event163"_e [([]{return true;})] / []{} = "s164"_s,
        "s164"_s + "event164"_e [([]{return true;})] / []{} = "s165"_s,
        "s165"_s + "event165"_e [([]{return true;})] / []{} = "s166"_s,
        "s166"_s + "event166"_e [([]{return true;})] / []{} = "s167"_s,
        "s167"_s + "event167"_e [([]{return true;})] / []{} = "s168"_s,
        "s168"_s + "event168"_e [([]{return true;})] / []{} = "s169"_s,
        "s169"_s + "event169"_e [([]{return true;})] / []{} = "s170"_s,
        "s170"_s + "event170"_e [([]{return true;})] / []{} = "s171"_s,
        "s171"_s + "event171"_e [([]{return true;})] / []{} = "s172"_s,
        "s172"_s + "event172"_e [([]{return true;})] / []{} = "s173"_s,
        "s173"_s + "event173"_e [([]{return true;})] / []{} = "s174"_s,
        "s174"_s + "event174"_e [([]{return true;})] / []{} = "s175"_s,
        "s175"_s + "event175"_e [([]{return true;})] / []{} = "s176"_s,
        "s176"_s + "event176"_e [([]{return true;})] / []{} = "s177"_s,
        "s177"_s + "event177"_e [([]{return true;})] / []{} = "s178"_s,
        "s178"_s + "event178"_e [([]{return true;})] / []{} = "s179"_s,
        "s179"_s + "event179"_e [([]{return true;})] / []{} = "s180"_s,
        "s180"_s + "event180"_e [([]{return true;})] / []{} = "s181"_s,
        "s181"_s + "event181"_e [([]{return true;})] / []{} = "s182"_s,
        "s182"_s + "event182"_e [([]{return true;})] / []{} = "s183"_s,
        "s183"_s + "event183"_e [([]{return true;})] / []{} = "s184"_s,
        "s184"_s + "event184"_e [([]{return true;})] / []{} = "s185"_s,
        "s185"_s + "event185"_e [([]{return true;})] / []{} = "s186"_s,
        "s186"_s + "event186"_e [([]{return true;})] / []{} = "s187"_s,
        "s187"_s + "event187"_e [([]{return true;})] / []{} = "s188"_s,
        "s188"_s + "event188"_e [([]{return true;})] / []{} = "s189"_s,
        "s189"_s + "event189"_e [([]{return true;})] / []{} = "s190"_s,
        "s190"_s + "event190"_e [([]{return true;})] / []{} = "s191"_s,
        "s191"_s + "event191"_e [([]{return true;})] / []{} = "s192"_s,
        "s192"_s + "event192"_e [([]{return true;})] / []{} = "s193"_s,
        "s193"_s + "event193"_e [([]{return true;})] / []{} = "s194"_s,
        "s194"_s + "event194"_e [([]{return true;})] / []{} = "s195"_s,
        "s195"_s + "event195"_e [([]{return true;})] / []{} = "s196"_s,
        "s196"_s + "event196"_e [([]{return true;})] / []{} = "s197"_s,
        "s197"_s + "event197"_e [([]{return true;})] / []{} = "s198"_s,
        "s198"_s + "event198"_e [([]{return true;})] / []{} = "s199"_s,
        "s199"_s + "event199"_e [([]{return true;})] / []{} = "s200"_s,
        "s200"_s + "event200"_e [([]{return true;})] / []{} = "s201"_s,
        "s201"_s + "event201"_e [([]{return true;})] / []{} = "s202"_s,
        "s202"_s + "event202"_e [([]{return true;})] / []{} = "s203"_s,
        "s203"_s + "event203"_e [([]{return true;})] / []{} = "s204"_s,
        "s204"_s + "event204"_e [([]{return true;})] / []{} = "s205"_s,
        "s205"_s + "event205"_e [([]{return true;})] / []{} = "s206"_s,
        "s206"_s + "event206"_e [([]{return true;})] / []{} = "s207"_s,
        "s207"_s + "event207"_e [([]{return true;})] / []{} = "s208"_s,
        "s208"_s + "event208"_e [([]{return true;})] / []{} = "s209"_s,
        "s209"_s + "event209"_e [([]{return true;})] / []{} = "s210"_s,
        "s210"_s + "event210"_e [([]{return true;})] / []{} = "s211"_s,
        "s211"_s + "event211"_e [([]{return true;})] / []{} = "s212"_s,
        "s212"_s + "event212"_e [([]{return true;})] / []{} = "s213"_s,
        "s213"_s + "event213"_e [([]{return true;})] / []{} = "s214"_s,
        "s214"_s + "event214"_e [([]{return true;})] / []{} = "s215"_s,
        "s215"_s + "event215"_e [([]{return true;})] / []{} = "s216"_s,
        "s216"_s + "event216"_e [([]{return true;})] / []{} = "s217"_s,
        "s217"_s + "event217"_e [([]{return true;})] / []{} = "s218"_s,
        "s218"_s + "event218"_e [([]{return true;})] / []{} = "s219"_s,
        "s219"_s + "event219"_e [([]{return true;})] / []{} = "s220"_s,
        "s220"_s + "event220"_e [([]{return true;})] / []{} = "s221"_s,
        "s221"_s + "event221"_e [([]{return true;})] / []{} = "s222"_s,
        "s222"_s + "event222"_e [([]{return true;})] / []{} = "s223"_s,
        "s223"_s + "event223"_e [([]{return true;})] / []{} = "s224"_s,
        "s224"_s + "event224"_e [([]{return true;})] / []{} = "s225"_s,
        "s225"_s + "event225"_e [([]{return true;})] / []{} = "s226"_s,
        "s226"_s + "event226"_e [([]{return true;})] / []{} = "s227"_s,
        "s227"_s + "event227"_e [([]{return true;})] / []{} = "s228"_s,
        "s228"_s + "event228"_e [([]{return true;})] / []{} = "s229"_s,
        "s229"_s + "event229"_e [([]{return true;})] / []{} = "s230"_s,
        "s230"_s + "event230"_e [([]{return true;})] / []{} = "s231"_s,
        "s231"_s + "event231"_e [([]{return true;})] / []{} = "s232"_s,
        "s232"_s + "event232"_e [([]{return true;})] / []{} = "s233"_s,
        "s233"_s + "event233"_e [([]{return true;})] / []{} = "s234"_s,
        "s234"_s + "event234"_e [([]{return true;})] / []{} = "s235"_s,
        "s235"_s + "event235"_e [([]{return true;})] / []{} = "s236"_s,
        "s236"_s + "event236"_e [([]{return true;})] / []{} = "s237"_s,
        "s237"_s + "event237"_e [([]{return true;})] / []{} = "s238"_s,
        "s238"_s + "event238"_e [([]{return true;})] / []{} = "s239"_s,
        "s239"_s + "event239"_e [([]{return true;})] / []{} = "s240"_s,
        "s240"_s + "event240"_e [([]{return true;})] / []{} = "s241"_s,
        "s241"_s + "event241"_e [([]{return true;})] / []{} = "s242"_s,
        "s242"_s + "event242"_e [([]{return true;})] / []{} = "s243"_s,
        "s243"_s + "event243"_e [([]{return true;})] / []{} = "s244"_s,
        "s244"_s + "event244"_e [([]{return true;})] / []{} = "s245"_s,
        "s245"_s + "event245"_e [([]{return true;})] / []{} = "s246"_s,
        "s246"_s + "event246"_e [([]{return true;})] / []{} = "s247"_s,
        "s247"_s + "event247"_e [([]{return true;})] / []{} = "s248"_s,
        "s248"_s + "event248"_e [([]{return true;})] / []{} = "s249"_s,
        "s249"_s + "event249"_e [([]{return true;})] / []{} = "s250"_s,
        "s250"_s + "event250"_e [([]{return true;})] / []{} = "s251"_s,
        "s251"_s + "event251"_e [([]{return true;})] / []{} = "s252"_s,
        "s252"_s + "event252"_e [([]{return true;})] / []{} = "s253"_s,
        "s253"_s + "event253"_e [([]{return true;})] / []{} = "s254"_s,
        "s254"_s + "event254"_e [([]{return true;})] / []{} = "s255"_s,
        "s255"_s + "event255"_e [([]{return true;})] / []{} = "s256"_s,
        "s256"_s + "event256"_e [([]{return true;})] / []{} = "s257"_s,
        "s257"_s + "event257"_e [([]{return true;})] / []{} = "s258"_s
      );
      // clang-format on
    }
  };
  static_expect(2 /*current_state=2*/ == sizeof(sml::sm<c>));
};
#endif
