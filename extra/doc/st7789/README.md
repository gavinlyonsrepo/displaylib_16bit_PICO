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
  * [Notes](#notes)
     * [Display offsets](#display-offsets)

## Overview

* Name: ST7789_TFT_PICO
* Description:

C++ Raspberry pi PICO library for a TFT SPI LCD, ST7789 Driver

## Test

There are  example files included. User picks the one they want 
by editing the CMakeLists.txt :: add_executable(${PROJECT_NAME}  section.
Comment in one path and one path only.

| Filename  | Function  | Note |
| --- | --- | --- |
| hello | Hello world  | --- |
| text | Text  + fonts | --- |
| graphics| Graphics | --- |
| functions_fps| Functions(like rotate scroll) + FPS test | --- |
| bmp_data | 1,8 & 16 bit colour bitmaps tests + bitmap FPS test | Bitmap data is stored in arrays on PICO |
| demos| A demo  | --- |

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
of spi interface chosen in option 0 if using hardware SPI. If no reset pin on display or hat, 
pass -1 as argument to disable reset pin.

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
| 1 | drawBitmap | bi-colour | 2048 bytes  | Data horizontally  addressed |
| 2 | drawBitmap8Data | 8 bit color RRRGGGBB  | 16384  | Data from array on PICO, Converted by software to 16-bit color |
| 3 | drawBitmap16Data | 16 bit color 565  | 32768  | Data from array on PICO |
| 4 | drawSpriteData  | 16 bit color  565 | 32768  | Data from array on PICO, Draws background color transparent | 


1. Bitmap size in kiloBytes = (screenWidth * screenHeight * bitsPerPixel)/(1024 * 8)
2. Math in bitmap size column assumes 128x128 bitmap.
3. The data array for 1 is created from image files using file data conversion tool [link](https://javl.github.io/image2cpp/)
4. The data array for 2-4 is created from BMP files using file data conversion tool [link](https://notisrac.github.io/FileToCArray/)

## Hardware

Connections as setup in main.cpp  test file.

| TFT PinNum | Pindesc |  HW SPI |
| --- | --- | --- |
| 1 | LED | VCC |
| 2 | SCLK | GPIO18 |
| 3 | SDA | GPIO19 |
| 4 | A0/DC | GPIO3  |
| 5 | RESET | GPIO4 |
| 6 | SS/CS | GPIO2 |
| 7 | GND | GND |
| 8 | VCC |  VCC |

1. NOTE connect LED backlight pin 1 thru a resistor to 3.3/5V VCC.
2. This is a 3.3V logic device do NOT connect the I/O logic lines to 5V logic device.
3. You can connect VCC to 5V if there is a 3.3 volt regulator on back of TFT module.
4. SW SPI pick any GPIO you like , HW SPI SCLK and SDA will be tied to spio interface.
5. Backlight on/off control is left to user.
6. If no reset pin on display or hat, pass -1 as argument to disable.
7. To operate without CS pin: pass -1 as argument for CS pin number in SetupGPIO()function
   and ground the CS pin on device side.

## Output

[![output pic](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7789output.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7789output.jpg)

## Notes

### Display offsets

The display initialisation requires an offset calculation which differs for different size and resolution displays.
This is in the code(Function AdjustWidthHeight()) but the many different size displays are not available for testing or dealt with.
If using a display other than 240x320(the default and size of ST7789 video ram , VRAM) and if user finds they cannot address all screen
or their data is offset. Try Setting the pixel width and height of your screen to 240X320 and do not write as 
much as possible to the part of the VRAM you cannot see.
For example  if you have a 240X280 display in 0 degree rotation, the VRAM is same for ALL displays sizes.
1. Set pixel Width = 240 and pixel height = 320
2. Do not write to the missing 40 pixels in the Y-axis, you still can but it is inefficient.

[![pic ](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/offset_st7789.png)](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/offset_st7789.png)

Also if you have a 240x135 size display and you see the issue described in 
[github issue 10 ](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/issues/10) 
You can alternately apply the fix there to function AdjustWidthHeight().
