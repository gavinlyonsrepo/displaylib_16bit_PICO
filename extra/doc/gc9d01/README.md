# GC9D01 Readme

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)

## Table of contents

* [Overview](#overview)
* [Test](#test)
* [Software](#software)
  * [Setup](#setup)
    * [User option 1](#user-option-1)
    * [User option 2](#user-option-2)
* [Hardware](#hardware)
* [Output](#output)
* [Notes and Issues](#notes-and-issues)

## Overview

* Name: GC9D01N
* Author: Gavin Lyons
* Description:

C++ Raspberry pi PICO library library for a TFT SPI LCD, GC9D01N Driver.

GC9D01N is a 262,144-color single-chip SOC driver for a-TFT liquid crystal display with resolution of 160RGBx160 dots, comprising a 240-channel source driver, a 32-channel gate driver, 57,600 bytes GRAM for graphic display data of 160RGBx160 dots, and power supply circuit.

1. Power modes, Invert, Scroll, Rotate supported.
2. Hardware & software SPI
3. Tested on 160x160 Round Display
also tested by third party on 40x160 & 60x160 bar display by third party.
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
| hello 50X160 | Hello world | for 50x160 display |
| hello 60X160 | Hello world | for 60x160 display |
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

### Setup

In the example main.cpp files. There are sections in "Setup()" function
where user can make adjustments.

1. USER OPTION 1 GPIO, SPI Speed + type
2. USER OPTION 2 Screen Size, Offsets, resolution, Gate type and Missing Pixel Fix.

#### User option 1

Two different constructors which one is called depends on 'bhardwareSPI',
true for hardware spi, false for software SPI.

Hardware SPI:

Here the user can pass the SPI Bus freq in kiloHertz, Currently set to 8 Mhz.
Max SPI speed on the PICO is 62.5Mhz.
There is a file with SPI test results for the FPS tests in extra/doc folder.
2nd parameter is the SPI interface(spi0 spi1 etc).
The 5 GPIO pins used, the clock and data lines must be the clock and data lines
of spi interface chosen in option 0.

Software SPI:

The optional GPIO software uS delay,which by default is zero.
Setting this higher can be used to slow down Software SPI
which may be beneficial on some setups.
The 5 GPIO pins used. Any GPIO can be used for these.

#### User option 2

There are options here user can adjust:

1. 2A screen pixel height
2. 2B screen pixel width
3. 2C Resolution and gate type.
4. 2D Pixel fix mode
5. 2E screen pixel X landscape offset, 90 and 270 rotations.
6. 2F screen Pixel Y landscape offset, 90 and 270 rotations.
7. 2G screen pixel X portrait offset, 0 and 180 rotations.
8. 2H screen pixel Y portrait offset, 0 and 180 rotations.

##### User option 2C Resolution

The GC9D01 controller supports multiple display sizes and gate driving modes.
Select the correct enum value(see Resolution_e) for your physical module and pass as parameter.
In addition to passing the parameter. User must also set a Macro in "Section User Options" at top
of gc9107.hpp , Dual gate mode 160x160 is on by default.

```cpp
// ========== Section User Options===========
// Choose ONE and ONE ONLY (comment out the others)
#define   GC9D01_DUAL_INIT 1  // 160x160 default
//#define GC9D01_DUAL_INIT_120x160  1 // May not exist on market?
//#define GC9D01_SINGLE_INIT_40x160 1
//#define GC9D01_SINGLE_INIT_50x160 1
//#define GC9D01_SINGLE_INIT_60x160 1
//#define GC9D01_SINGLE_INIT_80x160 1  // May not exist on market?
//  ========== End Section User Options===========
```

##### User option 2D – Pixel drawing workaround (PixelFixMode_e)*

This option does nothing in this library, User set to Off. See arduino
[source port for details ](https://github.com/gavinlyonsrepo/GC9D01_LTSM)
may implement in future versions.

##### User option 2E-2H Offsets

On some truncated displays offset's are required, As the active pixel area is a subset of the display
controller video RAM.I made this a user input
rather than hard-coded due to diversity of variant modules available, and known unknowns.

| Display type | offset | Notes |
| --- | --- | --- |
| 160x160 Dual Gate | 0,0,0,0 | Round display, Full GRAM exposed. No offsets required in any rotation. *Default* |
| 120x160 Dual Gate | Unknown | Expected to require offsets (~20 px) depending on rotation. Not hardware tested |
| 80x160 Single Gate | Unknown | Expected to require offsets (~40 px) depending on rotation. Not hardware tested |
| 60x160 Single Gate | -40,50,10,0 | Offsets required in all rotation, tested by third party |
| 50x160 Single Gate | -39,55,16,0 | Offsets required in all rotations ,Not hardware tested |
| 40x160 Single Gate | -60,60,0,0 | Offsets required for 90° and 270° rotations due to centered 40-pixel active area. tested by third party |

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
