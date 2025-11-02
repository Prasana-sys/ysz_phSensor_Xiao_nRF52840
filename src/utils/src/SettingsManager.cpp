#include "SettingsManager.h"

#include "../../core/src/config.h"
#include "../../core/src/globals.h"
// #include "../../utils/src/MSCFlash.h"

#include "tools.h"

#include <Arduino.h>

char buf[100]; // Internal buffer

void loadSettings() {
  if (!fat_file.open("/settings.ini", O_RDONLY)) {
    Serial.println("settings.ini not found, using defaults.");
    return;
  }

  while (fat_file.fgets(buf, sizeof(buf)) > 0) {
    char *s = trim(buf);
    if (*s == '#' || *s == ';' || *s == '\0') continue;

    char *eq = strchr(s, '=');
    if (!eq) continue;
    *eq = 0;
    char *key = trim(s);
    char *val = trim(eq + 1);

    if (!strcmp(key, "continuousScanningDeployment")) continuousScanningDeployment = atoi(val);
    else if (!strcmp(key, "numberMeasurementsDeployment")) numberMeasurementsDeployment = atol(val);
    else if (!strcmp(key, "numberMeasurementsPreDeployment")) numberMeasurementsPreDeployment = atol(val);
    else if (!strcmp(key, "sampleIntervalDeployment")) sampleIntervalDeployment = atol(val);
    else if (!strcmp(key, "sampleIntervalPreDeployment")) sampleIntervalPreDeployment = atoi(val);
  }

  fat_file.close();
  Serial.println("Settings loaded from Flash.");
}

void saveSettings() {
  // Remove old file
  if (fatfs.exists("/settings.ini")) fatfs.remove("/settings.ini");

  if (!fat_file.open("/settings.ini", FILE_WRITE)) {
    Serial.println("Failed to open settings.ini for writing!");
    return;
  }

  snprintf(buf, sizeof(buf), "continuousScanningDeployment=%d\n", continuousScanningDeployment);
  fat_file.write(buf);

  snprintf(buf, sizeof(buf), "numberMeasurementsDeployment=%lu\n", numberMeasurementsDeployment);
  fat_file.write(buf);

  snprintf(buf, sizeof(buf), "numberMeasurementsPreDeployment=%lu\n", numberMeasurementsPreDeployment);
  fat_file.write(buf);

  snprintf(buf, sizeof(buf), "sampleIntervalDeployment=%lu\n", sampleIntervalDeployment);
  fat_file.write(buf);

  snprintf(buf, sizeof(buf), "sampleIntervalPreDeployment=%d\n", sampleIntervalPreDeployment);
  fat_file.write(buf);

  if (!fat_file.sync()) digitalWrite(LED_RED, LOW);
  if (!fat_file.close()) digitalWrite(LED_RED, LOW);
  Serial.println("Settings saved to Flash.");
}

void testSettingsManager () {
  // TEST CODE
  loadSettings();

  Serial.println("Current Configuration:");
  Serial.printf("continuousScanning = %d\n", continuousScanningDeployment);
  Serial.printf("numMeasurements = %lu\n", numberMeasurementsDeployment);
  Serial.printf("numMeasurementsPreDeployment = %lu\n", numberMeasurementsPreDeployment);
  Serial.printf("sampleIntervalDeployment = %lu\n", sampleIntervalDeployment);
  Serial.printf("sampleInterval = %d\n", sampleIntervalPreDeployment);

  continuousScanningDeployment = 0; // 0 = Limited measurements, 1 = Continuous measurements, Setting for Deployment Mode Only
  numberMeasurementsDeployment = 10;
  numberMeasurementsPreDeployment = 10;
  sampleIntervalDeployment = 30; // in seconds, Setting for Deployment Mode Only
  sampleIntervalPreDeployment = 30; // in seconds, Setting for Pre-Deployment Mode Only

  /* When you write to the external flash through FatFs while the USB mass storage device (MSC) is mounted to your computer, there are two separate caching layers involved:

    FatFs write buffer (in your MCU’s memory)

    USB MSC cache + host OS buffer

    The critical detail:

    The OS (Windows/macOS/Linux) owns the disk when MSC is mounted, and it buffers writes internally.
    Your firmware’s writes only go into FatFs cache, which is not flushed to flash until a sync() and a USB transaction occurs after unmount.

    Because of this, the sequence of events is:

    fat_file.write() buffers data in RAM.

    fat_file.sync() pushes it to the flash driver cache, not necessarily to physical flash.

    If TinyUSBDevice.mounted() is still true (MSC active), the flash block might be locked by the host OS — your write gets deferred.

    The Serial.println("Settings saved to Flash."); line never executes because the USB/MSC stack is in a blocked transaction, waiting on an endpoint handshake or flash lock.

    After a reset, the USB connection resets too, the device reinitializes the MSC, and FatFs finally sees the pending data written — so you see the file appear on the second boot. 
  */
  // Setting unit not ready might not be required when saveSettings is actually used in Pre-Deployment and device doesnt have USB connected and device goes to shutoff anyway. 
  // usb_msc.setUnitReady(false);   // Tell host "disk is busy"
  // delay(50);                     // Give host time to release lock

  saveSettings();

  // msc_flush_cb();                // Make sure data is committed to flash

  // usb_msc.setUnitReady(true);    // Re-enable drive access to host

  while (1);
}