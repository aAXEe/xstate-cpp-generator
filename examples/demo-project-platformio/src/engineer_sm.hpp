/**
 * This header is automatically generated using the Xstate to C++ code generator:
 *    https://github.com/shuvalov-mdb/xstate-cpp-generator
 *    Copyright (c) 2020 Andrew Shuvalov
 *    License: MIT https://opensource.org/licenses/MIT
 *
 * Please do not edit. If changes are needed, regenerate using the TypeScript template 'engineer_generate.ts'.
 * Generated at Sun Jan 29 2023 13:09:23 GMT+0100 (Mitteleuropäische Normalzeit) from Xstate definition 'engineer_generate.ts'.
 * The simplest command line to run the generation:
 *     ts-node 'engineer_generate.ts'
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

namespace engineer_demo {

// All states declared in the SM EngineerSM.
enum class EngineerSMState {
    UNDEFINED_OR_ERROR_STATE = 0,

    sleeping,

    working,

    eating,

    weekend,

};

String EngineerSMStateToString(EngineerSMState state);

// @returns true if 'state' is a valid State.
bool isValidEngineerSMState(EngineerSMState state);

// All events declared in the SM EngineerSM.
enum class EngineerSMEvent {
    UNDEFINED_OR_ERROR_EVENT = 0,

    TIMER,

    HUNGRY,

    TIRED,

    ENOUGH,

};

String EngineerSMEventToString(EngineerSMEvent event);

// @returns true if 'event' is a valid Event.
bool isValidEngineerSMEvent(EngineerSMEvent event);

// As a transition could be conditional (https://xstate.js.org/docs/guides/guards.html#guards-condition-functions)
// one event is mapped to a vector of possible transitions.
using EngineerSMTransitionToStatesPair = std::pair<EngineerSMEvent,
        std::vector<EngineerSMState>>;

/**
 * All valid transitions from the specified state. The transition to state graph
 * is code genrated from the model and cannot change.
 */

const std::vector<EngineerSMTransitionToStatesPair>& EngineerSMValidTransitionsFromSleepingState();

const std::vector<EngineerSMTransitionToStatesPair>& EngineerSMValidTransitionsFromWorkingState();

const std::vector<EngineerSMTransitionToStatesPair>& EngineerSMValidTransitionsFromEatingState();

const std::vector<EngineerSMTransitionToStatesPair>& EngineerSMValidTransitionsFromWeekendState();


/**
 * Enum to indicate the current state transition phase in callbacks. This enum is used only for logging
 * and is not part of any State Machine logic.
 */
enum class EngineerSMTransitionPhase {
    UNDEFINED = 0,
    LEAVING_STATE,
    ENTERING_STATE,
    ENTERED_STATE,
    TRANSITION_NOT_FOUND
};

template <typename SMSpec> class EngineerSM;  // Forward declaration to use in Spec.

/**
 * Convenient default SM spec structure to parameterize the State Machine.
 * It can be replaced with a custom one if the SM events do not need any payload to be attached, and if there
 * is no guards, and no other advanced features.
 */
template <typename SMContext = std::nullptr_t>
struct DefaultEngineerSMSpec {
    /**
     * Generic data structure stored in the State Machine to keep some user-defined state that can be modified
     * when transitions happen.
     */
    using StateMachineContext = SMContext;

    /**
     * Each Event has a payload attached, which is passed in to the related callbacks.
     * The type should be movable for efficiency.
     */

    using EventTimerPayload = std::nullptr_t;

    using EventHungryPayload = std::nullptr_t;

    using EventTiredPayload = std::nullptr_t;

    using EventEnoughPayload = std::nullptr_t;


    /**
     * Actions are modeled in the Xstate definition, see https://xstate.js.org/docs/guides/actions.html.
     * This block is for transition actions.
     */

    static void startHungryTimer (EngineerSM<DefaultEngineerSMSpec>* sm, std::shared_ptr<EventTimerPayload>) {}

    static void startTiredTimer (EngineerSM<DefaultEngineerSMSpec>* sm, std::shared_ptr<EventTimerPayload>) {}

    static void checkEmail (EngineerSM<DefaultEngineerSMSpec>* sm, std::shared_ptr<EventHungryPayload>) {}


    /**
     * This block is for entry and exit state actions.
     */

    static void startWakeupTimer (EngineerSM<DefaultEngineerSMSpec>* sm) {}

    static void checkEmail (EngineerSM<DefaultEngineerSMSpec>* sm) {}

    static void startHungryTimer (EngineerSM<DefaultEngineerSMSpec>* sm) {}

    static void checkIfItsWeekend (EngineerSM<DefaultEngineerSMSpec>* sm) {}

    static void startShortTimer (EngineerSM<DefaultEngineerSMSpec>* sm) {}

    static void morningRoutine (EngineerSM<DefaultEngineerSMSpec>* sm) {}

};

/**
 *  State machine as declared in Xstate library for EngineerSM.
 *  SMSpec is a convenient template struct, which allows to specify various definitions used by generated code. In a simple
 *  case it's not needed and a convenient default is provided.
 *
 *  State Machine is not an abstract class and can be used without subclassing at all,
 *  though its functionality will be limited in terms of callbacks.
 *  Even though it's a templated class, a default SMSpec is provided to make a simple
 *  State Machine without any customization. In the most simple form, a working
 *  EngineerSM SM instance can be instantiated and used as in this example:
 *
 *    EngineerSM<> machine;
 *    auto currentState = machine.currentState();

 *    EngineerSM<>::TimerPayload payloadTIMER;      // ..and init payload with data
 *    machine.postEventTimer (std::move(payloadTIMER));

 *    EngineerSM<>::HungryPayload payloadHUNGRY;      // ..and init payload with data
 *    machine.postEventHungry (std::move(payloadHUNGRY));

 *    EngineerSM<>::TiredPayload payloadTIRED;      // ..and init payload with data
 *    machine.postEventTired (std::move(payloadTIRED));

 *    EngineerSM<>::EnoughPayload payloadENOUGH;      // ..and init payload with data
 *    machine.postEventEnough (std::move(payloadENOUGH));

 *
 *  Also see the generated unit tests in the example-* folders for more example code.
 */
template <typename SMSpec = DefaultEngineerSMSpec<std::nullptr_t>>
class EngineerSM {
  public:
    using TransitionToStatesPair = EngineerSMTransitionToStatesPair;
    using State = EngineerSMState;
    using Event = EngineerSMEvent;
    using TransitionPhase = EngineerSMTransitionPhase;
    using StateMachineContext = typename SMSpec::StateMachineContext;

    using TimerPayload = typename SMSpec::EventTimerPayload;

    using HungryPayload = typename SMSpec::EventHungryPayload;

    using TiredPayload = typename SMSpec::EventTiredPayload;

    using EnoughPayload = typename SMSpec::EventEnoughPayload;


    /**
     * Structure represents the current in-memory state of the State Machine.
     */
    struct CurrentState {
        State currentState = EngineerSMState::sleeping;
        /** previousState could be undefined if SM is at initial state */
        State previousState;
        /** The event that transitioned the SM from previousState to currentState */
        Event lastEvent;
        /** Timestamp of the last transition, or the class instantiation if at initial state */
        uint32_t lastTransitionTime = millis();
        /** Count of the transitions made so far */
        int totalTransitions = 0;
    };

    EngineerSM() {
    }

    virtual ~EngineerSM() {
        if (!isTerminated()) {
            Serial.println("State Machine EngineerSM is terminating "
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

    void postEventTimer (std::shared_ptr<TimerPayload> payload);

    void postEventHungry (std::shared_ptr<HungryPayload> payload);

    void postEventTired (std::shared_ptr<TiredPayload> payload);

    void postEventEnough (std::shared_ptr<EnoughPayload> payload);


    /**
     * All valid transitions from the current state of the State Machine.
     */
    const std::vector<EngineerSMTransitionToStatesPair>& validTransitionsFromCurrentState() const {
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

    virtual void onLeavingSleepingState(State nextState) {
        logTransition(EngineerSMTransitionPhase::LEAVING_STATE, State::sleeping, nextState);
    }

    virtual void onLeavingWorkingState(State nextState) {
        logTransition(EngineerSMTransitionPhase::LEAVING_STATE, State::working, nextState);
    }

    virtual void onLeavingEatingState(State nextState) {
        logTransition(EngineerSMTransitionPhase::LEAVING_STATE, State::eating, nextState);
    }

    virtual void onLeavingWeekendState(State nextState) {
        logTransition(EngineerSMTransitionPhase::LEAVING_STATE, State::weekend, nextState);
    }


    /**
     * 'onEnteringState' callbacks are invoked right before entering a new state. The internal
     * '_currentState' data still points to the existing state.
     * @param payload mutable payload, ownership remains with the caller. To take ownership of the payload
     *   override another calback from the 'onEntered*State' below.
     */

    virtual void onEnteringStateWorkingOnTIMER(State nextState, std::shared_ptr<TimerPayload> payload) {
        logTransition(EngineerSMTransitionPhase::ENTERING_STATE, _currentState.currentState, State::working);
    }

    virtual void onEnteringStateEatingOnHUNGRY(State nextState, std::shared_ptr<HungryPayload> payload) {
        logTransition(EngineerSMTransitionPhase::ENTERING_STATE, _currentState.currentState, State::eating);
    }

    virtual void onEnteringStateSleepingOnTIRED(State nextState, std::shared_ptr<TiredPayload> payload) {
        logTransition(EngineerSMTransitionPhase::ENTERING_STATE, _currentState.currentState, State::sleeping);
    }

    virtual void onEnteringStateWeekendOnENOUGH(State nextState, std::shared_ptr<EnoughPayload> payload) {
        logTransition(EngineerSMTransitionPhase::ENTERING_STATE, _currentState.currentState, State::weekend);
    }


    /**
     * 'onEnteredState' callbacks are invoked after SM moved to new state. The internal
     * '_currentState' data already points to the existing state.
     * It is guaranteed that the next transition will not start until this callback returns.
     * It is safe to call postEvent*() to trigger the next transition from this method.
     * @param payload ownership is transferred to the user.
     */

    virtual void onEnteredStateWorkingOnTIMER(std::shared_ptr<TimerPayload> payload) {
        logTransition(EngineerSMTransitionPhase::ENTERED_STATE, _currentState.currentState, State::working);
    }

    virtual void onEnteredStateEatingOnHUNGRY(std::shared_ptr<HungryPayload> payload) {
        logTransition(EngineerSMTransitionPhase::ENTERED_STATE, _currentState.currentState, State::eating);
    }

    virtual void onEnteredStateSleepingOnTIRED(std::shared_ptr<TiredPayload> payload) {
        logTransition(EngineerSMTransitionPhase::ENTERED_STATE, _currentState.currentState, State::sleeping);
    }

    virtual void onEnteredStateWeekendOnENOUGH(std::shared_ptr<EnoughPayload> payload) {
        logTransition(EngineerSMTransitionPhase::ENTERED_STATE, _currentState.currentState, State::weekend);
    }



    /**
     * All valid transitions from the specified state.
     */
    static inline const std::vector<TransitionToStatesPair>& validTransitionsFrom(EngineerSMState state) {
        switch (state) {

          case EngineerSMState::sleeping:
            return EngineerSMValidTransitionsFromSleepingState();

          case EngineerSMState::working:
            return EngineerSMValidTransitionsFromWorkingState();

          case EngineerSMState::eating:
            return EngineerSMValidTransitionsFromEatingState();

          case EngineerSMState::weekend:
            return EngineerSMValidTransitionsFromWeekendState();

          default: {
            Serial.print("invalid SM state ");
            Serial.println(EngineerSMStateToString(state));
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


template <typename SMSpec>
inline void EngineerSM<SMSpec>::postEventTimer (std::shared_ptr<EngineerSM::TimerPayload> payload) {
    if (_insideAccessContextLocked) {
        Serial.println("It is prohibited to post an event from inside the accessContextLocked()");
    }
    State currentState = _currentState.currentState;
    std::function<void()> eventCb{[ this, currentState, payload ] () mutable {
        _postEventHelper(currentState, EngineerSM::Event::TIMER, payload);
    }};
    _eventQueue.emplace(eventCb);
}


template <typename SMSpec>
inline void EngineerSM<SMSpec>::postEventHungry (std::shared_ptr<EngineerSM::HungryPayload> payload) {
    if (_insideAccessContextLocked) {
        Serial.println("It is prohibited to post an event from inside the accessContextLocked()");
    }
    State currentState = _currentState.currentState;
    std::function<void()> eventCb{[ this, currentState, payload ] () mutable {
        _postEventHelper(currentState, EngineerSM::Event::HUNGRY, payload);
    }};
    _eventQueue.emplace(eventCb);
}


template <typename SMSpec>
inline void EngineerSM<SMSpec>::postEventTired (std::shared_ptr<EngineerSM::TiredPayload> payload) {
    if (_insideAccessContextLocked) {
        Serial.println("It is prohibited to post an event from inside the accessContextLocked()");
    }
    State currentState = _currentState.currentState;
    std::function<void()> eventCb{[ this, currentState, payload ] () mutable {
        _postEventHelper(currentState, EngineerSM::Event::TIRED, payload);
    }};
    _eventQueue.emplace(eventCb);
}


template <typename SMSpec>
inline void EngineerSM<SMSpec>::postEventEnough (std::shared_ptr<EngineerSM::EnoughPayload> payload) {
    if (_insideAccessContextLocked) {
        Serial.println("It is prohibited to post an event from inside the accessContextLocked()");
    }
    State currentState = _currentState.currentState;
    std::function<void()> eventCb{[ this, currentState, payload ] () mutable {
        _postEventHelper(currentState, EngineerSM::Event::ENOUGH, payload);
    }};
    _eventQueue.emplace(eventCb);
}



template<typename SMSpec>
template<typename Payload>
void EngineerSM<SMSpec>::_postEventHelper (EngineerSM::State state,
    EngineerSM::Event event, std::shared_ptr<Payload> payload) {

    // Step 1: Invoke the guard callback. TODO: implement.

    // Step 2: check if the transition is valid.
    const std::vector<EngineerSMState>* targetStates = nullptr;
    const std::vector<EngineerSMTransitionToStatesPair>& validTransitions = validTransitionsFrom(state);
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
        if (newState == State::weekend) {
            _smIsTerminated = true;
        }
    }

    // Step 6: Invoke the 'entered the state' callback.
    _enteredStateHelper(event, newState, &payload);
}

template<typename SMSpec>
void EngineerSM<SMSpec>::_leavingStateHelper(State fromState, State newState) {
    switch (fromState) {

    case State::sleeping:
        onLeavingSleepingState (newState);

        SMSpec::morningRoutine(this);

        break;

    case State::working:
        onLeavingWorkingState (newState);

        break;

    case State::eating:
        onLeavingEatingState (newState);

        SMSpec::checkEmail(this);

        SMSpec::startHungryTimer(this);

        break;

    case State::weekend:
        onLeavingWeekendState (newState);

        break;

    default:
        break;
    }
}

template<typename SMSpec>
void EngineerSM<SMSpec>::_enteringStateHelper(Event event, State newState, void* payload) {
    switch (newState) {

    case State::sleeping:

        SMSpec::startWakeupTimer(this);

        break;

    case State::working:

        SMSpec::checkEmail(this);

        SMSpec::startHungryTimer(this);

        SMSpec::checkIfItsWeekend(this);

        break;

    case State::eating:

        SMSpec::startShortTimer(this);

        break;

    case State::weekend:

        break;

    default:
        break;
    }


    if (event == Event::TIMER && newState == State::working) {
        std::shared_ptr<TimerPayload>* typedPayload = static_cast<std::shared_ptr<TimerPayload>*>(payload);
        onEnteringStateWorkingOnTIMER(newState, *typedPayload);
        return;
    }

    if (event == Event::HUNGRY && newState == State::eating) {
        std::shared_ptr<HungryPayload>* typedPayload = static_cast<std::shared_ptr<HungryPayload>*>(payload);
        onEnteringStateEatingOnHUNGRY(newState, *typedPayload);
        return;
    }

    if (event == Event::TIRED && newState == State::sleeping) {
        std::shared_ptr<TiredPayload>* typedPayload = static_cast<std::shared_ptr<TiredPayload>*>(payload);
        onEnteringStateSleepingOnTIRED(newState, *typedPayload);
        return;
    }

    if (event == Event::ENOUGH && newState == State::weekend) {
        std::shared_ptr<EnoughPayload>* typedPayload = static_cast<std::shared_ptr<EnoughPayload>*>(payload);
        onEnteringStateWeekendOnENOUGH(newState, *typedPayload);
        return;
    }

}

template<typename SMSpec>
void EngineerSM<SMSpec>::_transitionActionsHelper(State fromState, Event event, void* payload) {


    if (fromState == State::sleeping && event == Event::TIMER) {
        std::shared_ptr<TimerPayload>* typedPayload = static_cast<std::shared_ptr<TimerPayload>*>(payload);
        SMSpec::startHungryTimer(this, *typedPayload);
    }

    if (fromState == State::sleeping && event == Event::TIMER) {
        std::shared_ptr<TimerPayload>* typedPayload = static_cast<std::shared_ptr<TimerPayload>*>(payload);
        SMSpec::startTiredTimer(this, *typedPayload);
    }



    if (fromState == State::working && event == Event::HUNGRY) {
        std::shared_ptr<HungryPayload>* typedPayload = static_cast<std::shared_ptr<HungryPayload>*>(payload);
        SMSpec::checkEmail(this, *typedPayload);
    }



    if (fromState == State::eating && event == Event::TIMER) {
        std::shared_ptr<TimerPayload>* typedPayload = static_cast<std::shared_ptr<TimerPayload>*>(payload);
        SMSpec::startHungryTimer(this, *typedPayload);
    }




}

template<typename SMSpec>
void EngineerSM<SMSpec>::_enteredStateHelper(Event event, State newState, void* payload) {

    if (event == Event::TIMER && newState == State::working) {
        std::shared_ptr<TimerPayload>* typedPayload = static_cast<std::shared_ptr<TimerPayload>*>(payload);
        onEnteredStateWorkingOnTIMER(*typedPayload);
        return;
    }

    if (event == Event::HUNGRY && newState == State::eating) {
        std::shared_ptr<HungryPayload>* typedPayload = static_cast<std::shared_ptr<HungryPayload>*>(payload);
        onEnteredStateEatingOnHUNGRY(*typedPayload);
        return;
    }

    if (event == Event::TIRED && newState == State::sleeping) {
        std::shared_ptr<TiredPayload>* typedPayload = static_cast<std::shared_ptr<TiredPayload>*>(payload);
        onEnteredStateSleepingOnTIRED(*typedPayload);
        return;
    }

    if (event == Event::ENOUGH && newState == State::weekend) {
        std::shared_ptr<EnoughPayload>* typedPayload = static_cast<std::shared_ptr<EnoughPayload>*>(payload);
        onEnteredStateWeekendOnENOUGH(*typedPayload);
        return;
    }

}

template<typename SMSpec>
void EngineerSM<SMSpec>::accessContextLocked(std::function<void(StateMachineContext& userContext)> callback) {
    // This variable is preventing the user from posting an event while inside the callback,
    // as it will be a deadlock.
    _insideAccessContextLocked = true;
    callback(_context);  // User can modify the context under lock.
    _insideAccessContextLocked = false;
}

template<typename SMSpec>
void EngineerSM<SMSpec>::logTransition(TransitionPhase phase, State currentState, State nextState) const {
    switch (phase) {
    case TransitionPhase::LEAVING_STATE:
        Serial.print("leaving ");
        Serial.print(EngineerSMStateToString(currentState));
        Serial.print(" transitioning to ");
        Serial.print(EngineerSMStateToString(nextState));
        break;
    case TransitionPhase::ENTERING_STATE:
        Serial.print("entering ");
        Serial.print(EngineerSMStateToString(nextState));
        Serial.print(" from ");
        Serial.print(EngineerSMStateToString(currentState));
        break;
    case TransitionPhase::ENTERED_STATE:
        Serial.print("entered ");
        Serial.print(EngineerSMStateToString(currentState));
        break;
    case TransitionPhase::TRANSITION_NOT_FOUND:
        Serial.print("transistion not found ");
        Serial.print(EngineerSMStateToString(currentState));
        break;
    default:
        Serial.print("error");
        break;
    }
        Serial.println();
}


}  // namespace

