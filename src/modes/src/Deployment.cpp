#include "Deployment.h"

#include "../../utils/src/SettingsManager.h"
#include "../../../config.h"
#include "../../sensors/src/BatteryMonitor.h"
#include "../../sensors/src/phSensor.h"

void runDeploymentState () {
  loadSettings();

  Serial.println("Current Configuration:");
  Serial.printf("continuousScanningDeployment = %d\n", continuousScanningDeployment);
  Serial.printf("numberMeasurementsDeployment = %lu\n", numberMeasurementsDeployment);
  Serial.printf("numberMeasurementsPreDeployment = %lu\n", numberMeasurementsPreDeployment);
  Serial.printf("numberSamplesPerHour = %d\n", numberSamplesPerHour);
  Serial.printf("sampleIntervalPreDeployment = %d\n", sampleIntervalPreDeployment);

  if (!continuousScanningDeployment){ // Limited Scanning
    for (int sample = 0; sample < numberMeasurementsDeployment; sample++) {
      float phVal, VpH, Vbatt, dieTemp;

      getBattVoltage(Vbatt);
      getpHValue(phVal, VpH, dieTemp);

      // Print to log file using buffer

      // Put to sleep according to number of samples per hour
    }
  }
  else { // Continuous scanning
    while (1) {

    }
  }

  while (1);
}