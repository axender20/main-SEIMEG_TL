#include "IOEx_SGB.h"
//! La implementacion para SGB, utiliza GPIOs de uC

#ifdef __SGB_MODEL__

const uint8_t output_pinout[MAX_OUTPUTS] = { 22, 23, 24, 25, 26, 27 };
const uint8_t input_pinout[MAX_INPUTS] = { 28, 29 };
const uint8_t control_pin_oe = 15;

const uint8_t max_value = (1 << MAX_OUTPUTS) - 1;

void init_ios() {
  //! Outs high-on
  for (uint8_t i = 0; i < MAX_OUTPUTS; i++) {
    pinMode(output_pinout[i], OUTPUT);
    digitalWrite(output_pinout[i], LOW);
  }

  for (uint8_t i = 0; i < MAX_INPUTS; i++) {
    pinMode(input_pinout[i], INPUT);
  }

  pinMode(control_pin_oe, OUTPUT);
  digitalWrite(control_pin_oe, LOW);
}

uint8_t read_inputs() {
  bool b1 = digitalRead(input_pinout[0]);
  bool b2 = digitalRead(input_pinout[1]);

  return (b1 << 1) | b2;
}

void write_outx_mask(uint8_t _new_mask) {
  if (_new_mask > max_value) return;

  for (uint8_t i = 0; i < MAX_OUTPUTS; i++) {
    bool pin_state = (_new_mask >> i) & 1;
    digitalWrite(output_pinout[i], pin_state);
  }
}

#endif //__SGB_MODEL__