#pragma once 
#ifndef _UART_USB_BRIDGE_DEFINITIONS_H_
#define _UART_USB_BRIDGE_DEFINITIONS_H_

#include "HardwareSerial.h"
#include "Model_Definition.h"

#ifdef __SGM_MODEL__
  const uint8_t uub_txd_pin = 8;
  const uint8_t uub_rxd_pin = 9;
  const uint8_t uub_cts_pin = 10;
  const uint8_t uub_rts_pin = 11;
  const uint32_t uub_max_buffer_size = 1024;
#define uart_usb_port Serial2
#elif defined(__SGB_MODEL__)
  const uint8_t uub_txd_pin = 8;
  const uint8_t uub_rxd_pin = 9;
  const uint8_t uub_cts_pin = 10;
  const uint8_t uub_rts_pin = 11;
  const uint32_t uub_max_buffer_size = 256;
#define uart_usb_port Serial2
#endif

const uint32_t uub_baud_rate = 115200;
const uint8_t uub_timeout = 100;
#endif