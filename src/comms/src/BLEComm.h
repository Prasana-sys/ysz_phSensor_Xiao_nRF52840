#ifndef BLECOMM_H
#define BLECOMM_H

#include "bluefruit.h"

extern uint16_t webappConnHandle;

// Add base BLE functions here
void setupServices(void);
void startAdv(void);
void connect_callback(uint16_t conn_handle);
void disconnect_callback(uint16_t conn_handle, uint8_t reason);
void PreDeploymentCommandWriteCallback(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len);
void PreDeploymentContScanDeployWriteCallback(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len);
void PreDeploymentNumMeasDeployWriteCallback(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len);
void PreDeploymentNumMeasPreDeployWriteCallback(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len);
void PreDeploymentSampleIntervalDeployWriteCallback(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len);
void PreDeploymentSampleIntervalPreDeployWriteCallback(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len);

void indicateData(uint32_t timestamp_ms, float pHVal, float VpH, float dieTemp, float Vbatt);

#endif // BLECOMM_H