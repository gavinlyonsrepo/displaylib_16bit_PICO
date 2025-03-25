# SSD1331 OLED Readme

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

* Name: SSD1331
* Description:

C++ Raspberry pi PICO library for a TFT SPI LCD, SSD1331 Driver.
2Contrast , dim mode, Invert colour, scroll, rotate, sleep modes supported.
SSD1331 96x 64 RGB Dot Matrix OLED/PLED Segment/Common Driver with Controller.

* Author: Gavin Lyons

## Software

### User options

In the example files, there are 3 sections in "Setup()" function 
where user can make adjustments. The constructor also takes a number of arguments.

 * User Option constructor() Color and contrast settings
 * User Option Setup()       SPI Settings
 * User Option Setup()       Setup the GPIO
 * User Option Setup()       Screen Setup 

*User Option  Color and contrast settings*

Here the user can select RGB or BGR mode depending on type of display, They can also
set the color contrast levels for the 3 channels (A B and C) for normal and Dim mode.
The range is 0-255. 

*User Option SPI Settings * 

Here the user can pass the SPI Bus freq in kiloHertz.
Max SPI speed on the PICO is 62.5Mhz. 2nd parameter is the SPI interface(spi0 spi1 etc). 
If users wants software SPI just call this method 
with just one argument for the optional GPIO software uS delay,
which by default is zero. Setting this higher can be used to slow down Software SPI 
which may be beneficial in some setups.  

*User Option Setup the GPIO pins

Set the five GPIO pins if using Hardware SPI clock and data pins will be tied to 
the chosen interface eg Spi0 CLK=18 DIN=19)

*User Option Screen size *

User can adjust screen pixel height, screen pixel width.

### File system

There are  example files included. User picks the one they want 
by editing the CMakeLists.txt :: add_executable(${PROJECT_NAME}  section.
Comment in one path and one path only.

| # | example file name  | Desc|
| ------ | ------ |  ------ |
| 1 | hello| hello world Basic use case |
| 2 | bitmap_functions | bitmap tests & function testing |
| 3 | demo | radar demo |

### Bitmap

Functions to support drawing bitmaps, icons & sprites.

| Num | Function Name | Colour support | bitmap size Max |  Note |
| ------ | ------ | ------ | ------ | ------ |
| 1 | drawIcon | bi-colour | (8 x (0-Max_y)) --  | Data vertically addressed |
| 2 | drawBitmap | bi-colour | 768 bytes  | Data horizontally  addressed |
| 3 | drawBitmap8Data | 8 bit color RRRGGGBB  | 6144  | Data from array on PICO, Converted by software to 16-bit color |
| 4 | drawBitmap16Data | 16 bit color 565  | 12288  | Data from array on PICO |
| 5 | drawBitmap24Data  | 24 bit color  | 18432  | Data from array on PICO, Converted by software to 16-bit color | 
| 6 | drawSpriteData  | 16 bit color  565 | 12288 | Data from array on PICO, Draws background color transparent | 


1. Bitmap size in kiloBytes = (screenWidth * screenHeight * bitsPerPixel)/(1024 * 8)
2. Math in bitmap size column 2-6  assumes 96x64 pixel screen.
3. The data array for 1 and 2 is created from image files using file data conversion tool [link](https://javl.github.io/image2cpp/)
4. The data array for 3 - 6  is created from BMP files using file data conversion tool [link](https://notisrac.github.io/FileToCArray/)



## Hardware

Connections as setup in main.cpp test file.

| TFT PinNum | Pindesc | RPI HW SPI | RPI SW SPI |
| --- | --- | --- | --- |
| 1 | GND | GND | GND |
| 2 | VCC | VCC 3.3V | VCC 3.3V  |
| 3 | SCLK | SPI_CLK | GPIO6 |
| 4 | SDA | SPI_MOSI | GPIO5 |
| 5 | RESET | GPI025  | GPIO25 |
| 6 | DC | GPIO24 | GPIO24  |
| 7 | CS | SPI_CE0 | GPIO21 |

1. Pick any GPIO you want for SW SPI,  for HW SPI: reset and DC lines are flexible.
2. User can select  SPI_CE0  or SPI_CE1 for HW SPI

[![Image SSD](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ssd1331.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ssd1331.jpg)

## Output

[![ demo pic 2](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ssd1331output.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ssd1331output.jpg)


