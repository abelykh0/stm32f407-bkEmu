# stm32f407-bkEmu
BK-0010 Emulator (using STM32F407 microcontroller)

![Breadboard](https://raw.githubusercontent.com/abelykh0/stm32f407-z80emu/master/doc/stm32f407-z80emu.jpg)

## What it can do
* Emulate BK-0010
* Load file in .BIN format from SD card

// TODO Video

## Installation
If you want to try my project, this is the only part that you need.

| Hardware      |    Qty|
| ------------- | -----:|
| [Black F407VET6](http://wiki.stm32duino.com/index.php?title=STM32F407) board | 1
| VGA connector | 1
| PS/2 Keyboard | 1
| Power supply 5V DC for keyboard | 1
| Resistors 470 Ohm | 3
| Resistors 680 Ohm | 3
| Resistors 2.2 KOhm | 2
| Resistors 3.3 KOhm | 2
| Breadboard | 1
| Jumper wires | 18
| ST-Link v2 or clone | 1

Software: Install free IDE [System Workbench for STM32](https://www.st.com/en/development-tools/sw4stm32.html/). I am using Windows 10, however STMicroelectronics claims that it also supports Linux and Mac.

How to connect wires:

| PIN | Description | Connect To | Output |
| --- | ----------- | ---------- | ------ |
| PE8  | Red 1   | Resistor 470 Ohm | VGA red (1)
| PE9  | Red 2   | Resistor 680 Ohm | VGA red (1)
| PE10 | Green 1 | Resistor 470 Ohm | VGA green (2)
| PE11 | Green 2 | Resistor 680 Ohm | VGA green (2)
| PE12 | Blue 1  | Resistor 470 Ohm | VGA blue (3)
| PE13 | Blue 2  | Resistor 680 Ohm | VGA blue (3)
| PD15 | HSync   | | VGA HSync (13)
| PD14 | VSync   | | VGA VSync (14)
| PB14 | CLK | Resistor 2K2 to keyboard CLK and resistor 3K3 to GND
| PB13 | DATA | Resistor 2K2 to keyboard DATA and resistor 3K3 to GND
| G | Ground | | VGA Ground (5,6,7,8,10), '-' of passive speaker 

## Third party software
This project uses several libraries (in addition to HAL drivers from STMicroelectronics):
* To display video using VGA: https://github.com/cbiffle/m4vgalib (which requires https://github.com/cbiffle/etl)
* BK-0010 emulator: https://github.com/emestee/bk-emulator
* FATFS for SD card: http://elm-chan.org/fsw/ff/00index_e.html

## Plans for the future / issues
* Load / save files in .COD format
* Save snapshot
* The speed is 12% faster than it is supposed to be
* Sound
