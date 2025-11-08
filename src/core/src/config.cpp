#include "config.h"

const uint8_t pHSensorADC = A0;

float standardCellPotential = 0.0; // TODO: Will need adjustment based on calibration

const uint8_t afeEnablePin = D7;

const uint8_t chargeLED = 23; // LOW for charging, HIGH not charging
const float BATT_LOW_VOLTAGE = 0.0; //TODO: Will need adjustment, perhaps 3.35?

const float VREF = 3.6;
const int ADC_MAX = (1 << ADC_RESOLUTION) - 1;

const uint8_t deploymentSwitch = D1;
// const uint8_t stagingSwitch = D2;

bool continuousScanningDeployment = DEFAULT_CONTINUOUS_SCANNING_DEPLOYMENT; // Default = 0
uint32_t numberMeasurementsDeployment = DEFAULT_NUMBER_MEASUREMENTS_DEPLOYMENT; // Default = 24
uint32_t numberMeasurementsPreDeployment = DEFAULT_NUMBER_MEASUREMENTS_PRE_DEPLOYMENT; // Default = 10
uint32_t sampleIntervalDeployment = DEFAULT_SAMPLE_INTERVAL_DEPLOYMENT; // Default = 3600
uint8_t sampleIntervalPreDeployment = DEFAULT_SAMPLE_INTERVAL_PRE_DEPLOYMENT; // Default = 10

// bool continuousScanningDeployment = 0; // Default = 0
// uint32_t numberMeasurementsDeployment = 10; // Default = 10
// uint32_t numberMeasurementsPreDeployment = 10; // Default = 10
// uint32_t sampleIntervalDeployment = 30; // Default = 3600
// uint8_t sampleIntervalPreDeployment = 30; // Default = 30