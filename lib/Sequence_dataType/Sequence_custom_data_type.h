#ifndef _SEQUENCE_DATA_TYPES_H_
#define _SEQUENCE_DATA_TYPES_H_

#include "Model_Definition.h"
#include "pico/stdlib.h"

#ifdef __SGM_MODEL__
typedef uint32_t uint_nst;
#elif defined(__SGB_MODEL__)
typedef uint8_t uint_nst;
#endif
#endif