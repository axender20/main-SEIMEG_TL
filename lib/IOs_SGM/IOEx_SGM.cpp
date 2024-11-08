#include "IOEx_SGM.h"

#ifdef __SGM_MODEL__

//! La implementacion para SGM utiliza 2 TCA9555 e i2c0 valido para SGM
#define ioe_wire Wire

const uint8_t control_pin_oe = 2;
const uint8_t ioe1_input_pinout[2] = { TCA_P07, TCA_P17 };
const uint8_t ioe2_input_pinout[2] = { TCA_P07, TCA_P17 };

const uint8_t ioe1_output_pinout[14] = { TCA_P00, TCA_P01, TCA_P02, TCA_P03, TCA_P04, TCA_P05, TCA_P06, TCA_P10, TCA_P11, TCA_P12, TCA_P13, TCA_P14, TCA_P15, TCA_P16 };
const uint8_t ioe2_output_pinout[14] = { TCA_P00, TCA_P01, TCA_P02, TCA_P03, TCA_P04, TCA_P05, TCA_P06, TCA_P10, TCA_P11, TCA_P12, TCA_P13, TCA_P14, TCA_P15, TCA_P16 };
// const uint8_t ioe2_output_pinout[14] = { TCA_P06, TCA_P05, TCA_P04, TCA_P03, TCA_P02, TCA_P01, TCA_P00, TCA_P16, TCA_P15, TCA_P14, TCA_P13, TCA_P12, TCA_P11, TCA_P10 };

const uint8_t ioex1_adr = 0x20;
const uint8_t ioex2_adr = 0x21;

TCA9555 ioex1(ioex1_adr, &ioe_wire);
TCA9555 ioex2(ioex2_adr, &ioe_wire);

const uint8_t i2c0_sda = 0;
const uint8_t i2c0_scl = 1;
const uint32_t i2c0_speed_clock = 400000;
const uint32_t max_value = (1 << MAX_OUTPUTS) - 1;

SemaphoreHandle_t semp_handler_wire_ioExpander;

void take_handler_iox();
void give_handler_iox();

void init_ios() {
  Wire.setSDA(i2c0_sda);
  Wire.setSCL(i2c0_scl);
  Wire.setClock(i2c0_speed_clock);
  Wire.begin();

  semp_handler_wire_ioExpander = xSemaphoreCreateBinary();
  give_handler_iox();

  ioex1.begin();
  ioex2.begin();

  //Initialize pinout
  for (uint8_t i = 0; i < 14; i++) {
    ioex1.pinMode1(ioe1_output_pinout[i], OUTPUT);
    ioex2.pinMode1(ioe2_output_pinout[i], OUTPUT);
  }

  for (uint8_t i = 0; i < 2; i++) {
    ioex1.pinMode1(ioe1_input_pinout[i], INPUT);
    ioex2.pinMode1(ioe2_input_pinout[i], INPUT);
  }

  ioex1.write16(~0x00);
  ioex2.write16(~0x00);

  pinMode(control_pin_oe, OUTPUT);
  digitalWrite(control_pin_oe, LOW);
}

uint8_t read_inputs() {
  uint8_t resul = 0;

  take_handler_iox();
  bool b1 = ioex1.read1(ioe1_input_pinout[0]);
  bool b2 = ioex1.read1(ioe1_input_pinout[1]);
  bool b3 = ioex2.read1(ioe2_input_pinout[0]);
  bool b4 = ioex2.read1(ioe2_input_pinout[1]);
  give_handler_iox();

  return (b1 << 3) | (b2 << 2) | (b3 << 1) | b4;
}

void write_outx_mask(uint32_t _new_mask1) {
  if (_new_mask1 > max_value) return;

  uint16_t mask_port0 = static_cast<uint16_t>((_new_mask1 >> 14) & 0xFFFF);
  uint16_t mask_port1 = static_cast<uint16_t>(_new_mask1 & 0xFFFF);

  uint16_t mask_resul_port0 = 0;
  uint16_t mask_resul_port1 = 0;

  for (uint8_t i = 0; i < 14; i++) {
    bool bit = (mask_port0 & (1ULL << i)) != 0;
    mask_resul_port0 |= (bit << ioe1_output_pinout[i]);

    bit = (mask_port1 & (1ULL << i)) != 0;
    mask_resul_port1 |= (bit << ioe2_output_pinout[i]);
  }

  take_handler_iox();
  ioex1.write16(~mask_resul_port1);
  ioex2.write16(~mask_resul_port0);
  give_handler_iox();
}

void take_handler_iox() {
  if (semp_handler_wire_ioExpander != NULL) {
    xSemaphoreTake(semp_handler_wire_ioExpander, portMAX_DELAY);
  }
}

void give_handler_iox() {
  if (semp_handler_wire_ioExpander != NULL) {
    xSemaphoreGive(semp_handler_wire_ioExpander);
  }
}

#endif //__SGB_MODEL__