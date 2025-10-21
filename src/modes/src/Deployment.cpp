#include "Deployment.h"

#include "../../utils/src/SettingsManager.h"
#include "../../../config.h"
#include "../../../globals.h"
#include "../../utils/src/MSCFlash.h"

void runDeploymentState () {
  // TEST CODE
  loadSettings();

  Serial.println("Current Configuration:");
  Serial.printf("continuousScanning = %d\n", continuousScanningDeployment);
  Serial.printf("numMeasurements = %lu\n", numberMeasurementsDeployment);
  Serial.printf("numMeasurementsPreDeployment = %lu\n", numberMeasurementsPreDeployment);
  Serial.printf("samplesPerHour = %d\n", numberSamplesPerHour);
  Serial.printf("sampleInterval = %d\n", sampleIntervalPreDeployment);

  continuousScanningDeployment = 0; // 0 = Limited measurements, 1 = Continuous measurements, Setting for Deployment Mode Only
  numberMeasurementsDeployment = 10;
  numberMeasurementsPreDeployment = 10;
  numberSamplesPerHour = 1; // MAX = 60 (Arbitrary Limit), 60 samples per hour = 1 sample per minute
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