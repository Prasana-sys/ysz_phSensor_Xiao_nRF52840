#ifndef TOOLS_H
#define TOOLS_H

#include <Arduino.h>

int fmt_hms(uint32_t ms, char* buffer, size_t bufferSize);
uint16_t safeAnalogRead(uint8_t pin);

#endif // TOOLS_H