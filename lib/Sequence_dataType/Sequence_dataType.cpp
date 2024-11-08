#include "Sequence_dataType.h"
uint8_t u8_buffer_aux_seq[MAX_LENGTH_SEQUENCE];
uint8_t u8_buffer_aux_seq_df[MAX_LENGTH_SEQUENCE_DEF];
uint8_t u8_buffer_aux_event[MAX_LENGTH_EVENT];

Sequence::Sequence() {
  for (uint8_t i = 0; i < MAX_ST; i++) {
    this->states[i] = 0x00;
    this->times[i] = 0x00;
  }
  this->length_changes = 0;
}

void Sequence::replace(uint8_t number_states, uint_nst _new_states[MAX_ST], uint16_t _new_times[MAX_ST]) {
  if ((number_states == 0) || (number_states > MAX_ST)) return;

  for (uint8_t i = 0; i < MAX_ST; i++) {
    this->times[i] = 0x00;
    this->states[i] = 0x00;
  }
  this->length_changes = 0;

  for (uint8_t i = 0; i < number_states; i++) {
    this->times[i] = _new_times[i];
    this->states[i] = _new_states[i];
  }
  this->length_changes = number_states;
}

void Sequence::set_state_at(uint8_t pos, uint_nst _new_state) {
  if (pos >= MAX_ST) return;
  this->states[pos] = _new_state;
}

uint_nst Sequence::get_state_at(uint8_t pos) {
  if (pos >= MAX_ST) return 0;
  return this->states[pos];
}

void Sequence::set_time_at(uint8_t pos, uint16_t _new_time) {
  if (pos >= MAX_ST) return;
  this->times[pos] = _new_time;
}

uint16_t Sequence::get_time_at(uint8_t pos) {
  if (pos >= MAX_ST) return 0;
  return times[pos];
}

bool Sequence::get_buffer_SEQ(uint8_t* _data) {
  for (size_t i = 0; i < MAX_LENGTH_SEQUENCE; i++) {
    _data[i] = 0x00;
  }

  _data[0] = length_changes;
  uint64_t dir_times = MAX_LENGTH_SEQUENCE - 100;

  for (size_t i = 0; i < length_changes; i++) {
    //Almacenar estados (Hardware dependency)
#ifdef __SGM_MODEL__
    uint_nst _state_value = states[i];
    uint8_t buff_u32_u8[4];
    buff_u32_u8[0] = (_state_value >> 24) & 0xFF; // Byte más significativo
    buff_u32_u8[1] = (_state_value >> 16) & 0xFF;
    buff_u32_u8[2] = (_state_value >> 8) & 0xFF;
    buff_u32_u8[3] = _state_value & 0xFF; // Byte menos significativo 

    size_t init_idx_write = (i * 4) + 1; //4 por uint32 y 1 por el inicio
    for (uint8_t t = 0; t < 4; t++)
      _data[init_idx_write + t] = buff_u32_u8[t];
#elif defined(__SGB_MODEL__)
    _data[i + 1] = states[i];
#else 
#error "Error: More than one model is defined. Only one model should be defined at a time."
#endif
    //Almacenar tiempos
    uint16_t value = times[i];
    //low_byte
    _data[dir_times + (i * 2)] = (uint8_t)(value & 0xFF);
    //high_byte    
    _data[dir_times + (i * 2) + 1] = (uint8_t)((value >> 8) & 0xFF);
  }
  return true;
}

void Sequence::replace_by_buffer(uint8_t* _data) {
  if (!(_data[0] >= 1 && _data[0] <= MAX_ST)) return;

  length_changes = _data[0];
  uint64_t dir_times = MAX_LENGTH_SEQUENCE - 100;

  for (size_t i = 0; i < length_changes; i++) {
#ifdef __SGM_MODEL__
    size_t idx = (i * 4) + 1;
    this->states[i] = (_data[idx] << 24) | (_data[idx + 1] << 16) | (_data[idx + 2] << 8) | _data[idx + 3];
#elif defined(__SGB_MODEL__)
    this->states[i] = _data[i + 1];
#else
#error "Error: More than one model is defined. Only one model should be defined at a time."
#endif
    this->times[i] = (uint16_t)(_data[dir_times + (i * 2)] | (_data[dir_times + (i * 2) + 1] << 8));
  }
}

Sequence_default::Sequence_default() {
  this->seq = Sequence();
  this->time_init = ShortTime();
  this->time_finit = ShortTime();
}

void Sequence_default::replace(ShortTime _time_i, ShortTime _time_f, Sequence _seq) {
  this->time_init = _time_i;
  this->time_finit = _time_f;
  this->seq = _seq;
}

void Sequence_default::update_sequence(Sequence _new_seq) {
  this->seq = _new_seq;
}

void Sequence_default::update_sequence(uint8_t number_states, uint_nst _new_states[MAX_ST], uint16_t _new_times[MAX_ST]) {
  this->seq.replace(number_states, _new_states, _new_times);
}

Sequence Sequence_default::get_sequence() {
  return this->seq;
}

void Sequence_default::set_times(ShortTime _time_init, ShortTime _time_finit) {
  this->time_init = _time_init;
  this->time_finit = _time_finit;
}

ShortTime Sequence_default::get_time_init() {
  return this->time_init;
}

ShortTime Sequence_default::get_time_finit() {
  return this->time_finit;
}

bool Sequence_default::get_buffer_SEQDF(uint8_t* _data) {
  for (size_t i = 0; i < MAX_LENGTH_SEQUENCE_DEF; i++) {
    _data[i] = 0x00;
  }

  uint16_t hi = this->get_time_init().get_hour_sec();
  _data[0] = (uint8_t)(hi & 0xFF);   //Low Byte
  _data[1] = (uint8_t)((hi >> 8) & 0xFF); //High Byte

  uint16_t hf = this->get_time_finit().get_hour_sec();
  _data[2] = (uint8_t)(hf & 0xFF);   //Low Byte
  _data[3] = (uint8_t)((hf >> 8) & 0xFF); //High Byte

  uint8_t buff_aux[MAX_LENGTH_SEQUENCE];
  this->seq.get_buffer_SEQ(buff_aux);

  for (size_t i = 0; i < MAX_LENGTH_SEQUENCE; i++) {
    _data[i + 4] = buff_aux[i];
  }

  return true;
}

void Sequence_default::replace_by_buffer_SEQDF(uint8_t* _data) {
  //TODO: Realizar validaciones

  uint16_t u16_time_init = ((uint16_t)_data[1] << 8) | (uint16_t)_data[0];
  this->time_init.fill_hour_sec(u16_time_init);

  uint16_t u16_time_finit = ((uint16_t)_data[3] << 8) | (uint16_t)_data[2];
  this->time_finit.fill_hour_sec(u16_time_finit);

  uint8_t buffer_seq_aux[MAX_LENGTH_SEQUENCE];

  for (size_t i = 0; i < MAX_LENGTH_SEQUENCE; i++) {
    buffer_seq_aux[i] = _data[i + 4];
  }

  this->seq.replace_by_buffer(buffer_seq_aux);
}

Event::Event() {
  this->seq_df = Sequence_default();
  this->date = ShortDate();
}

void Event::replace(ShortDate _date, Sequence_default _seq_def) {
  this->date = _date;
  this->seq_df = _seq_def;
}

void Event::update_sequence(Sequence _new_seq) {
  this->seq_df.update_sequence(_new_seq);
}

Sequence Event::get_sequence() {
  return this->seq_df.get_sequence();
}

void Event::set_date_time(ShortDate _date_event, ShortTime _time_init, ShortTime _time_finit) {
  this->date = _date_event;
  this->seq_df.set_times(_time_init, _time_finit);
}

ShortDate Event::get_date() {
  return this->date;
}

ShortTime Event::get_time_init() {
  return this->seq_df.get_time_init();
}

ShortTime Event::get_time_finit() {
  return this->seq_df.get_time_finit();
}

bool Event::get_buffer_EVT(uint8_t* _data) {
  for (size_t i = 0; i < MAX_LENGTH_EVENT; i++) {
    _data[i] = 0x00;
  }

  uint32_t u32_date = this->date.get_ShortDate_u32();

  _data[0] = (uint8_t)((u32_date >> 24) & 0xFF); // Byte más significativo
  _data[1] = (uint8_t)((u32_date >> 16) & 0xFF);
  _data[2] = (uint8_t)((u32_date >> 8) & 0xFF);
  _data[3] = (uint8_t)(u32_date & 0xFF); // Byte menos significativo

  uint8_t buffer_evt_aux[MAX_LENGTH_SEQUENCE_DEF];

  this->seq_df.get_buffer_SEQDF(buffer_evt_aux);

  for (size_t i = 0; i < MAX_LENGTH_SEQUENCE_DEF; i++) {
    _data[i + 4] = buffer_evt_aux[i];
  }

  return true;
}

void Event::replace_by_buffer_EVT(uint8_t* _data) {
  //TODO: Agregar validaciones
  uint32_t u32_date = ((uint32_t)_data[0] << 24) | ((uint32_t)_data[1] << 16)
    | ((uint32_t)_data[2] << 8) | (uint32_t)_data[3];

  this->date.fill_ShortDate(u32_date);

  uint8_t buffer_seq_df_aux[MAX_LENGTH_SEQUENCE_DEF];
  for (size_t i = 0; i < MAX_LENGTH_SEQUENCE_DEF; i++) {
    buffer_seq_df_aux[i] = _data[i + 4];
  }

  this->seq_df.replace_by_buffer_SEQDF(buffer_seq_df_aux);
}