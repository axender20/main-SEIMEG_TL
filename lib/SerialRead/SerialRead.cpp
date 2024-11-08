#include "SerialRead.h"

bool Uart_comunication::valid_initial_data() {
  //Validaciones
  if (!(buffer.length() > 0) || buffer == nullptr) return false;

  uint32_t check_sum = buffer.indexOf(check_sum_character);

  if (!(check_sum > 0)) return false;

  if (!(buffer.length() > 3)) return false;
  return true;
}

bool Uart_comunication::valid_check_sum() {
  uint32_t index_check_sum = buffer.indexOf(check_sum_character);

  int checkSum = buffer.substring(index_check_sum + 1, buffer.length()).toInt();

  if (!(checkSum > 0)) { return false; }

  buffer = buffer.substring(0, index_check_sum);

  uint8_t myCheckSum = 0;
  for (size_t i = 0; i < buffer.length(); i++) {
    myCheckSum += uint8_t(buffer[i]);
  }

  if (!(myCheckSum == checkSum)) { return false; }

  return true;
}

uint8_t Uart_comunication::get_new_data(String& buff) {
  buffer = "";

  buffer = _port->readString();

  if (!valid_initial_data())
    return comunication_errors::nack_checksum_error;

  if (!valid_check_sum())
    return comunication_errors::comunication_error;

  buff = buffer;

  return comunication_errors::success;
}

bool Uart_comunication::is_available() {
  if (_port->available())
    return true;

  return false;
}