#include "PreDeployment.h"

#include "bluefruit.h"

#include "../../core/src/config.h"
#include "../../core/src/globals.h"
#include "../../utils/src/SleepManager.h"
#include "../../utils/src/SettingsManager.h"
#include "../../comms/src/BLEComm.h"
#include "../../sensors/src/BatteryMonitor.h"
#include "../../sensors/src/phSensor.h"

void setupPreDeploymentState () {
  // Turn Green LED on to indicate entering Pre-Deployment State
  digitalWrite(LED_GREEN, LOW);

  loadSettings();

  // Setup BLE services and characteristics
  Serial.println("Setting up BLE services for Pre-Deployment configuration...");
  setupServicesPreDeployment();

  // Setup the advertising packet(s)
  Serial.println("Setting up the advertising");
  startAdvPreDeployment();

  Serial.println("Waiting for BLE connection for Pre-Deployment configuration...");
}

void runPreDeploymentLoop () {
  Serial.println("Starting Pre-Deployment measurements...");
  const uint32_t start_ms = millis();

  for (int sample = 0; sample < numberMeasurementsPreDeployment; sample++) {
    float phVal, VpH, Vbatt, dieTemp;

    getBattVoltage(Vbatt);
    // Serial.printf("Battery Voltage: %.3f V\n", Vbatt);
    getpHValue(phVal, VpH, dieTemp);
    // Serial.printf("pH Value: %.3f, pH Voltage: %.3f V, Die Temperature: %.2f C\n", phVal, VpH, dieTemp);

    indicateData(millis() - start_ms, phVal, VpH, dieTemp, Vbatt);
    // Put to sleep according to sample interval
    if (sample < numberMeasurementsPreDeployment - 1) {
      Serial.println("Sleeping until next measurement...");
      // lightSleep(sampleIntervalPreDeployment);
      delay(sampleIntervalPreDeployment * 1000);
    }
  }

  Serial.println("Sucess!!!");
  Serial.println("Completed all measurements in Pre-Deployment mode.");
  delay(1000);
  if (Bluefruit.connected()) {
    Bluefruit.disconnect(webappConnHandle);
    delay(100); // small delay to allow BLE events to complete
  }

  // Stop advertising
  Bluefruit.Advertising.stop();

  digitalWrite(LED_GREEN, HIGH); // Turn off Green LED
  digitalWrite(LED_BLUE, HIGH);  // Turn off Blue LED

  deepSleep(); // Sleep indefinitely after measurements are done
}