# ST7735 Readme

[![Website](https://img.shields.io/badge/Website-Link-blue.svg)](https://gavinlyonsrepo.github.io/)  [![Rss](https://img.shields.io/badge/Subscribe-RSS-yellow.svg)](https://gavinlyonsrepo.github.io//feed.xml)  [![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)

![ ig ](https://github.com/gavinlyonsrepo/pic_16F18346_projects/blob/master/images/st7735/pcb.jpg)

## Table of contents

  * [Overview](#overview)
  * [Test](#test)
  * [Software](#software)
      * [User Options](#user-options)
      * [Bitmap](#bitmap)
  * [Hardware](#hardware)
  * [Output](#output)

## Overview

* Name: ST7735_TFT_PICO
* Description:

0. Library for a TFT SPI LCD, ST7735 Driver
1. Raspberry pi PICO RP2040 library.
2. Inverse colour, rotate, sleep, idle  & vertical scroll modes supported.
3. 12 fonts included
4. Graphics + print class included.
5. Sprites, 1,16 and 24 bit colour Bitmaps supported.
6. Hardware and software  SPI

* Version:  1.7.2
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
| HELLO | Hello world  | --- |
| TEXT | Text  + fonts | --- |
| graphics| Graphics | --- |
| FUNCTIONS_FPS| Functions(like rotate scroll) + FPS test 702| --- |
| BMP_DATA | 1,16 & 24 bit bitmaps tests + FPS test 701| Bitmap data is stored in arrays on PICO |


## Software


### User Options

In the main.cpp example files. There are four sections in "Setup()" function 
where user can make adjustments to select for SPI type used, PCB type used and screen size.


0. USER OPTION 0 SPI_SPEED + TYPE 
1. USER OPTION 1 GPIO
2. USER OPTION 2 SCREEN SECTION 
3. USER OPTION 3 PCB_TYPE


*USER OPTION 0 SPI SPEED* 

Here the user can pass the SPI Bus freq in kiloHertz, Currently set to 8 Mhz.
Max SPI speed on the PICO is 62.5Mhz. There is a file with SPI test results for the FPS tests in extra/doc folder. "nd parameter is the SPI interface(spi0 spi1 etc). 

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

*USER OPTION 3 PCB Version*

Select your PCB controller type by passing an enum type to function  TFTInitPCBType.
Default is "TFT_ST7735R_Red".  If you select the wrong one if may still work but with inverted colors.

| Number | Description | Enum label| Tested on | 
| ---- | ---- | --- | --- |
| 1 | ST7735B controller| TFT_ST7735B | n/a | 
| 2 | ST7735R "Green Tab" | TFT_ST7735R_Green | n/a |
| 3 | ST7735R "Red Tab"   | TFT_ST7735R_Red | RED PCB v1.1, 1.44 , 128 x 128 pixels |
| 4 | ST7735S "Black Tab" | TFT_ST7735S_Black | RED PCB v1.2, 1.8 , 128 x 160 pixels |


### Bitmap

Functions to support drawing bitmaps, icons & sprites.

| Num | Function Name | Colour support | bitmap size Max |  Note |
| ------ | ------ | ------ | ------ | ------ |
| 1 | drawIcon | bi-colour | (8 x (0-Max_y)) 128 bytes max  | Data vertically addressed |
| 2 | drawBitmap | bi-colour | 2048 bytes  | Data horizontally  addressed |
| 3 | drawBitmap16Data | 16 bit color 565  | 32768  | Data from array on PICO |
| 4 | drawBitmap24Data  | 24 bit color  | 49152  | Data from array on PICO, Converted by software to 16-bit color | 
| 5 | drawSpriteData  | 16 bit color  565 | 32768  | Data from array on PICO, Draws background color tranparent | 


1. Bitmap size in kiloBytes = (screenWidth * screenHeight * bitsPerPixel)/(1024 * 8)
2. Math in bitmap size column 2-5  assumes 128x128 pixel screen.
3. The data array for 1 and 2 is created from image files using file data conversion tool [link](https://javl.github.io/image2cpp/)
4. The data array for 3 - 5  is created from BMP files using file data conversion tool [link](https://notisrac.github.io/FileToCArray/)

These functions will return error codes in event of an error, see  API docs for details.

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

1. NOTE connect LED backlight pin 1 thru a 150R/220R ohm resistor to 3.3/5V VCC.
2. This is a 3.3V logic device do NOT connect the I/O logic lines to 5V logic device.
3. You can connect VCC to 5V if there is a 3.3 volt regulator on back of TFT module.
4. SW SPI pick any GPIO you like , HW SPI SCLK and SDA will be tied to spio interface.
5. Backlight on/off control is left to user.

## Output

Output of some of the test routine's. Left to right

1. Bitmap (bi-color) A background and a foreground, 128 X 128 pixels.
2. Different sizes of default font. Size 2 3 4 & 5 shown.
3. Different Fonts at font size 2, fonts 1-6 All these fonts are scale-able
4. Graphics.
5. Clock Demo Test 11,  showing icons, and font 7 "BigNums"
6. 24 bit RGB test image displayed from a data array  ,  128 X 128 pixels.
7. 16 bit 565 test image displayed from a data array , 128 X 128 pixels.
8. Fonts 7-12 

![ pic ](https://github.com/gavinlyonsrepo/ST7735_TFT_PICO/blob/main/extra/doc/images/row1.jpg)
![ pic1 ](https://github.com/gavinlyonsrepo/ST7735_TFT_RPI/blob/main/extra/images/4.jpg)
![ p2](https://github.com/gavinlyonsrepo/ST7735_TFT_RPI/blob/main/extra/images/5.jpg)

