# SERIAL-BOOTLOADER (*only SGM*)

Version: 0.1.2-R1M
Version compilada ".elf" con las siguientes caracteristicas:

Modo Recovery pin GPIO_15 -> LOW

UART 1 -> SERIAL PORT
UART SPEED = 921600
GPIO 8 = TX
GPIO 9 = RX

Para subir el "BOTLOADER" ejecuta: 

cd serial-bootloader\SGM

..\tool-openocd-orp2040\bin\openocd.exe -f interface/cmsis-dap.cfg -c "transport select swd" -c "adapter speed 5000" -f target/rp2040.cfg -c "program bootloader-0.1.2-R1M.elf verify reset exit"

Para subir el proyecto mediante el bootloader 
*Es necesario compilar el proyecto con el script de enlace

cd serial-bootloader
{COM} el puerto al que esta conectado

./serial-flash.exe COM28 ..\..\.pio\build\release_SGM\firmware.elf