#include "SettingsManager.h"

#include "../../../config.h"
#include "../../../globals.h"

#include <Arduino.h>

char line[80]; // Internal buffer

char *trim(char *str) {
  while (isspace(*str)) str++;
  if (*str == 0) return str;
  char *end = str + strlen(str) - 1;
  while (end > str && isspace(*end)) *end-- = 0;
  return str;
}

void loadSettings() {
  if (!fat_file.open("/settings.ini", O_RDONLY)) {
    Serial.println("settings.ini not found, using defaults.");
    return;
  }

  while (fat_file.fgets(line, sizeof(line)) > 0) {
    char *s = trim(line);
    if (*s == '#' || *s == ';' || *s == '\0') continue;

    char *eq = strchr(s, '=');
    if (!eq) continue;
    *eq = 0;
    char *key = trim(s);
    char *val = trim(eq + 1);

    if (!strcmp(key, "continuousScanningDeployment")) continuousScanningDeployment = atoi(val);
    else if (!strcmp(key, "numberMeasurementsDeployment")) numberMeasurementsDeployment = atol(val);
    else if (!strcmp(key, "numberMeasurementsPreDeployment")) numberMeasurementsPreDeployment = atol(val);
    else if (!strcmp(key, "numberSamplesPerHour")) numberSamplesPerHour = atoi(val);
    else if (!strcmp(key, "sampleIntervalPreDeployment")) sampleIntervalPreDeployment = atoi(val);
  }

  fat_file.close();
  Serial.println("Settings loaded from Flash.");
}

void saveSettings() {
  // Remove old file
  if (fatfs.exists("/settings.ini")) fatfs.remove("/settings.ini");

  if (!fat_file.open("/settings.ini", FILE_WRITE)) {
    Serial.println("Failed to open settings.ini for writing!");
    return;
  }

  char buf[64];

  snprintf(buf, sizeof(buf), "continuousScanningDeployment=%d\n", continuousScanningDeployment);
  fat_file.write(buf);

  snprintf(buf, sizeof(buf), "numberMeasurementsDeployment=%lu\n", numberMeasurementsDeployment);
  fat_file.write(buf);

  snprintf(buf, sizeof(buf), "numberMeasurementsPreDeployment=%lu\n", numberMeasurementsPreDeployment);
  fat_file.write(buf);

  snprintf(buf, sizeof(buf), "numberSamplesPerHour=%d\n", numberSamplesPerHour);
  fat_file.write(buf);

  snprintf(buf, sizeof(buf), "sampleIntervalPreDeployment=%d\n", sampleIntervalPreDeployment);
  fat_file.write(buf);

  if (!fat_file.sync()) digitalWrite(LED_RED, LOW);
  if (!fat_file.close()) digitalWrite(LED_RED, LOW);
  Serial.println("Settings saved to Flash.");
}