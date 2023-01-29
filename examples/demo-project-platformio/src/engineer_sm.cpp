/**
 * This code is automatically generated using the Xstate to C++ code generator:
 *    https://github.com/shuvalov-mdb/xstate-cpp-generator , @author Andrew Shuvalov
 */

#include "engineer_sm.hpp"

namespace engineer_demo {

String EngineerSMStateToString(EngineerSMState state) {
    switch (state) {
        case EngineerSMState::UNDEFINED_OR_ERROR_STATE:
            return "UNDEFINED";

        case EngineerSMState::sleeping:
            return "EngineerSMState::sleeping";

        case EngineerSMState::working:
            return "EngineerSMState::working";

        case EngineerSMState::eating:
            return "EngineerSMState::eating";

        case EngineerSMState::weekend:
            return "EngineerSMState::weekend";

        default:
            return "ERROR";
    }
}

bool isValidEngineerSMState(EngineerSMState state) {
    if (state == EngineerSMState::UNDEFINED_OR_ERROR_STATE) { return true; }

    if (state == EngineerSMState::sleeping) { return true; }

    if (state == EngineerSMState::working) { return true; }

    if (state == EngineerSMState::eating) { return true; }

    if (state == EngineerSMState::weekend) { return true; }

    return false;
}

String EngineerSMEventToString(EngineerSMEvent event) {
    switch (event) {
        case EngineerSMEvent::UNDEFINED_OR_ERROR_EVENT:
            return "UNDEFINED";

        case EngineerSMEvent::TIMER:
            return "EngineerSMEvent::TIMER";

        case EngineerSMEvent::HUNGRY:
            return "EngineerSMEvent::HUNGRY";

        case EngineerSMEvent::TIRED:
            return "EngineerSMEvent::TIRED";

        case EngineerSMEvent::ENOUGH:
            return "EngineerSMEvent::ENOUGH";

        default:
            return "ERROR";
    }
}

bool isValidEngineerSMEvent(EngineerSMEvent event) {
    if (event == EngineerSMEvent::UNDEFINED_OR_ERROR_EVENT) { return true; }

    if (event == EngineerSMEvent::TIMER) { return true; }

    if (event == EngineerSMEvent::HUNGRY) { return true; }

    if (event == EngineerSMEvent::TIRED) { return true; }

    if (event == EngineerSMEvent::ENOUGH) { return true; }

    return false;
}


// static
const std::vector<EngineerSMTransitionToStatesPair>&
EngineerSMValidTransitionsFromSleepingState() {
    static const auto* transitions = new const std::vector<EngineerSMTransitionToStatesPair> {
    
    { EngineerSMEvent::TIMER, {
      
        EngineerSMState::working
      
     } },
    
    };
    return *transitions;
}


// static
const std::vector<EngineerSMTransitionToStatesPair>&
EngineerSMValidTransitionsFromWorkingState() {
    static const auto* transitions = new const std::vector<EngineerSMTransitionToStatesPair> {
    
    { EngineerSMEvent::HUNGRY, {
      
        EngineerSMState::eating
      
     } },
    
    { EngineerSMEvent::TIRED, {
      
        EngineerSMState::sleeping
      
     } },
    
    { EngineerSMEvent::ENOUGH, {
      
        EngineerSMState::weekend
      
     } },
    
    };
    return *transitions;
}


// static
const std::vector<EngineerSMTransitionToStatesPair>&
EngineerSMValidTransitionsFromEatingState() {
    static const auto* transitions = new const std::vector<EngineerSMTransitionToStatesPair> {
    
    { EngineerSMEvent::TIMER, {
      
        EngineerSMState::working
      
     } },
    
    { EngineerSMEvent::TIRED, {
      
        EngineerSMState::sleeping
      
     } },
    
    };
    return *transitions;
}


// static
const std::vector<EngineerSMTransitionToStatesPair>&
EngineerSMValidTransitionsFromWeekendState() {
    static const auto* transitions = new const std::vector<EngineerSMTransitionToStatesPair> {
    
    };
    return *transitions;
}




}  // namespace engineer_demo
