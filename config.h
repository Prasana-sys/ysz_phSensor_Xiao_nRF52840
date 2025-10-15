#ifndef CONFIG_H
#define CONFIG_H

const uint8_t pHSensorADC = A0;
// const uint8_t refGroundADC = A1;
// const uint8_t ref3v3ADC = A2;

// BAT Voltage read pin at Pin 32 PIN_VBAT
// Put VBAT_ENABLE to output sink (low) to read PIN_VBAT

// Get Die Temperature of nRF52840 but very inaccurate

const uint8_t AfeEnablePin = D7;

const float VREF = 3.6;
const int ADC_MAX = (1 << ADC_RESOLUTION) - 1;

const uint8_t deploymentSwitch = D1;
const uint8_t stagingSwitch = D2;


#endif // CONFIG_H