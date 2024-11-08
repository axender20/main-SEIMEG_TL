#pragma once
#ifndef _UTILITIES_HANDLER_H_
#define _UTILITIES_HANDLER_H_

//! Declaracion para habilitar el debug en este thread (hdu_inf:)
#define DEBUG_THREAD_USB_ENABLE

#if defined(DEBUG_THREAD_USB_ENABLE) && defined(DEBUG_ENABLE)
#include "Object_DEBUG.h"
#define H_DEBUG_PRINT(_info)(ob_debug_print(_info))
#define H_DEBUG_PRINTLN(_info) do {                     \
    object_debug.print("hdu_inf: ");                   \
    object_debug.println(_info);                      \
} while (0)
#else
#define H_DEBUG_PRINTLN(_info)
#define H_DEBUG_PRINT(_info)
#endif
//!---------------END DEBUG--------------------------------

#include "Arduino.h"
#include "SerialRead.h"
#include "General_Definitions.h"
#include "USBp_Definitions.h"
#include "Global.h"
#include "MAC_ADR.h"
#include "map_memory.h"
#include "ERW_nms.h"
#include "Sequence_dataType.h"
#include "Data.h"
#include "Utilities.h"
#include "Sequence_data_validation.h"

extern Uart_comunication usb_port;
extern String str_buffer_usb;

enum decode_options : uint8_t {
  CMD_no_cmd,
  CMD_query,
  CMD_enter_CONFIG,
  CMD_new_CONFIG,
  CMD_exit_CONFIG,
  CMD_enter_UPDATE,
  CMD_confirm_UPDATE,
  CMD_syntax_error,
};

void response_error(uint8_t error_code);
uint8_t decode_MSJ(String& _buff);
#endif