# Fonts Readme

There are 16 fonts packaged with library. Fonts can be added or removed by user.
All the Font data is in file displaylib_16_Font.cpp.

## Drawing method

There are two methods for drawing text. 
The character frame buffer method is default and is five times faster. 
Adjust which method is used using setTextCharPixelOrBuffer() function.

| num | method  | textCharPixelOrBuffer | Default| 
| ------ | ------ | ------ |  ------ | 
| 1 | Draw by pixel by pixel | true |  No | 
| 2 | Character frame buffer | false |  Yes | 

## dislib16_ADVANCED_FONTS_ENABLE

11 Fonts are enabled by default totaling 28760 bytes.
5 other fonts can be enabled by commenting in the macro:
'define dislib16_ADVANCED_FONTS_ENABLE' on line 17 of file displaylib_16_Font.hpp.
These additional fonts take 38084 bytes of memory. This macro is off by default.

## Font data table 

| num | Font enum name | character size XxY |  ASCII range | Size in bytes | Default Enabled|
| ------ | ------ | ------ | ------ |  ------ | ---- |
| 1 | font_default | 8x8 |  020-0x7E| 764 |Y |
| 2 | font_sinclairS  | 8x8 | 0x20-0x7E | 764 |Y |
| 3 | font_gll | 8x8 | 0x20-0x5A , NO lowercase letters | 476 |Y |
| 4 | font_pico | 8x12 | 0x20-0x7E  | 1144|Y |
| 5 | font_retro | 8x16 | 0x20-0x7E | 1524 |Y |
| 6 | font_mega | 16x16 | 0x20-0x7E | 3044 |Y |
| 7 | font_arialBold  | 16x16 | 0x20 0x7E |  3044 |Y |
| 8 | font_hallfetica | 16x16 | 0x20-0x7E | 3044 |Y |
| 9 | font_orla | 16x24 | 0x20-0x7E | 4564 |Y |
| 10 | font_arialRound| 16x24 | 0x20-0x7E | 4564 |Y |
| 11 | font_groTesk | 16x32 | 0x20-0x7A |  5828 |Y |
| 12 | font_inconsola | 24x32 | 0x20-0x7A | 8740 |N |
| 13 | font_mint | 24x32  | 0x20 -0x7A |  8740 |N |
| 14 | font_sixteenSeg | 32x48 | 0x2D-0x3A , 0-10 : . / - only | 2692 |N |
| 15 | font_sevenSeg | 32X50| 0x2D-0x3A , 0-10 : . / - only | 2804 |N |
| 16 | font_groTeskBig | 32X64| 0x20 - 0x5A, NO lowercase letters | 15108 |N |


Font size in bytes = ((X * (Y/8)) * numberOfCharacters) + (ControlBytes)

| Font class Function | Notes |
| ------ | ------ | 
| writeChar| draws single character |
| writeCharString | draws character array |
| print | Polymorphic print class which will print out many data types |

## Adding or removing a font

displaylib_16_Font.cpp file :

 * Add/remove the Font data array.
 * Add/remove the span to font data at the bottom of file.
 * Add/remove the font enum name from the switch case in function 'setFont'.

displaylib_16_Font.hpp file

 * Add/remove the associated *extern* span declaration for font.
 * Add/remove the font name from the enum 'display_Font_name_e'.

 * Re-compile library. 

A new ASCII font must have following font structure.
First 4 bytes are control bytes followed by **horizontally** addressed font data.
Also the fonts character width (or x-size) must be divisible evenly by 8.

```
// Example Font
// An 8 by 8 character size font starting at 
// ASCII offset 0x30 in ASCII table with 0x02 characters in font. 
// 0 and 1 , size 20 bytes, 4 Control bytes at start.
static const std::array<uint8_t, 20> FontBinaryExample =
{
0x08, 0x08, 0x30, 0x01,   // x-size, y-size, offset, (last character-offset : 0x31-0x30)
0x7C,0xC6,0xCE,0xD6,0xE6,0xC6,0x7C,0x00, // ASCII font data '0' : 0x30
0x18,0x38,0x18,0x18,0x18,0x18,0x7E,0x00 // ASCII font data  '1' : 0x31
};
```

## Sources

Fonts can be made from ttf files using [this tool](https://github.com/gavinlyonsrepo/Colossus_LTSM) also written by author.

Some of the fonts packaged with library came from [rinky dink electronics ](http://rinkydinkelectronics.com/)

## Font Images

Default 

[![Font Image](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/default.png)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/default.png)

Pico

[![Font Image](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/pico.png)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/pico.png)

SinclairS

[![Font Image](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/sinclair.png)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/sinclair.png)

Retro 

[![font image](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/retro.png)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/retro.png)

Arial Bold

[![Font image](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/arialbold.png)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/arialbold.png)

Hallfetica

[![font image](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/hall.png)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/hall.png)


Arial Round

[![font image](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/arialround.png)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/arialround.png)

Orla 

[![font image](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/orla.png)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/orla.png)

GroTesk

[![font image](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/grotesk.png)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/grotesk.png)

Inconsola

[![font image](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/inconsola.png)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/inconsola.png)

Mint

[![font image](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/mint.png)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/mint.png)

Sixteen Segment 

[![font image](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/ss.png)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/ss.png)

Seven  Segment 

[![font image](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/7seg.png)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/7seg.png)

GroTesk Big

[![font image](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/groteskbig.png)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/fonts/groteskbig.png)
