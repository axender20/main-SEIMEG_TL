#include "Object_DEBUG.h"

/// @brief Configuracion de pines para el DEBUG SERIAL
#ifdef __SGM_MODEL__
const uint8_t debug_pin_tx = 15;
const uint8_t debug_pin_rx = 14;
#elif defined(__SGB_MODEL__) 
const uint8_t debug_pin_tx = 12;
const uint8_t debug_pin_rx = 13;
#else
const uint8_t debug_pin_tx = 0xFF;
const uint8_t debug_pin_rx = 0xFF;
#endif

SerialPIO object_debug(debug_pin_tx, debug_pin_rx);
const ulong pio_debug_speed = 115200;

void init_ob_debug() {
  object_debug.begin(pio_debug_speed);
}
