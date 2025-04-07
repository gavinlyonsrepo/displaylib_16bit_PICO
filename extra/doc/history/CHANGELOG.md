# Changelog

* Version 1.5 Repository created  November 2022
	* Port of my Raspberry pi library ST7735_TFT_RPI version 1.5
	* [Github URL LINK](https://github.com/gavinlyonsrepo/ST7735_TFT_RPI)
	
* Version 1.6 April 2023
	* Minor update.
	* Doxygen Style Comments added, so an automated API can be created.
	* Refactor, Enums placed inside the  relevant Class. Font and BMP data moved to cpp files.

* Version 1.7 November 2023
	* Github pull request no 1 merged.
	* Hello world example added.
	* Font's number 9 & 10 added.

* Version 1.7.1 Jan 2024
	* Added fonts 11 & 12
	* Enhanced error handling on text and bitmap functions.
	* Increased Fonts 7&8 to 14 characters from 11.
	
* Version 1.7.2 Dec 2024
	* Added Sprite function as per github issue 5.
	* Replaced use of Malloc with row by row buffer writes.
	* Refactored fillRectangle to fillRectBuffer.

* Version 2.0.0 Mar 2025
	* Changed project name from ST7735_TFT_PICO to displaylib_16_PICO.
		The project will now support any 16-bit graphic display, that can be added.
	* Added support for ILI9341 & ST7789 TFT LCD , SSD1331 OLED.
	* Added new font system, see font readme for details.
	* Added advanced graphics mode.
	* Added support for 8-bit images(RRRGGGBB). These take up much less space
		in memory at expense of color resolution.

* Version 2.1.0 April 2025
	* Added Frame buffer mode