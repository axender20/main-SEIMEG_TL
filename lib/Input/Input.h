#pragma once
#ifndef _INPUT_H_
#define _INPUT_H_

#include "pico/stdlib.h"
#include "Arduino.h"

/*
  Implementacion para tratamientos de datos como entradas de tipo (bool)
  con estados: "Released, Release, Push, Pressed"

  ax20 v: 1.0.A8F 2/10/2024 
  Lib no utiliza ninguna funcion como entra, se actualiza con f:(update)(bool)
*/
#define I2C_EEPROM_VERSION          (F("1.0.A8F"))

#define DEFAULT_DEBOUNCE  100
#define DEFAULT_TRIGGER 0

enum INP_STATUS : uint8_t {
  NOT_PRESSED,
  PRESSED,
  PUSH,
  RELEASE
};

class Input {
private:
  uint8_t _t_estado;
  unsigned long _t_cont;
  bool _status;
  bool _prv_status;
  uint32_t _debounce;
  bool _triger;
public:
  Input();
  void set_trigger(bool new_trigger);
  void update(bool current_status);
  ///@attention Run update is full necesary  
  bool is_pressed();

  uint8_t get_status();
  void set_debounce(uint32_t _nDebounce) { this->_debounce = _nDebounce; }
};

#endif