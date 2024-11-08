#pragma once
#ifndef _SEQUENCE_DATETYPE_H_
#define _SEQUENCE_DATETYPE_H_

//? Dependencia directa con ShortData  

#include "pico/stdlib.h"
#include "Sequence_Definitions.h"
#include "ShortData.h"

class Sequence {
private:
  uint16_t times[MAX_ST];
  uint_nst states[MAX_ST];
  uint8_t length_changes;
public:
  Sequence();
  void replace(uint8_t number_states, uint_nst _new_states[MAX_ST], uint16_t _new_times[MAX_ST]);
  void set_state_at(uint8_t pos, uint_nst _new_state);
  uint_nst get_state_at(uint8_t pos);
  void set_time_at(uint8_t pos, uint16_t _new_time);
  uint16_t get_time_at(uint8_t pos);
  uint8_t get_cST() { return length_changes; }
  bool get_buffer_SEQ(uint8_t* _data);
  void replace_by_buffer(uint8_t* _data);
};

class Sequence_default {
private:
  Sequence seq;
  ShortTime time_init;
  ShortTime time_finit;
public:
  Sequence_default();
  void replace(ShortTime _time_i, ShortTime _time_f, Sequence _seq);
  void update_sequence(Sequence _new_seq);
  void update_sequence(uint8_t number_states, uint_nst _new_states[MAX_ST], uint16_t _new_times[MAX_ST]);
  Sequence get_sequence();
  void set_times(ShortTime _time_init, ShortTime _time_finit);
  ShortTime get_time_init();
  ShortTime get_time_finit();
  bool get_buffer_SEQDF(uint8_t* _data);
  void replace_by_buffer_SEQDF(uint8_t* _data);
};

class Event {
private:
  Sequence_default seq_df;
  ShortDate date;
public:
  Event();
  void replace(ShortDate _date, Sequence_default _seq_def);
  void update_sequence(Sequence _new_seq);
  Sequence get_sequence();
  void set_date_time(ShortDate _date_event, ShortTime _time_init, ShortTime _time_finit);
  ShortDate get_date();
  ShortTime get_time_init();
  ShortTime get_time_finit();
  bool get_buffer_EVT(uint8_t* _data);
  void replace_by_buffer_EVT(uint8_t* _data);
};

//! buffers

// Buffer para gestion primitiva de una secuencia
extern uint8_t u8_buffer_aux_seq[MAX_LENGTH_SEQUENCE];

// Buffer para gestion primitiva de una secuencia predeterminada
extern uint8_t u8_buffer_aux_seq_df[MAX_LENGTH_SEQUENCE_DEF];

// Buffer para gestion primitiva de un evento
extern uint8_t u8_buffer_aux_event[MAX_LENGTH_EVENT];

#endif