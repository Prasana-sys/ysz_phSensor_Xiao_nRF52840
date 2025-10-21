#include "config.h"

const uint8_t pHSensorADC = A0;

const uint8_t AfeEnablePin = D7;

const float VREF = 3.6;
const int ADC_MAX = (1 << ADC_RESOLUTION) - 1;

const uint8_t deploymentSwitch = D1;
const uint8_t stagingSwitch = D2;

bool continuousScanningDeployment = 1; // Default = 0
uint32_t numberMeasurementsDeployment = 11; // Default = 10
uint32_t numberMeasurementsPreDeployment = 12; // Default = 10
uint8_t numberSamplesPerHour = 2; // Default = 1
uint8_t sampleIntervalPreDeployment = 35; // Default = 30

// bool continuousScanningDeployment = 0; // Default = 0
// uint32_t numberMeasurementsDeployment = 10; // Default = 10
// uint32_t numberMeasurementsPreDeployment = 10; // Default = 10
// uint8_t numberSamplesPerHour = 1; // Default = 1
// uint8_t sampleIntervalPreDeployment = 30; // Default = 30