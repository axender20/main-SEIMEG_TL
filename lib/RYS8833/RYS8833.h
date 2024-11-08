#pragma once
#ifndef _RYS8833_H_
#define _RYS8833_H_

//! Es necesaria la libreria *DateTime* 
#include "Arduino.h"
#include "FreeRTOS.h"
#include "task.h"
#include "DateTime.h"
#include "DEBUG.h"

extern DateTime GPS_DT_Reference;

class RYS8833 {
private:
  SerialUART* _port;
  uint8_t _pin_tx;
  uint8_t _pin_rx;
  uint8_t _pin_rst;
  unsigned long _baudrate;
  String _buff;
  uint32_t _port_time_out;
  bool _port_is_init;

  bool _is_init;
  DateTime _gps_date_time;
  bool initializate();
  void sendComand(const char* _cmd);
  bool readResponse();

  typedef void (*FunctionDelay)(uint32_t);
  FunctionDelay _delay;

  bool try_decode_zda_response();
  bool isConected();
public:
  RYS8833();
  void setPortConfig(SerialUART* _ser_port, unsigned long _ser_baudrate, uint32_t _ser_timeOut);
  void setPinout(uint8_t _txD, uint8_t _rxD, uint8_t _rst);
  void enableFreeRtos(bool _use = true);

  bool begin();

  void configure();
  // uint8_t getStatus();
  // bool hardwareReset();

  void coldInit();
  bool sync(uint32_t _max_timeout);
  DateTime getSyncDateTime();
};
#endif  