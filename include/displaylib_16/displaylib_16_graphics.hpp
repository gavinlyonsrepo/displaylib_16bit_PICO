/*!
	@file    displaylib_16_graphics.hpp
	@author  Gavin Lyons
	@brief   header file for graphics library. This file handles the graphic methods
	@details Define dislib16_ADVANCED_GRAPHICS_ENABLE enables advanced graphic 
		functions added in V2.0.0, see readme for more details.
*/

#pragma once

// Section: Includes
#include <cstring> //strlen
#include <cstdio>
#include "hardware/spi.h"
#include <vector>
#include "displaylib_16_Font.hpp"
#include "displaylib_16_Print.hpp"

// ================================================================
// ===== USER OPTION 1 turns on advanced graphics if commented in =
// default is on
#define dislib16_ADVANCED_GRAPHICS_ENABLE
// ================================================================
// ===== USER OPTION 2 turns on screenBuffer mode if commented in =
// default is off
//#define dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
// ================================================================

/*!
	@brief Class to handle fonts and graphics of ST7735 display
 */
class displaylib_16_graphics :public display_Fonts, public Print
{

public:
	displaylib_16_graphics();
	~displaylib_16_graphics(){};

	// Enums
	/*! @brief 16-bit color definitions (RGB565 format).*/
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

	// buffer screen mode functions
#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
	DisLib16::Ret_Codes_e setBuffer(void);
	DisLib16::Ret_Codes_e clearBuffer(uint16_t color = C_BLACK);
	DisLib16::Ret_Codes_e writeBuffer(uint8_t bufferMode = 0);
	DisLib16::Ret_Codes_e destroyBuffer(void);
#endif 
	// Screen functions
	/*! @brief define in the sub class */
	virtual void setAddrWindow(uint16_t, uint16_t, uint16_t, uint16_t) = 0;

	void fillScreen(uint16_t color);
	void setCursor(int16_t x, int16_t y);
	void drawPixel(uint16_t, uint16_t, uint16_t);
	// Graphics functions
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
// Advanced graphics functions
#ifdef dislib16_ADVANCED_GRAPHICS_ENABLE
	void drawEllipse(int16_t cx, int16_t cy, int16_t semiMajorAxis, int16_t semiMinorAxis, bool fill, uint16_t color);
	DisLib16::Ret_Codes_e drawDotGrid(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t DotGridGap, uint16_t color);
	void drawQuadrilateral(int16_t x0, int16_t y0,int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color);
	void fillQuadrilateral(int16_t x0, int16_t y0,int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color);
	DisLib16::Ret_Codes_e  drawPolygon(int16_t x, int16_t y, uint8_t sides, int16_t diameter, float rotation, bool fill , uint16_t color);
	void drawLineAngle(int16_t x, int16_t y, int angle, uint8_t start, uint8_t length, int offset, uint16_t color);
	void drawArc(uint16_t cx, uint16_t cy, uint16_t radius, uint16_t thickness, float startAngle, float endAngle, uint16_t color);
	float getArcAngleMax() const;
	void setArcAngleMax(float arcAngleMax);
	int getArcAngleOffset() const;
	void setArcAngleOffset(int arcAngleOffset);
#endif
	// Text
	virtual size_t write(uint8_t) override;
	void setTextWrap(bool w);
	void setTextColor(uint16_t c);
	void setTextColor(uint16_t c, uint16_t bg);
	DisLib16::Ret_Codes_e  writeChar( int16_t x, int16_t y, char value );
	DisLib16::Ret_Codes_e  writeCharString( int16_t x, int16_t y, char *text);
	void setTextCharPixelOrBuffer(bool mode);
	bool getTextCharPixelOrBuffer() const;
	// Bitmap functions
	DisLib16::Ret_Codes_e drawBitmap(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t col, uint16_t bgcol, const std::span<const uint8_t> data);
	DisLib16::Ret_Codes_e drawBitmap8Data(uint16_t x, uint16_t y, const std::span<const uint8_t> data, uint16_t w, uint16_t h);
	DisLib16::Ret_Codes_e drawBitmap16Data(uint16_t x, uint16_t y, const std::span<const uint8_t> data, uint16_t w, uint16_t h);
	DisLib16::Ret_Codes_e drawSpriteData(uint16_t x, uint16_t y, const std::span<const uint8_t> data, uint16_t w, uint16_t h, uint16_t bgColor, bool printBg);
protected:
	// SPI functions & variables
	void writeCommand(uint8_t);
	void writeData(uint8_t);
	void spiWrite(uint8_t);
	void spiWriteSoftware(uint8_t spidata);
	uint8_t spiReadSoftware(void);
	void spiWriteDataBuffer(uint8_t *spidata, uint32_t len);
	bool _hardwareSPI;			  /**< True for Hardware SPI on , false for Software SPI on*/
	spi_inst_t *_pspiInterface;	  /**< SPI instance pointer*/
	uint16_t _speedSPIKHz;		  /**< SPI speed value in kilohertz*/
	uint16_t _SWSPIGPIODelay = 0; /**< uS GPIO Communications delay, SW SPI ONLY */
	// text variables
	bool _textwrap = true;			/**< wrap text around the screen on overflow*/
	uint16_t _textcolor = 0xFFFF;	/**< ForeGround color for text*/
	uint16_t _textbgcolor = 0x0000; /**< BackGround color for text*/
	// Screen variables
	int16_t _cursorX = 0; /**< Current pixel column postion of Cursor*/
	int16_t _cursorY = 0; /**< Current pixel row position of Cursor*/
	uint8_t _XStart;      /**< Records column offset postion */
	uint8_t _YStart;      /**< Records row offset postion */
	uint16_t _width;      /**< Records width TFT postion */
	uint16_t _height;     /**< Records Height TFT postion */
	// GPIO variables
	int8_t _display_DC;	   /**< GPIO Data or command line */
	int8_t _display_RST;   /**< GPIO Reset line */
	int8_t _display_CS;	   /**< GPIO Chip select line */
	int8_t _display_SCLK;  /**< GPIO Clock SPI Line */
	int8_t _display_SDATA; /**< GPIO MOSI data into display SPI Line */
	int8_t _display_MISO; /**< GPIO  MISO data out SPI Line */

private:
	// Helper functions
	inline void swapint16t(int16_t& a, int16_t& b)
	{
		int16_t t = a;
		a = b;
		b = t;
	}
	uint16_t convert8bitTo16bit(uint8_t RRRGGGBB);
	void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
	void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
#ifdef dislib16_ADVANCED_GRAPHICS_ENABLE
	void ellipseHelper(uint16_t cx, uint16_t cy, uint16_t x, uint16_t y, uint16_t color);
	void drawArcHelper(uint16_t cx, uint16_t cy, uint16_t radius, uint16_t thickness, float start, float end, uint16_t color);
	float sineFromDegrees(float angle);
	float cosineFromDegrees(float angle);
	float _arcAngleMax = 360.0f; /**< Maximum angle of Arc , used by drawArc*/
	int _arcAngleOffset= 0; /**< used by drawArc, offset for adjusting the starting angle of arc. default positive X-axis (0°)*/
#endif
	bool _textCharPixelOrBuffer = false;  /**< Text character is drawn by buffer(false) or pixel(true) */
#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
	std::vector <uint8_t> _screenBuffer; /**< Buffer for screen*/
#endif
};

// ********************** EOF *********************
