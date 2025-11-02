#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define DEFAULT_CONTINUOUS_SCANNING_DEPLOYMENT 0
#define DEFAULT_NUMBER_MEASUREMENTS_DEPLOYMENT 24
#define DEFAULT_NUMBER_MEASUREMENTS_PRE_DEPLOYMENT 10
#define DEFAULT_SAMPLE_INTERVAL_DEPLOYMENT 3600
#define DEFAULT_SAMPLE_INTERVAL_PRE_DEPLOYMENT 10

extern const uint8_t pHSensorADC;

extern float standardCellPotential; // E0 in Nernst equation

// BAT Voltage read pin at Pin 32 PIN_VBAT
// Put VBAT_ENABLE to output sink (low) to read PIN_VBAT
extern const uint8_t chargeLED; // P0_17 = 17  D23   GREEN CHARGE LED
extern const float BATT_LOW_VOLTAGE;

extern const uint8_t afeEnablePin;

extern const float VREF;
extern const int ADC_MAX;

extern const uint8_t deploymentSwitch;

// Default parameters for measurement readings
extern bool continuousScanningDeployment; // 0 = Limited measurements, 1 = Continuous measurements, Setting for Deployment Mode Only
extern uint32_t numberMeasurementsDeployment;
extern uint32_t numberMeasurementsPreDeployment;
extern uint32_t sampleIntervalDeployment; // in seconds, Setting for Deployment Mode Only
extern uint8_t sampleIntervalPreDeployment; // in seconds, Setting for Pre-Deployment Mode Only

#endif // CONFIG_H