#pragma once
#ifndef _SOFTWARE_BOOTLOADER_H_
#define _SOFTWARE_BOOTLOADER_H_

#include "pico/stdlib.h"
#include "DEBUG.h"
#include "hardware/watchdog.h"
#include "hardware/structs/watchdog.h"

//? Value to access to bootloader
#define BOOTLOADER_ENTRY_MAGIC 0xb105f00d

/// @brief fnc para acceder al bootloader (serial-flash) 
void entry_on_bootloader();

/// @brief fnc para resetear de forma segura (watchdog) al controlador
void software_reset();
#endif