// Minimal repro for issue #639: template depth explosion with large FSMs using shared dep types
#include <boost/sml.hpp>

namespace sml = boost::sml;

struct e {};
struct dep_a { int v = 0; };
struct dep_b { int v = 0; };

// 50 transitions all sharing the same two dep types: dep_a&, dep_b&
// This creates 100 elements in merge_deps before unique_t deduplicates to 2.
// The question is: does this cause template instantiation depth issues?
struct large_fsm {
    auto operator()() noexcept {
        using namespace sml;
        auto action = [](dep_a&, dep_b&) {};
        auto s00 = state<struct s00_>; auto s01 = state<struct s01_>;
        auto s02 = state<struct s02_>; auto s03 = state<struct s03_>;
        auto s04 = state<struct s04_>; auto s05 = state<struct s05_>;
        auto s06 = state<struct s06_>; auto s07 = state<struct s07_>;
        auto s08 = state<struct s08_>; auto s09 = state<struct s09_>;
        auto s10 = state<struct s10_>; auto s11 = state<struct s11_>;
        auto s12 = state<struct s12_>; auto s13 = state<struct s13_>;
        auto s14 = state<struct s14_>; auto s15 = state<struct s15_>;
        auto s16 = state<struct s16_>; auto s17 = state<struct s17_>;
        auto s18 = state<struct s18_>; auto s19 = state<struct s19_>;
        auto s20 = state<struct s20_>; auto s21 = state<struct s21_>;
        auto s22 = state<struct s22_>; auto s23 = state<struct s23_>;
        auto s24 = state<struct s24_>; auto s25 = state<struct s25_>;
        auto s26 = state<struct s26_>; auto s27 = state<struct s27_>;
        auto s28 = state<struct s28_>; auto s29 = state<struct s29_>;
        auto s30 = state<struct s30_>; auto s31 = state<struct s31_>;
        auto s32 = state<struct s32_>; auto s33 = state<struct s33_>;
        auto s34 = state<struct s34_>; auto s35 = state<struct s35_>;
        auto s36 = state<struct s36_>; auto s37 = state<struct s37_>;
        auto s38 = state<struct s38_>; auto s39 = state<struct s39_>;
        auto s40 = state<struct s40_>; auto s41 = state<struct s41_>;
        auto s42 = state<struct s42_>; auto s43 = state<struct s43_>;
        auto s44 = state<struct s44_>; auto s45 = state<struct s45_>;
        auto s46 = state<struct s46_>; auto s47 = state<struct s47_>;
        auto s48 = state<struct s48_>; auto s49 = state<struct s49_>;

        return make_transition_table(
            *s00 + event<e> / action = s01,
            s01 + event<e> / action = s02,
            s02 + event<e> / action = s03,
            s03 + event<e> / action = s04,
            s04 + event<e> / action = s05,
            s05 + event<e> / action = s06,
            s06 + event<e> / action = s07,
            s07 + event<e> / action = s08,
            s08 + event<e> / action = s09,
            s09 + event<e> / action = s10,
            s10 + event<e> / action = s11,
            s11 + event<e> / action = s12,
            s12 + event<e> / action = s13,
            s13 + event<e> / action = s14,
            s14 + event<e> / action = s15,
            s15 + event<e> / action = s16,
            s16 + event<e> / action = s17,
            s17 + event<e> / action = s18,
            s18 + event<e> / action = s19,
            s19 + event<e> / action = s20,
            s20 + event<e> / action = s21,
            s21 + event<e> / action = s22,
            s22 + event<e> / action = s23,
            s23 + event<e> / action = s24,
            s24 + event<e> / action = s25,
            s25 + event<e> / action = s26,
            s26 + event<e> / action = s27,
            s27 + event<e> / action = s28,
            s28 + event<e> / action = s29,
            s29 + event<e> / action = s30,
            s30 + event<e> / action = s31,
            s31 + event<e> / action = s32,
            s32 + event<e> / action = s33,
            s33 + event<e> / action = s34,
            s34 + event<e> / action = s35,
            s35 + event<e> / action = s36,
            s36 + event<e> / action = s37,
            s37 + event<e> / action = s38,
            s38 + event<e> / action = s39,
            s39 + event<e> / action = s40,
            s40 + event<e> / action = s41,
            s41 + event<e> / action = s42,
            s42 + event<e> / action = s43,
            s43 + event<e> / action = s44,
            s44 + event<e> / action = s45,
            s45 + event<e> / action = s46,
            s46 + event<e> / action = s47,
            s47 + event<e> / action = s48,
            s48 + event<e> / action = s49,
            s49 + event<e> / action = s00
        );
    }
};

test large_fsm_shared_deps = [] {
    dep_a a;
    dep_b b;
    sml::sm<large_fsm> sm{a, b};
    sm.process_event(e{});
};
