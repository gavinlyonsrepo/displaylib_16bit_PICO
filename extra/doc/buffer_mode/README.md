# Advanced Buffer Mode Readme

Advanced buffer mode allows the library to write to a global screen buffer (`_screenBuffer`) instead of directly writing to the VRAM of the display. This mode is off by default but can be enabled for specific use cases where buffering is advantageous.

Advanced buffer mode offers several key benefits. It enables double buffering, where all drawing operations are first performed in memory and then written to the display in a single operation. With batch drawing, multiple graphics operations can be completed before updating the screen, improving rendering efficiency. The memory-resident buffer also allows for custom effects like fading or blending to be applied before display. Additionally, since memory operations are faster than direct display writes, this approach leads to optimized performance for applications with frequent updates.

However, it comes with limitations. It requires significant heap memory and flushing the buffer to the display can introduce a performance overhead. Compatibility is also a concern on memory-constrained devices. Use advanced buffer mode when you need complex rendering, visual effects, or performance optimizations. For simpler applications where memory is limited, direct display drawing may be more suitable. Bear in mind many functions in normal
mode have some form of local buffered writes already. 

Some examples of SRAM usage for selected display sizes and PICO's. 

| Display size bytes | SRAM PICO | SRAM % used | 
| ---- | --- | --- |
| 96x64 12,288 | RP2040 264 KB| 5 | 
| 96x64 12,288 | RP2350 512 KB| 2 | 
| 240x320 153,600| RP2040 264 KB| 57| 
| 240x320 153,600| RP2350 512 KB| 30| 

## Enabling Advanced Buffer Mode

To enable advanced buffer mode, you need to define the macro `dislib16_ADVANCED_SCREEN_BUFFER_ENABLE`. This macro is located in the file `displaylib_16_graphics.hpp` (line 26 USER OPTION 2). By default, this macro is commented out or undefined. Once enabled, the library will use the `_screenBuffer` for many drawing operations instead of writing directly to the display VRAM.

## Usage

1. setBuffer, sets the buffer allocates memory for a buffer to cover entire screen, call at setup of Display operations.
2. destroyBuffer, destroys buffer de-allocates memory, call at end of Display operations.
3. clearBuffer, fills buffer with a colour, by default black.
4. writeBuffer, writes buffer in a single SPI buffered write to screen.

## Functions 

Once enabled The following functions will write to screen Buffer instead of 
VRAM of display.

1. drawPixel()
2. All Bitmap functions.
3. All Draw text functions IF **textCharPixelOrBuffer = true** 

The situation with graphic functions is complicated as some use 
fast draw methods of drawFastVLine() and drawFastHLine().
and fillRectBuffer uses a local buffer in function to write to display VRAM.
So depending on the graphic function code may write to screenBuffer and/or VRAM. 
Note fillRectBuffer is wrapped by fillScreen.

