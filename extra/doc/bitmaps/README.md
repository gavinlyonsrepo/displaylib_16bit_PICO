# Bitmap readme

Functions to support drawing bitmaps & sprites.

| Num | Function Name | Colour support | bitmap size | Note |
| ------ | ------ | ------ | ------ | ------ |
| 1 | drawBitmap | bi-colour | 2048 bytes | Data horizontally addressed |
| 2 | drawBitmap8Data | 8-bit color RRRGGGBB or 8-bit Greyscale GGGGGGGG | 16384 | Data from array on PICO, Converted by software to 16-bit color. greyScale param selects mode |
| 3 | drawBitmap16Data | 16 bit color 565 | 32768 | Data from array on PICO |
| 4 | drawSpriteData | 16 bit color  565 | 32768 | Data from array on PICO, Draws background color transparent |

1. Bitmap size in kiloBytes = (screenWidth X screenHeight X bitsPerPixel)/(1024 X 8)
2. Bitmap size in Bytes = (screenWidth X screenHeight X bitsPerPixel)/8
3. Math in bitmap size column 1-4 assumes 128x128 pixel screen.
4. The data arrays can be created from image files using file data [conversion tool link](https://github.com/gavinlyonsrepo/Guardian_LTSM)

Bit Per Pixel:

| Colour mode | Bits Per Pixel |
| ------ | ------ |
| bi-color | 1 |
| 8-bit RGB332 (RRRGGGBB) | 8 |
| 8-bit Greyscale (GGGGGGGG) | 8 |
| 16 bit | 16 |
| 24 bit | 24 |
