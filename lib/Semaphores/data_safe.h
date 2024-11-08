#pragma once
#ifndef _DATA_SAFE_H_

#include "Freertos_Definitions.h"
#include "semphr.h"

//Clase para un booleano seguro implementando freeertos
class Safe_bool {
private:
  bool value;
  SemaphoreHandle_t b_semaphore;
public:
  Safe_bool(bool _initial_value = false);
  ~Safe_bool();

  void set(bool _new_value);
  bool get();
};

class Safe_uint8_t {
private:
  uint8_t value;
  SemaphoreHandle_t u8_semaphore;
public:
  Safe_uint8_t(uint8_t _initial_value = 0);
  ~Safe_uint8_t();

  void set(uint8_t _new_value);
  uint8_t get();
};

#endif