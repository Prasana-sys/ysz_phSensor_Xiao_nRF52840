#include <Adafruit_TinyUSB.h> // included for Serial to compile

#include <Adafruit_LittleFS.h>
#include <ExternalFileSystem.h>

// Use SdFat - Adafruit Fork version 2.2.54 to use external file system library correctly

const uint8_t pHSensorADC = A0;
// BAT Voltage read pin at Pin 32 PIN_VBAT
// Put VBAT_ENABLE to output sink (low) to read PIN_VBAT
const uint8_t offsetRefADC = A2;
// Possible to get Die Temperature of nRF52840 but very inaccurate

const uint8_t AFE_ENABLE_PIN = D7;

static const float VREF = 3.3;
static const int ADC_MAX = (1 << ADC_RESOLUTION) - 1;

// PLC timing (for 60 Hz mains)
const uint32_t PLC_1_US  = 16667; // 1 PLC = 16.67 ms
const uint32_t PLC_10_US = PLC_1_US * 10; // 10 PLC ≈ 166.7 ms
const uint8_t WIN_PER_SEC = 6; // 6 × (10 PLC) ≈ 1 second

Adafruit_LittleFS_Namespace::File file(ExternalFS);

// uint32_t read_plc_avg(uint32_t window_us, uint32_t &count_out) {
//   uint32_t start = micros();
//   uint32_t total = 0;
//   uint32_t count = 0;

//   while ((micros() - start) < window_us) {
//     total += analogRead(pHSensorADC);
//     count++;
//   }

//   count_out = count;
//   return (count > 0) ? (total / count) : 0;
// }

// --- average both ADC channels within one PLC window ---
void read_plc_pair(uint32_t window_us,
                   uint32_t &avg_raw_pH,
                   uint32_t &avg_raw_off,
                   uint32_t &count_out) {
  uint32_t start = micros();
  uint64_t total_pH = 0;
  uint64_t total_off = 0;
  uint32_t count = 0;

  while ((micros() - start) < window_us) {
    total_pH  += analogRead(pHSensorADC);
    total_off += analogRead(offsetRefADC);
    count++;
  }

  count_out   = count;
  avg_raw_pH  = (count > 0) ? total_pH  / count : 0;
  avg_raw_off = (count > 0) ? total_off / count : 0;
}

// --- format elapsed time as hh:mm:ss ---
String fmt_hms(uint32_t ms) {
  uint32_t total_s = ms / 1000;
  int h = total_s / 3600;
  int m = (total_s % 3600) / 60;
  int s = total_s % 60;

  char buf[16];
  sprintf(buf, "%02d:%02d:%02d", h, m, s);
  return String(buf);
}

void setup() {
  // Set analog read resolution
  analogReadResolution(ADC_RESOLUTION);

  pinMode(VBAT_ENABLE, OUTPUT);
  digitalWrite(VBAT_ENABLE, HIGH); // Set low to read, set high when charging battery

  pinMode (PIN_CHARGING_CURRENT, OUTPUT); // battery charging current
  digitalWrite(PIN_CHARGING_CURRENT, HIGH); // Set to 50 mA

  pinMode(AFE_ENABLE_PIN, OUTPUT);
  digitalWrite(AFE_ENABLE_PIN, LOW);

  Serial.begin(115200);
  delay(1000);

  // CSV header
  // Serial.println("Time,PLC_us,Windows,Samples,Raw_ph,Raw_off,Volts_ph,Volts_off,Volts_corr,SPS");
  Serial.println("Time,Volts_ph,Volts_off,Volts_corr");
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
