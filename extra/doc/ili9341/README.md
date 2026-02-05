# ILI9341 LCD TFT Readme

[![Image TFT](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ili9341.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ili9341.jpg)

## Table of contents

  * [Overview](#overview)
  * [Software](#software)
      * [User Options](#user-options)
      * [File system](#file-system)
      * [Bitmap](#bitmap)
      * [Color codes](#color-codes)
  * [Hardware](#hardware)
  * [Output](#output)


## Overview

* Name: ILI9341
* Description:

C++ Raspberry pi PICO  library for a TFT SPI LCD,ILI9341 Driver

## Software

### User options

In the example files, there are 3 sections in "Setup()" function 
where user can make adjustments. 

 * User Option 0 Setup()       SPI Settings
 * User Option 1 Setup()       Setup the GPIO
 * User Option 2 Setup()       Screen Setup 

*User Option SPI Settings * 

Here the user can pass the SPI Bus freq in kiloHertz.
Max SPI speed on the PICO is 62.5Mhz. 2nd parameter is the SPI interface(spi0 spi1 etc). 
If users wants software SPI just call this method 
with just one argument for the optional GPIO software uS delay,
which by default is zero. Setting this higher can be used to slow down Software SPI 
which may be beneficial in some setups.  

*User Option Setup the GPIO pins

Set the five GPIO pins if using Hardware SPI clock and data pins will be tied to 
the chosen interface eg Spi0 CLK=18 DIN=19). The reset pin is optional if your display/hat uses
software reset , set to -1 to 'disable'.

*User Option Screen size *

User can adjust screen pixel height, screen pixel width.

### File system

There are example files included. User picks the one they want 
by editing the CMakeLists.txt :: add_executable(${PROJECT_NAME}  section.
Comment in one path and one path ONLY.

| # | example file name  | Description |
| ------ | ------ |  ------ |
| 1 | hello_world| Basic use case |
| 2 | text_graphic_functions | Tests text,graphics & function testing  |
| 3 | bmp_data | bitmap + FPS testing |
| 4 | demo | A demo |
| 5 | read_diag | Read diagnostic registers from ILI9341, needs extra GPIO: MISO |
| 6 | frame_buffer | Demo of advanced frame buffer mode |

### Bitmap

Functions to support drawing bitmaps

| Function Name | Colour support | Pixel max size KiB |  Note |
| ------ | ------ | ------ | ------ |
| drawBitmap | bi-colour array |  18.75  | Data horizontally  addressed |
| drawBitmap8Data | 8 bit color RRRGGGBB  | 75 |  Converted by software to 16-bit color |
| drawBitmap16 | 16 bit color 565 BMP files or array |  150  | ------ |
| drawSprite| 16 bit color 565 array |  150  | Does not use a buffer , draws pixel by pixel , ignores background chosen color|

1. Bitmap Size (in KiB)= Image Width×Image Height×Bits Per Pixel(8×1024)
2. Pixel size column assumes 240 by 320 screen.
3. The data array for 1 is created from image files using file data conversion tool [link](https://javl.github.io/image2cpp/)
4. The data array for 2-4  is created from BMP files using file data conversion tool [link](https://notisrac.github.io/FileToCArray/)

## Hardware

Tested and developed on:

* Size 2.4 inch SPI Serial  IPS color TFT LCD
* Resolution: 240 (H) RGB x 320 (V)
* Color Depth: 262K/65K (65K used)
* Control chip:ILI9341
* Display area 27.972 (H) x 32.634 (V)
* Logic voltage 3.3V
* Touch panel with XPT2046 IC

Connections as setup in main.cpp hello world test file.

| PinNum | Pin description | PICO| note |
| --- | --- | --- | --- |
| 1 | VCC | VCC | 3.3 or 5V ,CAUTION your display must have 3.3V regulator on back to connect to 5V |
| 2 | GND | GND | |
| 3 | CS | GPIO2 |Chip select,Use any GPIO for this,  see point 4 at bottom of table |
| 4 | RESET | GPIO3 |Reset, Use any GPIO for this, If no reset pin, pass -1 in here & display will use software rst|
| 5 | DC | GPIO4 | Data or command, Use any GPIO for this line |
| 6 | SDI(MOSI) | SPI_MOSI (Spi0 GPIO18)| HW SPI tied to interface, SW SPI any GPIO |
| 7 | SCLK | SPI_CLK (Spi0 GPIO19) | HW SPI tied to interface, SW SPI any GPIO |
| 8 | LED | VCC |CAUTION Your display may need current limit resistor|
| 9 | SDO(MISO) | SPI_MISO (spi0 GPIO16) |HW SPI tied to interface, SW SPI any GPIO, Only needed to read diagnostics from TFT|
| 10| T_CLK | SPICLK | Only needed for xpt2046 touchscreen(not implemented yet) |
| 11| T_CS | SPICE1 |Only needed for xpt2046 touchscreen(not implemented yet)XPT2046 Chip select |
| 12| T_DIN | SPIMOSI |Only needed for xpt2046 touchscreen(not implemented yet)|
| 13 | T_DO | SPIMISO |Only needed for xpt2046 touchscreen(not implemented yet)|
| 14 | T_IRQ | GPIO22 |Only needed for xpt2046 touchscreen(not implemented yet) |

1. This is a 3.3V logic device do NOT connect the I/O logic lines to 5V logic device.
2. LED Backlight control is left to user.
3. Pins marked with T_ prefix are related to the touchscreen IC XP2046 if user is not using the touch
screen do not connect these. The touch screen and TFT share the SPI bus but have different chip select lines. TFT SPI settings(Speed, active chip select) should be refreshed after ever read cycle of XPT2046 sensor, see example.
4. To operate without CS pin: pass -1 as argument for CS pin number in SetupGPIO()functions
   and ground the CS pin on device side.
5. If no Reset pin present: pass -1 as argument for Reset pin number in SetupGPIO()functions

## Output

[![output pic 1](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ili9341output3.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ili9341output3.jpg)


[![output pic 2](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ili9341output2.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ili9341output2.jpg)

