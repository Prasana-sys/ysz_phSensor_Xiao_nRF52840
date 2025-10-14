// Delete SdFat /Documents/Arduino/libraries/SdFat
// Only use libraries provided in BSP!!!

#include <Adafruit_TinyUSB.h> // included for Serial to compile

#include <Adafruit_LittleFS.h>
#include <ExternalFileSystem.h>

#include "config.h"
#include "globals.h"

// PLC timing (for 60 Hz mains)
const uint32_t PLC_1_US  = 16667; // 1 PLC = 16.67 ms
const uint32_t PLC_10_US = PLC_1_US * 10; // 10 PLC ≈ 166.7 ms
const uint8_t WIN_PER_SEC = 6; // 6 × (10 PLC) ≈ 1 second

void setup() {
  // Set analog read resolution
  analogReadResolution(ADC_RESOLUTION);
  // initialise ADC wireing_analog_nRF52.c:73
  analogReference(AR_DEFAULT);        // default 0.6V*6=3.6V  wireing_analog_nRF52.c:73

  pinMode(VBAT_ENABLE, OUTPUT);
  digitalWrite(VBAT_ENABLE, LOW); // Set low to read, never set high

  pinMode (PIN_CHARGING_CURRENT, OUTPUT); // battery charging current
  digitalWrite(PIN_CHARGING_CURRENT, HIGH); // Set to 50 mA

  pinMode(AFE_ENABLE_PIN, OUTPUT);
  digitalWrite(AFE_ENABLE_PIN, LOW);

  Serial.begin(115200);
  delay(1000);

  // Check what state we are in
}

void loop() {
  static uint32_t start_ms = millis();

  uint32_t second_start = millis();
  uint64_t sum_code_times_n_pH  = 0;
  uint64_t sum_code_times_n_off = 0;
  uint32_t sum_n = 0;

  for (int i = 0; i < WIN_PER_SEC; i++) {
    uint32_t n = 0;
    uint32_t avg_raw_pH = 0;
    uint32_t avg_raw_off = 0;
    
    read_plc_pair(PLC_10_US, avg_raw_pH, avg_raw_off, n);

    sum_code_times_n_pH  += (uint64_t)avg_raw_pH  * n;
    sum_code_times_n_off += (uint64_t)avg_raw_off * n;
    sum_n += n;
  }

  uint32_t avg_raw_pH  = (sum_n > 0) ? (sum_code_times_n_pH  / sum_n) : 0;
  uint32_t avg_raw_off = (sum_n > 0) ? (sum_code_times_n_off / sum_n) : 0;

  float volts_pH   = (avg_raw_pH  * VREF) / ADC_MAX;
  float volts_off  = (avg_raw_off * VREF) / ADC_MAX;

  float volts_corr = (avg_raw_pH) * (VREF / avg_raw_off);

  // Effective samples/sec
  uint32_t sps = (uint64_t)(sum_n * 1000000UL) / (PLC_10_US * WIN_PER_SEC);

  // Timestamp since start
  String stamp = fmt_hms(millis() - start_ms);

  Serial.print(stamp); Serial.print(",");
  // Serial.print(PLC_10_US); Serial.print(",");
  // Serial.print(WIN_PER_SEC); Serial.print(",");
  // Serial.print(sum_n); Serial.print(",");
  // Serial.print(avg_raw_pH); Serial.print(",");
  // Serial.print(avg_raw_off); Serial.print(",");
  Serial.print(volts_pH, 4); Serial.print(",");
  Serial.print(volts_off, 4); Serial.print(",");
  Serial.print(volts_corr, 4); Serial.println();
  // Serial.println(sps);// Serial.println();

}
