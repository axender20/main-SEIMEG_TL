#include "MAC_ADR.h"

uint8_t MAC_ADDRES[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

//TODO: Crear y agregar implementacion para la MAC

void load_mac_address() {
  //TODO: Obtener la direccion MAC almacenada en la memoria EEPROM
}

void NEW_write_mac_addres(uint8_t* NEW_MAC) {
  //? Example:

  /*
  uint8_t mxc[6] = { 0x3A, 0XF1, 0X22, 0XA1, 0X45, 0XFF};
  NEW_write_mac_addres(mxc);

  while (1) {
    tight_loop_contents();
  }
  */

  //TODO: Agregar implementacion para la memoria EEPROM
  // eeprom.writeBlock(_mxa_mac_addres, NEW_MAC, 6);
}

const char* get_mac_ctrs() {
  static char mac_address[18]; // Almacena la dirección MAC en formato de cadena
  snprintf(mac_address, sizeof(mac_address), "%02X:%02X:%02X:%02X:%02X:%02X",
    MAC_ADDRES[0], MAC_ADDRES[1], MAC_ADDRES[2], MAC_ADDRES[3], MAC_ADDRES[4], MAC_ADDRES[5]);
  return mac_address;
}
