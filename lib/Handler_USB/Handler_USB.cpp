#include "Handler_USB.h"

/// @brief Thread tick (1 tick refresh)
TickType_t last_wake_thread_usb, interval_thread_usb = 1;
void thread_usb(void* parametres) {
  BaseType_t delayed;
  last_wake_thread_usb = xTaskGetTickCount();
  while (1) {
    task_usb();
    delayed = xTaskDelayUntil(&last_wake_thread_usb, interval_thread_usb);
    if (delayed == pdFALSE) {
      last_wake_thread_usb = xTaskGetTickCount();
    }
  }
  vTaskDelete(NULL);
}

void task_usb() {
  if (usb_port.is_available()) {
    uint8_t error_usb = usb_port.get_new_data(str_buffer_usb);
    H_DEBUG_PRINTLN(str_buffer_usb);
    if (error_usb != comunication_errors::success) {
      if (error_usb == comunication_errors::comunication_error)
        response_error(0);
      else
        response_error(1);
      const char* error_resp = (error_usb == nack_checksum_error) ? "ERROR Check_sum" : "ERROR Cumunication";
      H_DEBUG_PRINTLN(error_resp);
    }
    else {
      //Msj recibido de forma correcta -> interpretarlo
      uint8_t error_code = decode_MSJ(str_buffer_usb);
      if (error_code == decode_options::CMD_no_cmd) {
        H_DEBUG_PRINTLN("NO CMD");
        response_error(2);
      }
      else if (error_code == decode_options::CMD_syntax_error) {
        H_DEBUG_PRINTLN("CMD w -syntaxis error");
        response_error(3);
      }
      else if (error_code == decode_options::CMD_query) {
        H_DEBUG_PRINTLN("QUERY REQUEST");
      }
      else if (error_code == decode_options::CMD_enter_CONFIG) {
        H_DEBUG_PRINTLN("entry CFG MODE");
        ctrllm_entry_config_mode();
        uart_usb_port.print("<SG_IC_OK>");
      }
      else if (error_code == decode_options::CMD_new_CONFIG) {
        H_DEBUG_PRINTLN("NEW CFG");
        ctrllm_refesh_config_time();
      }
      else if (error_code == decode_options::CMD_exit_CONFIG) {
        H_DEBUG_PRINTLN("exit CFG MODE");
        uart_usb_port.print("<SG_NC_OK>");
        ctrllm_exit_config_mode();
      }
      else if (error_code == decode_options::CMD_enter_UPDATE) {
        H_DEBUG_PRINTLN("entry UPDATE MODE");
        ctrllm_entry_update_mode();
        uart_usb_port.print("<UPDATE_OK>");
      }
      else if (error_code == decode_options::CMD_confirm_UPDATE) {
        H_DEBUG_PRINTLN("confirm bootloader MODE");
        uart_usb_port.print("<N_F_OK>");
        delay_frms(50);
        ctrllm_entry_bootloader_mode();
      }
    }
  }
}

void init_usb_uart_bridge() {
  uart_usb_port.setPinout(uub_txd_pin, uub_rxd_pin);
  uart_usb_port.setFIFOSize(uub_max_buffer_size);
  uart_usb_port.setPollingMode();
  uart_usb_port.setTimeout(uub_timeout);
  uart_usb_port.begin(uub_baud_rate);
  usb_port.attach_port(&uart_usb_port);
  H_DEBUG_PRINTLN("__init hdl USB__");
}

