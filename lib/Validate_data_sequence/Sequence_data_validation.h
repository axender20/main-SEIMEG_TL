#pragma once
#ifndef _SEQUENCE_VALIDATION_H_
#define _SEQUENCE_VALIDATION_H_

#include "Sequence_Definitions.h"
#include "Sequence_dataType.h"
#include "Arduino.h"
#include "Utilities.h"

extern Sequence sdv_sequence;
/// @brief Valida el formato (_buff) como "Sequence". Si es valida extrae todos los parametros y los almacena en sdv_sequence.
/// @param _buff (&String) 
/// @return (bool) resultado de la validacion
bool validate_str_sequence(String& _buff);

extern Sequence_default sdv_sequence_dff;
/// @brief Valida el formato (_buff) como "Sequence_default". Si es valida extrae todos los parametros y los almacena en sdv_sequence_deff.
/// @param _buff (&String) 
/// @return (bool) resultado de la validacion
bool validate_str_deff_sequence(String& _buff);

extern Event sdv_event;
/// @brief Valida el formato (_buff) como "Event". Si es valida extrae todos los parametros y los almacena en sdv_event.
/// @param _buff (&String) 
/// @return (bool) resultado de la validacion
bool validate_str_event(String& _buff);
#endif