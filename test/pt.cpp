//
// Copyright (c) 2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include "msm/msm.hpp"

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};
struct e6 {};
struct e7 {};
struct e8 {};
struct e9 {};
struct e10 {};
struct e11 {};
struct e12 {};
struct e13 {};
struct e14 {};
struct e15 {};
struct e16 {};
struct e17 {};
struct e18 {};
struct e19 {};
struct e20 {};
struct e21 {};
struct e22 {};
struct e23 {};
struct e24 {};
struct e25 {};
struct e26 {};
struct e27 {};
struct e28 {};
struct e29 {};
struct e30 {};
struct e31 {};
struct e32 {};
struct e33 {};
struct e34 {};
struct e35 {};
struct e36 {};
struct e37 {};
struct e38 {};
struct e39 {};
struct e40 {};
struct e41 {};
struct e42 {};
struct e43 {};
struct e44 {};
struct e45 {};
struct e46 {};
struct e47 {};
struct e48 {};
struct e49 {};
struct e50 {};
struct e51 {};
struct e52 {};
struct e53 {};
struct e54 {};
struct e55 {};
struct e56 {};
struct e57 {};
struct e58 {};
struct e59 {};
struct e60 {};
struct e61 {};
struct e62 {};
struct e63 {};
struct e64 {};
struct e65 {};
struct e66 {};
struct e67 {};
struct e68 {};
struct e69 {};
struct e70 {};
struct e71 {};
struct e72 {};
struct e73 {};
struct e74 {};
struct e75 {};
struct e76 {};
struct e77 {};
struct e78 {};
struct e79 {};
struct e80 {};
struct e81 {};
struct e82 {};
struct e83 {};
struct e84 {};
struct e85 {};
struct e86 {};
struct e87 {};
struct e88 {};
struct e89 {};
struct e90 {};
struct e91 {};
struct e92 {};
struct e93 {};
struct e94 {};
struct e95 {};
struct e96 {};
struct e97 {};
struct e98 {};
struct e99 {};
struct e100 {};

auto guard = [] { return true; };
auto action = [] {};

test transitions_table = [] {
  struct c {
    auto configure() const noexcept {
      using namespace msm;
      init_state<> idle;
      state<> s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s16, s17, s18, s19, s20, s21, s22, s23,
          s24, s25, s26, s27, s28, s29, s30, s31, s32, s33, s34, s35, s36, s37, s38, s39, s40, s41, s42, s43, s44, s45,
          s46, s47, s48, s49, s50, s51, s52, s53, s54, s55, s56, s57, s58, s59, s60, s61, s62, s63, s64, s65, s66, s67,
          s68, s69, s70, s71, s72, s73, s74, s75, s76, s77, s78, s79, s80, s81, s82, s83, s84, s85, s86, s87, s88, s89,
          s90, s91, s92, s93, s94, s95, s96, s97, s98, s99, s100;

      // clang-format off
    return make_transition_table(
	   idle == s1 + event<e1> [guard] / action
	 , s1 == s2 + event<e2> [guard] / action
	 , s2 == s3 + event<e3> [guard] / action
	 , s3 == s4 + event<e4> [guard] / action
	 , s4 == s5 + event<e5> [guard] / action
	 , s5 == s6 + event<e6> [guard] / action
	 , s6 == s7 + event<e7> [guard] / action
	 , s7 == s8 + event<e8> [guard] / action
	 , s8 == s9 + event<e9> [guard] / action
	 , s9 == s10 + event<e10> [guard] / action
	 , s10 == s11 + event<e11> [guard] / action
	 , s11 == s12 + event<e12> [guard] / action
	 , s12 == s13 + event<e13> [guard] / action
	 , s13 == s14 + event<e14> [guard] / action
	 , s14 == s15 + event<e15> [guard] / action
	 , s15 == s16 + event<e16> [guard] / action
	 , s16 == s17 + event<e17> [guard] / action
	 , s17 == s18 + event<e18> [guard] / action
	 , s18 == s19 + event<e19> [guard] / action
	 , s19 == s20 + event<e20> [guard] / action
	 , s20 == s21 + event<e21> [guard] / action
	 , s21 == s22 + event<e22> [guard] / action
	 , s22 == s23 + event<e23> [guard] / action
	 , s23 == s24 + event<e24> [guard] / action
	 , s24 == s25 + event<e25> [guard] / action
	 , s25 == s26 + event<e26> [guard] / action
	 , s26 == s27 + event<e27> [guard] / action
	 , s27 == s28 + event<e28> [guard] / action
	 , s28 == s29 + event<e29> [guard] / action
	 , s29 == s30 + event<e30> [guard] / action
	 , s30 == s31 + event<e31> [guard] / action
	 , s31 == s32 + event<e32> [guard] / action
	 , s32 == s33 + event<e33> [guard] / action
	 , s33 == s34 + event<e34> [guard] / action
	 , s34 == s35 + event<e35> [guard] / action
	 , s35 == s36 + event<e36> [guard] / action
	 , s36 == s37 + event<e37> [guard] / action
	 , s37 == s38 + event<e38> [guard] / action
	 , s38 == s39 + event<e39> [guard] / action
	 , s39 == s40 + event<e40> [guard] / action
	 , s40 == s41 + event<e41> [guard] / action
	 , s41 == s42 + event<e42> [guard] / action
	 , s42 == s43 + event<e43> [guard] / action
	 , s43 == s44 + event<e44> [guard] / action
	 , s44 == s45 + event<e45> [guard] / action
	 , s45 == s46 + event<e46> [guard] / action
	 , s46 == s47 + event<e47> [guard] / action
	 , s47 == s48 + event<e48> [guard] / action
	 , s48 == s49 + event<e49> [guard] / action
	 , s49 == s50 + event<e50> [guard] / action
	 , s50 == s51 + event<e51> [guard] / action
	 , s51 == s52 + event<e52> [guard] / action
	 , s52 == s53 + event<e53> [guard] / action
	 , s53 == s54 + event<e54> [guard] / action
	 , s54 == s55 + event<e55> [guard] / action
	 , s55 == s56 + event<e56> [guard] / action
	 , s56 == s57 + event<e57> [guard] / action
	 , s57 == s58 + event<e58> [guard] / action
	 , s58 == s59 + event<e59> [guard] / action
	 , s59 == s60 + event<e60> [guard] / action
	 , s60 == s61 + event<e61> [guard] / action
	 , s61 == s62 + event<e62> [guard] / action
	 , s62 == s63 + event<e63> [guard] / action
	 , s63 == s64 + event<e64> [guard] / action
	 , s64 == s65 + event<e65> [guard] / action
	 , s65 == s66 + event<e66> [guard] / action
	 , s66 == s67 + event<e67> [guard] / action
	 , s67 == s68 + event<e68> [guard] / action
	 , s68 == s69 + event<e69> [guard] / action
	 , s69 == s70 + event<e70> [guard] / action
	 , s70 == s71 + event<e71> [guard] / action
	 , s71 == s72 + event<e72> [guard] / action
	 , s72 == s73 + event<e73> [guard] / action
	 , s73 == s74 + event<e74> [guard] / action
	 , s74 == s75 + event<e75> [guard] / action
	 , s75 == s76 + event<e76> [guard] / action
	 , s76 == s77 + event<e77> [guard] / action
	 , s77 == s78 + event<e78> [guard] / action
	 , s78 == s79 + event<e79> [guard] / action
	 , s79 == s80 + event<e80> [guard] / action
	 , s80 == s81 + event<e81> [guard] / action
	 , s81 == s82 + event<e82> [guard] / action
	 , s82 == s83 + event<e83> [guard] / action
	 , s83 == s84 + event<e84> [guard] / action
	 , s84 == s85 + event<e85> [guard] / action
	 , s85 == s86 + event<e86> [guard] / action
	 , s86 == s87 + event<e87> [guard] / action
	 , s87 == s88 + event<e88> [guard] / action
	 , s88 == s89 + event<e89> [guard] / action
	 , s89 == s90 + event<e90> [guard] / action
	 , s90 == s91 + event<e91> [guard] / action
	 , s91 == s92 + event<e92> [guard] / action
	 , s92 == s93 + event<e93> [guard] / action
	 , s93 == s94 + event<e94> [guard] / action
	 , s94 == s95 + event<e95> [guard] / action
	 , s95 == s96 + event<e96> [guard] / action
	 , s96 == s97 + event<e97> [guard] / action
	 , s97 == s98 + event<e98> [guard] / action
	 , s98 == s99 + event<e99> [guard] / action
	 , s99 == s100 + event<e100> [guard] / action
    );
      // clang-format on
    }
  };

  msm::sm<c> sm{c{}};
  sm.process_event(e1());
  sm.process_event(e2());
  sm.process_event(e3());
  sm.process_event(e4());
  sm.process_event(e5());
  sm.process_event(e6());
  sm.process_event(e7());
  sm.process_event(e8());
  sm.process_event(e9());
  sm.process_event(e10());
  sm.process_event(e11());
  sm.process_event(e12());
  sm.process_event(e13());
  sm.process_event(e14());
  sm.process_event(e15());
  sm.process_event(e16());
  sm.process_event(e17());
  sm.process_event(e18());
  sm.process_event(e19());
  sm.process_event(e20());
  sm.process_event(e21());
  sm.process_event(e22());
  sm.process_event(e23());
  sm.process_event(e24());
  sm.process_event(e25());
  sm.process_event(e26());
  sm.process_event(e27());
  sm.process_event(e28());
  sm.process_event(e29());
  sm.process_event(e30());
  sm.process_event(e31());
  sm.process_event(e32());
  sm.process_event(e33());
  sm.process_event(e34());
  sm.process_event(e35());
  sm.process_event(e36());
  sm.process_event(e37());
  sm.process_event(e38());
  sm.process_event(e39());
  sm.process_event(e40());
  sm.process_event(e41());
  sm.process_event(e42());
  sm.process_event(e43());
  sm.process_event(e44());
  sm.process_event(e45());
  sm.process_event(e46());
  sm.process_event(e47());
  sm.process_event(e48());
  sm.process_event(e49());
  sm.process_event(e50());
  sm.process_event(e51());
  sm.process_event(e52());
  sm.process_event(e53());
  sm.process_event(e54());
  sm.process_event(e55());
  sm.process_event(e56());
  sm.process_event(e57());
  sm.process_event(e58());
  sm.process_event(e59());
  sm.process_event(e60());
  sm.process_event(e61());
  sm.process_event(e62());
  sm.process_event(e63());
  sm.process_event(e64());
  sm.process_event(e65());
  sm.process_event(e66());
  sm.process_event(e67());
  sm.process_event(e68());
  sm.process_event(e69());
  sm.process_event(e70());
  sm.process_event(e71());
  sm.process_event(e72());
  sm.process_event(e73());
  sm.process_event(e74());
  sm.process_event(e75());
  sm.process_event(e76());
  sm.process_event(e77());
  sm.process_event(e78());
  sm.process_event(e79());
  sm.process_event(e80());
  sm.process_event(e81());
  sm.process_event(e82());
  sm.process_event(e83());
  sm.process_event(e84());
  sm.process_event(e85());
  sm.process_event(e86());
  sm.process_event(e87());
  sm.process_event(e88());
  sm.process_event(e89());
  sm.process_event(e90());
  sm.process_event(e91());
  sm.process_event(e92());
  sm.process_event(e93());
  sm.process_event(e94());
  sm.process_event(e95());
  sm.process_event(e96());
  sm.process_event(e97());
  sm.process_event(e98());
  sm.process_event(e99());
  sm.process_event(e100());
};
