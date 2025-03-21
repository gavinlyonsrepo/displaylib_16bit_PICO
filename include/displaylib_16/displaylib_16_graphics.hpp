/*!
	@file     displaylib_16_graphics.hpp
	@author   Gavin Lyons
	@brief    header file for graphics library. This file handles the graphic methods
*/

#pragma once

// Section: Includes
#include <cstring> //strlen
#include <cstdio>
#include <cstdlib>
#include "hardware/spi.h"
#include "displaylib_16_Font.hpp"
#include "displaylib_16_Print.hpp"

// Section defines

/*!
	@brief Class to handle fonts and graphics of ST7735 display
 */
class displaylib_16_graphics : public Print
{

public:
	displaylib_16_graphics();
	~displaylib_16_graphics(){};

	// Enums
	/*!
	 * @brief 16-bit color definitions (RGB565 format).
	 */
	enum  pixel_color565_e : uint16_t
	{
		C_BLACK   = 0x0000, /**< Black */
		C_BLUE    = 0x001F, /**< Blue */
		C_RED     = 0xF800, /**< Red */
		C_GREEN   = 0x07E0, /**< Green */
		C_CYAN    = 0x07FF, /**< Cyan */
		C_MAGENTA = 0xF81F, /**< Magenta */
		C_YELLOW  = 0xFFE0, /**< Yellow */
		C_WHITE   = 0xFFFF, /**< White */
		C_TAN     = 0xED01, /**< Tan */
		C_GREY    = 0x9CD1, /**< Grey */
		C_BROWN   = 0x6201, /**< Brown */
		C_DGREEN  = 0x01C0, /**< Dark Green */
		C_ORANGE  = 0xFC00, /**< Orange */
		C_NAVY    = 0x000F, /**< Navy */
		C_DCYAN   = 0x03EF, /**< Dark Cyan */
		C_MAROON  = 0x7800, /**< Maroon */
		C_PURPLE  = 0x780F, /**< Purple */
		C_OLIVE   = 0x7BE0, /**< Olive */
		C_LGREY   = 0xC618, /**< Light Grey */
		C_DGREY   = 0x7BEF, /**< Dark Grey */
		C_GYELLOW = 0xAFE5, /**< Greenish Yellow */
		C_PINK    = 0xFC18, /**< Pink */
		C_LBLUE   = 0x7E5F, /**< Light Blue */
		C_BEIGE   = 0xB5D2  /**< Beige */
	};
	/*! TFT rotate modes in degrees*/
	enum  display_rotate_e : uint8_t
	{
		Degrees_0 = 0, /**< No rotation 0 degrees*/
		Degrees_90,    /**< Rotation 90 degrees*/
		Degrees_180,   /**< Rotation 180 degrees*/
		Degrees_270    /**< Rotation 270 degrees*/
	};
	/*! Font type 1-12 */
	enum Font_Type_e : uint8_t
	{
		Font_Default = 1,	/**< Default Font, Full extended ASCII */
		Font_Thick = 2,		/**< Thick font , no lower case letters*/
		Font_Seven_Seg = 3,	/**< Seven Segment Font */
		Font_Wide = 4,		/**< Wide font no lowercase letters*/
		Font_Tiny = 5,		/**< Tiny font */
		Font_HomeSpun = 6,	/**< HomeSpun Font */
		Font_Bignum = 7,		/**< Bignum numbers only */
		Font_Mednum = 8,		/**< Mednum number only */
		Font_ArialRound = 9, /**< Arial round font */
		Font_ArialBold = 10, /**< Arial bold font */
		Font_Mia = 11,		/**< Mia font */
		Font_Dedica = 12		/**< dedica font */
	};


	virtual size_t write(uint8_t);
	/*! @brief define in the sub class */
	virtual void setAddrWindow(uint16_t, uint16_t, uint16_t, uint16_t) = 0;
	void fillScreen(uint16_t color);
	void setCursor(int16_t x, int16_t y);

	// Shapes and lines
	void drawPixel(uint16_t, uint16_t, uint16_t);
	void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
	void drawFastVLine(uint16_t x, uint16_t y, uint16_t h, uint16_t color);
	void drawFastHLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color);

	void drawRectWH(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
	DisLib16::Ret_Codes_e fillRectBuffer(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
	void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

	void drawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color);
	void fillRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color);

	void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
	void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

	void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
	void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

	// Text
	void setTextWrap(bool w);
	void FontNum(Font_Type_e FontNumber);
	DisLib16::Ret_Codes_e drawChar(uint16_t x, uint16_t y, uint8_t c, uint16_t color, uint16_t bg, uint8_t size);
	DisLib16::Ret_Codes_e drawText(uint16_t x, uint16_t y, char *_text, uint16_t color, uint16_t bg, uint8_t size);
	DisLib16::Ret_Codes_e drawChar(uint16_t x, uint16_t y, uint8_t c, uint16_t color, uint16_t bg);
	DisLib16::Ret_Codes_e drawText(uint16_t x, uint16_t y, char *pText, uint16_t color, uint16_t bg);
	void setTextColor(uint16_t c);
	void setTextColor(uint16_t c, uint16_t bg);
	void setTextSize(uint8_t s);

	// Bitmap & Icon
	DisLib16::Ret_Codes_e drawIcon(uint16_t x, uint16_t y, uint16_t w, uint16_t color, uint16_t bgcol, const std::span<const uint8_t> data);
	DisLib16::Ret_Codes_e drawSpriteData(uint16_t x, uint16_t y, const std::span<const uint8_t> data, uint16_t w, uint16_t h, uint16_t bgColor, bool printBg);
	DisLib16::Ret_Codes_e drawBitmap(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t col, uint16_t bgcol, const std::span<const uint8_t> data);
	DisLib16::Ret_Codes_e drawBitmap8Data(uint16_t x, uint16_t y, const std::span<const uint8_t> data, uint16_t w, uint16_t h);
	DisLib16::Ret_Codes_e drawBitmap16Data(uint16_t x, uint16_t y, const std::span<const uint8_t> data, uint16_t w, uint16_t h);
	DisLib16::Ret_Codes_e drawBitmap24Data(uint16_t x, uint16_t y, const std::span<const uint8_t> data, uint16_t w, uint16_t h);



protected:
	void pushColor(uint16_t color);
	uint16_t Color565(uint16_t, uint16_t, uint16_t);
	///@cond
	void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
	void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
	///@endcond
	void writeCommand(uint8_t);
	void writeData(uint8_t);
	void spiWrite(uint8_t);
	void spiWriteSoftware(uint8_t spidata);
	void spiWriteDataBuffer(uint8_t *spidata, uint32_t len);

	bool _wrap = true;				/**< wrap text around the screen on overflow*/
	uint16_t _textcolor = 0xFFFF;	/**< ForeGround color for text*/
	uint16_t _textbgcolor = 0x0000; /**< BackGround color for text*/
	uint8_t _textSize = 1;			/**< Size of text , fonts 1-6 only*/

	int16_t _cursorX = 0; /**< Current pixel column postion of Cursor*/
	int16_t _cursorY = 0; /**< Current pixel row position of Cursor*/
	uint8_t _XStart;      /**< Records column offset postion */
	uint8_t _YStart;      /**< Records row offset postion */
	uint16_t _width;   /**< Records width TFT postion */
	uint16_t _height;  /**< Records Height TFT postion */

	int8_t _display_DC;	   /**< GPIO Data or command line */
	int8_t _display_RST;   /**< GPIO Reset line */
	int8_t _display_CS;	   /**< GPIO Chip select line */
	int8_t _display_SCLK;  /**< GPIO Clock SPI Line */
	int8_t _display_SDATA; /**< GPIO MOSI data in  SPI Line */

	bool _hardwareSPI;			  /**< True for Hardware SPI on , false fpr Software SPI on*/
	spi_inst_t *_pspiInterface;	  /**< SPI instance pointer*/
	uint16_t _speedSPIKHz;		  /**< SPI speed value in kilohertz*/
	uint16_t _SWSPIGPIODelay = 0; /**< uS GPIO Communications delay, SW SPI ONLY */
private:
	///@cond
	inline void swapint16t(int16_t& a, int16_t& b) 
	{
		int16_t t = a;
		a = b;
		b = t;
	}
	uint16_t convert8bitTo16bit(uint8_t RRRGGGBB); 

	///@endcond
	
	/*! Width of the font in bits each representing a bytes sized column*/
	enum TFT_Font_width_e : uint8_t
	{
		Font_width_3 = 3,  /**< 3 tiny font */
		Font_width_4 = 4,  /**< 4 seven segment font */
		Font_width_5 = 5,  /**< 5 default font */
		Font_width_6 = 6,  /**< dedica font  */
		Font_width_7 = 7,  /**< 7 homespun & thick font*/
		Font_width_8 = 8,  /**< 8 wide & mia font*/
		Font_width_16 = 16 /**< 16 font 7-10*/
	};

	/*! font offset in the ASCII table*/
	enum TFT_Font_offset_e : uint8_t
	{
		Font_offset_none = 0x00,	 /**< extended ASCII */
		Font_offset_space = 0x20, /**< Starts at Space, alphanumeric */
		Font_offset_minus = 0x2D, /**< Starts at Minus, extended numeric */
		Font_offset_zero = 0x30	 /**< Starts at zero, numeric */
	};

	/*! Height of the font in bits*/
	enum TFT_Font_height_e : uint8_t
	{
		Font_height_8 = 8,	/**< 8 bit font 1-6 at size 1*/
		Font_height_12 = 12, /**< 12 bit font 12 */
		Font_height_16 = 16, /**< 16 bit font 8, 10 11*/
		Font_height_24 = 24, /**< 24 bit font 9 */
		Font_height_32 = 32	/**< 32 bit font 7 */
	};

	/*! Number of ASCII characters in Font */
	enum FontLength_e : uint8_t
	{
		FontLenNumeric = 14,			/**< extended Numeric 0x2D - 0x3A */
		FontLenAlphaNumNoLCase = 59, /**< reduced Alphanumeric 0x20-0x5A*/
		FontLenAlphaNum = 95,		/**< Full Alphanumeric 0x20-0x7E */
		FontLenAll = 255				/**< Full Range  0-0xFF */
	};

	uint8_t _FontNumber = 1;		  /**< Store current font */
	uint8_t _CurrentFontWidth = 5;	  /**< Store current font width */
	uint8_t _CurrentFontoffset = 0;	  /**< Store current offset width */
	uint8_t _CurrentFontheight = 8;	  /**< Store current offset height */
	uint8_t _CurrentFontLength = 254; /**<Store current font number of characters */
};

// ********************** EOF *********************
