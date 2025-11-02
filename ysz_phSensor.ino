// Delete SdFat /Documents/Arduino/libraries/SdFat
// Only use libraries provided in BSP!!!

#include <Arduino.h>

#include <Adafruit_TinyUSB.h> // included for Serial to compile

#include "src/core/src/config.h"
#include "src/core/src/globals.h"

#include "src/utils/src/MSCFlash.h" // read/write/flush callbacks for MSC
#include "src/modes/src/ChargingState.h"
#include "src/modes/src/Deployment.h"
#include "src/modes/src/BLEReadout.h"
#include "src/modes/src/PreDeployment.h"

#define DEBUG_MODE 0 // 0/1/2/3 - Default, Deployment, BLE Readout, Pre-Deployment

void setup() {

  // TODO: Color code each mode with LED colors
  pinMode(LED_RED, OUTPUT); // LED_BUILTIN
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  // Set analog read resolution
  analogReadResolution(ADC_RESOLUTION);
  // initialise ADC wireing_analog_nRF52.c:73
  analogReference(AR_DEFAULT);        // default 0.6V*6=3.6V  wireing_analog_nRF52.c:73
  analogSampleTime(40);             // default 3uS　3,5,10,15,20,40

  // Enable DC-DC converter
  NRF_POWER->DCDCEN = 1;            // Enable DC/DC converter for REG1 stage, switched to internal 1.3 V core voltage
                                    // POWER SAVINGS!!!

  // Bluefruit.begin(); // Need to use SoftDevice when Bluefruit begins and no direct register access allowed
  // sd_power_system_off() in deep sleep for full system shutdown
  // sd_temp_get() to get temperature when Bluefruit begins

  pinMode(chargeLED, INPUT_PULLUP);       // sleep current increases significantly without pull-up  

  pinMode(VBAT_ENABLE, OUTPUT);
  digitalWrite(VBAT_ENABLE, LOW); // Set low to read battery voltage

  pinMode (PIN_CHARGING_CURRENT, OUTPUT); // battery charging current
  digitalWrite(PIN_CHARGING_CURRENT, HIGH); // Set to 50 mA

  pinMode (PIN_VBAT, INPUT);
  pinMode (pHSensorADC, INPUT);

  pinMode(afeEnablePin, OUTPUT);
  digitalWrite(afeEnablePin, LOW);

  pinMode(deploymentSwitch, INPUT_PULLUP); // SPST switch to GND when active

  flash.begin(my_flash_devices, flashDevices);
  
  if (!DEBUG_MODE){
    // Set disk vendor id, product id and revision with string up to 8, 16, 4 characters respectively
    usb_msc.setID("YSZ_PH", "External Flash", "1.0");

    // Set callback
    usb_msc.setReadWriteCallback(msc_read_cb, msc_write_cb, msc_flush_cb);

    // Set disk size, block size should be 512 regardless of spi flash page size
    usb_msc.setCapacity(flash.size()/512, 512);

    // MSC is ready for read/write
    usb_msc.setUnitReady(true);
    
    usb_msc.begin();
  }

  // USB_MSC must begin before starting serial
  Serial.begin(115200);
  if (DEBUG_MODE) {
    while ( !Serial ) delay(100); // wait for UART connection (For debug/testing only)
  }

  Serial.print("Reset reason: 0x");
  Serial.println(NRF_POWER->RESETREAS, HEX);
  NRF_POWER->RESETREAS = 0xFFFFFFFF; // clear flags

  // Init file system on the flash
  fatfs.begin(&flash);

  if ( !fat_root.open("/") ){
    Serial.println("open root failed");
    digitalWrite(LED_RED, LOW);
    while(1);
  }

  fat_root.close();

  // STATE SELECTOR

  // First, check if USB connected
  if ((NRF_POWER->USBREGSTATUS & POWER_USBREGSTATUS_VBUSDETECT_Msk) && !DEBUG_MODE) {
    Serial.println("USB connected, entering Charging State");
    runChargingState(); // Check if data connection is active inside here using TinyUSBDevice.mounted()
  } 
  else if (!digitalRead(deploymentSwitch) || DEBUG_MODE == 1) {
    Serial.println("Entering Deployment State");
    runDeploymentState();
  }
  // else if (!digitalRead(stagingSwitch) || DEBUG_MODE) {
  else {
    Serial.println("Entering Staging State");

    // Check if log.csv exists
    if (!fatfs.exists("/log.csv") || DEBUG_MODE == 3) {
      Serial.println("log.csv not found on flash.");
      Serial.println("Entering Pre-Deployment State");
      setupPreDeploymentState();
    }
    else {
      Serial.println("log.csv found on flash!");
      Serial.println("Entering BLE Readout State");
      runBLEReadoutState();
    }
  }
  // else {
  //   Serial.println("Entering Testing State");
  //   // Fall through to testing state
  // }

}

void loop() {
  // Do nothing
  // All loops managed in respective state
}
