#include "Logger.h"

#include "../../../globals.h"

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

// --- format elapsed time as hh:mm:ss ---
int fmt_hms(uint32_t ms, char* buffer, size_t bufferSize) {
  uint32_t total_s = ms / 1000;
  int h = total_s / 3600;
  int m = (total_s % 3600) / 60;
  int s = total_s % 60;

  return snprintf(buffer, bufferSize,"%02d:%02d:%02d", h, m, s);
}

void logSaveData(uint32_t timestamp_ms, float pHVal, float VpH, float dieTemp, float Vbatt) {
  // Open log file in write append create mode
  if (!fat_file.open("/log.csv", FILE_WRITE)) {
    Serial.println("Failed to open log file!");
    digitalWrite(LED_RED, LOW);
    while(1);
  }

  // Write data in CSV format
  char buf[128];

  int cx = fmt_hms(timestamp_ms, buf, sizeof(buf));

  snprintf(buf+cx, sizeof(buf)-cx, ",%.3f,%.3f,%.2f,%.3f\n", pHVal, VpH, dieTemp, Vbatt);
  // Use below line if implementing additional characteristics in future
  // snprintf(buf+cx, sizeof(buf)-cx, ",%.3f,%.3f,%.2f,%.3f,%lu\n", pHVal, VpH, dieTemp, Vbatt, timestamp_ms);
  fat_file.write(buf);

  // Close the file
  fat_file.close();
  Serial.println("Data logged to log.csv");
  Serial.print("Logged Data: ");
  Serial.print(buf);
}