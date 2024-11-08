#pragma once
#ifndef _SOFTWARE_BOOTLOADER_H_
#define _SOFTWARE_BOOTLOADER_H_

#include "pico/stdlib.h"
#include "DEBUG.h"
#include "hardware/watchdog.h"
#include "hardware/structs/watchdog.h"

//? Value to access to bootloader
#define ENTRY_MAGIC 0xb105f00d

/// @brief fnc para acceder al bootloader (serial-flash) 
void entry_on_bootloader();
#endif