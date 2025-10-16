#include "SettingsManager.h"

#include "../../../config.h"
#include "../../../globals.h"

#include <Arduino.h>

void loadSettings() {
	File file = fatfs.open("/settings.ini", FILE_READ);
	if (!file) {
		Serial.println("settings.ini not found, using defaults.");
		return;
	}

	while (file.available()) {
		String line = file.readStringUntil('\n');
		line.trim();

		// Skip empty or comment lines
		if (line.length() == 0 || line.startsWith("#") || line.startsWith(";"))
			continue;

		// Split on '='
		int sep = line.indexOf('=');
		if (sep == -1)
			continue;

		String key = line.substring(0, sep);
		String value = line.substring(sep + 1);
		key.trim();
		value.trim();

		// Match keys to variables
		if (key == "continuousScanning")
			continuousScanningDeployment = value.toInt();
		else if (key == "numMeasurements")
			numberMeasurementsDeployment = value.toInt();
		else if (key == "numMeasurementsPreDeployment")
			numberMeasurementsPreDeployment = value.toInt();
		else if (key == "samplesPerHour")
			numberSamplesPerHour = value.toInt();
		else if (key == "sampleInterval")
			sampleIntervalPreDeployment = value.toInt();
	}

	file.close();
	Serial.println("Settings loaded successfully.");
}

void saveSettings() {
  File file = fatfs.open("/settings.ini", FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open settings.ini for writing!");
    return;
  }

  file.println("[Deployment]");
  file.print("continuousScanning="); file.println(continuousScanningDeployment);
  file.print("numMeasurements="); file.println(numberMeasurementsDeployment);

  file.println();
  file.println("[PreDeployment]");
  file.print("numMeasurementsPreDeployment="); file.println(numberMeasurementsPreDeployment);
  file.print("samplesPerHour="); file.println(numberSamplesPerHour);
  file.print("sampleInterval="); file.println(sampleIntervalPreDeployment);

  file.close();
  Serial.println("Settings saved successfully.");
}