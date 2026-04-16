# Bitmap readme

Functions to support drawing bitmaps & sprites.

| Num | Function Name | Colour support | bitmap size | colour depth | Note |
| ------ | ------ | ------ | ------ | ------ | ---- |
| 1 | drawBitmap | bi-colour | 2048 | 2 | Data horizontally addressed |
| 2 | drawBitmap4Data | 4-bit color RBGG-RBGG | 8192 | 16 | Data from array on PICO, Converted by software to 16-bit color. |
| 3 | drawBitmap8Data | 8-bit color RRRGGGBB | 16384 | 256 | Data from array on PICO, Converted by software to 16-bit color. |
| 4 | drawBitmap16Data | 16 bit color 565 | 32768 | 65536 | Data from array on PICO |
| 5 | drawSpriteData | 16 bit color  565 | 32768 | 65536 | Data from array on PICO, Draws background color transparent |

1. Bitmap size in kiloBytes = (screenWidth X screenHeight X bitsPerPixel)/(1024 X 8)
2. Bitmap size in Bytes = (screenWidth X screenHeight X bitsPerPixel)/8
3. Math in bitmap size column assumes 128x128 pixel screen.
4. The data arrays can be created from image files using file data [conversion tool link](https://github.com/gavinlyonsrepo/Guardian_LTSM) also written by author.

Bit Per Pixel:

| Colour mode | Bits Per Pixel |
| ------ | ------ |
| bi-color | 1 |
| 4-bit RBGG-RBGG | 4 |
| 8-bit RGB332 (RRRGGGBB) | 8 |
| 16 bit | 16 |

Available 4 bit color palette preview(drawBitmap4Data),
These are generated from a look up table
in code which can be tweaked or changed. With some minor changes to code
multiple look up tables could be added to give various palettes.

[![ 4bit pic ](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/rgb_4bit_pal.png)](https://github.com/gavinlyonsrepo/displaylib_16bit_PICO/blob/main/extra/image/rgb_4bit_pal.png)
