// cppcheck-suppress missingIncludeSystem
#include <boost/sml.hpp>

namespace sml = boost::sml;

struct trigger {};
struct MyDep {
    int called = 0;
};

// Generic lambda: without sml::make_action<> the args_ probe instantiates op() with
// the event type and concludes the callable wants the event, not the dep.
// sml::make_action<MyDep&> provides an explicit concrete signature so function_traits
// can extract the right dep type.
test action_wrap_generic_action = [] {
    struct SM {
        auto operator()() const {
            using namespace sml;
            return make_transition_table(
                *"s1"_s + event<trigger> / make_action<MyDep&>([](auto& dep) { dep.called++; }) = X);
        }
    };
    MyDep dep;
    sml::sm<SM> sm{dep};
    sm.process_event(trigger{});
    expect(dep.called == 1);
    expect(sm.is(sml::X));
};

test action_wrap_generic_guard = [] {
    struct SM {
        auto operator()() const {
            using namespace sml;
            return make_transition_table(
                *"s1"_s + event<trigger>[make_action<MyDep&>([](auto& dep) { return dep.called == 0; })] = X);
        }
    };
    MyDep dep;
    sml::sm<SM> sm{dep};
    sm.process_event(trigger{});
    expect(sm.is(sml::X));
};

test action_wrap_event_and_dep = [] {
    struct SM {
        auto operator()() const {
            using namespace sml;
            return make_transition_table(
                *"s1"_s + event<trigger> /
                    make_action<const trigger&, MyDep&>([](const auto& /*e*/, auto& dep) { dep.called++; }) = X);
        }
    };
    MyDep dep;
    sml::sm<SM> sm{dep};
    sm.process_event(trigger{});
    expect(dep.called == 1);
    expect(sm.is(sml::X));
};

#if __cplusplus >= 202002L
template <typename T>
concept HasCalled = requires(T t) { t.called; };

test action_wrap_constrained_auto = [] {
    struct SM {
        auto operator()() const {
            using namespace sml;
            return make_transition_table(
                *"s1"_s + event<trigger> / make_action<MyDep&>([](HasCalled auto& dep) { dep.called++; }) = X);
        }
    };
    MyDep dep;
    sml::sm<SM> sm{dep};
    sm.process_event(trigger{});
    expect(dep.called == 1);
    expect(sm.is(sml::X));
};
#endif
