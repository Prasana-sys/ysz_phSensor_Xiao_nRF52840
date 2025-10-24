#include "config.h"

const uint8_t pHSensorADC = A0;

const uint8_t afeEnablePin = D7;

const uint8_t chargeLED = 23;
const float BATT_LOW_VOLTAGE = 0.0; // Will need adjustment

const float VREF = 3.6;
const int ADC_MAX = (1 << ADC_RESOLUTION) - 1;

const uint8_t deploymentSwitch = D1;
const uint8_t stagingSwitch = D2;

bool continuousScanningDeployment = 1; // Default = 0
uint32_t numberMeasurementsDeployment = 11; // Default = 10
uint32_t numberMeasurementsPreDeployment = 12; // Default = 10
uint32_t sampleIntervalDeployment = 60; // Default = 3600
uint8_t sampleIntervalPreDeployment = 35; // Default = 30

// bool continuousScanningDeployment = 0; // Default = 0
// uint32_t numberMeasurementsDeployment = 10; // Default = 10
// uint32_t numberMeasurementsPreDeployment = 10; // Default = 10
// uint32_t sampleIntervalDeployment = 30; // Default = 3600
// uint8_t sampleIntervalPreDeployment = 30; // Default = 30