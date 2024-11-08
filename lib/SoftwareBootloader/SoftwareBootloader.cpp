#include "SoftwareBootloader.h"

void entry_on_bootloader() {
#ifdef USE_BOOTLOADER
  DEBUG_PRINTLN("! Entry on BOOTLOADER_MODE");
  hw_clear_bits(&watchdog_hw->ctrl, WATCHDOG_CTRL_ENABLE_BITS);
  watchdog_hw->scratch[5] = ENTRY_MAGIC;
  watchdog_hw->scratch[6] = ~ENTRY_MAGIC;
  watchdog_reboot(0, 0, 0);
  while (1) tight_loop_contents();
#else
  DEBUG_PRINTLN("!No enable BOOTLOADER");
  delay(1000);
  rp2040.restart();
#endif
}