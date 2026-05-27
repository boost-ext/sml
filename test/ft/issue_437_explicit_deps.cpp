// Issue #437: Dependency silently dropped from pool when only used via generic lambda
// (i.e. no explicitly-typed action/guard parameter names the dep type).
//
// Repro: a dep that is only accessed inside a generic 4-arg lambda via
//   sml::aux::get<Dep&>(deps)
// never appears in the auto-deduced dep_list, so the pool<> never contains Dep,
// and the static_cast from pool<> → pool_type<Dep&> fails at compile time.
//
// Fix: allow the user to declare explicit deps via sml::deps<Dep> SM policy so
// the pool is widened to include those types even when no action/guard signature
// names them explicitly.
// cppcheck-suppress missingIncludeSystem
#include <boost/sml.hpp>

namespace sml = boost::sml;

struct event1 {};
struct event2 {};

struct MyDep {
    int value = 42;
};

// Only generic 4-arg lambdas — MyDep never appears in any signature.
// Without the fix this fails to compile because MyDep is not in dep_list.
test generic_lambda_with_explicit_deps = [] {
    struct table {
        auto operator()() const {
            using namespace sml;
            auto action = [](auto /*e*/, auto& /*sm*/, auto& deps, auto& /*states*/) {
                aux::get<MyDep&>(deps).value = 99;
            };
            auto guard = [](auto /*e*/, auto& /*sm*/, auto& deps, auto& /*states*/) {
                return aux::get<MyDep&>(deps).value == 99;
            };
            return make_transition_table(
                *"idle"_s + event<event1> / action = "middle"_s,
                 "middle"_s + event<event2> [guard] = X
            );
        }
    };

    MyDep dep;
    // sml::deps<MyDep> policy tells the pool to include MyDep even though no
    // action/guard parameter list names it explicitly.
    sml::sm<table, sml::deps<MyDep>> sm{dep};

    sm.process_event(event1{});
    expect(dep.value == 99);  // action ran

    sm.process_event(event2{});
    expect(sm.is(sml::X));   // guard passed
};

test generic_lambda_multi_explicit_deps = [] {
    struct Dep1 { int x = 1; };
    struct Dep2 { int y = 2; };

    struct table {
        auto operator()() const {
            using namespace sml;
            auto action = [](auto /*e*/, auto& /*sm*/, auto& deps, auto& /*states*/) {
                aux::get<Dep1&>(deps).x = 10;
                aux::get<Dep2&>(deps).y = 20;
            };
            return make_transition_table(
                *"s"_s + event<event1> / action = X
            );
        }
    };

    Dep1 d1;
    Dep2 d2;
    sml::sm<table, sml::deps<Dep1, Dep2>> sm{d1, d2};

    sm.process_event(event1{});
    expect(d1.x == 10);
    expect(d2.y == 20);
    expect(sm.is(sml::X));
};
