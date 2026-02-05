# GC9D01 Readme

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)

## Table of contents

  * [Overview](#overview)
  * [Test](#test)
  * [Software](#software)
	  * [User Options](#user-options)
	  * [Bitmap](#bitmap)
  * [Hardware](#hardware)
  * [Output](#output)
  * [Notes and Issues](#notes-and-issues)

## Overview

* Name: GC9D01N
* Author: Gavin Lyons
* Description:

C++ Raspberry pi PICO library library for a TFT SPI LCD, GC9D01N Driver.

GC9D01N is a 262,144-color single-chip SOC driver for a-TFT liquid crystal display with
resolution of 160RGBx160 dots, comprising a 240-channel source driver, a 32-channel gate
driver, 57,600 bytes GRAM for graphic display data of 160RGBx160 dots, and power supply
circuit.

1. Power modes, Invert, Scroll, Rotate supported.
2. Hardware & software SPI
3. Tested on 160x160 Round Display
also tested by third party on 40x160 bar display by third party.
4. Raspberry Pi PICO C++ SDK eco-system library.
5. 16 ASCII fonts included, fonts can easily be removed or added.
6. Advanced graphics class included.
7. Advanced frame buffer mode included.
8. Bitmaps supported: 1, 8 and 16 bit.
9. Sprites supported.

* [DataSheet](https://buydisplay.com/download/ic/GC9D01N.pdf)

## Test

There are example files included. User picks the one they want
by editing the CMakeLists.txt :: add_executable(${PROJECT_NAME}  section.
Comment in one path and one path only. Check Note below table.

| Filename | Function | Note |
| --- | --- | --- |
| hello | Hello world | for 160X160 display |
| hello 40X160 | Hello world | for 40x160 display |
| bmp data | 1,8 & 16 bit bitmaps tests + bitmap FPS tests | Bitmap data is stored in arrays on PICO |
| demo | A demo showing a round gauge | --- |
| text graphics functions | Text + Graphics + Functions | --- |

*Note:* All examples for GC9D01 are for advanced buffer mode. Advanced buffer mode must be enabled for these examples to work. See advanced buffer mode readme(link in main readme) for details. This mode is non-default.
User must enable advanced buffer mode by defining the macro `dislib16_ADVANCED_SCREEN_BUFFER_ENABLE` in the file `displaylib_16_graphics.hpp` (line 26 USER OPTION 2). Once enabled, the library will use the `_screenBuffer` for many drawing operations instead of writing directly to the display VRAM.
The reason for this is unresolved issues with pixel by pixel drawing in non-buffered mode in some functions and rotations. See arduino
[source port for details](https://github.com/gavinlyonsrepo/GC9D01_LTSM)

Some examples of SRAM usage for the frame buffer used in advanced buffer mode for selected display sizes and PICO's.

| Display size bytes | SRAM PICO | SRAM % used |
| ---- | --- | --- |
| 40x160 12,800 | RP2040 264 KB | 4.8 |
| 40x160 12,800 | RP2350 512 KB | 2.5 |
| 160x160 51,200 | RP2040 264 KB | 19.4 |
| 160x160 51,200 | RP2350 512 KB | 10 |

## Software

### User Options

In the main.cpp example files. There are four sections in "Setup()" function
where user can make adjustments to select for SPI type used, PCB type used and screen size.

0. USER OPTION 0 SPI_SPEED + TYPE
1. USER OPTION 1 GPIO
2. USER OPTION 2 SCREEN SECTION

#### USER OPTION 0 SPI SPEED

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

#### USER OPTIONS 2

There are options here user can adjust:

1. 2A screen pixel height
2. 2B screen pixel width
3. 2C Resolution and gate type.
4. 2D Pixel fix mode
5. 2E screen pixel X offset
6. 2F screen Pixel Y offset

*USER OPTION 2C- Resolution & Gate Configuration (Resolution_e)*

The GC9D01 controller supports multiple display sizes and gate driving modes.
Select the correct enum value for your physical module.
In addition to passing the parameter.

| Enum Value | Resolution | Gate Mode | Typical Module Description |
| ---------------------- | ---------- | ------ | --- |
| `RGB160x160_DualGate` | 160×160 | Dual | Most round GC9D01 displays **Default** |
| `RGB120x160_DualGate` | 120×160 | Dual | Some rectangular / bar-type modules |
| `RGB80x160_SingleGate` | 80×160 | Single | Smaller rectangular / bar displays |
| `RGB40x160_SingleGate` | 40×160 | Single | Very narrow bar-type or specialty modules |

*USER OPTION 2D – Pixel drawing workaround (PixelFixMode_e)*

This option does nothing in this library, User set to Off. See arduino
[source port for details ](https://github.com/gavinlyonsrepo/GC9D01_LTSM)
may implement in future versions.

*USER OPTION 2E and 2F*

On some displays an offset is required, I made this a user input
rather than hard-coded due to diversity of variant modules available.

| Display type | X offset | Y offset | Notes |
| ------------- | -------- | -------- | ----- |
| `160x160 Dual Gate` | 0 | 0 | No offsets required in any rotation. Default |
| `120x160 Dual Gate` | TBD | TBD | Expected to require centering offsets (~20 px) depending on rotation. Not hardware tested. |
| `80x160 Single Gate` | TBD | TBD | Expected to require centering offsets (~40 px) depending on rotation. Not hardware tested. |
| `40x160 Single Gate` | -60 | 60 | **Tested**. Offsets required for 90° and 270° rotations due to centered 40-pixel active area. |

### Bitmap

Functions to support drawing bitmaps & sprites.

| Num | Function Name | Colour support | bitmap size calc | Note |
| ------ | ------ | ------ | ------ | ------ |
| 1 | drawBitmap | bi-colour | 2048 bytes | Data horizontally addressed |
| 2 | drawBitmap8Data | 8 bit color RRRGGGBB | 16384 | Data from array on PICO, Converted by software to 16-bit color |
| 3 | drawBitmap16Data | 16 bit color 565 | 32768 | Data from array on PICO |
| 4 | drawSpriteData | 16 bit color  565 | 32768 | Data from array on PICO, Draws background color transparent |


1. Bitmap size in kiloBytes = (screenWidth * screenHeight * bitsPerPixel)/(1024 * 8)
2. Math in bitmap size column 1-4 assumes 128x128 bitmap size.
3. The data array for 1 is created from image files using file data conversion tool [link](https://javl.github.io/image2cpp/)
4. The data array for 2-4  is created from BMP files using file data conversion tool [link](https://notisrac.github.io/FileToCArray/)

## Hardware

[![pic ](https://github.com/gavinlyonsrepo/GC9D01_LTSM/blob/main/extras/images/gc2.jpg)](https://github.com/gavinlyonsrepo/GC9D01_LTSM/blob/main/extras/images/gc2.jpg)

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
| BL Backlight | VCC |

1. This is a 3.3V logic device do NOT connect the I/O logic lines to 5V logic device.
2. SW SPI pick any GPIO you like , HW SPI SCLK and SDA will be tied to spio interface.
3. Backlight on/off control is left to user.
4. If no Reset pin present: pass -1 as argument for Reset pin number in SetupGPIO()functions
5. To operate without CS pin: pass -1 as argument for CS pin number in SetupGPIO()function and ground the CS pin on device side.

## Output

160x160 Round Display:

[![pic2 ](https://github.com/gavinlyonsrepo/GC9D01_LTSM/blob/main/extras/images/output.jpg)](https://github.com/gavinlyonsrepo/GC9D01_LTSM/blob/main/extras/images/output.jpg)

40x160 Bar Display:

[![pic4 ](https://github.com/gavinlyonsrepo/GC9D01_LTSM/blob/main/extras/images/output2.jpg)](https://github.com/gavinlyonsrepo/GC9D01_LTSM/blob/main/extras/images/output2.jpg)

## Notes and Issues
