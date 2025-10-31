#include "SleepManager.h"

#include "../../../globals.h"

SPIClass SPI_2(NRF_SPIM2, PIN_QSPI_IO1, PIN_QSPI_SCK, PIN_QSPI_IO0);

void lightSleep(uint32_t sleepDuration) {
  Serial.println("Entering light sleep mode...");
  
  fat_root.close();
  fatfs.end();
  flash.end();

  pinMode(PIN_QSPI_CS, OUTPUT);   // CS#
  pinMode(PIN_QSPI_IO2, OUTPUT);  // WP#
  pinMode(PIN_QSPI_IO3, OUTPUT);  // HOLD#

  SPI_2.begin();
  digitalWrite(PIN_QSPI_CS, LOW);
  SPI_2.transfer(0xB9);           // 0xB9 enter External Flash Deep Power-down
  digitalWrite(PIN_QSPI_CS, HIGH);
  SPI_2.end();

  delay(sleepDuration * 1000); // Sleep for specified duration in seconds

  SPI_2.begin();
  digitalWrite(PIN_QSPI_CS, LOW);
  SPI_2.transfer(0xAB);           // 0xAB release Deep Power-down
  digitalWrite(PIN_QSPI_CS, HIGH);
  SPI_2.end();

  flash.begin(my_flash_devices, flashDevices);
  fatfs.begin(&flash);
  Serial.println("Mounted filesystem!");

  if ( !fat_root.open("/") ){
    Serial.println("open root failed");
    while(1);
  }

  Serial.println("Woke up from light sleep mode.");
}

void deepSleep() {
  Serial.println("Entering deep sleep mode...");

  fat_root.close();
  fatfs.end();
  flash.end();

  pinMode(PIN_QSPI_CS, OUTPUT);   // CS#
  pinMode(PIN_QSPI_IO2, OUTPUT);  // WP#
  pinMode(PIN_QSPI_IO3, OUTPUT);  // HOLD#

  SPI_2.begin();
  digitalWrite(PIN_QSPI_CS, LOW);
  SPI_2.transfer(0xB9);           // 0xB9 enter External Flash Deep Power-down
  digitalWrite(PIN_QSPI_CS, HIGH);
  SPI_2.end();

  sd_power_system_off(); // Will never wake-up from here except external reset
  NRF_POWER->SYSTEMOFF = 1;

  Serial.println("Woke up from deep sleep mode."); // Should never reach here
}