#pragma once
#ifndef _SERIAL_READ_H_
#define _SERIAL_READ_H_

#include "Arduino.h"
#define check_sum_character '|'

enum comunication_errors : uint8_t {
  success,
  nack_checksum_error,
  comunication_error
};

class Uart_comunication {
private:
  SerialUART* _port;
  String buffer;
  bool valid_initial_data();
  bool valid_check_sum();

public:
  void attach_port(SerialUART* _serial_port) { this->_port = _serial_port; }
  uint8_t get_new_data(String& buff);
  bool is_available();
};

#endif