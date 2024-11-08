#include "Sequence_data_validation.h"

Sequence sdv_sequence;
Sequence_default sdv_sequence_dff;
Event sdv_event;

bool validate_str_sequence(String& _buff) {
  //? Validacion realizada en base al formato "(NC/ST/TM)"
  //Syntaxis de la secuencia "("NC/ST/TM")"

  if (!(_buff[0] == '(' && _buff[_buff.length() - 1 == ')'])) {
    return false;
  }

  //Ubicar los indices "/""
  int f_index = _buff.indexOf('/');
  int s_index = _buff.indexOf('/', f_index + 1);

  if (f_index == -1 || s_index == -1) return false;

  int states_n = 0;
  states_n = _buff.substring(1, f_index).toInt();

  //Validar numero de estados
  if (!(states_n > 0 && states_n <= MAX_ST)) return false;

  //* ---------------- STATES ---------------- */
  uint_nst aux_buff_ST[states_n];
  for (uint8_t i = 0; i < states_n; i++) {
    aux_buff_ST[i] = 0x00;
  }

  String num_aux = "";
  uint8_t cont = 0;

  for (int i = f_index + 1; i <= s_index; i++) {
    //Salto de linea o final
    if (_buff[i] == ',' || i == s_index) {
      //valid value 
      uint32_t u32_st_value = 0;
      bool val = hex_str_to_u32(num_aux, u32_st_value);
      if (!val) return false;

      if (!(u32_st_value <= states_max_value)) return false;
      num_aux = "";

      if (!(cont < states_n)) return false;
      aux_buff_ST[cont] = static_cast<uint_nst>(u32_st_value);
      cont++;
    }
    else {
      num_aux += _buff[i];
    }
  }
  if (states_n != cont) return false;

  //* ---------------- TIMES ---------------- */
  uint16_t aux_buff_TM[states_n];
  for (uint8_t i = 0; i < states_n; i++) {
    aux_buff_TM[i] = 0x00;
  }

  num_aux = "";
  cont = 0;
  // Recorrer todos los tiempos para validarlos
  for (int i = s_index + 1; i < _buff.length(); i++) {
    //Salto de linea o final
    if (_buff[i] == ',' || i == _buff.length() - 1) {
      //Valid value
      uint32_t u32_tm_value;
      bool val = hex_str_to_u32(num_aux, u32_tm_value);
      if (!val) return false;
      if (!(u32_tm_value > 0 && u32_tm_value <= times_max_value)) return false;
      num_aux = "";

      if (!(cont < states_n)) return false;
      aux_buff_TM[cont] = static_cast<uint16_t>(u32_tm_value);
      cont++;
    }
    else {
      num_aux += _buff[i];
    }
  }
  if (states_n != cont) return false;

  sdv_sequence.replace(states_n, aux_buff_ST, aux_buff_TM);
  return true;
}

bool validate_str_deff_sequence(String& _buff) {
  //? Validacion realizada en base al formato "HI:HF:(seq)"
  //Syntax example: "300:1320:(2/10,0/5,5)"

  int index_f = _buff.indexOf(':');
  int index_s = _buff.indexOf(':', index_f + 1);

  if (index_f == -1 || index_s == -1 || _buff[_buff.length() - 1] != ')') return false;

  //* ---------------- horary init ---------------- */
  String aux_t = _buff.substring(0, index_f);
  uint32_t u32_horary = 0;

  bool val = str_to_u32(aux_t, u32_horary);
  if (!val) return false;

  if (!(u32_horary < MAX_MINUTES_OF_DAY)) return false;
  ShortTime _buff_time_init;
  _buff_time_init.fill_by_minutes(static_cast<uint16_t>(u32_horary));

  //* ---------------- horary finit ---------------- */
  aux_t = _buff.substring(index_f + 1, index_s);
  val = str_to_u32(aux_t, u32_horary);

  if (!val) return false;
  if (!(u32_horary < MAX_MINUTES_OF_DAY)) return false;
  ShortTime _buff_time_finit;
  _buff_time_finit.fill_by_minutes(static_cast<uint16_t>(u32_horary));

  _buff = _buff.substring(index_s + 1, _buff.length());

  if (!validate_str_sequence(_buff)) return false;

  if (!(_buff_time_init < _buff_time_finit)) return false;

  sdv_sequence_dff.replace(_buff_time_init, _buff_time_finit, sdv_sequence);
  return true;
}

bool validate_str_event(String& _buff) {
  //? Validacion realizada en base al formato "Fch:Hi:Hf:(seq)"
  //Syntax example: "20241206:300:1320:(2/10,0/5,5)"

  int index_f = _buff.indexOf(':');
  if (index_f == -1 || _buff[_buff.length() - 1] != ')') return false;

  String aux = _buff.substring(0, index_f);

  uint32_t u32_year = 0;
  bool val = str_to_u32(aux, u32_year);
  if (!val) return false;

  uint8_t yy = 0, mm = 0, dd = 0;

  yy = u32_year / 10000;
  mm = (u32_year / 100) % 100;
  dd = u32_year % 100; 

  bool valid = ShortDate::valid_date(yy, mm, dd);
  if (!valid) return false;

  _buff = _buff.substring(index_f + 1, _buff.length());
  valid = validate_str_deff_sequence(_buff);

  if (!valid) return false;

  ShortDate _buff_date(yy, mm, dd);
  sdv_event.replace(_buff_date, sdv_sequence_dff);

  return true;
}
