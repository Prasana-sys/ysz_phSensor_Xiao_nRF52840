#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

int fmt_hms(uint32_t ms, char* buffer, size_t bufferSize);
void logSaveData(uint32_t timestamp_ms, float pHVal, float VpH, float dieTemp, float Vbatt);

#endif // LOGGER_H