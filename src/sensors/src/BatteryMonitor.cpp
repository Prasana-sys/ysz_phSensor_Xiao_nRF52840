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