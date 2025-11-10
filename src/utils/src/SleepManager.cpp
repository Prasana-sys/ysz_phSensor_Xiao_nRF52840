#include "SleepManager.h"

#include "../../core/src/globals.h"
#include "../../core/src/config.h"

SPIClass SPI_2(NRF_SPIM2, PIN_QSPI_IO1, PIN_QSPI_SCK, PIN_QSPI_IO0);

void lightSleep(uint32_t sleepDuration) {
  Serial.println("Entering light sleep mode...");

  digitalWrite(afeEnablePin, LOW); // Turn off AFE to save power

  // When 4.2V, which is the maximum battery voltage, is applied, the voltage at D0/P0.31 has a margin of more than 100mV to VDD+0.3V.
  // TLDR: Safe to set HIGH
  digitalWrite(VBAT_ENABLE, HIGH); // Set high to stop battery read
  
  fat_root.close();
  fatfs.end();
  flash.end();

  pinMode(PIN_QSPI_CS, OUTPUT);   // CS#
  pinMode(PIN_QSPI_IO2, OUTPUT);  // WP#
  pinMode(PIN_QSPI_IO3, OUTPUT);  // HOLD#
  digitalWrite(PIN_QSPI_CS, HIGH);
  digitalWrite(PIN_QSPI_IO2, HIGH);
  digitalWrite(PIN_QSPI_IO3, HIGH);

  SPI_2.begin();
  digitalWrite(PIN_QSPI_CS, LOW);
  SPI_2.transfer(0xB9);           // 0xB9 enter External Flash Deep Power-down
  digitalWrite(PIN_QSPI_CS, HIGH);
  SPI_2.end();

  // flashTransport.runCommand(0xB9); // Enter Deep Power-down

  // digitalWrite(LED_GREEN, LOW); // Turn on GREEN LED before sleep

  delay((sleepDuration * 1000) - 1000); // Sleep for specified duration in seconds minus 1 second for wake-up process

  // digitalWrite(LED_GREEN, HIGH); // Turn off GREEN LED after wake-up

  SPI_2.begin();
  digitalWrite(PIN_QSPI_CS, LOW);
  SPI_2.transfer(0xAB);           // 0xAB release Deep Power-down
  digitalWrite(PIN_QSPI_CS, HIGH);
  SPI_2.end();

  // flashTransport.runCommand(0xAB); // Release from Deep Power-down

  digitalWrite(afeEnablePin, HIGH); // Turn AFE back on

  flash.begin(my_flash_devices, flashDevices);
  fatfs.begin(&flash);
  Serial.println("Mounted filesystem!");

  if ( !fat_root.open("/") ){
    Serial.println("open root failed");
    digitalWrite(LED_RED, LOW);
    while(1);
  }

  digitalWrite(VBAT_ENABLE, LOW); // Set low to enable battery reads

  Serial.println("Woke up from light sleep mode.");

  delay(1000); // Allow some time for AFE system to stabilize after wake-up
}

void deepSleep() {
  Serial.println("Entering deep sleep mode...");

  digitalWrite(afeEnablePin, LOW); // Turn off AFE to save power

  digitalWrite(VBAT_ENABLE, HIGH); // Set high to stop battery read

  fat_root.close();
  fatfs.end();
  flash.end();

  pinMode(PIN_QSPI_CS, OUTPUT);   // CS#
  pinMode(PIN_QSPI_IO2, OUTPUT);  // WP#
  pinMode(PIN_QSPI_IO3, OUTPUT);  // HOLD#
  digitalWrite(PIN_QSPI_CS, HIGH);
  digitalWrite(PIN_QSPI_IO2, HIGH);
  digitalWrite(PIN_QSPI_IO3, HIGH);

  SPI_2.begin();
  digitalWrite(PIN_QSPI_CS, LOW);
  SPI_2.transfer(0xB9);           // 0xB9 enter External Flash Deep Power-down
  digitalWrite(PIN_QSPI_CS, HIGH);
  SPI_2.end();

  sd_power_system_off(); // Will never wake-up from here except external reset
  NRF_POWER->SYSTEMOFF = 1;

  Serial.println("Woke up from deep sleep mode."); // Should never reach here
}