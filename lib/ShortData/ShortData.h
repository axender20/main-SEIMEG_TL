#pragma once
#ifndef SHORT_DATA
#define SHORT_DATA

#include "pico/stdio.h"
#include "cstdio"

#define MAX_MINUTES_OF_DAY 1440
#define MAX_SECONDS_OF_DAY 86400

class ShortTime {
private:
  uint8_t hh;
  uint8_t mm;
  uint8_t ss;

public:
  ShortTime(uint8_t _hh = 0, uint8_t _mm = 0, uint8_t _ss = 0)
    : hh(_hh), mm(_mm), ss(_ss) {}

  /// @brief Unicamente datos del tipo hh:mm
  /// @param _time uint16_t unicamente datos de hora - minuto
  void fill_hour_sec(uint16_t _time);

  /// @brief Obtiene el valor de uint16 de hh:mm:00
  /// @return uint16_t minutos totales 
  uint16_t get_hour_sec();

  /// @brief Funcion para llenar el short data, basado en tiempo (minutos despues de media noche)
  /// @param minutes minutos en intrvalo (0 a 1439) min
  void fill_by_minutes(uint16_t _minutes);

  /// @brief Funcion para llenar el short data, basado en tiempo (segundos de un dia)
  /// @param _seconds segundos en intervalo (0 a 86399) seg
  void fill_by_seconds(uint32_t _seconds);

  /// @brief Obtiene el total de segundos transcurridos
  /// @return El total de segundos 
  uint32_t get_total_seconds();

  /// @brief Suma de forma circular los segundos del dia
  /// @param _seconds Segundos que se van a sumar 
  void plus_seconds(uint32_t _seconds);

  /// @brief Funcion para devolver los minutos totales de la hora
  /// @return uint16_t minutos (despues de media noche) de la hora 
  uint16_t to_minutes();

  const char* to_cstr();

  bool is_valid();

  static bool valid_time(uint8_t _hour, uint8_t _min, uint8_t _seg);

  bool operator<(const ShortTime& other) const {
    if (hh != other.hh) return hh < other.hh;
    if (mm != other.mm) return mm < other.mm;
    return ss < other.ss;
  }

  bool operator==(const ShortTime& other) const {
    return hh == other.hh && mm == other.mm && ss == other.ss;
  }

  bool operator>(const ShortTime& other) const {
    return !(*this < other || *this == other);
  }
};

class ShortDate {
private:
  uint8_t year;
  uint8_t month;
  uint8_t day;

public:
  ShortDate(uint8_t _yy = 24, uint8_t _mm = 1, uint8_t _day = 1) :
    year(_yy), month(_mm), day(_day) {};

  void fill_ShortDate(uint32_t short_date);

  const char* to_cstr();

  uint32_t get_ShortDate_u32();

  static bool valid_date(uint8_t _yy, uint8_t _mm, uint8_t _dd);

  bool is_valid();

  // Sobrecarga del operador ==
  bool operator==(const ShortDate& other) const {
    return (year == other.year) && (month == other.month) && (day == other.day);
  }

  // Sobrecarga del operador <
  bool operator<(const ShortDate& other) const {
    if (year != other.year)
      return year < other.year;
    if (month != other.month)
      return month < other.month;
    return day < other.day;
  }

  // Sobrecarga del operador >
  bool operator>(const ShortDate& other) const {
    if (year != other.year)
      return year > other.year;
    if (month != other.month)
      return month > other.month;
    return day > other.day;
  }

};

class Horary {
private:
  ShortTime _t_i;
  ShortTime _t_f;

public:
  Horary();

  void replace(ShortTime time_init, ShortTime time_finit);
  ShortTime get_time_init() { return this->_t_i; }
  ShortTime get_time_finit() { return this->_t_f; }

  uint8_t inHoraryTo(ShortTime _time_compare);

  /// @brief Funcion para remplacen en base un buffer de 4 elementos uint8_t
  /// @param data Buffer maximo 4 elementos
  void replace_by_buffer(uint8_t* data);

  bool is_valid();
};

#endif