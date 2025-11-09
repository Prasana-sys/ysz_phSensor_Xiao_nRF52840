#include "BLEComm.h"

#include "../../core/src/globals.h"
#include "../../core/src/config.h"

#include "../../modes/src/PreDeployment.h"
#include "../../modes/src/BLEReadout.h"
#include "../../utils/src/SettingsManager.h"
#include "../../utils/src/tools.h"

uint16_t webappConnHandle = 0xFFFF; // Invalid handle

void setupServicesPreDeployment() {
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX); // Set max MTU to 247 bytes
  Bluefruit.begin(1, 0);
  Bluefruit.setName("YSZ pH Sensor");
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

  pms.begin();

  // Properties = Read + Indicate
  // Permission = Open to read, cannot write
  pmsLogEntry.setProperties(CHR_PROPS_READ | CHR_PROPS_INDICATE);
  pmsLogEntry.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  pmsLogEntry.setMaxLen(128);
  pmsLogEntry.begin();
  pmsLogEntry.write8(0xFF); // Initial dummy value

  cs.begin();

  // Properties = Read + Write + Indicate
  // Permission = Open to read, Open to write
  csCommand.setProperties(CHR_PROPS_READ | CHR_PROPS_WRITE | CHR_PROPS_INDICATE);
  csCommand.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  csCommand.setFixedLen(1);
  csCommand.begin();
  csCommand.setWriteCallback(PreDeploymentCommandWriteCallback);
  csCommand.write8(0xFF); // Initial dummy value

  csContScanDeploy.setProperties(CHR_PROPS_READ | CHR_PROPS_WRITE | CHR_PROPS_INDICATE);
  csContScanDeploy.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  csContScanDeploy.setFixedLen(1);
  csContScanDeploy.begin();
  csContScanDeploy.write8(continuousScanningDeployment ? 0x01 : 0x00);
  csContScanDeploy.setWriteCallback(PreDeploymentContScanDeployWriteCallback);

  csNumMeasDeploy.setProperties(CHR_PROPS_READ | CHR_PROPS_WRITE | CHR_PROPS_INDICATE);
  csNumMeasDeploy.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  csNumMeasDeploy.setMaxLen(4);
  csNumMeasDeploy.begin();
  csNumMeasDeploy.write32(numberMeasurementsDeployment);
  csNumMeasDeploy.setWriteCallback(PreDeploymentNumMeasDeployWriteCallback);  

  csNumMeasPreDeploy.setProperties(CHR_PROPS_READ | CHR_PROPS_WRITE | CHR_PROPS_INDICATE);
  csNumMeasPreDeploy.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  csNumMeasPreDeploy.setMaxLen(4);
  csNumMeasPreDeploy.begin();
  csNumMeasPreDeploy.write32(numberMeasurementsPreDeployment);
  csNumMeasPreDeploy.setWriteCallback(PreDeploymentNumMeasPreDeployWriteCallback);

  csSampleIntervalDeploy.setProperties(CHR_PROPS_READ | CHR_PROPS_WRITE | CHR_PROPS_INDICATE);
  csSampleIntervalDeploy.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  csSampleIntervalDeploy.setMaxLen(4);
  csSampleIntervalDeploy.begin();
  csSampleIntervalDeploy.write32(sampleIntervalDeployment);
  csSampleIntervalDeploy.setWriteCallback(PreDeploymentSampleIntervalDeployWriteCallback);

  csSampleIntervalPreDeploy.setProperties(CHR_PROPS_READ | CHR_PROPS_WRITE | CHR_PROPS_INDICATE);
  csSampleIntervalPreDeploy.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  csSampleIntervalPreDeploy.setFixedLen(1);
  csSampleIntervalPreDeploy.begin();
  csSampleIntervalPreDeploy.write8(sampleIntervalPreDeployment);
  csSampleIntervalPreDeploy.setWriteCallback(PreDeploymentSampleIntervalPreDeployWriteCallback);
}

void startAdvPreDeployment(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include PMS and CS Service UUIDs
  Bluefruit.Advertising.addService(pms);
  Bluefruit.Advertising.addService(cs);

  // Secondary Scan Response packet (optional)
  // Since there is no room for 'Name' in Advertising packet
  Bluefruit.ScanResponse.addName();
  
  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
}

void setupServicesReadout(void) {
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX); // Set max MTU to 247 bytes
  Bluefruit.begin(1, 0);
  Bluefruit.setName("YSZ pH Sensor");
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

  pms.begin();

  // Properties = Read + Indicate
  // Permission = Open to read, cannot write
  pmsLogEntry.setProperties(CHR_PROPS_READ | CHR_PROPS_INDICATE);
  pmsLogEntry.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  pmsLogEntry.setMaxLen(128);
  pmsLogEntry.begin();
  pmsLogEntry.write8(0xFF); // Initial dummy value

  // cs.begin();

  // Properties = Read + Write + Indicate
  // Permission = Open to read, Open to write
  csCommand.setProperties(CHR_PROPS_READ | CHR_PROPS_WRITE | CHR_PROPS_INDICATE);
  csCommand.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  csCommand.setFixedLen(1);
  csCommand.begin();
  csCommand.setWriteCallback(ReadoutCommandWriteCallback);
  csCommand.write8(0xFF); // Initial dummy value
}

void startAdvReadout(void) {
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include PMS and CS Service UUIDs
  Bluefruit.Advertising.addService(pms);
  Bluefruit.Advertising.addService(cs);

  // Secondary Scan Response packet (optional)
  // Since there is no room for 'Name' in Advertising packet
  Bluefruit.ScanResponse.addName();
  
  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
}

// callback invoked when central connects
void connect_callback(uint16_t conn_handle)
{
  Serial.println("Webapp Connected.");
  webappConnHandle = conn_handle;

  digitalWrite(LED_BLUE, LOW); // Keep Blue LED on to indicate connection

  // lsbLED.write8(0x01);

  // connection_count++;
  // Serial.print("Connection count: ");
  // Serial.println(connection_count);

  // // Keep advertising if not reaching max
  // if (connection_count < MAX_PRPH_CONNECTION)
  // {
  //   Serial.println("Keep advertising");
  //   Bluefruit.Advertising.start(0);
  // }
}

/**
 * Callback invoked when a connection is dropped
 * @param conn_handle connection where this event happens
 * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
 */
void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  Serial.println("Webapp Disconnected.");
  webappConnHandle = 0xFFFF; // Invalidate handle
  // (void) conn_handle;
  // (void) reason;

  // setLED(false);
  // lsbLED.write8(0x00);

  // Serial.println();
  // Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);

  // connection_count--;
}

void PreDeploymentCommandWriteCallback(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len) {
  if (len < 1) return;
  uint8_t cmd = data[0];

  switch (cmd) {
    case 0x01:  // SAVE SETTINGS
      Serial.println("BLE Command: SAVE SETTINGS");
      saveSettings();
      csContScanDeploy.write8(continuousScanningDeployment ? 0x01 : 0x00);
      csNumMeasDeploy.write32(numberMeasurementsDeployment);
      csNumMeasPreDeploy.write32(numberMeasurementsPreDeployment);
      csSampleIntervalDeploy.write32(sampleIntervalDeployment);
      csSampleIntervalPreDeploy.write8(sampleIntervalPreDeployment);
      break;

    case 0x02:  // CONTINUE TO PRE-DEPLOYMENT
      Serial.println("BLE Command: CONTINUE TO PRE-DEPLOYMENT");
      // saveSettings();
      runPreDeploymentLoop(); // Exit function to transition to sending measurements
      break;

    case 0x03:  // RESET SETTINGS
      Serial.println("BLE Command: RESET SETTINGS TO DEFAULTS");
      continuousScanningDeployment = DEFAULT_CONTINUOUS_SCANNING_DEPLOYMENT;
      numberMeasurementsDeployment = DEFAULT_NUMBER_MEASUREMENTS_DEPLOYMENT;
      numberMeasurementsPreDeployment = DEFAULT_NUMBER_MEASUREMENTS_PRE_DEPLOYMENT;
      sampleIntervalDeployment = DEFAULT_SAMPLE_INTERVAL_DEPLOYMENT;
      sampleIntervalPreDeployment = DEFAULT_SAMPLE_INTERVAL_PRE_DEPLOYMENT;
      saveSettings();          // reload defaults
      csContScanDeploy.write8(continuousScanningDeployment ? 0x01 : 0x00);
      csNumMeasDeploy.write32(numberMeasurementsDeployment);
      csNumMeasPreDeploy.write32(numberMeasurementsPreDeployment);
      csSampleIntervalDeploy.write32(sampleIntervalDeployment);
      csSampleIntervalPreDeploy.write8(sampleIntervalPreDeployment);
      break;

    case 0x04:  // PING MODE
      Serial.println("BLE Command: PING MODE");
      chr->indicate8(0xAB);              // Pre-Deployment Mode (0xAB)
      break;

    default:
      Serial.print("Unknown command received: 0x");
      Serial.println(cmd, HEX);
      break;
  }
}

void PreDeploymentContScanDeployWriteCallback(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len) {
  if (len > 1) return;
  continuousScanningDeployment = (data[0] != 0);
  Serial.print("Updated continuousScanningDeployment: ");
  Serial.println(continuousScanningDeployment);
  chr->indicate8(0xAA);              // Acknowledge (0xAA)
  chr->write8(continuousScanningDeployment ? 0x01 : 0x00);
}

void PreDeploymentNumMeasDeployWriteCallback(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len) {
  if (len > 4) return;
  uint32_t value = *((uint32_t*)data);
  numberMeasurementsDeployment = value;
  Serial.print("Updated numberMeasurementsDeployment: ");
  Serial.println(numberMeasurementsDeployment);
  chr->indicate8(0xAA);              // Acknowledge (0xAA)
  chr->write32(numberMeasurementsDeployment);
}

void PreDeploymentNumMeasPreDeployWriteCallback(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len) {
  if (len > 4) return;
  uint32_t value = *((uint32_t*)data);
  numberMeasurementsPreDeployment = value;
  Serial.print("Updated numberMeasurementsPreDeployment: ");
  Serial.println(numberMeasurementsPreDeployment);
  chr->indicate8(0xAA);              // Acknowledge (0xAA)
  chr->write32(numberMeasurementsPreDeployment);
}

void PreDeploymentSampleIntervalDeployWriteCallback(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len) {
  if (len > 4) return;
  uint32_t value = *((uint32_t*)data);
  sampleIntervalDeployment = value;
  Serial.print("Updated sampleIntervalDeployment: ");
  Serial.println(sampleIntervalDeployment);
  chr->indicate8(0xAA);              // Acknowledge (0xAA)
  chr->write32(sampleIntervalDeployment);
}

void PreDeploymentSampleIntervalPreDeployWriteCallback(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len) {
  if (len > 1) return;
  sampleIntervalPreDeployment = data[0];
  Serial.print("Updated sampleIntervalPreDeployment: ");
  Serial.println(sampleIntervalPreDeployment);
  chr->indicate8(0xAA);              // Acknowledge (0xAA)
  chr->write8(sampleIntervalPreDeployment);
}

void ReadoutCommandWriteCallback(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len) {
  if (len > 1) return;
  uint8_t cmd = data[0];

  switch (cmd) {
    case 0x04:  // PING MODE
      Serial.println("BLE Command: PING MODE");
      chr->indicate8(0xCD);              // Readout Mode (0xCD)
      break;

    case 0x12: // START DATA TRANSFER
      Serial.println("BLE Command: START DATA TRANSFER");
      runBLEReadoutLoop(); // Exit function to transition to sending measurements
      break;


    case 0x13: // DELETE LOG FILE
      Serial.println("BLE Command: DELETE LOG FILE");
      if (fatfs.remove("/log.csv")) {
        Serial.println("log.csv deleted successfully.");
        chr->indicate8(0xAA);              // Acknowledge success (0xAA)
      }
      else {
        Serial.println("Failed to delete log.csv or file does not exist.");
        chr->indicate8(0xFF);              // Acknowledge failure (0xFF)
      }
      break;

    default:
      Serial.print("Unknown command received: 0x");
      Serial.println(cmd, HEX);
      break;
  }
}

void indicateData(uint32_t timestamp_ms, float pHVal, float VpH, float dieTemp, float Vbatt) {
  if ( !( Bluefruit.connected(webappConnHandle) && pmsLogEntry.indicateEnabled(webappConnHandle) ) ) {
    Serial.println("Failed to indicate data!");
    digitalWrite(LED_RED, LOW);
    return;
  }

  // Write data in CSV format
  char buf[128];

  int cx = fmt_hms(timestamp_ms, buf, sizeof(buf));

  snprintf(buf+cx, sizeof(buf)-cx, ",%.3f,%.3f,%.2f,%.3f\n", pHVal, VpH, dieTemp, Vbatt);
  // Use below line if implementing additional characteristics in future
  // snprintf(buf+cx, sizeof(buf)-cx, ",%.3f,%.3f,%.2f,%.3f,%lu\n", pHVal, VpH, dieTemp, Vbatt, timestamp_ms);
  pmsLogEntry.indicate(webappConnHandle, buf);

  Serial.println("Data sent to webapp");
  Serial.print("Logged Data: ");
  Serial.print(buf);
}

void indicateData(const char* dataStr) {
  if ( !( Bluefruit.connected(webappConnHandle) && pmsLogEntry.indicateEnabled(webappConnHandle) ) ) {
    Serial.println("Failed to indicate data!");
    digitalWrite(LED_RED, LOW);
    return;
  }

  pmsLogEntry.indicate(webappConnHandle, dataStr);

  Serial.println("Data sent to webapp");
  Serial.print("Logged Data: ");
  Serial.print(dataStr);
}

void indicateNoData(void) {
  if ( !( Bluefruit.connected(webappConnHandle) && pmsLogEntry.indicateEnabled(webappConnHandle) ) ) {
    Serial.println("Failed to indicate no data!");
    digitalWrite(LED_RED, LOW);
    return;
  }

  const char* msg = "NO DATA\n";
  pmsLogEntry.indicate(webappConnHandle, msg);

  Serial.println("No data indicated to webapp");
}