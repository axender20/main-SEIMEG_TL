#pragma once
#ifndef _HANDLER_USB_H_

#include "pico/stdlib.h"
#include "Freertos_Definitions.h"
#include "Utilities_handler_USB.h"
#include "USBp_Definitions.h"
#include "Controller_MODEs.h"

void thread_usb(void* parametres);
void task_usb();
void init_usb_uart_bridge();


#endif