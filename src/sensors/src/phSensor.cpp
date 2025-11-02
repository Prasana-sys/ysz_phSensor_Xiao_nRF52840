#include "phSensor.h"

#include "../../core/src/config.h"
#include "../../core/src/globals.h"
#include "TemperatureSensor.h"
#include "../../utils/src/tools.h"

void getpHValue(float& pHVal, float& VpH, float& dieTemp) {

  getTemperature(dieTemp);
  getpHVoltage(VpH);

  pHVal = (standardCellPotential - VpH) / ( (2.303 * universalGasConstant * dieTemp) / (faradayConstant) );

  return;
}

void getpHVoltage(float& VpH) {
  uint16_t Vadc = 0;

  for (int i = 0; i < 16; i++) { // Averaging over 16 samples
    Vadc = Vadc + analogRead(pHSensorADC);
  }

  // Vadc += safeAnalogRead(pHSensorADC);

  VpH = (Vadc/16.0) * (VREF / ADC_MAX);
}