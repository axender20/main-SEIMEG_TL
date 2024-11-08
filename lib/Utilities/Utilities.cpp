#include "Utilities.h"


bool validDateISO8601(const char* dateTV) {
  if (strlen(dateTV) != 19) {
    return false;
  }

  for (int i = 0; i < 19; i++) {
    if (i == 4 || i == 7) {
      // Verificar guiones en las posiciones 4 y 7
      if (dateTV[i] != '-') {
        return false;
      }
    }
    else if (i == 10) {
      // Verificar 'T' en la posición 10
      if (dateTV[i] != 'T') {
        return false;
      }
    }
    else if (i == 13 || i == 16) {
      // Verificar dos puntos en las posiciones 13 y 16
      if (dateTV[i] != ':') {
        return false;
      }
    }
    else {
      // Verificar dígitos en otras posiciones
      if (!isdigit(dateTV[i])) {
        return false;
      }
    }
  }

  int yy = 0, mm = 0, dd = 0;
  yy = atoi(dateTV);
  mm = atoi(dateTV + 5);
  dd = atoi(dateTV + 8);

  bool dateValid = ShortDate::valid_date(yy - 2000, mm, dd);
  if (!dateValid) return false;

  int hh = 0, min = 0, ss = 0;
  hh = atoi(dateTV + 11);
  min = atoi(dateTV + 14);
  ss = atoi(dateTV + 17);

  bool timeValid = ShortTime::valid_time(hh, min, ss);
  if (!timeValid) return false;

  return true;
}

bool hex_str_to_u32(String& _Hex_str, uint32_t& value) {
  if (!(_Hex_str.length() > 0 && _Hex_str.length() <= 8)) return false;

  for (size_t i = 0; i < _Hex_str.length(); i++) {
    if (!isHexadecimalDigit(_Hex_str[i])) return false;
  }

  uint32_t u32_value = strtoull(_Hex_str.c_str(), nullptr, 16);

  if (!(u32_value >= 0 && u32_value <= 0xFFFFFFFF)) {
    return false;
  }

  value = u32_value;
  return true;
}

bool str_to_u32(String& _str, uint32_t& u32_value) {
  if (!(_str.length() > 0 && _str.length() <= 10)) return false;

  for (size_t i = 0; i < _str.length(); i++) {
    if (!isDigit(_str[i])) return false;
  }

  int64_t value = strtoll(_str.c_str(), nullptr, 10);

  if (!(value >= 0 && value <= 0xFFFFFFFF)) return false;

  u32_value = static_cast<uint32_t> (value);
  return true;
}
