// This test is automatically generated, do not edit.

#include "{{it.properties.pathForIncludes}}/{{it.generator.outputHeaderShortname}}"

#include <gtest/gtest.h>

namespace {{it.properties.namespace }} {
namespace {

TEST(StaticSMTests, TransitionsInfo) {
{{@foreach(it.machine.states) => key, val}}
    {
        auto transitions = {{it.generator.class()}}ValidTransitionsFrom{{it.generator.capitalize(key)}}State();
        for (const auto& transition : transitions) {
            EXPECT_TRUE(isValid{{it.generator.class()}}Event(transition.first));
        }
    }
{{/foreach}}
}

/**
 * This generated unit test demostrates the simplest usage of State Machine without
 * subclassing.
 */
TEST(StaticSMTests, States) {
    {{it.generator.class()}}<> machine;
    int count = 0;
    for (; count < 10; ++count) {
        auto currentState = machine.currentState();
        ASSERT_EQ(currentState.totalTransitions, count);
        auto validTransitions = machine.validTransitionsFromCurrentState();
        if (validTransitions.empty()) {
            break;
        }
        // Make a random transition.
        const {{it.generator.class()}}TransitionToStatesPair& transition = validTransitions[std::rand() % validTransitions.size()];
        const {{it.generator.class()}}Event event = transition.first;
        switch (event) {
{{@each(it.generator.events()) => val, index}}
        case {{it.generator.class()}}Event::{{val}}: {
            {{it.generator.class()}}<>::{{it.generator.capitalize(val)}}Payload payload;
            machine.postEvent{{it.generator.capitalize(val)}} (std::move(payload));
        } break;
{{/each}}
        default:
            ASSERT_TRUE(false) << "This should never happen";
        }

        currentState = machine.currentState();
        ASSERT_EQ(currentState.lastEvent, event);
    }
    std::cout << "Made " << count << " transitions" << std::endl;
}

// User context is some arbitrary payload attached to the State Machine. If none is supplied,
// some dummy empty context still exists.
struct UserContext {
    std::string hello = "This is my context";
    int data = 1;
    // We will count how many times the payload ID of 1 was observed.
    int countOfIdOneSeen = 0;
    std::optional<std::string> dataToKeepWhileInState;
};

// Every Event can have some arbitrary user defined payload. It can be
// any type, as class or some STL type like std::unique_ptr or std::vector.

{{@each(it.generator.events()) => val, index}}
// Sample payload for the {{it.generator.capitalize(val)}} event.
// The only restriction - it cannot be named Event{{it.generator.capitalize(val)}}Payload
// because this name is reserved for the Spec structure.
struct My{{it.generator.capitalize(val)}}Payload {
    int data = 42;
    std::string str = "Hi";
    int someID = {{index}};
    static constexpr char staticText[] = "it's {{it.generator.capitalize(val)}} payload";
};
{{/each}}

// Spec struct contains just a bunch of 'using' declarations to stich all types together
// and avoid variable template argument for the SM class declaration.
struct MySpec {
    // Spec should always contain some 'using' for the StateMachineContext.
    using StateMachineContext = UserContext;

    // Then it should have a list of 'using' declarations for every event payload.
{{@each(it.generator.events()) => val, index}}
    // The name Event{{it.generator.capitalize(val)}}Payload is reserved by convention for every event.
    using Event{{it.generator.capitalize(val)}}Payload = My{{it.generator.capitalize(val)}}Payload;
{{/each}}
};

// And finally the more feature rich State Machine can be subclassed from the generated class
// {{it.generator.class()}}, which gives the possibility to overload the virtual methods.
class MyTestStateMachine : public {{it.generator.class()}}<MySpec> {
  public:
    ~MyTestStateMachine() final;

    // Overload the logging method to use the log system of your project.
    void logTransition(TransitionPhase phase, State currentState, State nextState) const final {
        std::cout << "MyTestStateMachine the phase " << phase;
        switch (phase) {
        case TransitionPhase::LEAVING_STATE:
            std::cout << currentState << ", transitioning to " << nextState;
            break;
        case TransitionPhase::ENTERING_STATE:
            std::cout << nextState << " from " << currentState;
            break;
        case TransitionPhase::ENTERED_STATE:
            std::cout << currentState;
            break;
        default:
            assert(false && "This is impossible");
            break;
        }
        std::cout << std::endl;
    }

    // Overload 'onLeaving' method to cleanup some state or do some other action.
{{@foreach(it.machine.states) => key, val}}
    void onLeaving{{it.generator.capitalize(key)}}State(State nextState) final {
        logTransition({{it.generator.class()}}TransitionPhase::LEAVING_STATE, State::{{key}}, nextState);
        accessContextLocked([this] (StateMachineContext& userContext) {
            userContext.dataToKeepWhileInState.reset();  // As example we erase some data in the context.
        });
    }
{{/foreach}}

};

class SMTestFixture : public ::testing::Test {
  public:
    void SetUp() override {
        _sm.reset(new MyTestStateMachine);
    }

{{@each(it.generator.events()) => val, index}}
    void postEvent{{val}}() {
    case {{it.generator.class()}}Event::{{val}}: {
        {{it.generator.class()}}<>::{{it.generator.capitalize(val)}}Payload payload;
        _sm->postEvent{{it.generator.capitalize(val)}} (std::move(payload));
    } break;
    }
{{/each}}

    std::unique_ptr<MyTestStateMachine> _sm;
};

TEST_F(SMTestFixture, States) {
    int count = 0;
    for (; count < 10; ++count) {
        auto currentState = _sm->currentState();
        ASSERT_EQ(currentState.totalTransitions, count);
        auto validTransitions = _sm->validTransitionsFromCurrentState();
        if (validTransitions.empty()) {
            break;
        }
        // Make a random transition.
        const {{it.generator.class()}}TransitionToStatesPair& transition = validTransitions[std::rand() % validTransitions.size()];
        const {{it.generator.class()}}Event event = transition.first;
        switch (event) {
{{@each(it.generator.events()) => val, index}}
        case {{it.generator.class()}}Event::{{val}}: {
            {{it.generator.class()}}<>::{{it.generator.capitalize(val)}}Payload payload;
            _sm->postEvent{{it.generator.capitalize(val)}} (std::move(payload));
        } break;
{{/each}}
        default:
            ASSERT_TRUE(false) << "This should never happen";
        }

        currentState = _sm->currentState();
        ASSERT_EQ(currentState.lastEvent, event);
    }
    std::cout << "Made " << count << " transitions" << std::endl;
}

}  // namespace
}  // namespace {{it.properties.namespace }}
