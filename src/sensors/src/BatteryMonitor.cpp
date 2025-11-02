#include "BatteryMonitor.h"

#include "../../core/src/config.h"
#include "../../utils/src/tools.h"

float getBattVoltage() {
  uint16_t Vadc = 0;
  float Vbatt;

  // for (int i = 0; i < 16; i++) { // Averaging over 16 samples
  //   Vadc = Vadc + analogRead(PIN_VBAT);
  // }

  // Vadc += safeAnalogRead(PIN_VBAT);
  
  Vbatt = (Vadc/16.0) * ((2.961 * VREF) / ADC_MAX); // Resistance ratio 2.961
  return Vbatt;
}

void getBattVoltage(float& Vbatt) {
  uint16_t Vadc = 0;

  for (int i = 0; i < 16; i++) { // Averaging over 16 samples
    Vadc += analogRead(PIN_VBAT);
  }

  // Vadc += safeAnalogRead(PIN_VBAT);

  Vbatt = (Vadc/16.0) * ((2.961 * VREF) / ADC_MAX); // Resistance ratio 2.961
}

// // PLC timing (for 60 Hz mains)
// const uint32_t PLC_1_US  = 16667; // 1 PLC = 16.67 ms
// const uint32_t PLC_10_US = PLC_1_US * 10; // 10 PLC ≈ 166.7 ms
// const uint8_t WIN_PER_SEC = 6; // 6 × (10 PLC) ≈ 1 second

// // --- average both ADC channels within one PLC window ---
// void read_plc_pair(uint32_t window_us,
//                    uint32_t &avg_raw_pH,
//                    uint32_t &avg_raw_off,
//                    uint32_t &count_out) {
//   uint32_t start = micros();
//   uint64_t total_pH = 0;
//   uint64_t total_off = 0;
//   uint32_t count = 0;

//   while ((micros() - start) < window_us) {
//     total_pH  += analogRead(pHSensorADC);
//     total_off += analogRead(offsetRefADC);
//     count++;
//   }

//   count_out   = count;
//   avg_raw_pH  = (count > 0) ? total_pH  / count : 0;
//   avg_raw_off = (count > 0) ? total_off / count : 0;
// }

// void read_battery_voltage() {
//   static uint32_t start_ms = millis();

//   uint32_t second_start = millis();
//   uint64_t sum_code_times_n_pH  = 0;
//   uint64_t sum_code_times_n_off = 0;
//   uint32_t sum_n = 0;

//   for (int i = 0; i < WIN_PER_SEC; i++) {
//     uint32_t n = 0;
//     uint32_t avg_raw_pH = 0;
//     uint32_t avg_raw_off = 0;
    
//     read_plc_pair(PLC_10_US, avg_raw_pH, avg_raw_off, n);

//     sum_code_times_n_pH  += (uint64_t)avg_raw_pH  * n;
//     sum_code_times_n_off += (uint64_t)avg_raw_off * n;
//     sum_n += n;
//   }

//   uint32_t avg_raw_pH  = (sum_n > 0) ? (sum_code_times_n_pH  / sum_n) : 0;
//   uint32_t avg_raw_off = (sum_n > 0) ? (sum_code_times_n_off / sum_n) : 0;

//   float volts_pH   = (avg_raw_pH  * VREF) / ADC_MAX;
//   float volts_off  = (avg_raw_off * VREF) / ADC_MAX;

//   float volts_corr = (avg_raw_pH) * (VREF / avg_raw_off);

//   // Effective samples/sec
//   uint32_t sps = (uint64_t)(sum_n * 1000000UL) / (PLC_10_US * WIN_PER_SEC);

//   // Timestamp since start
//   String stamp = fmt_hms(millis() - start_ms);

//   Serial.print(stamp); Serial.print(",");
//   // Serial.print(PLC_10_US); Serial.print(",");
//   // Serial.print(WIN_PER_SEC); Serial.print(",");
//   // Serial.print(sum_n); Serial.print(",");
//   // Serial.print(avg_raw_pH); Serial.print(",");
//   // Serial.print(avg_raw_off); Serial.print(",");
//   Serial.print(volts_pH, 4); Serial.print(",");
//   Serial.print(volts_off, 4); Serial.print(",");
//   Serial.print(volts_corr, 4); Serial.println();
//   // Serial.println(sps);// Serial.println();
// }