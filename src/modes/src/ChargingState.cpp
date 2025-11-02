#include "ChargingState.h"

#include "Adafruit_TinyUSB.h"
#include "../../core/src/globals.h"

void runChargingState() {
  // Always charging when USB connected to power source (Power bank or wall outlet)

  // TODO: Green Charge LED should be on automatically when USB connected

  if (TinyUSBDevice.mounted()) {
    Serial.println("TinyUSB Mass Storage External Flash active");
  }

  while (1) { // Infinite loop, only leaves state upon power reset

    if (TinyUSBDevice.mounted()) {
      if (fat_fs_changed) {
        fat_fs_changed = false;

        if (!fat_root.open("/")) {
          Serial.println("open root failed");
          return;
        }

        Serial.println("Flash contents:");

        // Open next file in root.
        // Warning, openNext starts at the current directory position
        // so a rewind of the directory may be required.
        while (fat_file.openNext(&fat_root, O_RDONLY)) {
          fat_file.printFileSize(&Serial);
          Serial.write(' ');
          fat_file.printName(&Serial);
          if (fat_file.isDir()) {
            // Indicate a directory.
            Serial.write('/');
          }
          Serial.println();
          fat_file.close();
        }

        fat_root.close();

        Serial.println();
        delay(1000); // refresh every 0.5 second
      }
    }
    else {
      delay(1000); // wait for mount
    }

  }
}