# SERIAL-BOOTLOADER (*only SGB*)

Version: "rb0.0.1" view: https://github.com/axender20/BT-SGB.git
Version compilada ".elf" con las siguientes caracteristicas:

Modo Recovery pin GPIO_12 -> LOW

UART 1 -> SERIAL PORT
UART SPEED = 921600
GPIO 8 = TX
GPIO 9 = RX

# Para subir el "BOTLOADER" ejecuta: 

cd serial-bootloader/SGB

..\tool-openocd-orp2040\bin\openocd.exe -f interface/cmsis-dap.cfg -c "transport select swd" -c "adapter speed 5000" -f target/rp2040.cfg -c "program bootloader_rb0.0.1.elf verify reset exit"

# Para subir el proyecto mediante el bootloader 
*Es necesario compilar el proyecto en "release_SGB"

cd serial-bootloader
{COM} el puerto al que esta conectado

..\./serial-flash.exe COM28 ..\..\.pio\build\release_SGB\firmware.elf
