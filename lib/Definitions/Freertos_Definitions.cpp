#include "Freertos_Definitions.h"

void delay_frms(uint32_t _time_ms) {
  if (!(_time_ms > 0)) return;
  TickType_t last_count_get = xTaskGetTickCount();
  TickType_t ticks = _time_ms * TICKS_PER_MS;
  vTaskDelayUntil(&last_count_get, ticks);
}