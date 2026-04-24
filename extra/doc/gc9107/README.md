# GC9107 Readme

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)

## Table of contents

* [Overview](#overview)
* [Test](#test)
* [Software](#software)
  * [User Options](#user-options)
* [Hardware](#hardware)

## Overview

* Name: GC9107
* Description:

C++ Raspberry pi PICO  library for a TFT SPI LCD, GC9107  Driver.

The GC9107 is a 262,144-color single-chip SOC driver for a-TFT liquid crystal display with resolution of 128RGBx160dots, comprising a 384-channel source driver, a 160-channel gate driver, 46,080 bytes GRAM for graphic display data of 128RGBx160 dots, and power supply circuit.

1. Power modes, Invert, Scroll, Rotate, Bitmaps supported.
2. Hardware & software SPI
3. Tested on 60x160 Bar Display

* [DataSheet](https://www.buydisplay.com/download/ic/GC9107.pdf)

## Test

There are example files included. User picks the one they want
by editing the CMakeLists.txt :: add_executable(${PROJECT_NAME}  section.
Comment in one path and one path only.

| Filename | Function | Note |
| --- | --- | --- |
| hello | Hello world | --- |
| frame_buffer | Testing frame Buffer mode | dislib16_ADVANCED_SCREEN_BUFFER_ENABLE must be enabled user option 2 |
| bmp_data | bitmaps tests | Bitmap data is stored in arrays on PICO |
| text_graphics_functions | Text + Graphics + Functions | --- |

## Software

### User Options

In the main.cpp example files. There are four sections in "Setup()" function
where user can make adjustments to select for SPI type used, PCB type used and screen size.

0. USER OPTION 0 SPI_SPEED + TYPE
1. USER OPTION 1 GPIO
2. USER OPTION 2 SCREEN SECTION

#### USER OPTION 0 SPI SPEED + TYPE

Here the user can pass the SPI Bus freq in kiloHertz, Currently set to 8 Mhz.
Max SPI speed on the PICO is 62.5Mhz.
There is a file with SPI test results for the FPS tests in extra/doc folder.
2nd parameter is the SPI interface(spi0 spi1 etc).

If users wants software SPI just call this method
with just one argument for the optional GPIO software uS delay,
which by default is zero. Setting this higher can be used to slow down Software SPI
which may be beneficial in  some setups.

#### USER OPTION 1 GPIO

The 5 GPIO pins used, the clock and data lines must be the clock and data lines
of spi interface chosen in option 0 if using hardware SPI.

#### USER OPTION 2 Screen size

In the main.cpp file, in USER OPTION 2 .
User can adjust Video RAM(VRAM) size (width & height) in pixels, memory base
and colour order (RGB or BGR)

There are two memory base variants called "Display data GRAM mapping" in datasheet
Which I refer to as model A and B, These require different init routines.

A. 128x128 mapping (MEMORY_BASE_GM_128x128)
B. 128x160 mapping (MEMORY_BASE_GM_128x160)

In addition there are number of different truncated displays on market ar type displays: Model A 128x115, Model B 96x160 and Model B 60x160. The user should treat these truncated displays as if they are 128x128 or 128x160 in terms of memory base and screen size setup, and just not use the pixels outside the visible area. This is because the controller is still treating the display as if it is full VRAM and the pixels outside the visible area are just not connected to anything. See table and offsets diagram below.

Known displays types on market with GC9107 controller:

| Display Type | Memory Base | VRAM size setup (WbyH) | Offsets | Tested |
| --- | --- | --- | --- | --- |
| A 128x128 Square | MEMORY_BASE_GM_128x128 | 128 , 128 | 0,0 | No device |
| A 128x115 Round Truncated | MEMORY_BASE_GM_128x128 | 128 , 128 | 0,7 | No device |
| B 60x160 Truncated | MEMORY_BASE_GM_128x160 | 128 , 160 | 34,0 | YES |
| B 96x160 Truncated | MEMORY_BASE_GM_128x160 | 128 , 160 | 16,0 | No device |
| B 128x160 Full | MEMORY_BASE_GM_128x160 | 128 , 160 | 0,0 | No device |

If using a truncated display:

1. Define the VRAM size as if it is a full display, for that model.
2. Set and use required offset.
3. Best not to use fillScreen() function as this will write to the whole VRAM including the pixels outside the visible area which is inefficient. Instead use fillRectBuffer() to fill the visible area of the display.
4. In portrait mode text wrap wil not work.
5. Certain error handling functions may not work as they are based on screen size but they are not critical for most applications.

Example Offset calculation for a truncated display(60x160) is as follows.

[![ offsets ](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc91.png)](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc91.png)

## Hardware

[![ display pic ](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc92.png)](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/gc92.png)

Connections as setup in main.cpp test file.

| Pin desc | HW SPI |
| --- | --- |
| GND Ground | GND |
| VCC | 3.3 VCC |
| SCL Clock | GPIO 18 |
| SDA Data in | GPIO 19 |
| RST Reset | GPIO 4 |
| DC Data or command | GPIO 3 |
| CS Chip select | GPIO 2 |
| BL Backlight | VCC thru resistor |

1. This is a 3.3V logic device do NOT connect the I/O logic lines to 5V logic device.
2. SW SPI pick any GPIO you like , HW SPI SCLK and SDA will be tied to spio interface.
3. Backlight on/off control is left to user.
4. If no Reset pin present: pass -1 as argument for Reset pin number in SetupGPIO()functions
5. To operate without CS pin: pass -1 as argument for CS pin number in SetupGPIO()function and ground the CS pin on device side.
