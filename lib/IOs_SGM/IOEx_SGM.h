#pragma once
#ifndef _IOS_EXPANDER_H_
#define _IOS_EXPANDER_H_

#include "General_Definitions.h"

#ifdef __SGM_MODEL__

#include "Arduino.h"
#include "TCA9555.h"
#include "Freertos_Definitions.h"

void init_ios();
uint8_t read_inputs();
void write_outx_mask(uint32_t _new_mask1);

extern SemaphoreHandle_t semp_handler_wire_ioExpander;
#endif //__SGB_MODEL__

#endif