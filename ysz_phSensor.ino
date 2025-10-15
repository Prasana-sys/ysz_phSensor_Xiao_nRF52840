// Delete SdFat /Documents/Arduino/libraries/SdFat
// Only use libraries provided in BSP!!!

#include <Arduino.h>

#include <Adafruit_TinyUSB.h> // included for Serial to compile

#include "config.h"
#include "globals.h"

#include "src/utils/src/MSCFlash.h" // read/write/flush callbacks for MSC
#include "src/modes/src/ChargingState.h"
#include "src/modes/src/Deployment.h"

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  // Set analog read resolution
  analogReadResolution(ADC_RESOLUTION);
  // initialise ADC wireing_analog_nRF52.c:73
  analogReference(AR_DEFAULT);        // default 0.6V*6=3.6V  wireing_analog_nRF52.c:73

  pinMode(VBAT_ENABLE, OUTPUT);
  digitalWrite(VBAT_ENABLE, LOW); // Set low to read battery, never set high

  pinMode (PIN_CHARGING_CURRENT, OUTPUT); // battery charging current
  digitalWrite(PIN_CHARGING_CURRENT, HIGH); // Set to 50 mA

  pinMode(AfeEnablePin, OUTPUT);
  digitalWrite(AfeEnablePin, LOW);

  pinMode(deploymentSwitch, INPUT_PULLUP); // SPDT switch to GND when active
  pinMode(stagingSwitch, INPUT_PULLUP);

  flash.begin();

  // Set disk vendor id, product id and revision with string up to 8, 16, 4 characters respectively
  usb_msc.setID("YSZ_PH", "External Flash", "1.0");

  // Set callback
  usb_msc.setReadWriteCallback(msc_read_cb, msc_write_cb, msc_flush_cb);

  // Set disk size, block size should be 512 regardless of spi flash page size
  usb_msc.setCapacity(flash.size()/512, 512);

  // MSC is ready for read/write
  usb_msc.setUnitReady(true);
  
  usb_msc.begin();

  // USB_MSC must begin before starting serial
  Serial.begin(115200);
  while ( !Serial ) delay(100); // wait for UART connection (For debug/testing only)

  // State Selector

  // First, check if USB connected
  if (NRF_POWER->USBREGSTATUS & POWER_USBREGSTATUS_VBUSDETECT_Msk) {
    Serial.println("USB connected, entering Charging State");
    runChargingState (); // Check if data connection is active inside here using TinyUSBDevice.mounted()
  } 
  else if (!digitalRead(deploymentSwitch)) {
    Serial.println("Entering Deployment State");
    runDeploymentState ();
  }
  else if (!digitalRead(stagingSwitch)) {
    Serial.println("Entering Staging State");
    // runStagingState();
  }
  else {
    Serial.println("Entering Testing State");
    // Fall through to testing state
  }


}

void loop() {
  // Do nothing
  // All loops managed in respective state
}
