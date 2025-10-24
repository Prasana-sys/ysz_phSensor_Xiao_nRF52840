#include "phSensor.h"

#include "../../../config.h"
#include "TemperatureSensor.h"

float getpHValue() {
  
  // Just return voltage for now
  return getpHVoltage();
}

float getpHVoltage() {
  uint16_t Vadc = 0;
  float VpH;

  for (int i = 0; i < 16; i++) { // Averaging over 16 samples
    Vadc = Vadc + analogRead(pHSensorADC);
  }

  VpH = (Vadc/16.0) * (VREF / ADC_MAX);
  return VpH;
}

void getpHValue(float& pHVal, float& VpH, float& dieTemp) {
  
  // Just return voltage for now
  getTemperature(dieTemp);
  getpHVoltage(VpH);

  // Use Nernst equation later to convert voltage to pH
  pHVal = 0;

  return;
}

void getpHVoltage(float& VpH) {
  uint16_t Vadc = 0;

  for (int i = 0; i < 16; i++) { // Averaging over 16 samples
    Vadc = Vadc + analogRead(pHSensorADC);
  }

  VpH = (Vadc/16.0) * (VREF / ADC_MAX);
}