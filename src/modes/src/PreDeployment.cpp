#include "PreDeployment.h"

#include "bluefruit.h"

#include "../../../config.h"
#include "../../../globals.h"
#include "../../utils/src/SleepManager.h"
#include "../../utils/src/SettingsManager.h"

// callback invoked when central connects
void connect_callback(uint16_t conn_handle)
{
  Serial.println("BLE Connected for Pre-Deployment configuration.");
  // (void) conn_handle;

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
  Serial.println("BLE Disconnected from Pre-Deployment configuration.");
  // (void) conn_handle;
  // (void) reason;

  // setLED(false);
  // lsbLED.write8(0x00);

  // Serial.println();
  // Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);

  // connection_count--;
}

void runPreDeploymentState () {
  Bluefruit.begin(1, 0);
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

  pms.begin();
  // Properties = Read + Indicate
  // Permission = Open to read, cannot write
  pmsLogEntry.setProperties(CHR_PROPS_READ | CHR_PROPS_INDICATE);
  pmsLogEntry.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  pmsLogEntry.setFixedLen(1);
  pmsLogEntry.begin();

  Serial.println("Waiting for BLE connection for Pre-Deployment configuration...");
}