#include "Input.h"

/// @brief Implementacion para un boton
Input::Input() {
  _t_estado = INP_STATUS::NOT_PRESSED;
  _triger = DEFAULT_TRIGGER;
  _debounce = DEFAULT_DEBOUNCE;
  _t_cont = 0;
  _status = !_triger;
  _prv_status = !_triger;
}

void Input::set_trigger(bool new_trigger){  
  _t_estado = INP_STATUS::NOT_PRESSED;
  _triger = new_trigger;
  _status = !_triger;
  _prv_status = !_triger; 
  _t_cont = 0;
}

void Input::update(bool current_status) {
  if (_status == current_status)
    _t_cont = 0;
  else {
    if (_t_cont == 0) {
      _t_cont = millis();
    }
    else {
      if (millis() - _t_cont >= _debounce) {
        _status = !_status;
      }
    }
  }

  if (_prv_status == !_triger && _status == !_triger) {
    _t_estado = INP_STATUS::NOT_PRESSED;
  }
  else if (_prv_status == !_triger && _status == _triger) {
    _t_estado = INP_STATUS::PUSH;
  }
  else if (_prv_status == _triger && _status == _triger) {
    _t_estado = INP_STATUS::PRESSED;
  }
  else if (_prv_status == _triger && _status == !_triger) {
    _t_estado = INP_STATUS::RELEASE;
  }
  _prv_status = _status;
}

uint8_t Input::get_status() {
  return this->_t_estado;
}

bool Input::is_pressed() {
  ////update(current_status);
  bool resul = (get_status() == INP_STATUS::PUSH) ? true : false;
  return resul;
}