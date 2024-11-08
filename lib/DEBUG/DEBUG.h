#pragma once
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "pico/stdlib.h"

#ifndef DEBUG_ENABLE
#define DEBUG_OB
#define DEBUG_INIT(_baud)
#define DEBUG_PRINTLN(_var)
#define DEBUG_PRINT
#endif

#ifdef DEBUG_ENABLE
#include "Object_DEBUG.h"
#define DEBUG_OB object_debug
#define DEBUG_INIT()(init_ob_debug())
#define DEBUG_PRINTLN(data)(ob_debug_println(data))
#define DEBUG_PRINT(data)(ob_debug_print(data))
#endif

#endif