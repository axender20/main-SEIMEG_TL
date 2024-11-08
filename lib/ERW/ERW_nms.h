#pragma once
#ifndef _ERW_NAMESPACE_H_
#define _ERW_NAMESPACE_H_

#include "Erw_Definitions.h"
#include "Freertos_Definitions.h"
#include "DateTime.h"
#include "EEPROM.h"
#include "DS3231.h"

//TODO: Pendiente de realizar, una vez se completen todas las necesidades del proyecto

/// @brief Obtiene de forma segura (freertos managment) la hora actual del RTC DS3231
/// @return DateTimem (now) 
DateTime get_safe_now();

/// @brief Funcion para cambiar de forma nativa la fecha-hora del RTC
/// @param new DateTime 
void native_adjust_time_rtc(DateTime _ndt);

/// @brief Escribe un bloque de forma segura (freertos managment) en la memoria eeprom
void eeprom_safe_write_block(const uint16_t memoryAddress, const uint8_t * buffer, const uint16_t length);

/// @brief Escribe un bloque de forma nativa, en la memoria eeprom
void eeprom_native_write_block(const uint16_t memoryAddress, const uint8_t * buffer, const uint16_t length);

void eeprom_native_update_byte(const uint16_t memoryAddress, const uint8_t data);

void eeprom_native_set_block(const uint16_t memoryAddress, const uint8_t data, const uint16_t length);

#endif