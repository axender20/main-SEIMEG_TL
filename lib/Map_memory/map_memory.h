#pragma once
#ifndef _MAP_MEM_H_
#define _MAP_MEM_H_

#include "pico/stdlib.h"
#include "General_Definitions.h"
#include "Sequence_Definitions.h"


enum map_adress : uint16_t {
  /// @brief Versionado del tipo 0.0.0 (3 Bytes)
  _mxa_version = 0,
  //*END 3

  /// @brief Mac adress 00:00:00:00:00:00 (6 Bytes)
  _mxa_mac_addres = 4,
  //*END 10

  /// @brief Secuencia principal (Secuencia) (501 Bytes)
  _mxa_main_seq = 10,
  //*END 511

  /// @brief Horario de funcionamiento (en) (1Byte) + (hi:hf:secuencia) (505 Bytes)
  _mxa_horary_enable = 520,
  _mxa_horary_seq = 521,
  //*END 1026

  /// @brief Horarios para secuencias predeterminadas del dia normal (hi:hf:secuencia) (505 Bytes)
  _mxa_seq_default_enable = 1035,
  _mxa_seq_default_1 = 1036,
  _mxa_seq_default_2 = 1541,
  _mxa_seq_default_3 = 2046,
  _mxa_seq_default_4 = 2551,
  _mxa_seq_default_5 = 3056,
  //*END 3561

  /// @brief Eventos (Fecha:hi:hf:secuencia) (509 Bytes)
  _mxa_events_enable = 3569,
  _mxa_event_1 = 3570,
  _mxa_event_2 = 4079,
  _mxa_event_3 = 4588,
  _mxa_event_4 = 5097,
  _mxa_event_5 = 5606,
  _mxa_event_6 = 6115,
  _mxa_event_7 = 6624,
  _mxa_event_8 = 7133,
  _mxa_event_9 = 7642,
  _mxa_event_10 = 8151,
  _mxa_event_11 = 8660,
  _mxa_event_12 = 9169,
  _mxa_event_13 = 9678,
  _mxa_event_14 = 10187,
  _mxa_event_15 = 10696,
  _mxa_event_16 = 11205,
  _mxa_event_17 = 11714,
  _mxa_event_18 = 12223,
  _mxa_event_19 = 12732,
  _mxa_event_20 = 13241,
  //*END 13750

  /// @brief Secuencias para los botones enable (1Byte) Seq (501)Bytes
  _mxa_btns_enable = 14000,
  _mxa_btn_1 = 14001,
  _mxa_btn_2 = 14502,
  _mxa_btn_3 = 15003,
  _mxa_btn_4 = 15504,
  //*END 16005

    /// @brief Configuracion para sincronizacion (hf:tiempo) (7 Bytes)
  _mxa_sync = 16100,
  //*END 16109

  /// @brief Modo de configuracion, 0 -> normal, 1 -> sync GPS, 2 -> btn
  _mxa_mode = 16200,
  //*END 16201

  /// @brief Descripcion del funcionamiento del controlador
  _mxa_dsg = 16250
};

extern const uint16_t array_mxa_seq_defaults[MAX_DEFAULT_SEQ];
extern const uint16_t array_mxa_events[MAX_EVENTS];
extern const uint16_t array_mxa_buttons[MAX_SEQ_BUTTONS];

extern uint16_t space_memory_horary;
extern uint16_t space_memory_defaults;
extern uint16_t space_memory_events;
extern uint16_t space_memory_btsn;
extern uint16_t space_memory_sync;

#endif