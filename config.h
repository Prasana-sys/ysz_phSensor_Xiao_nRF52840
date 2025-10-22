#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

extern const uint8_t pHSensorADC;
// const uint8_t refGroundADC = A1;
// const uint8_t ref3v3ADC = A2;

// BAT Voltage read pin at Pin 32 PIN_VBAT
// Put VBAT_ENABLE to output sink (low) to read PIN_VBAT
extern const uint8_t chargeLED; // P0_17 = 17  D23   GREEN CHARGE LED

// Get Die Temperature of nRF52840 but very inaccurate

extern const uint8_t afeEnablePin;

extern const float VREF;
extern const int ADC_MAX;

extern const uint8_t deploymentSwitch;
extern const uint8_t stagingSwitch;

// Default parameters for measurement readings
extern bool continuousScanningDeployment; // 0 = Limited measurements, 1 = Continuous measurements, Setting for Deployment Mode Only
extern uint32_t numberMeasurementsDeployment;
extern uint32_t numberMeasurementsPreDeployment;
extern uint8_t numberSamplesPerHour; // MAX = 60 (Arbitrary Limit), 60 samples per hour = 1 sample per minute
extern uint8_t sampleIntervalPreDeployment; // in seconds, Setting for Pre-Deployment Mode Only

#endif // CONFIG_H