#pragma once
#ifndef _GPS_DEFINITIONS_H_

#include "pico/stdlib.h"
#include "Model_Definition.h"

#ifdef __SGM_MODEL__
#define gps_port Serial1
const uint8_t gps_rst = 27;
const uint8_t gps_tx = 28;
const uint8_t gps_rx = 29;
#elif defined(__SGB_MODEL__)
#define gps_port 
#else 
#error "Error: More than one model is defined. Only one model should be defined at a time."
#endif

const unsigned long gps_bdr = 115200;
const uint32_t gps_timeout = 100;

#endif