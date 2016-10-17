//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/msm-lite.hpp>

namespace msm = boost::msm::lite;

test transition_sizeof = [] {
  using namespace msm;
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
    auto t = "state"_s + "event"_e[([i] {})] / [] {};
    static_expect(sizeof(i) == sizeof(t));
  }

  {
    auto t = "state"_s + "event"_e[([] {})] / [i] {};
    static_expect(sizeof(i) == sizeof(t));
  }

  {
    auto t = "state"_s + "event"_e[([] {})] / [&i] {};
    static_expect(sizeof(&i) == sizeof(t));
  }
};

test sm_sizeof_empty = [] {
  struct empty {
    auto operator()() noexcept {
      using namespace msm;
      return make_transition_table();
    }
  };

  static_expect(1 /*current_state=1*/ == sizeof(msm::sm<empty>{}));
};

test sm_sizeof_default_guard_action = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
        *"idle"_s + "event1"_e = "s1"_s,
        "s1"_s + "event2"_e = X,
        "s2"_s + "event2"_e [([]{})] = X,
        "s3"_s + "event3"_e / []{} = X,
        "s4"_s + "event4"_e [([]{})] / []{} = X,
        "s5"_s + "event5"_e = X
      );
      // clang-format on
    }
  };

  static_expect(1 /*current_state=1*/ == sizeof(msm::sm<c>{}));
};

test sm_sizeof_no_capture = [] {
  struct no_capture_transition {
    auto operator()() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
        *"idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{},
         "idle"_s + "event"_e [([]{})] / []{}
      );
      // clang-format on
    }
  };
  static_expect(1 /*current_state=1*/ == sizeof(msm::sm<no_capture_transition>));
};

test sm_sizeof_more_than_256_transitions = [] {
  struct c {
    auto operator()() noexcept {
      using namespace msm;
      // clang-format off
      return make_transition_table(
        *"s1"_s + "event1"_e [([]{})] / []{} = "s2"_s,
        "s2"_s + "event2"_e [([]{})] / []{} = "s3"_s,
        "s3"_s + "event3"_e [([]{})] / []{} = "s4"_s,
        "s4"_s + "event4"_e [([]{})] / []{} = "s5"_s,
        "s5"_s + "event5"_e [([]{})] / []{} = "s6"_s,
        "s6"_s + "event6"_e [([]{})] / []{} = "s7"_s,
        "s7"_s + "event7"_e [([]{})] / []{} = "s8"_s,
        "s8"_s + "event8"_e [([]{})] / []{} = "s9"_s,
        "s9"_s + "event9"_e [([]{})] / []{} = "s10"_s,
        "s10"_s + "event10"_e [([]{})] / []{} = "s11"_s,
        "s11"_s + "event11"_e [([]{})] / []{} = "s12"_s,
        "s12"_s + "event12"_e [([]{})] / []{} = "s13"_s,
        "s13"_s + "event13"_e [([]{})] / []{} = "s14"_s,
        "s14"_s + "event14"_e [([]{})] / []{} = "s15"_s,
        "s15"_s + "event15"_e [([]{})] / []{} = "s16"_s,
        "s16"_s + "event16"_e [([]{})] / []{} = "s17"_s,
        "s17"_s + "event17"_e [([]{})] / []{} = "s18"_s,
        "s18"_s + "event18"_e [([]{})] / []{} = "s19"_s,
        "s19"_s + "event19"_e [([]{})] / []{} = "s20"_s,
        "s20"_s + "event20"_e [([]{})] / []{} = "s21"_s,
        "s21"_s + "event21"_e [([]{})] / []{} = "s22"_s,
        "s22"_s + "event22"_e [([]{})] / []{} = "s23"_s,
        "s23"_s + "event23"_e [([]{})] / []{} = "s24"_s,
        "s24"_s + "event24"_e [([]{})] / []{} = "s25"_s,
        "s25"_s + "event25"_e [([]{})] / []{} = "s26"_s,
        "s26"_s + "event26"_e [([]{})] / []{} = "s27"_s,
        "s27"_s + "event27"_e [([]{})] / []{} = "s28"_s,
        "s28"_s + "event28"_e [([]{})] / []{} = "s29"_s,
        "s29"_s + "event29"_e [([]{})] / []{} = "s30"_s,
        "s30"_s + "event30"_e [([]{})] / []{} = "s31"_s,
        "s31"_s + "event31"_e [([]{})] / []{} = "s32"_s,
        "s32"_s + "event32"_e [([]{})] / []{} = "s33"_s,
        "s33"_s + "event33"_e [([]{})] / []{} = "s34"_s,
        "s34"_s + "event34"_e [([]{})] / []{} = "s35"_s,
        "s35"_s + "event35"_e [([]{})] / []{} = "s36"_s,
        "s36"_s + "event36"_e [([]{})] / []{} = "s37"_s,
        "s37"_s + "event37"_e [([]{})] / []{} = "s38"_s,
        "s38"_s + "event38"_e [([]{})] / []{} = "s39"_s,
        "s39"_s + "event39"_e [([]{})] / []{} = "s40"_s,
        "s40"_s + "event40"_e [([]{})] / []{} = "s41"_s,
        "s41"_s + "event41"_e [([]{})] / []{} = "s42"_s,
        "s42"_s + "event42"_e [([]{})] / []{} = "s43"_s,
        "s43"_s + "event43"_e [([]{})] / []{} = "s44"_s,
        "s44"_s + "event44"_e [([]{})] / []{} = "s45"_s,
        "s45"_s + "event45"_e [([]{})] / []{} = "s46"_s,
        "s46"_s + "event46"_e [([]{})] / []{} = "s47"_s,
        "s47"_s + "event47"_e [([]{})] / []{} = "s48"_s,
        "s48"_s + "event48"_e [([]{})] / []{} = "s49"_s,
        "s49"_s + "event49"_e [([]{})] / []{} = "s50"_s,
        "s50"_s + "event50"_e [([]{})] / []{} = "s51"_s,
        "s51"_s + "event51"_e [([]{})] / []{} = "s52"_s,
        "s52"_s + "event52"_e [([]{})] / []{} = "s53"_s,
        "s53"_s + "event53"_e [([]{})] / []{} = "s54"_s,
        "s54"_s + "event54"_e [([]{})] / []{} = "s55"_s,
        "s55"_s + "event55"_e [([]{})] / []{} = "s56"_s,
        "s56"_s + "event56"_e [([]{})] / []{} = "s57"_s,
        "s57"_s + "event57"_e [([]{})] / []{} = "s58"_s,
        "s58"_s + "event58"_e [([]{})] / []{} = "s59"_s,
        "s59"_s + "event59"_e [([]{})] / []{} = "s60"_s,
        "s60"_s + "event60"_e [([]{})] / []{} = "s61"_s,
        "s61"_s + "event61"_e [([]{})] / []{} = "s62"_s,
        "s62"_s + "event62"_e [([]{})] / []{} = "s63"_s,
        "s63"_s + "event63"_e [([]{})] / []{} = "s64"_s,
        "s64"_s + "event64"_e [([]{})] / []{} = "s65"_s,
        "s65"_s + "event65"_e [([]{})] / []{} = "s66"_s,
        "s66"_s + "event66"_e [([]{})] / []{} = "s67"_s,
        "s67"_s + "event67"_e [([]{})] / []{} = "s68"_s,
        "s68"_s + "event68"_e [([]{})] / []{} = "s69"_s,
        "s69"_s + "event69"_e [([]{})] / []{} = "s70"_s,
        "s70"_s + "event70"_e [([]{})] / []{} = "s71"_s,
        "s71"_s + "event71"_e [([]{})] / []{} = "s72"_s,
        "s72"_s + "event72"_e [([]{})] / []{} = "s73"_s,
        "s73"_s + "event73"_e [([]{})] / []{} = "s74"_s,
        "s74"_s + "event74"_e [([]{})] / []{} = "s75"_s,
        "s75"_s + "event75"_e [([]{})] / []{} = "s76"_s,
        "s76"_s + "event76"_e [([]{})] / []{} = "s77"_s,
        "s77"_s + "event77"_e [([]{})] / []{} = "s78"_s,
        "s78"_s + "event78"_e [([]{})] / []{} = "s79"_s,
        "s79"_s + "event79"_e [([]{})] / []{} = "s80"_s,
        "s80"_s + "event80"_e [([]{})] / []{} = "s81"_s,
        "s81"_s + "event81"_e [([]{})] / []{} = "s82"_s,
        "s82"_s + "event82"_e [([]{})] / []{} = "s83"_s,
        "s83"_s + "event83"_e [([]{})] / []{} = "s84"_s,
        "s84"_s + "event84"_e [([]{})] / []{} = "s85"_s,
        "s85"_s + "event85"_e [([]{})] / []{} = "s86"_s,
        "s86"_s + "event86"_e [([]{})] / []{} = "s87"_s,
        "s87"_s + "event87"_e [([]{})] / []{} = "s88"_s,
        "s88"_s + "event88"_e [([]{})] / []{} = "s89"_s,
        "s89"_s + "event89"_e [([]{})] / []{} = "s90"_s,
        "s90"_s + "event90"_e [([]{})] / []{} = "s91"_s,
        "s91"_s + "event91"_e [([]{})] / []{} = "s92"_s,
        "s92"_s + "event92"_e [([]{})] / []{} = "s93"_s,
        "s93"_s + "event93"_e [([]{})] / []{} = "s94"_s,
        "s94"_s + "event94"_e [([]{})] / []{} = "s95"_s,
        "s95"_s + "event95"_e [([]{})] / []{} = "s96"_s,
        "s96"_s + "event96"_e [([]{})] / []{} = "s97"_s,
        "s97"_s + "event97"_e [([]{})] / []{} = "s98"_s,
        "s98"_s + "event98"_e [([]{})] / []{} = "s99"_s,
        "s99"_s + "event99"_e [([]{})] / []{} = "s100"_s,
        "s100"_s + "event100"_e [([]{})] / []{} = "s101"_s,
        "s101"_s + "event101"_e [([]{})] / []{} = "s102"_s,
        "s102"_s + "event102"_e [([]{})] / []{} = "s103"_s,
        "s103"_s + "event103"_e [([]{})] / []{} = "s104"_s,
        "s104"_s + "event104"_e [([]{})] / []{} = "s105"_s,
        "s105"_s + "event105"_e [([]{})] / []{} = "s106"_s,
        "s106"_s + "event106"_e [([]{})] / []{} = "s107"_s,
        "s107"_s + "event107"_e [([]{})] / []{} = "s108"_s,
        "s108"_s + "event108"_e [([]{})] / []{} = "s109"_s,
        "s109"_s + "event109"_e [([]{})] / []{} = "s110"_s,
        "s110"_s + "event110"_e [([]{})] / []{} = "s111"_s,
        "s111"_s + "event111"_e [([]{})] / []{} = "s112"_s,
        "s112"_s + "event112"_e [([]{})] / []{} = "s113"_s,
        "s113"_s + "event113"_e [([]{})] / []{} = "s114"_s,
        "s114"_s + "event114"_e [([]{})] / []{} = "s115"_s,
        "s115"_s + "event115"_e [([]{})] / []{} = "s116"_s,
        "s116"_s + "event116"_e [([]{})] / []{} = "s117"_s,
        "s117"_s + "event117"_e [([]{})] / []{} = "s118"_s,
        "s118"_s + "event118"_e [([]{})] / []{} = "s119"_s,
        "s119"_s + "event119"_e [([]{})] / []{} = "s120"_s,
        "s120"_s + "event120"_e [([]{})] / []{} = "s121"_s,
        "s121"_s + "event121"_e [([]{})] / []{} = "s122"_s,
        "s122"_s + "event122"_e [([]{})] / []{} = "s123"_s,
        "s123"_s + "event123"_e [([]{})] / []{} = "s124"_s,
        "s124"_s + "event124"_e [([]{})] / []{} = "s125"_s,
        "s125"_s + "event125"_e [([]{})] / []{} = "s126"_s,
        "s126"_s + "event126"_e [([]{})] / []{} = "s127"_s,
        "s127"_s + "event127"_e [([]{})] / []{} = "s128"_s,
        "s128"_s + "event128"_e [([]{})] / []{} = "s129"_s,
        "s129"_s + "event129"_e [([]{})] / []{} = "s130"_s,
        "s130"_s + "event130"_e [([]{})] / []{} = "s131"_s,
        "s131"_s + "event131"_e [([]{})] / []{} = "s132"_s,
        "s132"_s + "event132"_e [([]{})] / []{} = "s133"_s,
        "s133"_s + "event133"_e [([]{})] / []{} = "s134"_s,
        "s134"_s + "event134"_e [([]{})] / []{} = "s135"_s,
        "s135"_s + "event135"_e [([]{})] / []{} = "s136"_s,
        "s136"_s + "event136"_e [([]{})] / []{} = "s137"_s,
        "s137"_s + "event137"_e [([]{})] / []{} = "s138"_s,
        "s138"_s + "event138"_e [([]{})] / []{} = "s139"_s,
        "s139"_s + "event139"_e [([]{})] / []{} = "s140"_s,
        "s140"_s + "event140"_e [([]{})] / []{} = "s141"_s,
        "s141"_s + "event141"_e [([]{})] / []{} = "s142"_s,
        "s142"_s + "event142"_e [([]{})] / []{} = "s143"_s,
        "s143"_s + "event143"_e [([]{})] / []{} = "s144"_s,
        "s144"_s + "event144"_e [([]{})] / []{} = "s145"_s,
        "s145"_s + "event145"_e [([]{})] / []{} = "s146"_s,
        "s146"_s + "event146"_e [([]{})] / []{} = "s147"_s,
        "s147"_s + "event147"_e [([]{})] / []{} = "s148"_s,
        "s148"_s + "event148"_e [([]{})] / []{} = "s149"_s,
        "s149"_s + "event149"_e [([]{})] / []{} = "s150"_s,
        "s150"_s + "event150"_e [([]{})] / []{} = "s151"_s,
        "s151"_s + "event151"_e [([]{})] / []{} = "s152"_s,
        "s152"_s + "event152"_e [([]{})] / []{} = "s153"_s,
        "s153"_s + "event153"_e [([]{})] / []{} = "s154"_s,
        "s154"_s + "event154"_e [([]{})] / []{} = "s155"_s,
        "s155"_s + "event155"_e [([]{})] / []{} = "s156"_s,
        "s156"_s + "event156"_e [([]{})] / []{} = "s157"_s,
        "s157"_s + "event157"_e [([]{})] / []{} = "s158"_s,
        "s158"_s + "event158"_e [([]{})] / []{} = "s159"_s,
        "s159"_s + "event159"_e [([]{})] / []{} = "s160"_s,
        "s160"_s + "event160"_e [([]{})] / []{} = "s161"_s,
        "s161"_s + "event161"_e [([]{})] / []{} = "s162"_s,
        "s162"_s + "event162"_e [([]{})] / []{} = "s163"_s,
        "s163"_s + "event163"_e [([]{})] / []{} = "s164"_s,
        "s164"_s + "event164"_e [([]{})] / []{} = "s165"_s,
        "s165"_s + "event165"_e [([]{})] / []{} = "s166"_s,
        "s166"_s + "event166"_e [([]{})] / []{} = "s167"_s,
        "s167"_s + "event167"_e [([]{})] / []{} = "s168"_s,
        "s168"_s + "event168"_e [([]{})] / []{} = "s169"_s,
        "s169"_s + "event169"_e [([]{})] / []{} = "s170"_s,
        "s170"_s + "event170"_e [([]{})] / []{} = "s171"_s,
        "s171"_s + "event171"_e [([]{})] / []{} = "s172"_s,
        "s172"_s + "event172"_e [([]{})] / []{} = "s173"_s,
        "s173"_s + "event173"_e [([]{})] / []{} = "s174"_s,
        "s174"_s + "event174"_e [([]{})] / []{} = "s175"_s,
        "s175"_s + "event175"_e [([]{})] / []{} = "s176"_s,
        "s176"_s + "event176"_e [([]{})] / []{} = "s177"_s,
        "s177"_s + "event177"_e [([]{})] / []{} = "s178"_s,
        "s178"_s + "event178"_e [([]{})] / []{} = "s179"_s,
        "s179"_s + "event179"_e [([]{})] / []{} = "s180"_s,
        "s180"_s + "event180"_e [([]{})] / []{} = "s181"_s,
        "s181"_s + "event181"_e [([]{})] / []{} = "s182"_s,
        "s182"_s + "event182"_e [([]{})] / []{} = "s183"_s,
        "s183"_s + "event183"_e [([]{})] / []{} = "s184"_s,
        "s184"_s + "event184"_e [([]{})] / []{} = "s185"_s,
        "s185"_s + "event185"_e [([]{})] / []{} = "s186"_s,
        "s186"_s + "event186"_e [([]{})] / []{} = "s187"_s,
        "s187"_s + "event187"_e [([]{})] / []{} = "s188"_s,
        "s188"_s + "event188"_e [([]{})] / []{} = "s189"_s,
        "s189"_s + "event189"_e [([]{})] / []{} = "s190"_s,
        "s190"_s + "event190"_e [([]{})] / []{} = "s191"_s,
        "s191"_s + "event191"_e [([]{})] / []{} = "s192"_s,
        "s192"_s + "event192"_e [([]{})] / []{} = "s193"_s,
        "s193"_s + "event193"_e [([]{})] / []{} = "s194"_s,
        "s194"_s + "event194"_e [([]{})] / []{} = "s195"_s,
        "s195"_s + "event195"_e [([]{})] / []{} = "s196"_s,
        "s196"_s + "event196"_e [([]{})] / []{} = "s197"_s,
        "s197"_s + "event197"_e [([]{})] / []{} = "s198"_s,
        "s198"_s + "event198"_e [([]{})] / []{} = "s199"_s,
        "s199"_s + "event199"_e [([]{})] / []{} = "s200"_s,
        "s200"_s + "event200"_e [([]{})] / []{} = "s201"_s,
        "s201"_s + "event201"_e [([]{})] / []{} = "s202"_s,
        "s202"_s + "event202"_e [([]{})] / []{} = "s203"_s,
        "s203"_s + "event203"_e [([]{})] / []{} = "s204"_s,
        "s204"_s + "event204"_e [([]{})] / []{} = "s205"_s,
        "s205"_s + "event205"_e [([]{})] / []{} = "s206"_s,
        "s206"_s + "event206"_e [([]{})] / []{} = "s207"_s,
        "s207"_s + "event207"_e [([]{})] / []{} = "s208"_s,
        "s208"_s + "event208"_e [([]{})] / []{} = "s209"_s,
        "s209"_s + "event209"_e [([]{})] / []{} = "s210"_s,
        "s210"_s + "event210"_e [([]{})] / []{} = "s211"_s,
        "s211"_s + "event211"_e [([]{})] / []{} = "s212"_s,
        "s212"_s + "event212"_e [([]{})] / []{} = "s213"_s,
        "s213"_s + "event213"_e [([]{})] / []{} = "s214"_s,
        "s214"_s + "event214"_e [([]{})] / []{} = "s215"_s,
        "s215"_s + "event215"_e [([]{})] / []{} = "s216"_s,
        "s216"_s + "event216"_e [([]{})] / []{} = "s217"_s,
        "s217"_s + "event217"_e [([]{})] / []{} = "s218"_s,
        "s218"_s + "event218"_e [([]{})] / []{} = "s219"_s,
        "s219"_s + "event219"_e [([]{})] / []{} = "s220"_s,
        "s220"_s + "event220"_e [([]{})] / []{} = "s221"_s,
        "s221"_s + "event221"_e [([]{})] / []{} = "s222"_s,
        "s222"_s + "event222"_e [([]{})] / []{} = "s223"_s,
        "s223"_s + "event223"_e [([]{})] / []{} = "s224"_s,
        "s224"_s + "event224"_e [([]{})] / []{} = "s225"_s,
        "s225"_s + "event225"_e [([]{})] / []{} = "s226"_s,
        "s226"_s + "event226"_e [([]{})] / []{} = "s227"_s,
        "s227"_s + "event227"_e [([]{})] / []{} = "s228"_s,
        "s228"_s + "event228"_e [([]{})] / []{} = "s229"_s,
        "s229"_s + "event229"_e [([]{})] / []{} = "s230"_s,
        "s230"_s + "event230"_e [([]{})] / []{} = "s231"_s,
        "s231"_s + "event231"_e [([]{})] / []{} = "s232"_s,
        "s232"_s + "event232"_e [([]{})] / []{} = "s233"_s,
        "s233"_s + "event233"_e [([]{})] / []{} = "s234"_s,
        "s234"_s + "event234"_e [([]{})] / []{} = "s235"_s,
        "s235"_s + "event235"_e [([]{})] / []{} = "s236"_s,
        "s236"_s + "event236"_e [([]{})] / []{} = "s237"_s,
        "s237"_s + "event237"_e [([]{})] / []{} = "s238"_s,
        "s238"_s + "event238"_e [([]{})] / []{} = "s239"_s,
        "s239"_s + "event239"_e [([]{})] / []{} = "s240"_s,
        "s240"_s + "event240"_e [([]{})] / []{} = "s241"_s,
        "s241"_s + "event241"_e [([]{})] / []{} = "s242"_s,
        "s242"_s + "event242"_e [([]{})] / []{} = "s243"_s,
        "s243"_s + "event243"_e [([]{})] / []{} = "s244"_s,
        "s244"_s + "event244"_e [([]{})] / []{} = "s245"_s,
        "s245"_s + "event245"_e [([]{})] / []{} = "s246"_s,
        "s246"_s + "event246"_e [([]{})] / []{} = "s247"_s,
        "s247"_s + "event247"_e [([]{})] / []{} = "s248"_s,
        "s248"_s + "event248"_e [([]{})] / []{} = "s249"_s,
        "s249"_s + "event249"_e [([]{})] / []{} = "s250"_s,
        "s250"_s + "event250"_e [([]{})] / []{} = "s251"_s,
        "s251"_s + "event251"_e [([]{})] / []{} = "s252"_s,
        "s252"_s + "event252"_e [([]{})] / []{} = "s253"_s,
        "s253"_s + "event253"_e [([]{})] / []{} = "s254"_s,
        "s254"_s + "event254"_e [([]{})] / []{} = "s255"_s,
        "s255"_s + "event255"_e [([]{})] / []{} = "s256"_s,
        "s256"_s + "event256"_e [([]{})] / []{} = "s257"_s,
        "s257"_s + "event257"_e [([]{})] / []{} = "s258"_s
      );
      // clang-format on
    }
  };
  static_expect(2 /*current_state=2*/ == sizeof(msm::sm<c>));
};
