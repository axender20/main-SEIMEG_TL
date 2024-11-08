#pragma once
#ifndef _OBJECT_DEBUG_H_

//? Si desea cambiar la implementacion es necesario modificar el objeto para el DEBUG

//! SEIMEG TL - SerialPIO (uart DEBUG)
#include "SerialPIO.h"
#include "Model_Definition.h"
extern SerialPIO object_debug;

//Funciones necesarias tenr un DEBUG
void init_ob_debug();
#define ob_debug_println(_val)(object_debug.println(_val))
#define ob_debug_print(_val)(object_debug.print(_val))
#endif