#ifndef GLOBALS_H
#define GLOBALS_H

#include "Adafruit_SPIFlash.h"
#include "Adafruit_TinyUSB.h"
#include "SdFat.h"

// const uint8_t chargeLED = 23; // P0_17 = 17  D23   GREEN CHARGE LED

Adafruit_FlashTransport_QSPI flashTransport;
Adafruit_SPIFlash flash(&flashTransport);

// USB Mass Storage object
Adafruit_USBD_MSC usb_msc;

// file system object from SdFat
FatFileSystem fatfs;

#endif // GLOBALS_H