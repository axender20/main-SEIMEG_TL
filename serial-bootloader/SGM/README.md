# SERIAL-BOOTLOADER (*only SGM*)

Version: "rm0.0.1" view: https://github.com/axender20/BT-SGM.git
Version compilada ".elf" con las siguientes caracteristicas:

Modo Recovery pin GPIO_15 -> LOW

UART 1 -> SERIAL PORT
UART SPEED = 921600
GPIO 8 = TX
GPIO 9 = RX

# Para subir el "BOTLOADER" ejecuta: 

cd serial-bootloader\SGM

..\tool-openocd-orp2040\bin\openocd.exe -f interface/cmsis-dap.cfg -c "transport select swd" -c "adapter speed 5000" -f target/rp2040.cfg -c "program bootloader_rm0.0.1.elf verify reset exit"

# Para subir el proyecto mediante el bootloader 
*Es necesario compilar el proyecto en "release_SGM"

cd serial-bootloader\SGM
{COM} el puerto al que esta conectado

..\./serial-flash.exe COM28 ..\..\.pio\build\release_SGM\firmware.elf