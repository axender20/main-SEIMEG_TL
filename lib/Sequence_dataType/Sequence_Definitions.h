#pragma once
#ifndef _SEQ_DEFINITIONS_
#define _SEQ_DEFINITIONS_

#include "pico/stdlib.h"
#include "Sequence_custom_data_type.h"

#define MAX_EVENTS 20
#define MAX_DEFAULT_SEQ 5

//!Numero de estados maximo de una secuencia
#define MAX_ST 50

#if defined(__SGM_MODEL__)
#define MAX_SEQ_BUTTONS 4
  //! max_value out = 0xFFFFFFF -> uint32 (4Bytes)
// 1 Byte(c), 200 Bytes(s), 100 Bytes(t)
#define MAX_LENGTH_SEQUENCE 301

#elif defined(__SGB_MODEL__)
#define MAX_SEQ_BUTTONS 1
  //! max_value out = 0xFF -> uint8 (1Bytes)
// 1 Byte(c), 50 Bytes(s), 100 Bytes(t)
#define MAX_LENGTH_SEQUENCE 151
#endif

// 2 Byte(hi), 2 Byte(hf), (Seq)
#define MAX_LENGTH_SEQUENCE_DEF (MAX_LENGTH_SEQUENCE + 2 + 2)
// 4 Byte(date), (Seq_def)
#define MAX_LENGTH_EVENT (MAX_LENGTH_SEQUENCE_DEF + 4)

#include "General_Definitions.h"
#define states_max_value ((1 << MAX_OUTPUTS) - 1)
#define times_max_value 0xFFF
#endif