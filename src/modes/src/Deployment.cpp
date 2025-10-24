#include "Deployment.h"

#include "../../utils/src/SettingsManager.h"
#include "../../../config.h"
#include "../../sensors/src/BatteryMonitor.h"
#include "../../sensors/src/phSensor.h"
#include "../../utils/src/Logger.h"
#include "../../utils/src/SleepManager.h"

void runDeploymentState () {
  loadSettings();

  Serial.println("Current Configuration:");
  Serial.printf("continuousScanningDeployment = %d\n", continuousScanningDeployment);
  Serial.printf("numberMeasurementsDeployment = %lu\n", numberMeasurementsDeployment);
  Serial.printf("numberMeasurementsPreDeployment = %lu\n", numberMeasurementsPreDeployment);
  Serial.printf("sampleIntervalDeployment = %lu\n", sampleIntervalDeployment);
  Serial.printf("sampleIntervalPreDeployment = %d\n", sampleIntervalPreDeployment);

  const uint32_t start_ms = millis();

  if (!continuousScanningDeployment){ // Limited Scanning
    for (int sample = 0; sample < numberMeasurementsDeployment; sample++) {
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
      
      // Put to sleep according to sample interval
      lightSleep(sampleIntervalDeployment);
    }

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