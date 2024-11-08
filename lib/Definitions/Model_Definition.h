#pragma once
#ifndef _MODEL_DEFINITION_H_
#define _MODEL_DEFINITION_H_

#ifdef D_MAIN_ENV
//! Seleccione su tipo de controlador (Basico por defecto)
// #define __SGM_MODEL__
#define __SGB_MODEL__ 
// #define __SGA_MODEL__  
#endif

#if (!defined(__SGA_MODEL__)) && (!defined(__SGM_MODEL__)) && (!defined(__SGB_MODEL__))
#define __SGB_MODEL__ 
#endif

#if (defined(__SGM_MODEL__) + defined(__SGB_MODEL__) + defined(__SGA_MODEL__)) > 1
#error "Error: More than one model is defined. Only one model should be defined at a time."
#endif

#endif