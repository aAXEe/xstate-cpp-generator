#include "engineer_sm.hpp"
#include "softwareTimer.h"

using namespace std::chrono_literals;

namespace engineer_demo {

SoftwareTimer hungryTimer(1s);
SoftwareTimer tiredTimer(2s);
SoftwareTimer wakeupTimer(2s);
SoftwareTimer shortTimer(100ms);

struct EngineerContext {
  // The demo will end after the Engineer wakes up 7 times.
  int wakeUpCount = 0;
};

struct EngineerSpec {
  // Spec should always contain some 'using' for the StateMachineContext.
  using StateMachineContext = EngineerContext;

  // Then it should have a list of 'using' declarations for every event payload.
  using EventTimerPayload = std::nullptr_t;
  using EventHungryPayload = std::nullptr_t;
  using EventTiredPayload = std::nullptr_t;
  using EventEnoughPayload = std::nullptr_t;

  /**
   * This block is for transition actions.
   */
  static void startHungryTimer(EngineerSM<EngineerSpec> *sm,
                               std::shared_ptr<EventTimerPayload> payload) {
    Serial.println("Start HungryTimer from timer event");
    hungryTimer.start();
  }
  static void startTiredTimer(EngineerSM<EngineerSpec> *sm,
                              std::shared_ptr<EventTimerPayload> payload) {
    Serial.println("Start TiredTimer from timer event");
    tiredTimer.start();
  }
  static void checkEmail(EngineerSM<EngineerSpec> *sm,
                         std::shared_ptr<EventHungryPayload> payload) {
    Serial.println("Checking Email, while being hugry! ok...");
  }

  /**
   * This block is for entry and exit state actions.
   */
  static void startWakeupTimer(EngineerSM<EngineerSpec> *sm) {
    Serial.println("Do startWakeupTimer");
    wakeupTimer.start();
  }
  static void checkEmail(EngineerSM<EngineerSpec> *sm) {
    Serial.println("Checking Email, hmmm...");
  }

  static void checkIfItsWeekend(EngineerSM<EngineerSpec> *sm) {
    bool post = false;
    sm->accessContextLocked([&post](StateMachineContext &userContext) {
      if (userContext.wakeUpCount >= 6) {
        Serial.println("Wow it's weekend!");
        post = true;
      }
    });
    if (post) {
      // To avoid deadlock this should be invoked outside of the
      // accessContextLocked() method.
      sm->postEventEnough(std::nullptr_t());
    }
  }

  static void startHungryTimer(EngineerSM<EngineerSpec> *sm) {
    Serial.println("Start HungryTimer");
    hungryTimer.start();
  }

  static void startShortTimer(EngineerSM<EngineerSpec> *sm) {
    Serial.println("Start short Timer");
    shortTimer.start();
  }

  static void morningRoutine(EngineerSM<EngineerSpec> *sm) {
    sm->accessContextLocked([](StateMachineContext &userContext) {
      ++userContext.wakeUpCount;
      Serial.printf("This is my %u day of working...\n", userContext.wakeUpCount);
    });
  }

  static void startTiredTimer(EngineerSM<EngineerSpec> *sm) {
    Serial.println("Start TiredTimer");
    tiredTimer.start();
  }
};

} // namespace engineer_demo

using namespace engineer_demo;
EngineerSM<engineer_demo::EngineerSpec> stateMachine;
void setup() {
  Serial.begin(115200);
  Serial.println("hi");
  
    // Kick off the state machine with a timer event...
    stateMachine.postEventTimer(std::nullptr_t());
}

void loop() {
  stateMachine.loop();

  hungryTimer.loop();
  if (hungryTimer.wasTimeout()) {
    Serial.println("Ok, I'm hungry");
    stateMachine.postEventHungry(std::nullptr_t());
  }
  tiredTimer.loop();
  if (tiredTimer.wasTimeout()) {
    Serial.println("Ok, I'm tired");
    stateMachine.postEventTired(std::nullptr_t());
  }
  shortTimer.loop();
  if (shortTimer.wasTimeout()) {
    Serial.println("Hey, timer is ringing.");
    stateMachine.postEventTimer(std::nullptr_t());
  }
  wakeupTimer.loop();
  if (wakeupTimer.wasTimeout()) {
    Serial.println("Hey wake up");
    stateMachine.postEventTimer(std::nullptr_t());
  }
}
