# GC9A01A Readme

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)

## Table of contents

  * [Overview](#overview)
  * [Test](#test)
  * [Software](#software)
      * [User Options](#user-options)
      * [Bitmap](#bitmap)
  * [Hardware](#hardware)
  * [Output](#output)

## Overview

* Name: GC9A01A
* Description:

C++ Raspberry pi PICO  library for a TFT SPI LCD, GC9A01A Driver.

GC9A01 is a 262,144-color single-chip SOC driver for a-TFT liquid crystal display with resolution of
240RGBx240 dots, comprising a 360-channel source driver, a 32-channel gate driver, 129,600 bytes
GRAM for graphic display data of 240RGBx240 dots, and power supply circuit.

1. Power modes, Invert , Scroll, Rotate, Bitmaps supported.
2. Hardware & software SPI
3. Tested on 240X240 Round Display


* [DataSheet](https://buydisplay.com/download/ic/GC9A01A.pdf)

## Test

There are example files included. User picks the one they want
by editing the CMakeLists.txt :: add_executable(${PROJECT_NAME}  section.
Comment in one path and one path only.

| Filename  | Function  | Note |
| --- | --- | --- |
| hello | Hello world  | --- |
| frame_buffer | Testing frame Buffer mode | dislib16_ADVANCED_SCREEN_BUFFER_ENABLE must be enabled user option 2 |
| bmp_data| 1,8 & 16 bit bitmaps tests + bitmap FPS tests| Bitmap data is stored in arrays on PICO |
| demos| A demo showing a round gauge | ---|
| text_graphics_functions| Text  + Graphics + Functions | --- |

## Software

### User Options

In the main.cpp example files. There are four sections in "Setup()" function
where user can make adjustments to select for SPI type used, PCB type used and screen size.


0. USER OPTION 0 SPI_SPEED + TYPE
1. USER OPTION 1 GPIO
2. USER OPTION 2 SCREEN SECTION


*USER OPTION 0 SPI SPEED*

Here the user can pass the SPI Bus freq in kiloHertz, Currently set to 8 Mhz.
Max SPI speed on the PICO is 62.5Mhz.
There is a file with SPI test results for the FPS tests in extra/doc folder.
2nd parameter is the SPI interface(spi0 spi1 etc).

If users wants software SPI just call this method
with just one argument for the optional GPIO software uS delay,
which by default is zero. Setting this higher can be used to slow down Software SPI
which may be beneficial in  some setups.

*USER OPTION 1 GPIO*

The 5 GPIO pins used, the clock and data lines must be the clock and data lines
of spi interface chosen in option 0 if using hardware SPI.

*USER OPTION 2 Screen size*

In the main.cpp file, in USER OPTION 2 .
User can adjust screen pixel height, screen pixel width.

### Bitmap

Functions to support drawing bitmaps & sprites.

| Num | Function Name | Colour support | bitmap size calc |  Note |
| ------ | ------ | ------ | ------ | ------ |
| 1 | drawBitmap | bi-colour | 2048 bytes  | Data horizontally addressed |
| 2 | drawBitmap8Data | 8 bit color RRRGGGBB  | 16384  | Data from array on PICO, Converted by software to 16-bit color |
| 3 | drawBitmap16Data | 16 bit color 565  | 32768  | Data from array on PICO |
| 4 | drawSpriteData  | 16 bit color  565 | 32768  | Data from array on PICO, Draws background color transparent |


1. Bitmap size in kiloBytes = (screenWidth * screenHeight * bitsPerPixel)/(1024 * 8)
2. Math in bitmap size column 1-4 assumes 128x128 bitmap size.
3. The data array for 1 is created from image files using file data conversion tool [link](https://javl.github.io/image2cpp/)
4. The data array for 2-4  is created from BMP files using file data conversion tool [link](https://notisrac.github.io/FileToCArray/)

## Hardware

[![pic ](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc1.png)](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc1.png)

Connections as setup in main.cpp test file.

| Pin desc |  HW SPI |
| --- | --- |
| GND Ground | GND |
| VCC |  3.3 VCC  |
| SCL Clock | GPIO 18 |
| SDA Data in | GPIO 19 |
| RST Reset| GPIO 4 |
| DC Data or command | GPIO 3  |
| CS Chip select | GPIO 2 |
| BL Backlight  | VCC |

1. This is a 3.3V logic device do NOT connect the I/O logic lines to 5V logic device.
2. SW SPI pick any GPIO you like , HW SPI SCLK and SDA will be tied to spio interface.
3. Backlight on/off control is left to user.

## Output

[![ Demo pic ](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc2.jpg)](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc2.jpg)
