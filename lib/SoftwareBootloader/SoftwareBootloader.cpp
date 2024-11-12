#include "SoftwareBootloader.h"
void do_reboot(bool to_bootloader) {
  hw_clear_bits(&watchdog_hw->ctrl, WATCHDOG_CTRL_ENABLE_BITS);
  if (to_bootloader) {
    watchdog_hw->scratch[5] = BOOTLOADER_ENTRY_MAGIC;
    watchdog_hw->scratch[6] = ~BOOTLOADER_ENTRY_MAGIC;
  }
  else {
    watchdog_hw->scratch[5] = 0;
    watchdog_hw->scratch[6] = 0;
  }
  watchdog_reboot(0, 0, 0);
  while (1) {
    tight_loop_contents();
    asm("");
  }
}

void entry_on_bootloader() {
#ifdef USE_BOOTLOADER
  DEBUG_PRINTLN("! Entry on BOOTLOADER_MODE");
  do_reboot(true);
#else
  DEBUG_PRINTLN("!No enable BOOTLOADER");
  delay(1000);
  rp2040.restart();
#endif
}

void software_reset() {
#ifdef USE_BOOTLOADER
  do_reboot(false);
#else
  rp2040.restart();
#endif
}