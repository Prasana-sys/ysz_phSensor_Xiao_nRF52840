#ifndef GLOBALS_H
#define GLOBALS_H

#include "Adafruit_SPIFlash.h"
#include "Adafruit_TinyUSB.h"
#include "SdFat.h"
#include "bluefruit.h"

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
    extern Adafruit_FlashTransport_QSPI flashTransport; // This is what is being used in the project

  #elif defined(EXTERNAL_FLASH_USE_SPI)
    extern Adafruit_FlashTransport_SPI flashTransport;

  #else
    #error No QSPI/SPI flash are defined on your board variant.h !
  #endif
#endif

// Flash device definition for P25Q16H
#define P25Q16H                                                                \
  {                                                                            \
    .total_size = (1 << 21), /* 2 MiB */                                       \
        .start_up_time_us = 5000, .manufacturer_id = 0x85,                     \
    .memory_type = 0x60, .capacity = 0x15, .max_clock_speed_mhz = 104,         \
    .quad_enable_bit_mask = 0x02, .has_sector_protection = false,              \
    .supports_fast_read = true, .supports_qspi = true,                         \
    .supports_qspi_writes = true, .write_status_register_split = false,        \
    .single_status_byte = false, .is_fram = false,                             \
  }

extern const SPIFlash_Device_t my_flash_devices[];

extern const int flashDevices;

extern Adafruit_SPIFlash flash;

// USB Mass Storage object
extern Adafruit_USBD_MSC usb_msc;

// file system object from SdFat
extern FatFileSystem fatfs;

extern FatFile fat_root;
extern FatFile fat_file;

// Set to true when PC write to flash
extern bool fat_fs_changed;

extern BLEService pms;
extern BLECharacteristic pmsLogEntry;

extern BLEService cs;
extern BLECharacteristic csCommand;
extern BLECharacteristic csContScanDeploy;
extern BLECharacteristic csNumMeasDeploy;
extern BLECharacteristic csNumMeasPreDeploy;
extern BLECharacteristic csSampleIntervalDeploy;
extern BLECharacteristic csSampleIntervalPreDeploy;

#endif // GLOBALS_H