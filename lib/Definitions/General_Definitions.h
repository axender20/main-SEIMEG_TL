#pragma once
#ifndef _GEN_DEF_H_
#define _GEN_DEF_H_

//? Posible firts version
///@brief Version de gestion "0.1.2a snph-sb"
#define __FIRMWARE_VERSION__ "0.0.1"

#include "pico/stdlib.h"
#include "Model_Definition.h"

//TODO+: Agregar informacion "DESARROLLAR" para __SGA_MODEL__

#if defined(__SGM_MODEL__)
#define __HARDWARE_VESION__ "SGM"
#define __HARDWARE_MODEL__ "SGM-0028M"
#define MAX_INPUTS 4
#define MAX_OUTPUTS 28
const uint8_t led_pcb = 12;

#elif defined(__SGB_MODEL__)
#define __HARDWARE_VESION__ "SGB"
#define __HARDWARE_MODEL__ "SGB-0023B"
#define MAX_INPUTS 2
#define MAX_OUTPUTS 6
const uint8_t led_pcb = 14;

#elif defined(__SGA_MODEL__)
#endif

#define __ENABLE_CHAR__ 0x0E
#define __EXT__ 0x03

#endif