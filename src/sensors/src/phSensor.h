#ifndef PHSENSOR_H
#define PHSENSOR_H

float getpHValue(); // Pass in temperature values later or getTempandVoltage when calculating final pH in function itself?
float getpHVoltage();

void getpHValue(float& pHVal, float& VpH, float& dieTemp);
void getpHVoltage(float& VpH);

#endif // PHSENSOR_H