#include "Controller_MODEs.h"
#include "SoftwareBootloader.h"

//? Tiempos de espera en ms
#define MAX_WAITING_TIME_CONFIG 10000u
#define MAX_WAITING_TIME_UPDATE 5000u

//? Funcion timer update 
void timer_update_callback() {
  fnc_snd_cmd_timeout();
  DEBUG_PRINTLN("R- cfg timeout");
  ctrllm_exit_update_mode();
}

void timer_config_callback() {
  fnc_snd_cmd_timeout();
  DEBUG_PRINTLN("R- upd timeout");
  ctrllm_exit_config_mode();
}

//? Funcion que se ejecutara si el timer se cumple
void(*fnc_hdl_timer_execute)() = nullptr;
void fnc_hdl_timer(TimerHandle_t xTimer) {
  if (fnc_hdl_timer_execute) {
    fnc_hdl_timer_execute();
  }
}

//? Funcion para crear e iniciar el timer-handler
TimerHandle_t timer_hdl_mode;
void init_timer_hdl_mode(uint32_t _ntime) {
  if (!(_ntime > 0)) return;

  //! Se debe asignar una funcion de tipo void name();
  const TickType_t timer_duration = pdMS_TO_TICKS(_ntime);
  timer_hdl_mode = xTimerCreate(
    "tm_mode",
    timer_duration,
    pdFALSE,
    (void*)0,
    fnc_hdl_timer
  );

  if (timer_hdl_mode != NULL) {
    xTimerStart(timer_hdl_mode, 100);
  }
}

void ctrllm_refesh_config_time() {
  if (timer_hdl_mode != NULL) {
    xTimerReset(timer_hdl_mode, 100);
  }
}

void pause_all_hanlders_wousb() {
  //TODO: revisar y validar la detencion correcta de todos los tskh, excepto USB - CONTROLLER
}

void ctrllm_entry_bootloader_mode() {
  entry_on_bootloader();
}

void ctrllm_entry_config_mode() {
  //TODO: Configuraciones previas para ingresar de forma correcta al modo CONFIG
  pause_all_hanlders_wousb();
  fnc_hdl_timer_execute = &timer_config_callback;
  init_timer_hdl_mode(MAX_WAITING_TIME_CONFIG);
}

void ctrllm_entry_update_mode() {
  //TODO: Configuraciones previas para entrar de forma segura al modo UPDATE
  pause_all_hanlders_wousb();
  fnc_hdl_timer_execute = &timer_update_callback;
  init_timer_hdl_mode(MAX_WAITING_TIME_UPDATE);
}

void ctrllm_exit_config_mode() {
  //TODO: Agregar validaciones para reiniciar de forma segura al uC
  rp2040.restart();
}

void ctrllm_exit_update_mode() {
  //TODO: Agregar validaciones para reiniciar de forma segura al uC
  rp2040.restart();
}
