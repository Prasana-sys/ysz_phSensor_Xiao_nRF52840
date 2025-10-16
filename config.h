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

// Default parameters for measurement readings
bool continuousScanningDeployment = 0; // 0 = Limited measurements, 1 = Continuous measurements, Setting for Deployment Mode Only
uint32_t numberMeasurementsDeployment = 10;
uint32_t numberMeasurementsPreDeployment = 10;
uint8_t numberSamplesPerHour = 1; // MAX = 60 (Arbitrary Limit), 60 samples per hour = 1 sample per minute
uint8_t sampleIntervalPreDeployment = 30; // in seconds, Setting for Pre-Deployment Mode Only

#endif // CONFIG_H