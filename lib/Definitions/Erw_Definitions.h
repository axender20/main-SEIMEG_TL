#pragma once
#ifndef _WIRE_ETHC_H_
#define _WIRE_ETHC_H_

#include "pico/stdlib.h"
#include "Wire.h"

// !Implementacion para el puerto I2C de los componentes EEPROM/RTC del mismo bus

#include "Model_Definition.h"

#if defined(__SGM_MODEL__)
  //* Definiciones wire SGM
const uint8_t ewr_i2c_sda = 6;
const uint8_t ewr_i2c_scl = 7;
const uint8_t eeprom_WP_pin = 5;
#define erw_port Wire1
#define MAX_BYTES_EEPROM 65535U

#elif defined(__SGB_MODEL__)
  //* Definiciones wire SGB
const uint8_t ewr_i2c_sda = 6;
const uint8_t ewr_i2c_scl = 7;
const uint8_t eeprom_WP_pin = 2;
#define erw_port Wire1
#endif

const uint8_t i2c_nack_time = 25;
const uint32_t ewr_i2c_speed_clock = 400000;
#define _EECHAR_RESET_ 0xFF
#endif