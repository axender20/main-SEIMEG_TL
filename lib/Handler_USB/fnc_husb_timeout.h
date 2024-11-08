#pragma once
#ifndef _FNC_HUBS_TIMEOUT_H_
#define _FNC_HUBS_TIMEOUT_H_

#include "Arduino.h"
#include "USBp_Definitions.h"

inline void fnc_snd_cmd_timeout() {
  uart_usb_port.print("<N_C_TO>");
}
#endif