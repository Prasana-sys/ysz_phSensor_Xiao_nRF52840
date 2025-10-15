Delete local SdFat library /Documents/Arduino/libraries/SdFat
Only use libraries provided in BSP!!!

Add this line of code to C:\Users\[USERNAME]\AppData\Local\Arduino15\packages\Seeeduino\hardware\nrf52\1.1.10\libraries\Adafruit_SPIFlash\src\flash_devices.h at the end

// Settings for the Puya Semiconductor P25Q16H 2MiB QSPI flash.
// Datasheet:
// https://www.puyasemi.com/uploadfiles/2021/12/202112201130233023.pdf
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

https://github.com/adafruit/Adafruit_SPIFlash/blob/master/src/flash_devices.h

Also this line in C:\Users\[USERNAME]\AppData\Local\Arduino15\packages\Seeeduino\hardware\nrf52\1.1.10\libraries\Adafruit_SPIFlash\src\Adafruit_SPIFlashBase.cpp

/// List of all possible flash devices used by Adafruit boards
static const SPIFlash_Device_t possible_devices[] = {
    // Main devices used in current Adafruit products
    GD25Q16C, GD25Q32C, GD25Q64C, S25FL116K, S25FL216K,

    // Only a handful of production run
    W25Q16FW, W25Q64JV_IQ,

    // Fujitsu FRAM
    MB85RS64V, MB85RS1MT, MB85RS2MTA, MB85RS4MT,

    // Other common flash devices
    W25Q16JV_IQ, W25Q32JV_IQ, AT25SF041, AT25DF081A, P25Q16H};

Useful links:

https://github.com/0hotpotman0/BLE_52840_Core/blob/main/libraries/Bluefruit52Lib/examples/Hardware/deep_Sleep/deep_Sleep.ino