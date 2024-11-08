#pragma once
#ifndef _IOEx_SGB_H_
#define _IOEx_SGB_H_

#include "General_Definitions.h"

#ifdef __SGB_MODEL__
#include "Arduino.h"

void init_ios();
uint8_t read_inputs();
void write_outx_mask(uint8_t _new_mask);

#endif //SGB_MODEL

#endif