#pragma once
#ifndef _CONTROLLER_MODES_H_
#define _CONTROLLER_MODES_H_

//!To enter or exit any mode it is necessary to manipulate the TASK handlers
#include "USB/TSK_Handler_USB.h"
#include "Freertos_Definitions.h"
#include "timers.h"
#include "fnc_husb_timeout.h"

/// @brief Accede al modo bootloader (serial-flash)
void ctrllm_entry_bootloader_mode();

/// @brief Detiene todos las funciones del controlador e inicia un timer que sale de forma automatica si no se realiza nigun cambio
void ctrllm_entry_config_mode();

/// @brief Detiene todos las funciones del controlador e inicia un timer que sale de forma automatica si no se realiza nigun cambio
void ctrllm_entry_update_mode();

/// @brief Realiza un software reset para aplicar la nueva configuracion
void ctrllm_exit_config_mode();

/// @brief Realiza un software reset sin ningun cambio 
void ctrllm_exit_update_mode();

/// @brief Reinicia el temporizador de configuraciones
void ctrllm_refesh_config_time();
#endif