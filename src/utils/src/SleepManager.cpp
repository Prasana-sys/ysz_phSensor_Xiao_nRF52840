#include "SleepManager.h"

void lightSleep(uint32_t sleepDuration) {
  // Implement light sleep functionality here

  Serial.println("Entering light sleep mode...");
  delay(1000); // Simulate light sleep with delay
  Serial.println("Woke up from light sleep mode.");
}

void deepSleep() {
  // Implement deep sleep functionality here

  Serial.println("Entering deep sleep mode...");
  while (1); // Simulate deep sleep by halting execution
}