#include "map_memory.h"

const uint16_t array_mxa_seq_defaults[MAX_DEFAULT_SEQ] = {
_mxa_seq_default_1, _mxa_seq_default_2,
_mxa_seq_default_3, _mxa_seq_default_4,
_mxa_seq_default_5
};

const uint16_t array_mxa_events[MAX_EVENTS] = {
_mxa_event_1, _mxa_event_2, _mxa_event_3, _mxa_event_4, _mxa_event_5,
_mxa_event_6, _mxa_event_7, _mxa_event_8, _mxa_event_9, _mxa_event_10,
_mxa_event_11, _mxa_event_12, _mxa_event_13, _mxa_event_14, _mxa_event_15,
_mxa_event_16, _mxa_event_17, _mxa_event_18, _mxa_event_19, _mxa_event_20
};

#ifdef __SGM_MODEL__
const uint16_t array_mxa_buttons[MAX_SEQ_BUTTONS] = {
_mxa_btn_1, _mxa_btn_2, _mxa_btn_3, _mxa_btn_4
};
#elif defined(__SGB_MODEL__) 
const uint16_t array_mxa_buttons[MAX_SEQ_BUTTONS] = {
  _mxa_btn_1
};
#endif

//!(n) -> end
uint16_t space_memory_horary = _mxa_horary_enable - (1026);
uint16_t space_memory_defaults = _mxa_seq_default_enable - (3561);
uint16_t space_memory_events = _mxa_events_enable - (13750);
uint16_t space_memory_btsn = _mxa_btns_enable - (16005);
uint16_t space_memory_sync = _mxa_sync - (16107);