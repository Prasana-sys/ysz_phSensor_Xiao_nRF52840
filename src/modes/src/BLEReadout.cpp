#include "BLEReadout.h"

#include "../../../globals.h"
#include "../../comms/src/BLEComm.h"
#include "../../utils/src/tools.h"
#include "../../utils/src/SleepManager.h"

void runBLEReadoutState () {
  // Setup BLE services and characteristics
  Serial.println("Setting up BLE services for BLE Readout state...");
  setupServicesReadout();

  // Setup the advertising packet(s)
  Serial.println("Setting up the advertising");
  startAdvReadout();

  Serial.println("Waiting for BLE connection for BLE Readout state...");
}

void runBLEReadoutLoop () {
  Serial.println("Entering BLE Readout loop...");

  char buf[128];

  if (!fat_file.open("/log.csv", O_RDONLY)) {
    Serial.println("log.csv not found, returning nothing.");
    indicateNoData();
    return;
  }

  while (fat_file.fgets(buf, sizeof(buf)) > 0) {
    char *s = trim(buf);
    
    indicateData(s);
  }
  
  Serial.println("Sucess!!!");
  Serial.println("Completed BLE Readout of all logged data.");

  fat_file.close();

  // TODO: Shutdown if BLE Readout complete?
  // Serial.println("BLE Readout session ended.");
  // delay(1000);
  // if (Bluefruit.connected()) {
  //   Bluefruit.disconnect(webappConnHandle);
  //   delay(100); // small delay to allow BLE events to complete
  // }

  // // Stop advertising
  // Bluefruit.Advertising.stop();

  // deepSleep(); // Sleep indefinitely after BLE Readout is done
}