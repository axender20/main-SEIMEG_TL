#include "Arduino.h"
#include "DEBUG.h"
#include "Controller_MODEs.h"
#include "Handler_USB.h"
#include "metadata.h"

#define UART_TASK_PRIO (tskIDLE_PRIORITY) + 2

void setup() {
  DEBUG_INIT();
  DEBUG_PRINTLN("__Init code__");

  init_usb_uart_bridge();
  xTaskCreate(thread_usb, "tsk_usb", 32358, NULL, UART_TASK_PRIO, &taskHandler_USB);
}

void loop() {
  tight_loop_contents();
}


