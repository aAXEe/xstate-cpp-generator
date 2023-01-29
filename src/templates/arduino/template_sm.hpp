/**
 * This header is automatically generated using the Xstate to C++ code generator:
 *    https://github.com/shuvalov-mdb/xstate-cpp-generator
 *    Copyright (c) 2020 Andrew Shuvalov
 *    License: MIT https://opensource.org/licenses/MIT
 *
 * Please do not edit. If changes are needed, regenerate using the TypeScript template '{{it.properties.tsScriptName}}'.
 * Generated at {{it.generator.annotation()}} from Xstate definition '{{it.properties.tsScriptName}}'.
 * The simplest command line to run the generation:
 *     ts-node '{{it.properties.tsScriptName}}'
 */

#pragma once

#include <Arduino.h>

#include <cassert>
#include <functional>
#include <memory>
#include <queue>
#include <sstream>
#include <tuple>
#include <vector>
#include <chrono>

namespace {{it.properties.namespace }} {

// All states declared in the SM {{it.generator.class()}}.
enum class {{it.generator.class()}}State {
    UNDEFINED_OR_ERROR_STATE = 0,
{{@foreach(it.machine.states) => key, val}}
    {{key}},
{{/foreach}}
};

String {{it.generator.class()}}StateToString({{it.generator.class()}}State state);

// @returns true if 'state' is a valid State.
bool isValid{{it.generator.class()}}State({{it.generator.class()}}State state);

// All events declared in the SM {{it.generator.class()}}.
enum class {{it.generator.class()}}Event {
    UNDEFINED_OR_ERROR_EVENT = 0,
{{@each(it.generator.events()) => val, index}}
    {{val}},
{{/each}}
};

String {{it.generator.class()}}EventToString({{it.generator.class()}}Event event);

// @returns true if 'event' is a valid Event.
bool isValid{{it.generator.class()}}Event({{it.generator.class()}}Event event);

// As a transition could be conditional (https://xstate.js.org/docs/guides/guards.html#guards-condition-functions)
// one event is mapped to a vector of possible transitions.
using {{it.generator.class()}}TransitionToStatesPair = std::pair<{{it.generator.class()}}Event,
        std::vector<{{it.generator.class()}}State>>;

/**
 * All valid transitions from the specified state. The transition to state graph
 * is code genrated from the model and cannot change.
 */
{{@foreach(it.machine.states) => key, val}}
const std::vector<{{it.generator.class()}}TransitionToStatesPair>& {{it.generator.class()}}ValidTransitionsFrom{{it.generator.capitalize(key)}}State();
{{/foreach}}

/**
 * Enum to indicate the current state transition phase in callbacks. This enum is used only for logging
 * and is not part of any State Machine logic.
 */
enum class {{it.generator.class()}}TransitionPhase {
    UNDEFINED = 0,
    LEAVING_STATE,
    ENTERING_STATE,
    ENTERED_STATE,
    TRANSITION_NOT_FOUND
};

template <typename SMSpec> class {{it.generator.class()}};  // Forward declaration to use in Spec.

/**
 * Convenient default SM spec structure to parameterize the State Machine.
 * It can be replaced with a custom one if the SM events do not need any payload to be attached, and if there
 * is no guards, and no other advanced features.
 */
template <typename SMContext = std::nullptr_t>
struct Default{{it.generator.class()}}Spec {
    /**
     * Generic data structure stored in the State Machine to keep some user-defined state that can be modified
     * when transitions happen.
     */
    using StateMachineContext = SMContext;

    /**
     * Each Event has a payload attached, which is passed in to the related callbacks.
     * The type should be movable for efficiency.
     */
{{@each(it.generator.events()) => val, index}}
    using Event{{it.generator.capitalize(val)}}Payload = std::nullptr_t;
{{/each}}

    /**
     * Actions are modeled in the Xstate definition, see https://xstate.js.org/docs/guides/actions.html.
     * This block is for transition actions.
     */
{{@each(it.generator.allTransitionActions()) => pair, index}}
    static void {{pair[1]}} ({{it.generator.class()}}<Default{{it.generator.class()}}Spec>* sm, std::shared_ptr<Event{{it.generator.capitalize(pair[0])}}Payload>) {}
{{/each}}

    /**
     * This block is for entry and exit state actions.
     */
{{@each(it.generator.allEntryExitActions()) => action, index}}
    static void {{action}} ({{it.generator.class()}}<Default{{it.generator.class()}}Spec>* sm) {}
{{/each}}
};

/**
 *  State machine as declared in Xstate library for {{it.generator.class()}}.
 *  SMSpec is a convenient template struct, which allows to specify various definitions used by generated code. In a simple
 *  case it's not needed and a convenient default is provided.
 *
 *  State Machine is not an abstract class and can be used without subclassing at all,
 *  though its functionality will be limited in terms of callbacks.
 *  Even though it's a templated class, a default SMSpec is provided to make a simple
 *  State Machine without any customization. In the most simple form, a working
 *  {{it.generator.class()}} SM instance can be instantiated and used as in this example:
 *
 *    {{it.generator.class()}}<> machine;
 *    auto currentState = machine.currentState();
{{@each(it.generator.events()) => val, index}}
 *    {{it.generator.class()}}<>::{{it.generator.capitalize(val)}}Payload payload{{val}};      // ..and init payload with data
 *    machine.postEvent{{it.generator.capitalize(val)}} (std::move(payload{{val}}));
{{/each}}
 *
 *  Also see the generated unit tests in the example-* folders for more example code.
 */
template <typename SMSpec = Default{{it.generator.class()}}Spec<std::nullptr_t>>
class {{it.generator.class()}} {
  public:
    using TransitionToStatesPair = {{it.generator.class()}}TransitionToStatesPair;
    using State = {{it.generator.class()}}State;
    using Event = {{it.generator.class()}}Event;
    using TransitionPhase = {{it.generator.class()}}TransitionPhase;
    using StateMachineContext = typename SMSpec::StateMachineContext;
{{@each(it.generator.events()) => val, index}}
    using {{it.generator.capitalize(val)}}Payload = typename SMSpec::Event{{it.generator.capitalize(val)}}Payload;
{{/each}}

    /**
     * Structure represents the current in-memory state of the State Machine.
     */
    struct CurrentState {
        State currentState = {{it.generator.class()}}State::{{it.generator.initialState()}};
        /** previousState could be undefined if SM is at initial state */
        State previousState;
        /** The event that transitioned the SM from previousState to currentState */
        Event lastEvent;
        /** Timestamp of the last transition, or the class instantiation if at initial state */
        uint32_t lastTransitionTime = millis();
        /** Count of the transitions made so far */
        int totalTransitions = 0;
    };

    {{it.generator.class()}}() {
    }

    virtual ~{{it.generator.class()}}() {
        if (!isTerminated()) {
            Serial.println("State Machine {{it.generator.class()}} is terminating "
                "without reaching the final state.");
        }
    }

    /**
     * Returns a copy of the current state, skipping some fields.
     */
    CurrentState currentState() const {
        CurrentState aCopy;  // We will not copy the event queue.
        aCopy.currentState = _currentState.currentState;
        aCopy.previousState = _currentState.previousState;
        aCopy.lastEvent = _currentState.lastEvent;
        aCopy.totalTransitions = _currentState.totalTransitions;
        aCopy.lastTransitionTime = _currentState.lastTransitionTime;
        return aCopy;
    }

    /**
     * The only way to change the SM state is to post an event.
     * If the event queue is empty the transition will be processed in the current thread.
     * If the event queue is not empty, this adds the event into the queue and returns immediately. The events
     * in the queue will be processed sequentially by the same thread that is currently processing the front of the queue.
     */
{{@each(it.generator.events()) => val, index}}
    void postEvent{{it.generator.capitalize(val)}} (std::shared_ptr<{{it.generator.capitalize(val)}}Payload> payload);
{{/each}}

    /**
     * All valid transitions from the current state of the State Machine.
     */
    const std::vector<{{it.generator.class()}}TransitionToStatesPair>& validTransitionsFromCurrentState() const {
        return validTransitionsFrom(_currentState.currentState);
    }

    /**
     * Provides a mechanism to access the internal user-defined Context (see SMSpec::StateMachineContext).
     * Warning: it is not allowed to call postEvent(), or currentState(), or any other method from inside
     * this callback as it will be a deadlock.
     * @param callback is executed safely under lock for full R/W access to the Context. Thus, this method
     *   can be invoked concurrently from any thread and any of the callbacks declared below.
     */
    void accessContextLocked(std::function<void(StateMachineContext& userContext)> callback);

    /**
     * @returns true if State Machine reached the final state. Note that final state is optional.
     */
    bool isTerminated() const {
        return _smIsTerminated;
    }

    /**
     * The block of virtual callback methods the derived class can override to extend the SM functionality.
     * All callbacks are invoked without holding the internal lock, thus it is allowed to call SM methods from inside.
     */

    /**
     * Overload this method to log or mute the case when the default generated method for entering, entered
     * or leaving the state is not overloaded. By default it just prints to stdout. The default action is very
     * useful for the initial development. In production. it's better to replace it with an appropriate
     * logging or empty method to mute.
     */
    virtual void logTransition(TransitionPhase phase, State currentState, State nextState) const;

    /**
     * 'onLeavingState' callbacks are invoked right before entering a new state. The internal
     * '_currentState' data still points to the current state.
     */
{{@foreach(it.machine.states) => key, val}}
    virtual void onLeaving{{it.generator.capitalize(key)}}State(State nextState) {
        logTransition({{it.generator.class()}}TransitionPhase::LEAVING_STATE, State::{{key}}, nextState);
    }
{{/foreach}}

    /**
     * 'onEnteringState' callbacks are invoked right before entering a new state. The internal
     * '_currentState' data still points to the existing state.
     * @param payload mutable payload, ownership remains with the caller. To take ownership of the payload
     *   override another calback from the 'onEntered*State' below.
     */
{{@each(it.generator.allEventToStatePairs()) => pair, index}}
    virtual void onEnteringState{{it.generator.capitalize(pair[1])}}On{{pair[0]}}(State nextState, std::shared_ptr<{{it.generator.capitalize(pair[0])}}Payload> payload) {
        logTransition({{it.generator.class()}}TransitionPhase::ENTERING_STATE, _currentState.currentState, State::{{pair[1]}});
    }
{{/each}}

    /**
     * 'onEnteredState' callbacks are invoked after SM moved to new state. The internal
     * '_currentState' data already points to the existing state.
     * It is guaranteed that the next transition will not start until this callback returns.
     * It is safe to call postEvent*() to trigger the next transition from this method.
     * @param payload ownership is transferred to the user.
     */
{{@each(it.generator.allEventToStatePairs()) => pair, index}}
    virtual void onEnteredState{{it.generator.capitalize(pair[1])}}On{{pair[0]}}(std::shared_ptr<{{it.generator.capitalize(pair[0])}}Payload> payload) {
        logTransition({{it.generator.class()}}TransitionPhase::ENTERED_STATE, _currentState.currentState, State::{{pair[1]}});
    }
{{/each}}


    /**
     * All valid transitions from the specified state.
     */
    static inline const std::vector<TransitionToStatesPair>& validTransitionsFrom({{it.generator.class()}}State state) {
        switch (state) {
{{@foreach(it.machine.states) => key, val}}
          case {{it.generator.class()}}State::{{key}}:
            return {{it.generator.class()}}ValidTransitionsFrom{{it.generator.capitalize(key)}}State();
{{/foreach}}
          default: {
            Serial.print("invalid SM state ");
            Serial.println({{it.generator.class()}}StateToString(state));
            static std::vector<TransitionToStatesPair> empty;
            return empty;
          } break;
        }
    }
    
    void loop(){
        if (_smIsTerminated) {
            return;
        }
        if(_eventQueue.empty()){
            return;
        }

        std::function<void()> nextCallback;
        nextCallback = std::move(_eventQueue.front());
        _eventQueue.pop();
        
        if (nextCallback) {
            nextCallback();
        }
    }

  private:
    template<typename Payload>
    void _postEventHelper(State state, Event event, std::shared_ptr<Payload> payload);

    void _leavingStateHelper(State fromState, State newState);

    // The implementation will cast the void* of 'payload' back to full type to invoke the callback.
    void _enteringStateHelper(Event event, State newState, void* payload);

    void _transitionActionsHelper(State fromState, Event event, void* payload);

    // The implementation will cast the void* of 'payload' back to full type to invoke the callback.
    void _enteredStateHelper(Event event, State newState, void* payload);

    CurrentState _currentState;

    // The SM can process events only in a serialized way. This queue stores the events to be processed.
    std::queue<std::function<void()>> _eventQueue;

    bool _insideAccessContextLocked = false;
    bool _smIsTerminated = false;

    // Arbitrary user-defined data structure, see above.
    typename SMSpec::StateMachineContext _context;
};

/******   Internal implementation  ******/

{{@each(it.generator.events()) => val, index}}
template <typename SMSpec>
inline void {{it.generator.class()}}<SMSpec>::postEvent{{it.generator.capitalize(val)}} (std::shared_ptr<{{it.generator.class()}}::{{it.generator.capitalize(val)}}Payload> payload) {
    if (_insideAccessContextLocked) {
        Serial.println("It is prohibited to post an event from inside the accessContextLocked()");
    }
    State currentState = _currentState.currentState;
    std::function<void()> eventCb{[ this, currentState, payload ] () mutable {
        _postEventHelper(currentState, {{it.generator.class()}}::Event::{{val}}, payload);
    }};
    _eventQueue.emplace(eventCb);
}

{{/each}}

template<typename SMSpec>
template<typename Payload>
void {{it.generator.class()}}<SMSpec>::_postEventHelper ({{it.generator.class()}}::State state,
    {{it.generator.class()}}::Event event, std::shared_ptr<Payload> payload) {

    // Step 1: Invoke the guard callback. TODO: implement.

    // Step 2: check if the transition is valid.
    const std::vector<{{it.generator.class()}}State>* targetStates = nullptr;
    const std::vector<{{it.generator.class()}}TransitionToStatesPair>& validTransitions = validTransitionsFrom(state);
    for (const auto& transitionEvent : validTransitions) {
        if (transitionEvent.first == event) {
            targetStates = &transitionEvent.second;
        }
    }

    if (targetStates == nullptr || targetStates->empty()) {
        logTransition(TransitionPhase::TRANSITION_NOT_FOUND, state, state);
        return;
    }

    // This can be conditional if guards are implemented...
    State newState = (*targetStates)[0];

    // Step 3: Invoke the 'leaving the state' callback.
    _leavingStateHelper(state, newState);

    // Step 4: Invoke the 'entering the state' callback.
    _enteringStateHelper(event, newState, &payload);

    // ... and the transiton actions.
    _transitionActionsHelper(state, event, &payload);

    {
        // Step 5: do the transition.
        _currentState.previousState = _currentState.currentState;
        _currentState.currentState = newState;
        _currentState.lastTransitionTime = millis();
        _currentState.lastEvent = event;
        ++_currentState.totalTransitions;
        if (newState == State::{{it.generator.finalState()}}) {
            _smIsTerminated = true;
        }
    }

    // Step 6: Invoke the 'entered the state' callback.
    _enteredStateHelper(event, newState, &payload);
}

template<typename SMSpec>
void {{it.generator.class()}}<SMSpec>::_leavingStateHelper(State fromState, State newState) {
    switch (fromState) {
{{@foreach(it.machine.states) => key, val}}
    case State::{{key}}:
        onLeaving{{it.generator.capitalize(key)}}State (newState);
{{@each(it.generator.allExitActions(key)) => action, index}}
        SMSpec::{{action}}(this);
{{/each}}
        break;
{{/foreach}}
    default:
        break;
    }
}

template<typename SMSpec>
void {{it.generator.class()}}<SMSpec>::_enteringStateHelper(Event event, State newState, void* payload) {
    switch (newState) {
{{@foreach(it.machine.states) => key, val}}
    case State::{{key}}:
{{@each(it.generator.allEntryActions(key)) => action, index}}
        SMSpec::{{action}}(this);
{{/each}}
        break;
{{/foreach}}
    default:
        break;
    }

{{@each(it.generator.allEventToStatePairs()) => pair, index}}
    if (event == Event::{{pair[0]}} && newState == State::{{pair[1]}}) {
        std::shared_ptr<{{it.generator.capitalize(pair[0])}}Payload>* typedPayload = static_cast<std::shared_ptr<{{it.generator.capitalize(pair[0])}}Payload>*>(payload);
        onEnteringState{{it.generator.capitalize(pair[1])}}On{{pair[0]}}(newState, *typedPayload);
        return;
    }
{{/each}}
}

template<typename SMSpec>
void {{it.generator.class()}}<SMSpec>::_transitionActionsHelper(State fromState, Event event, void* payload) {
{{@foreach(it.machine.states) => state, val}}
{{@each(it.generator.allTransitionActions(state)) => pair, index}}
    if (fromState == State::{{state}} && event == Event::{{pair[0]}}) {
        std::shared_ptr<{{it.generator.capitalize(pair[0])}}Payload>* typedPayload = static_cast<std::shared_ptr<{{it.generator.capitalize(pair[0])}}Payload>*>(payload);
        SMSpec::{{pair[1]}}(this, *typedPayload);
    }
{{/each}}
{{/foreach}}
}

template<typename SMSpec>
void {{it.generator.class()}}<SMSpec>::_enteredStateHelper(Event event, State newState, void* payload) {
{{@each(it.generator.allEventToStatePairs()) => pair, index}}
    if (event == Event::{{pair[0]}} && newState == State::{{pair[1]}}) {
        std::shared_ptr<{{it.generator.capitalize(pair[0])}}Payload>* typedPayload = static_cast<std::shared_ptr<{{it.generator.capitalize(pair[0])}}Payload>*>(payload);
        onEnteredState{{it.generator.capitalize(pair[1])}}On{{pair[0]}}(*typedPayload);
        return;
    }
{{/each}}
}

template<typename SMSpec>
void {{it.generator.class()}}<SMSpec>::accessContextLocked(std::function<void(StateMachineContext& userContext)> callback) {
    // This variable is preventing the user from posting an event while inside the callback,
    // as it will be a deadlock.
    _insideAccessContextLocked = true;
    callback(_context);  // User can modify the context under lock.
    _insideAccessContextLocked = false;
}

template<typename SMSpec>
void {{it.generator.class()}}<SMSpec>::logTransition(TransitionPhase phase, State currentState, State nextState) const {
    switch (phase) {
    case TransitionPhase::LEAVING_STATE:
        Serial.print("leaving ");
        Serial.print({{it.generator.class()}}StateToString(currentState));
        Serial.print(" transitioning to ");
        Serial.print({{it.generator.class()}}StateToString(nextState));
        break;
    case TransitionPhase::ENTERING_STATE:
        Serial.print("entering ");
        Serial.print({{it.generator.class()}}StateToString(nextState));
        Serial.print(" from ");
        Serial.print({{it.generator.class()}}StateToString(currentState));
        break;
    case TransitionPhase::ENTERED_STATE:
        Serial.print("entered ");
        Serial.print({{it.generator.class()}}StateToString(currentState));
        break;
    case TransitionPhase::TRANSITION_NOT_FOUND:
        Serial.print("transistion not found ");
        Serial.print({{it.generator.class()}}StateToString(currentState));
        break;
    default:
        Serial.print("error");
        break;
    }
        Serial.println();
}


}  // namespace

