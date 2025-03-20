# Fonts Readme

Font data table:

| num | enum name | Char size XbyY | ASCII range | Size bytes | Size Scale-able |
| ------ | ------ | ------ | ------ |  ------ | ----- |
| 1 | $_Default | 5x8 |0-0xFF, Full Extended|1275 |Y|
| 2 | $_Thick   | 7x8 |0x20-0x5A, no lowercase letters |406|Y|
| 3 | $_SevenSeg  | 4x8 |0x20-0x7A |360|Y|
| 4 | $_Wide | 8x8 |0x20-0x5A, no lowercase letters|464|Y|
| 5 | $_Tiny | 3x8 |0x20-0x7E |285|Y|
| 6 | $_Homespun  | 7x8 |0x20-0x7E |658|Y|
| 7 | $_Bignum | 16x32 |0x2D-0x3A ,0-10 - . / : |896|N|
| 8 | $_Mednum | 16x16 |0x2D-0x3A ,0-10 - . / :|448|N|
| 9 | $_ArialRound| 16x24 | 0x20-0x7E |4608|N|
| 10 | $_ArialBold | 16x16 |0x20-0x7E |3072|N|
| 11 | $_Mia| 8x16 | 0x20-0x7E |1520|N|
| 12 | $_Dedica | 6x12 |0x20-0x7E |1152|N|

1. $ = Font
2. A print class is available to print out many data types.
3. Fonts 1-6 are byte high(at text size 1) scale-able fonts, columns of padding added by SW.
4. Font 7-8 are large numerical fonts and cannot be scaled(just one size).
5. Fonts 9-12 Alphanumeric fonts and cannot be scaled(just one size)
These fonts are optional and can be removed from program
by commenting out the relevant TFT_OPTIONAL_FONT_X define in the  ST7735_TFT_Font.hpp file

Font Methods:

| Font num | Method | Size parameter | Notes |
| ------ | ------ | ------ |  ------ |
| 1-6 | drawChar|Y| draws single  character |
| 1-6 | drawText |Y| draws character array |
| 7-12 | drawChar|N| draws single  character |
| 7-12 | drawText|N| draws character array |
| 1-12 | print |~| Polymorphic print class which will print out many data types |

These  functions return a number in event of an error, such as wrong font chosen , 
ASCII character outside chosen fonts range, character out of screen 
bounds and invalid character array pointer object.
