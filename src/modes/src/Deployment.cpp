#include "Deployment.h"

#include "../../utils/src/SettingsManager.h"
#include "../../core/src/config.h"
#include "../../core/src/globals.h"
#include "../../sensors/src/BatteryMonitor.h"
#include "../../sensors/src/phSensor.h"
#include "../../utils/src/Logger.h"
#include "../../utils/src/SleepManager.h"

void runDeploymentState () {
  digitalWrite(afeEnablePin, HIGH); // Enable AFE

  // Flash White LED to indicate entering Deployment State
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
    delay(1000);
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, HIGH);
    delay(1000);
  } 

  loadSettings();

  Serial.println("Current Configuration:");
  Serial.printf("continuousScanningDeployment = %d\n", continuousScanningDeployment);
  Serial.printf("numberMeasurementsDeployment = %lu\n", numberMeasurementsDeployment);
  Serial.printf("numberMeasurementsPreDeployment = %lu\n", numberMeasurementsPreDeployment);
  Serial.printf("sampleIntervalDeployment = %lu\n", sampleIntervalDeployment);
  Serial.printf("sampleIntervalPreDeployment = %d\n", sampleIntervalPreDeployment);

  // TODO: add deployment start delay setting?

  const uint32_t start_ms = millis();

  if (!continuousScanningDeployment){ // Limited Scanning
    for (int sample = 0; sample < numberMeasurementsDeployment; sample++) {
      float phVal, VpH, Vbatt, dieTemp;

      getBattVoltage(Vbatt);
      Serial.printf("Battery Voltage: %.3f V\n", Vbatt);
      getpHValue(phVal, VpH, dieTemp);
      Serial.printf("pH Value: %.3f, pH Voltage: %.3f V, Die Temperature: %.2f C\n", phVal, VpH, dieTemp);

      if (Vbatt > BATT_LOW_VOLTAGE){
        logSaveData(millis() - start_ms, phVal, VpH, dieTemp, Vbatt);
        // Put to sleep according to sample interval
        if (sample < numberMeasurementsDeployment - 1) {
          Serial.println("Sleeping until next measurement...");
          lightSleep(sampleIntervalDeployment);
        }
      }
      else {
        Serial.println("Battery Low Voltage! Stopping further measurements.");
        deepSleep(); // Sleep indefinitely
      }
    }

    Serial.println("Completed all measurements in limited scanning mode.");
    delay(1000);
    deepSleep(); // Sleep indefinitely after measurements are done
  }
  else { // Continuous scanning
    while (1) {
      float phVal, VpH, Vbatt, dieTemp;

      getBattVoltage(Vbatt);
      getpHValue(phVal, VpH, dieTemp);

      if (Vbatt > BATT_LOW_VOLTAGE){
        logSaveData(millis() - start_ms, phVal, VpH, dieTemp, Vbatt);
      }
      else {
        Serial.println("Battery Low Voltage! Stopping further measurements.");
        deepSleep(); // Sleep indefinitely
      }
      
      // Put to sleep according to number of samples per hour
      lightSleep(sampleIntervalDeployment);
    }
  }

  while (1);
}