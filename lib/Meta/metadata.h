#pragma once
#ifndef _METADATA_H_
#define _METADATA_H_

#include "General_Definitions.h"
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define METADATA_STRING "v:" TOSTRING(__FIRMWARE_VERSION__) " m:" TOSTRING(__HARDWARE_MODEL__)

__attribute__((section(".note.metadata"), used))
const char meta_data[] = METADATA_STRING;
#endif