#ifndef GLOBALS_H
#define GLOBALS_H

#include "Adafruit_SPIFlash.h"
#include "Adafruit_TinyUSB.h"
#include "SdFat.h"

// const uint8_t chargeLED = 23; // P0_17 = 17  D23   GREEN CHARGE LED

// Uncomment to run example with custom SPI and SS e.g with FRAM breakout
// #define CUSTOM_CS   A5
// #define CUSTOM_SPI  SPI

#if defined(CUSTOM_CS) && defined(CUSTOM_SPI)
  extern Adafruit_FlashTransport_SPI flashTransport;

#elif defined(ARDUINO_ARCH_ESP32)
  // ESP32 use same flash device that store code.
  // Therefore there is no need to specify the SPI and SS
  extern Adafruit_FlashTransport_ESP32 flashTransport;

#else
  // On-board external flash (QSPI or SPI) macros should already
  // defined in your board variant if supported
  // - EXTERNAL_FLASH_USE_QSPI
  // - EXTERNAL_FLASH_USE_CS/EXTERNAL_FLASH_USE_SPI
  #if defined(EXTERNAL_FLASH_USE_QSPI)
    extern Adafruit_FlashTransport_QSPI flashTransport;

  #elif defined(EXTERNAL_FLASH_USE_SPI)
    extern Adafruit_FlashTransport_SPI flashTransport;

  #else
    #error No QSPI/SPI flash are defined on your board variant.h !
  #endif
#endif

// No need for this as internal library files should have this device to use as default
// static const SPIFlash_Device_t my_flash_devices[] = {
//     P25Q16H,
// };

// const int flashDevices = 1;

extern Adafruit_SPIFlash flash;

// USB Mass Storage object
extern Adafruit_USBD_MSC usb_msc;

// file system object from SdFat
extern FatFileSystem fatfs;

extern FatFile fat_root;
extern FatFile fat_file;

// Set to true when PC write to flash
extern bool fat_fs_changed;

#endif // GLOBALS_H