#include "ERW_nms.h"

DateTime get_safe_now() {
  return DateTime(2024, 10, 16);
}

void native_adjust_time_rtc(DateTime _ndt) {
  //TODO: Agregar nueva implementacion para RTC (auxilio)
}

void eeprom_safe_write_block(const uint16_t memoryAddress, const uint8_t* buffer, const uint16_t length) {
  //TODO: Agregar implementacion para alamcenar datos usando freertos
}

void eeprom_native_write_block(const uint16_t memoryAddress, const uint8_t* buffer, const uint16_t length) {
  //TODO: Agregar implementacion para alamcenar datos utilizando el wire de forma directa
}

void eeprom_native_update_byte(const uint16_t memoryAddress, const uint8_t data) {
  //TODO: Agregar implementacion para actualizar un byte de la memoria eeprom
}

void eeprom_native_set_block(const uint16_t memoryAddress, const uint8_t data, const uint16_t length){
  //TODO: Agregar implementacion para cambiar un blocke de la memoria eeprom 
}
