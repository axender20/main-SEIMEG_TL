#pragma once
#ifndef _MAD_ADR_H_
#define _MAD_ADR_H_

#include "pico/stdlib.h"
#include "cstdio"

extern uint8_t MAC_ADDRES[6];

/// @brief Obtiene la direccion MAC de la memoria EEPROM
void load_mac_address();

/// @brief Funcion para escribir la memoria MAC (only DEV use)
/// @param NEW_MAC Point (uint8_t[6]) new MAC
void NEW_write_mac_addres(uint8_t* NEW_MAC);

/// @brief Retorna la direccion MAC en formato "XX:XX:XX:XX:XX:XX"
const char* get_mac_ctrs();
#endif