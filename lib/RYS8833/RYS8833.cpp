#include "RYS8833.h"
//! Horario minimo para verificar GPS UTC 01/01/2024 
DateTime GPS_DT_Reference(2024, 1, 1, 0, 0, 0);

#define TICKS_PER_MS (configTICK_RATE_HZ / 1000)

#define LGTH_CMDs 5
const char* GPS_CMDs[LGTH_CMDs] = {
  "@GSTP",
  "@ABUP 0",
  "@BSSL 0x80",
  "@GNS 0x3F",
  "@GCD"
};

void delay_freertos(uint32_t _time_ms) {
  if (!(_time_ms > 0)) return;
  TickType_t ultimo_tiempo = xTaskGetTickCount();
  TickType_t ticks = _time_ms * TICKS_PER_MS;
  vTaskDelayUntil(&ultimo_tiempo, ticks);
}

void delay_os(uint32_t _time_ms) {
  delay(_time_ms);
}

bool RYS8833::initializate() {
  bool resul = false;
  if (_pin_tx != 0xFF && _port != nullptr) resul = true;
  return resul;
}

void RYS8833::sendComand(const char* _cmd) {
  //? Todos los comandos deben terminar con /r/n;
  _port->print(_cmd);
  _port->print("\r\n");
  _delay(10);
}


/// @brief Obtiene la respuesta del controlador en un tiempo limite (timeout del puerto) el resultado se almacena en el buffer 
/// @return bool -> respuesta  
bool RYS8833::readResponse() {
  bool resul = false;
  ulong _t_cont = millis();

  while (true) {
    if (_port->available() > 0) {
      _buff = _port->readString();
      // DEBUG_PRINTLN(_buff);
      resul = true;
      break;
    }

    if (millis() - _t_cont >= _port_time_out) {
      break;
    }
    _delay(1);
  }
  return resul;
}

bool RYS8833::try_decode_zda_response() {
  //? Modelo de menjase para recepcion de ubicacion
  //!Format: $GNZDA,hhmmss.ss,xx,xx,xxxx,xx,xx*hh<CR><LF>
  //*Example: $GNZDA,000255.00,06,01,1980,,*7D\r\n

  String _aux = _buff;
  int fI = _aux.indexOf("$");
  int sI = _aux.indexOf("\r\n");

  //!La cadena de caracteres no tiene el formato correcto
  if (fI == -1 || sI == -1) return false;
  _aux = _aux.substring(fI, sI);

  int tI = _aux.indexOf('*');
  if (tI == -1) return false;

  String _str_check_sum = _aux.substring(tI + 1);
  uint8_t _initial_check_sum = static_cast<uint8_t>(std::strtoul(_str_check_sum.c_str(), nullptr, 16));

  _aux = _aux.substring(fI + 1, tI);
  uint8_t check_sum = 0;
  //! First valide checkSum
  for (uint8_t i = 0; i < _aux.length(); i++) {
    check_sum ^= _aux.charAt(i);
  }

  if (check_sum != _initial_check_sum) return false;

  //? GNZDA,hhmmss.ss,xx,xx,xxxx,xx,xx

  uint8_t _hh = 0, _mm = 0, _ss = 0, _day = 0, _month = 0;
  uint16_t _year = 0;
  String str_split = "";

  uint8_t n_cont = 0;
  for (uint8_t i = 0; i < _aux.length(); i++) {
    if (_aux.charAt(i) == ',') {
      switch (n_cont) {
      case 1: {
        //? UTC
        double _time_double = atof(str_split.c_str());
        uint32_t _time_u32 = static_cast<int>(_time_double);
        _hh = _time_u32 / 10000;
        _mm = (_time_u32 / 100) % 100;
        _ss = _time_u32 % 100;
        break;
      }

      case 2: {
        //? Day
        _day = atoi(str_split.c_str());
        break;
      }

      case 3: {
        //? Month
        _month = atoi(str_split.c_str());
        break;
      }

      case 4: {
        //? Year
        _year = atoi(str_split.c_str());
        break;
      }

      default:
        break;
      }
      n_cont++;
      str_split = "";
    }
    else {
      str_split += _aux.charAt(i);
    }
  }

  if (_year >= GPS_DT_Reference.year()) {
    DateTime _dt(_year, _month, _day, _hh, _mm, _ss);
    DateTime _dt_horary_Central = _dt - TimeSpan(0, 6, 0, 0);
    DEBUG_PRINTLN(_dt.timestamp());
    DEBUG_PRINTLN(_dt_horary_Central.timestamp());
    this->_gps_date_time = _dt_horary_Central;
    return true;
  }
  return false;
}

RYS8833::RYS8833() {
  _port = nullptr;
  _pin_tx = 0xFF;
  _pin_rx = 0xFF;
  _pin_rst = 0xFF;
  _baudrate = 0;
  _buff = "";
  _port_time_out = 0;
  _delay = delay_os;
  _is_init = false;
  _port_is_init = false;
  _gps_date_time = DateTime(0, 0, 0);
}

void RYS8833::setPortConfig(SerialUART* _ser_port, unsigned long _ser_baudrate, uint32_t _ser_timeOut) {
  _port = _ser_port;
  _baudrate = _ser_baudrate;
  _port_time_out = _ser_timeOut;
}

void RYS8833::setPinout(uint8_t _txD, uint8_t _rxD, uint8_t _rst) {
  //TODO: Validar que los pines sean GPIOs

  _pin_rx = _rxD;
  _pin_tx = _txD;
  _pin_rst = _rst;
}

void RYS8833::enableFreeRtos(bool _use) {
  _delay = (_use) ? delay_freertos : delay_os;
}

bool RYS8833::begin() {
  if (!initializate()) return false;
  //! Implementacion especifica RP2040
  _port->setPinout(_pin_tx, _pin_rx);
  _port->setTimeout(_port_time_out);
  _port->begin(_baudrate);
  _port_is_init = true;
  pinMode(_pin_rst, OUTPUT);
  digitalWrite(_pin_rst, HIGH);
  _delay(1000);
  DEBUG_PRINTLN("Complete initialized");
  return isConected();
}

bool RYS8833::isConected() {
  sendComand(GPS_CMDs[0]);
  bool _response = readResponse();
  //! No hay respuesta, error del controlador
  if (!_response) return false;

  bool resul = false;
  if (_buff.indexOf("[GSTP] Done") != -1) resul = true;
  return resul;
}

void RYS8833::configure() {
  //Enviar comandos para configurar al controlador
  bool response = false;
  sendComand(GPS_CMDs[1]);
  response = readResponse();

  sendComand(GPS_CMDs[2]);
  response = readResponse();

  sendComand(GPS_CMDs[3]);
  response = readResponse();
}

void RYS8833::coldInit() {
  sendComand(GPS_CMDs[4]);
  readResponse();
  _is_init = true;
}

bool RYS8833::sync(uint32_t _max_timeout) {
  if (!_port_is_init) {
    _port->begin(_baudrate);
    _port_is_init = true;
  }

  if (!_is_init) coldInit();

  bool resul = false;
  ulong _t_cont = millis();
  while (true) {
    bool sync_response = readResponse();
    if (sync_response) {
      bool n_try = try_decode_zda_response();
      if (n_try) return true;
    }

    if (millis() - _t_cont >= (_max_timeout * 1000)) break;
    _delay(1);
  }
  _port->end();
  _port_is_init = false;
  return resul;
}

DateTime RYS8833::getSyncDateTime(){
  return this->_gps_date_time;
}
