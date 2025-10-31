#include "TemperatureSensor.h"

#include <Arduino.h>
#include "nrf_temp.h"
#include "bluefruit.h"

float getTemperature() {
  // Start temperature measurement
  NRF_TEMP->TASKS_START = 1;

  // Wait for data ready
  while (NRF_TEMP->EVENTS_DATARDY == 0);
  NRF_TEMP->EVENTS_DATARDY = 0;  // Clear event flag

  // Read and convert to °C (value is in 0.25°C units)
  int32_t temp_raw = NRF_TEMP->TEMP;
  float temp_c = temp_raw / 4.0f;

  // Stop the sensor to save power
  NRF_TEMP->TASKS_STOP = 1;

  return temp_c;
}

void getTemperature(float& dieTemp) {
  // Read and convert to °C (value is in 0.25°C units)
  if (Bluefruit.connected()) {
    // Use SoftDevice API if connected
    int32_t temp_raw;
    sd_temp_get(&temp_raw);
    dieTemp = temp_raw / 4.0f;
    return;
  }
  else {
    // Start temperature measurement
    NRF_TEMP->TASKS_START = 1;

    // Wait for data ready
    while (NRF_TEMP->EVENTS_DATARDY == 0);
    NRF_TEMP->EVENTS_DATARDY = 0;  // Clear event flag

    // Read and convert to °C (value is in 0.25°C units)
    int32_t temp_raw = NRF_TEMP->TEMP;
    dieTemp = temp_raw / 4.0f;

    // Stop the sensor to save power
    NRF_TEMP->TASKS_STOP = 1;
  }
}