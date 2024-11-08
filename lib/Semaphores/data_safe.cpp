#include "data_safe.h"

Safe_bool::Safe_bool(bool _initial_value) {
  value = _initial_value;
  b_semaphore = xSemaphoreCreateBinary();
  if (b_semaphore != nullptr) {
    xSemaphoreGive(b_semaphore);
  }
}

Safe_bool::~Safe_bool() {
  if (b_semaphore != nullptr) {
    vSemaphoreDelete(b_semaphore);
  }
}

void Safe_bool::set(bool _new_value) {
  if (b_semaphore != nullptr) {
    if (xSemaphoreTake(b_semaphore, portMAX_DELAY) == pdTRUE) {
      value = _new_value;
      xSemaphoreGive(b_semaphore);
    }
  }
}

bool Safe_bool::get() {
  bool resul = false;
  if (b_semaphore != nullptr) {
    if (xSemaphoreTake(b_semaphore, portMAX_DELAY) == pdTRUE) {
      resul = this->value;
      xSemaphoreGive(b_semaphore);
    }
  }
  return resul;
}

Safe_uint8_t::Safe_uint8_t(uint8_t _initial_value) {
  value = _initial_value;
  u8_semaphore = xSemaphoreCreateBinary();
  if (u8_semaphore != nullptr) {
    xSemaphoreGive(u8_semaphore);
  }
}

Safe_uint8_t::~Safe_uint8_t() {
  if (u8_semaphore != nullptr) {
    vSemaphoreDelete(u8_semaphore);
  }
}

void Safe_uint8_t::set(uint8_t _new_value) {
  if (u8_semaphore != nullptr) {
    if (xSemaphoreTake(u8_semaphore, portMAX_DELAY) == pdTRUE) {
      value = _new_value;
      xSemaphoreGive(u8_semaphore);
    }
  }
}

uint8_t Safe_uint8_t::get() {
  uint8_t resul = 0x00;
  if (u8_semaphore != nullptr) {
    if (xSemaphoreTake(u8_semaphore, portMAX_DELAY) == pdTRUE) {
      resul = this->value;
      xSemaphoreGive(u8_semaphore);
    }
  }
  return resul;
}