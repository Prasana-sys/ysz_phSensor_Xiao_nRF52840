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

const SPIFlash_Device_t my_flash_devices[] = {
    P25Q16H,
};

const int flashDevices = 1;

Adafruit_USBD_MSC usb_msc;
FatFileSystem fatfs;
FatFile fat_root;
FatFile fat_file;
bool fat_fs_changed = true; // // to print contents initially

const float universalGasConstant = 8.314; // J/(mol·K)
const uint32_t faradayConstant = 96485; // C/mol

/* GATT Services and Characteristics UUIDs

 * pH Measurement Service (PMS): 019A2890-2324-7D95-8F76-8DE7146B560E
 * Timestamp (uint32) (Indicate / Read) : 019A2891-2324-7D95-8F76-8DE7146B560E 
 * pH Value (float) (Indicate / Read) : 019A2892-2324-7D95-8F76-8DE7146B560E
 * pH Voltage (float) (Indicate / Read) : 019A2893-2324-7D95-8F76-8DE7146B560E
 * Die Temp (float) (Indicate / Read) : 019A2894-2324-7D95-8F76-8DE7146B560E
 * Battery Volt (float) (Indicate / Read) : 019A2895-2324-7D95-8F76-8DE7146B560E
 * Log Entry (247 bytes) (Indicate) : 019A2896-2324-7D95-8F76-8DE7146B560E
 * 
 * Config Service (CS): 019A2990-2324-7D95-8F76-8DE7146B560E
 * Command (uint8) (Write) : 019A2991-2324-7D95-8F76-8DE7146B560E
 * continuousScanningDeployment (uint8) (Read / Write) : 019A2992-2324-7D95-8F76-8DE7146B560E
 * numberMeasurementsDeployment (uint32) (Read / Write) : 019A2993-2324-7D95-8F76-8DE7146B560E
 * numberMeasurementsPreDeployment (uint32) (Read / Write) : 019A2994-2324-7D95-8F76-8DE7146B560E
 * sampleIntervalDeployment (uint32) (Read / Write) : 019A2995-2324-7D95-8F76-8DE7146B560E
 * sampleIntervalPreDeployment (uint8) (Read / Write) : 019A2996-2324-7D95-8F76-8DE7146B560E

*/

// pH Measurement Service (PMS)
const uint8_t PMS_UUID_SERVICE[] = 
{
  0x0E, 0x56, 0x6B, 0x14, 0xE7, 0x8D, 0x76, 0x8F, 
  0x95, 0x7D, 0x24, 0x23, 0x90, 0x28, 0x9A, 0x01
};

const uint8_t PMS_UUID_TIMESTAMP_CHAR[] = 
{ 
  0x0E, 0x56, 0x6B, 0x14, 0xE7, 0x8D, 0x76, 0x8F, 
  0x95, 0x7D, 0x24, 0x23, 0x91, 0x28, 0x9A, 0x01 
};

const uint8_t PMS_UUID_PHVALUE_CHAR[] = 
{ 
  0x0E, 0x56, 0x6B, 0x14, 0xE7, 0x8D, 0x76, 0x8F, 
  0x95, 0x7D, 0x24, 0x23, 0x92, 0x28, 0x9A, 0x01 
};

const uint8_t PMS_UUID_PHVOLTAGE_CHAR[] = 
{ 
  0x0E, 0x56, 0x6B, 0x14, 0xE7, 0x8D, 0x76, 0x8F, 
  0x95, 0x7D, 0x24, 0x23, 0x93, 0x28, 0x9A, 0x01 
};

const uint8_t PMS_UUID_DIETEMP_CHAR[] = 
{ 
  0x0E, 0x56, 0x6B, 0x14, 0xE7, 0x8D, 0x76, 0x8F, 
  0x95, 0x7D, 0x24, 0x23, 0x94, 0x28, 0x9A, 0x01 
};

const uint8_t PMS_UUID_BATTVOLT_CHAR[] = 
{ 
  0x0E, 0x56, 0x6B, 0x14, 0xE7, 0x8D, 0x76, 0x8F, 
  0x95, 0x7D, 0x24, 0x23, 0x95, 0x28, 0x9A, 0x01 
};

const uint8_t PMS_UUID_LOGENTRY_CHAR[] = 
{ 
  0x0E, 0x56, 0x6B, 0x14, 0xE7, 0x8D, 0x76, 0x8F, 
  0x95, 0x7D, 0x24, 0x23, 0x96, 0x28, 0x9A, 0x01 
};

BLEService pms(PMS_UUID_SERVICE);
BLECharacteristic pmsLogEntry(PMS_UUID_LOGENTRY_CHAR);
// Other characteristics are not implemented but reserved

// Config Service (CS)
const uint8_t CS_UUID_SERVICE[] =
{ 
  0x0E, 0x56, 0x6B, 0x14, 0xE7, 0x8D, 0x76, 0x8F, 
  0x95, 0x7D, 0x24, 0x23, 0x90, 0x29, 0x9A, 0x01 
};

const uint8_t CS_UUID_COMMAND_CHAR[] = 
{ 
  0x0E, 0x56, 0x6B, 0x14, 0xE7, 0x8D, 0x76, 0x8F, 
  0x95, 0x7D, 0x24, 0x23, 0x91, 0x29, 0x9A, 0x01 
};

const uint8_t CS_UUID_CONTSCANDEPLOY_CHAR[] = 
{ 
  0x0E, 0x56, 0x6B, 0x14, 0xE7, 0x8D, 0x76, 0x8F, 
  0x95, 0x7D, 0x24, 0x23, 0x92, 0x29, 0x9A, 0x01 
};

const uint8_t CS_UUID_NUMMEASDEPLOY_CHAR[] = 
{ 
  0x0E, 0x56, 0x6B, 0x14, 0xE7, 0x8D, 0x76, 0x8F, 
  0x95, 0x7D, 0x24, 0x23, 0x93, 0x29, 0x9A, 0x01 
};

const uint8_t CS_UUID_NUMMEASPREDEPLOY_CHAR[] = 
{ 
  0x0E, 0x56, 0x6B, 0x14, 0xE7, 0x8D, 0x76, 0x8F, 
  0x95, 0x7D, 0x24, 0x23, 0x94, 0x29, 0x9A, 0x01 
};

const uint8_t CS_UUID_SAMPLEINTERVALDEPLOY_CHAR[] = 
{ 
  0x0E, 0x56, 0x6B, 0x14, 0xE7, 0x8D, 0x76, 0x8F, 
  0x95, 0x7D, 0x24, 0x23, 0x95, 0x29, 0x9A, 0x01 
};

const uint8_t CS_UUID_SAMPLEINTERVALPREDEPLOY_CHAR[] = 
{ 
  0x0E, 0x56, 0x6B, 0x14, 0xE7, 0x8D, 0x76, 0x8F, 
  0x95, 0x7D, 0x24, 0x23, 0x96, 0x29, 0x9A, 0x01 
};

BLEService cs(CS_UUID_SERVICE);
BLECharacteristic csCommand(CS_UUID_COMMAND_CHAR);
BLECharacteristic csContScanDeploy(CS_UUID_CONTSCANDEPLOY_CHAR);
BLECharacteristic csNumMeasDeploy(CS_UUID_NUMMEASDEPLOY_CHAR);
BLECharacteristic csNumMeasPreDeploy(CS_UUID_NUMMEASPREDEPLOY_CHAR);
BLECharacteristic csSampleIntervalDeploy(CS_UUID_SAMPLEINTERVALDEPLOY_CHAR);
BLECharacteristic csSampleIntervalPreDeploy(CS_UUID_SAMPLEINTERVALPREDEPLOY_CHAR);