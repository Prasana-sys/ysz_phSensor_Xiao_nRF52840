#include "globals.h"

// Define storage transport
#if defined(CUSTOM_CS) && defined(CUSTOM_SPI)
  Adafruit_FlashTransport_SPI flashTransport(CUSTOM_CS, CUSTOM_SPI);
#elif defined(ARDUINO_ARCH_ESP32)
  Adafruit_FlashTransport_ESP32 flashTransport;
#else
  #if defined(EXTERNAL_FLASH_USE_QSPI)
    Adafruit_FlashTransport_QSPI flashTransport;
  #elif defined(EXTERNAL_FLASH_USE_SPI)
    Adafruit_FlashTransport_SPI flashTransport(EXTERNAL_FLASH_USE_CS, EXTERNAL_FLASH_USE_SPI);
  #endif
#endif

Adafruit_SPIFlash flash(&flashTransport);

Adafruit_USBD_MSC usb_msc;
FatFileSystem fatfs;
FatFile fat_root;
FatFile fat_file;
bool fat_fs_changed = false;