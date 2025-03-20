# ST7789 Readme

[![Website](https://img.shields.io/badge/Website-Link-blue.svg)](https://gavinlyonsrepo.github.io/)  [![Rss](https://img.shields.io/badge/Subscribe-RSS-yellow.svg)](https://gavinlyonsrepo.github.io//feed.xml)  [![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)

[![Image TFT](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7789.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7789.jpg)

## Table of contents

  * [Overview](#overview)
  * [Test](#test)
  * [Software](#software)
      * [User Options](#user-options)
      * [Bitmap](#bitmap)
  * [Hardware](#hardware)
  * [Output](#output)

## Overview

* Name: ST7789_TFT_PICO
* Description:

0. Library for a TFT SPI LCD, ST7789 Driver
1. Raspberry pi PICO RP2040 library.
2. Inverse colour, rotate, sleep modes supported.
3. 12 fonts included.
4. Graphics + print class included.
5. bi-color, 16 bit and 24 colour Bitmaps supported.
6. Hardware and software SPI

* Author: Gavin Lyons
* Developed on Toolchain:
	1. Raspberry pi PICO RP2040
	2. SDK(2.1.0) C++
	3. compiler G++ for arm-none-eabi(13.2.1)
	4. CMAKE(VERSION 3.18) , VScode(1.95.3)
	5. Linux Mint 22.1


## Test

There are  example files included. User picks the one they want 
by editing the CMakeLists.txt :: add_executable(${PROJECT_NAME}  section.
Comment in one path and one path only.

| Filename  | Function  | Note |
| --- | --- | --- |
| ST7789_TFT_HELLO | Hello world  | --- |
| ST7789_TFT_TEXT | Text  + fonts | --- |
| ST7789_TFT_GRAPHICS| Graphics | --- |
| ST7789_TFT_FUNCTIONS_FPS| Functions(like rotate scroll) + FPS test | --- |
| ST7789_TFT_BMP_DATA | 1, 16 & 24 bit colour bitmaps tests +  FPS test | Bitmap data is stored in arrays on PICO |


## Software


### User Options

In the main.cpp example files. There are four sections in "Setup()" function 
where user can make adjustments to select for SPI type used, PCB type used and screen size.


0. USER OPTION 0 SPI_SPEED + TYPE 
1. USER OPTION 1 GPIO
2. USER OPTION 2 SCREEN SECTION 


*USER OPTION 0 SPI SPEED* 

Here the user can pass the SPI Bus freq in kiloHertz, Currently set to 8 Mhz.
Max SPI speed on the PICO is 62.5Mhz. There is a file with SPI test results for the FPS tests in extra/doc folder. 2nd parameter is the SPI interface(spi0 spi1 etc). 

If users wants software SPI just call this method 
with just one argument for the optional GPIO software uS delay,
which by default is zero. Setting this higher can be used to slow down Software SPI 
which may be beneficial in  some setups.   

*USER OPTION 1 GPIO*

The 5 GPIO pins used, the clock and data lines must be the clock and data lines 
of spi interface chosen in option 0 if using hardware SPI.

*USER OPTION 2 Screen size  + Offsets*

In the main.cpp file, in USER OPTION 2 .
User can adjust screen pixel height, screen pixel width and x & y screen offsets.
These offsets can be used in the event of screen damage or manufacturing errors around edge 
such as cropped data or defective pixels.
The function TFTInitScreenSize sets them.


### Bitmap

Functions to support drawing bitmaps, 

| Num | Function Name | Colour support | test bitmap data size |  Note |
| ------ | ------ | ------ | ------ | ------ |
| 1 | drawIcon | bi-colour | (8 x (0-Max_y))  | Data vertically addressed |
| 2 | drawBitmap | bi-colour | 2048 bytes  | Data horizontally  addressed |
| 3 | drawBitmap16Data | 16 bit color 565  | 32768  | Data from array on PICO |
| 4 | drawBitmap24Data  | 24 bit color  | 49152  | Data from array on PICO, Converted by software to 16-bit color  |
| 5 | drawSpriteData  | 16 bit color  565 | 32768  | Data from array on PICO, Draws background color transparent | 


1. Bitmap size in kiloBytes = (screenWidth * screenHeight * bitsPerPixel)/(1024 * 8)
2. Math in bitmap size column 2-5  assumes 128x128 bitmap.
3. The data array for 1 and 2 is created from image files using file data conversion tool [link](https://javl.github.io/image2cpp/)
4. The data array for 3 and 5 is created from BMP files using file data conversion tool [link](https://notisrac.github.io/FileToCArray/)

These functions will return an error code in event of an error.

## Hardware

Connections as setup in main.cpp  test file.

| TFT PinNum | Pindesc |  HW SPI |
| --- | --- | --- | 
| 1 | LED | VCC |   
| 2 | SCLK | GPIO18 |
| 3 | SDA | GPIO19 |
| 4 | A0/DC |  GPIO3  |
| 5 | RESET |   GPIO17 |
| 6 | SS/CS |  GPIO2 |
| 7 | GND | GND |
| 8 | VCC |  VCC  |

1. NOTE connect LED backlight pin 1 thru a resistor to 3.3/5V VCC.
2. This is a 3.3V logic device do NOT connect the I/O logic lines to 5V logic device.
3. You can connect VCC to 5V if there is a 3.3 volt regulator on back of TFT module.
4. SW SPI pick any GPIO you like , HW SPI SCLK and SDA will be tied to spio interface.
5. Backlight on/off control is left to user.

## Output

[![output pic](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7789output.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7789output.jpg)


