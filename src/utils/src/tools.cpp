#include "tools.h"

#include "nrf.h"
#include "variant.h"
#include "bluefruit.h"
#include <nrfx_saadc.h>

// --- format elapsed time as hh:mm:ss ---
int fmt_hms(uint32_t ms, char* buffer, size_t bufferSize) {
  uint32_t total_s = ms / 1000;
  int h = total_s / 3600;
  int m = (total_s % 3600) / 60;
  int s = total_s % 60;

  return snprintf(buffer, bufferSize,"%02d:%02d:%02d", h, m, s);
}

uint16_t safeAnalogRead(uint8_t pin) {

  Serial.println("Starting SAADC read...");

  static bool saadc_initialized = false;

  nrfx_err_t err;
  nrf_saadc_value_t value = 0;

  pin = g_ADigitalPinMap[pin];
  nrf_saadc_input_t psel = NRF_SAADC_INPUT_DISABLED;

  switch ( pin ) {
    case 2:
      psel = NRF_SAADC_INPUT_AIN0;
      break;

    case 3:
      psel = NRF_SAADC_INPUT_AIN1;
      break;

    case 4:
      psel = NRF_SAADC_INPUT_AIN2;
      break;

    case 5:
      psel = NRF_SAADC_INPUT_AIN3;
      break;

    case 28:
      psel = NRF_SAADC_INPUT_AIN4;
      break;

    case 29:
      psel = NRF_SAADC_INPUT_AIN5;
      break;

    case 30:
      psel = NRF_SAADC_INPUT_AIN6;
      break;

    case 31:
      psel = NRF_SAADC_INPUT_AIN7;
      break;

    default:
      return 0;
  }

  if (!saadc_initialized)
  {
    err = nrfx_saadc_init(NRFX_SAADC_DEFAULT_CONFIG_IRQ_PRIORITY);
    if (err != NRFX_SUCCESS && err != NRFX_ERROR_INVALID_STATE)
    {
      Serial.printf("SAADC init failed: 0x%X\n", err);
      return -1;
    }
    saadc_initialized = true;
  }

  nrfx_saadc_channel_t channel_cfg = NRFX_SAADC_DEFAULT_CHANNEL_SE(psel, 0);
  err = nrfx_saadc_channels_config(&channel_cfg, 1);
  if (err != NRFX_SUCCESS)
  {
    Serial.printf("Channel config failed: 0x%X\n", err);
    return -1;
  }

  err = nrfx_saadc_simple_mode_set(
      (1 << 0),
      NRF_SAADC_RESOLUTION_14BIT,
      NRF_SAADC_OVERSAMPLE_DISABLED,
      NULL);
  if (err != NRFX_SUCCESS)
  {
    Serial.printf("Simple mode set failed: 0x%X\n", err);
    return -1;
  }

  err = nrfx_saadc_buffer_set(&value, 1);
  if (err != NRFX_SUCCESS)
  {
    Serial.printf("Buffer set failed: 0x%X\n", err);
    return -1;
  }

  err = nrfx_saadc_mode_trigger();
  if (err != NRFX_SUCCESS)
  {
    Serial.printf("Trigger failed: 0x%X\n", err);
    return -1;
  }

  nrfx_saadc_uninit();
  saadc_initialized = false;

  if (value < 0)
    value = 0;

  return value;

  // uint32_t psel = SAADC_CH_PSELP_PSELP_NC;

  // static uint32_t saadcReference = SAADC_CH_CONFIG_REFSEL_Internal;
  // static uint32_t saadcGain      = SAADC_CH_CONFIG_GAIN_Gain1_6;
  // static uint32_t saadcSampleTime = SAADC_CH_CONFIG_TACQ_3us;
  // static bool saadcBurst = SAADC_CH_CONFIG_BURST_Disabled;

  // volatile int16_t value = 0;

  // if (pin >= PINS_COUNT) {
  //   return 0;
  // }

  // pin = g_ADigitalPinMap[pin];

  // switch ( pin ) {
  //   case 2:
  //     psel = SAADC_CH_PSELP_PSELP_AnalogInput0;
  //     break;

  //   case 3:
  //     psel = SAADC_CH_PSELP_PSELP_AnalogInput1;
  //     break;

  //   case 4:
  //     psel = SAADC_CH_PSELP_PSELP_AnalogInput2;
  //     break;

  //   case 5:
  //     psel = SAADC_CH_PSELP_PSELP_AnalogInput3;
  //     break;

  //   case 28:
  //     psel = SAADC_CH_PSELP_PSELP_AnalogInput4;
  //     break;

  //   case 29:
  //     psel = SAADC_CH_PSELP_PSELP_AnalogInput5;
  //     break;

  //   case 30:
  //     psel = SAADC_CH_PSELP_PSELP_AnalogInput6;
  //     break;

  //   case 31:
  //     psel = SAADC_CH_PSELP_PSELP_AnalogInput7;
  //     break;

  //   default:
  //     return 0;
  // }

  // NRF_SAADC->RESOLUTION = SAADC_RESOLUTION_VAL_12bit;

  // Serial.println("Starting SAADC read...");

  // // Enable SAADC if not already on
  // NRF_SAADC->ENABLE = (SAADC_ENABLE_ENABLE_Enabled << SAADC_ENABLE_ENABLE_Pos);

  // // Disable all channels
  // for (int i = 0; i < 8; i++) {
  //   NRF_SAADC->CH[i].PSELN = SAADC_CH_PSELP_PSELP_NC;
  //   NRF_SAADC->CH[i].PSELP = SAADC_CH_PSELP_PSELP_NC;
  // }

  // // Configure channel 0
  // NRF_SAADC->CH[0].CONFIG = ((SAADC_CH_CONFIG_RESP_Bypass     << SAADC_CH_CONFIG_RESP_Pos)   & SAADC_CH_CONFIG_RESP_Msk)
  //                           | ((SAADC_CH_CONFIG_RESP_Bypass   << SAADC_CH_CONFIG_RESN_Pos)   & SAADC_CH_CONFIG_RESN_Msk)
  //                           | ((saadcGain                     << SAADC_CH_CONFIG_GAIN_Pos)   & SAADC_CH_CONFIG_GAIN_Msk)
  //                           | ((saadcReference                << SAADC_CH_CONFIG_REFSEL_Pos) & SAADC_CH_CONFIG_REFSEL_Msk)
  //                           | ((saadcSampleTime               << SAADC_CH_CONFIG_TACQ_Pos)   & SAADC_CH_CONFIG_TACQ_Msk)
  //                           | ((SAADC_CH_CONFIG_MODE_SE       << SAADC_CH_CONFIG_MODE_Pos)   & SAADC_CH_CONFIG_MODE_Msk)
  //                           | ((saadcBurst                    << SAADC_CH_CONFIG_BURST_Pos)   & SAADC_CH_CONFIG_BURST_Msk);
  // NRF_SAADC->CH[0].PSELN = psel;
  // NRF_SAADC->CH[0].PSELP = psel;

  // NRF_SAADC->RESULT.PTR = (uint32_t)&value;
  // NRF_SAADC->RESULT.MAXCNT = 1; // One sample

  // // Clear events
  // NRF_SAADC->EVENTS_END = 0;
  // NRF_SAADC->EVENTS_STARTED = 0;
  // NRF_SAADC->EVENTS_STOPPED = 0;

  // NRF_SAADC->TASKS_START = 0x01UL;

  // while (!NRF_SAADC->EVENTS_STARTED) {
  //   sd_app_evt_wait();  // yield to BLE stack
  // }
  // NRF_SAADC->EVENTS_STARTED = 0;

  // // Take one sample
  // NRF_SAADC->TASKS_SAMPLE = 1;

  // while (!NRF_SAADC->EVENTS_END) {
  //   sd_app_evt_wait();  // let SoftDevice handle radio
  // }
  // NRF_SAADC->EVENTS_END = 0;

  //  // Stop SAADC
  // NRF_SAADC->TASKS_STOP = 1;
  // while (!NRF_SAADC->EVENTS_STOPPED) {
  //   sd_app_evt_wait();
  // }
  // NRF_SAADC->EVENTS_STOPPED = 0;

  // if (value < 0) {
  //   value = 0;
  // }

  // NRF_SAADC->ENABLE = (SAADC_ENABLE_ENABLE_Disabled << SAADC_ENABLE_ENABLE_Pos);

  // return (uint16_t)value;
}