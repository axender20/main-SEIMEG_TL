#pragma once
#ifndef _FREERTOS_CUSTOM_DEF_H_
#define _FREERTOS_CUSTOM_DEF_H_

#include "Arduino.h"
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "semphr.h"

#define TICKS_PER_MS (configTICK_RATE_HZ / 1000)
void delay_frms(uint32_t _time_ms);
#endif //! -> END FreertosDEF