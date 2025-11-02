# YSZ pH Sensor XIAO nRF52840  

The XIAO nRF52840 in this project receives analog Data from the Analog Front End (AFE), which interfaces with pH and reference electrodes, saves the data to an external QSPI Flash memory and can later send the data wirelessly to a Webapp via BLE. 

[Link to webapp](https://prasana-sys.github.io/ysz_phSensor_Xiao_nRF52840/)  

## How to compile and use  
Download Arduino IDE 2.3.5 (or higher)  
Delete local SdFat library that comes pre-installed with Arduino IDE  
> Usually in /Documents/Arduino/libraries/SdFat  

Navigate to File > Preferences, and fill "Additional Boards Manager URLs" with the url below:  
https://files.seeedstudio.com/arduino/package_seeeduino_boards_index.json  

Open Boards Manager in Arduino IDE and install "Seeed nRF52 Boards" Board Support Package (BSP)  
Do not install the mbed-enabled BSP and if modifying code in this repo, only use the libraries provided in the non-mbed BSP!  

After installing the board package, navigate to Tools > Board and select "Seeed XIAO nRF52840".  

Do not modify file structure, i.e., maintain /src/MODULE_NAME/src/FILES_HERE for submodules to compile along with .ino file.

## State Diagrams and Flowcharts



## Useful links:

https://wiki.seeedstudio.com/XIAO_BLE/
https://github.com/Seeed-Studio/Adafruit_nRF52_Arduino
https://github.com/0hotpotman0/BLE_52840_Core

## OLD Compilation Instructions (DO NOT USE)  

Add this line of code to ```C:\Users\[USERNAME]\AppData\Local\Arduino15\packages\Seeeduino\hardware\nrf52\1.1.10\libraries\Adafruit_SPIFlash\src\flash_devices.h``` at the end

```// Settings for the Puya Semiconductor P25Q16H 2MiB QSPI flash.
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
```

https://github.com/adafruit/Adafruit_SPIFlash/blob/master/src/flash_devices.h

Also this line in ```C:\Users\[USERNAME]\AppData\Local\Arduino15\packages\Seeeduino\hardware\nrf52\1.1.10\libraries\Adafruit_SPIFlash\src\Adafruit_SPIFlashBase.cpp```

```
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

```