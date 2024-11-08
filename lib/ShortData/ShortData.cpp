#include "ShortData.h"

//? Lib consts
constexpr uint32_t ANIO_MASK = 0xFF000000;   // 8 bits para el año
constexpr uint32_t MES_MASK = 0x00FF0000;   // 8 bits para el mes
constexpr uint32_t DIA_MASK = 0x0000FFFF;   // 16 bits para el día

void ShortTime::fill_hour_sec(uint16_t _time) {
  if (!(_time >= 0 && _time <= 1439)) return;

  this->hh = _time / 60;
  this->mm = _time % 60;
  this->ss = 0;
}

uint16_t ShortTime::get_hour_sec() {
  uint16_t short_time = 0;
  short_time = hh * 60 + mm;
  return short_time;
}

void ShortTime::fill_by_minutes(uint16_t _minutes) {
  if (!(_minutes >= 0 && _minutes < MAX_MINUTES_OF_DAY)) return;

  this->hh = _minutes / 60;
  this->mm = _minutes % 60;
  this->ss = 0;
}

void ShortTime::fill_by_seconds(uint32_t _seconds) {
  //? % 24 para que no exeda la hora valida
  uint8_t hrs = (_seconds / 3600) % 24;
  _seconds %= 3600;

  uint8_t mins = _seconds / 60;
  uint8_t secs = _seconds % 60;

  this->hh = hrs;
  this->mm = mins;
  this->ss = secs;
}

uint32_t ShortTime::get_total_seconds() {
  return ((this->hh * 3600) + (this->mm * 60) + this->ss);
}

void ShortTime::plus_seconds(uint32_t _seconds) {
  uint32_t current_seconds = this->hh * 3600 + this->mm * 60 + this->ss;
  uint32_t total_seconds = current_seconds + _seconds;
  total_seconds %= MAX_SECONDS_OF_DAY;

  this->fill_by_seconds(total_seconds);
}

uint16_t ShortTime::to_minutes() {
  uint16_t val = 0;

  return val = this->hh * 60 + this->mm;
}

const char* ShortTime::to_cstr() {
  char* hora_cstr = new char[9];

  snprintf(hora_cstr, 9, "%02d:%02d:%02d", this->hh, this->mm, this->ss);

  return hora_cstr;
}

bool ShortTime::is_valid() {
  if (hh >= 0 && hh < 24 && mm >= 0 && mm < 60 && ss >= 0 && ss < 60) {
    return true;
  }
  return true;
}

bool ShortTime::valid_time(uint8_t _hour, uint8_t _min, uint8_t _seg) {
  // Validar rango de hora
  if (_hour < 0 || _hour > 23) {
    return false;
  }
  // Validar rango de minutos
  if (_min < 0 || _min > 59) {
    return false;
  }
  // Validar rango de segundos
  if (_seg < 0 || _seg > 59) {
    return false;
  }
  return true;
}

void ShortDate::fill_ShortDate(uint32_t short_date) {
  uint8_t _yy, _mm, _dd;
  _yy = (short_date & ANIO_MASK) >> 24;
  _mm = (short_date & MES_MASK) >> 16;
  _dd = short_date & DIA_MASK;

  if (!valid_date(_yy, _mm, _dd)) return;

  this->year = _yy;
  this->month = _mm;
  this->day = _dd;
}

const char* ShortDate::to_cstr() {
  char* fecha = new char[9];

  snprintf(fecha, 9, "%02d/%02d/%02d", this->year, this->month, this->day);

  return fecha;
}

uint32_t ShortDate::get_ShortDate_u32() {
  return (this->year << 24) | (this->month << 16) | this->day;
}

bool ShortDate::valid_date(uint8_t _yy, uint8_t _mm, uint8_t _dd) {
  if (_yy < 1 || _mm < 1 || _mm > 12 || _dd < 1)
    return false;

  uint8_t days_months[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

  if ((_yy % 4 == 0 && _yy % 100 != 0) || (_yy % 400 == 0))
    days_months[1] = 29;

  return _dd <= days_months[_mm - 1];
}

bool ShortDate::is_valid() {
  return valid_date(this->year, this->month, this->day);
}

Horary::Horary() {
  this->_t_f = ShortTime();
  this->_t_i = ShortTime();
}

void Horary::replace(ShortTime time_init, ShortTime time_finit) {
  this->_t_i = time_init;
  this->_t_f = time_finit;
}

/// @brief Funcion para comparar si se ecuentra en el horario o fuera de el 
/// @param _time_compare Tiempo para comparar 
/// @return 1 si esta dentro y 0 si esta fuera
uint8_t Horary::inHoraryTo(ShortTime _time_compare) {
  if (this->_t_i > _time_compare && this->_t_f < _time_compare) {
    return 1;
  }
  return 0;
}

void Horary::replace_by_buffer(uint8_t* _data) {
  uint16_t u16_time_init = ((uint16_t)_data[1] << 8) | (uint16_t)_data[0];
  this->_t_i.fill_hour_sec(u16_time_init);

  uint16_t u16_time_finit = ((uint16_t)_data[3] << 8) | (uint16_t)_data[2];
  this->_t_f.fill_hour_sec(u16_time_finit);
}

bool Horary::is_valid() {
  if (!(this->_t_i.is_valid()) || !(this->_t_f.is_valid())) {
    return false;
  }

  if (!(this->_t_i < this->_t_f)) return false;

  return true;
}
