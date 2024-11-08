#pragma once
#ifndef _UTILITIES_H_
#define _UTILITIES_H_

// General utilities modules
#include "pico/stdlib.h"
#include "DateTime.h"
#include "ShortData.h"

bool validDateISO8601(const char* dateTV);
bool hex_str_to_u32(String& _Hex_str, uint32_t &value);
bool str_to_u32(String& _str, uint32_t &value);
#endif