#ifndef SOFTWARE_TIMER_H
#define SOFTWARE_TIMER_H

#include <Arduino.h>
#include <chrono>
class SoftwareTimer {
public:
  SoftwareTimer(std::chrono::milliseconds timeout)
      : m_timeout(timeout){

        };
  void start() {
    m_startTimestamp = millis();
    m_isRunning = true;
    m_hadTimeout=false;
    m_timeoutSeen = false;
  }
  void stop() { m_isRunning = false; }
  bool isRunning() const { return m_isRunning; }
  void loop() {
    if (!m_isRunning)
      return;
    auto currentTime = millis();
    if (currentTime - m_startTimestamp <
        std::chrono::duration_cast<std::chrono::milliseconds>(m_timeout)
            .count())
      return;
    m_isRunning = false;
    m_hadTimeout = true;
  }

  bool wasTimeout() {
    if (!m_hadTimeout)
      return false;
    if (m_timeoutSeen)
      return false;
    m_timeoutSeen = true;
    return true;
  }

private:
  std::chrono::milliseconds m_timeout;
  uint32_t m_startTimestamp;
  bool m_isRunning{false};
  bool m_timeoutSeen;
  bool m_hadTimeout;
};

#endif