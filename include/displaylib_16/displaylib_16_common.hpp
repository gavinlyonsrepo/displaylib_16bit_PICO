/*!
	@file     displaylib_16_common.hpp
	@author   Gavin Lyons
	@brief    Library header file for  common data to all displays
*/

#include <cstdint>
#include <cstdio>

#pragma once

extern "C" char* sbrk(int incr);
extern char __StackTop;		// Provided by linker

extern char __bss_end__;	// End of .bss segment
/*! namespace for common functions and data*/
namespace DisLib16{
/*! Enum to define a standard return code for most functions that return failures*/
enum Ret_Codes_e : uint8_t
{
	Success = 0,                 /**< Success, Function ran without defined Error*/
	Reserved = 1,                /**< Reserved */
	WrongFont = 2,               /**< Wrong Font selected*/
	CharScreenBounds = 3,        /**< Text Character is out of Screen bounds, Check x and y*/
	CharFontASCIIRange = 4,      /**< Text Character is outside of chosen Fonts ASCII range, Check the selected Fonts ASCII range.*/
	CharArrayNullptr = 5,        /**< Text Character Array is an invalid pointer object*/
	FontPtrNullptr = 6,          /**< Pointer to the font is an invalid pointer object*/
	FontNotEnabled = 7,          /**< Is the font you selected enabled in _font.hpp? */
	BitmapNullptr = 8,           /**< The Bitmap data array is an invalid pointer object*/
	BitmapScreenBounds = 9,      /**< The Bitmap starting point is outside screen bounds, check x and y*/
	BitmapLargerThanScreen = 10, /**< The Bitmap is larger than screen, check  w and h*/
	BitmapVerticalSize = 11,     /**< A vertical Bitmap's height must be divisible by 8.*/
	BitmapHorizontalSize = 12,   /**< A horizontal Bitmap's width must be divisible by 8*/
	BitmapSize = 13,             /**< Size of the Bitmap is incorrect: BitmapSize(vertical)!=(w*(h/8),BitmapSize(horizontal)!=(w/8)*h*/
	BufferSize = 14,             /**< Size of the Buffer is incorrect: BufferSize(vertical)!=(w*(h/8)*/
	BufferNullptr = 15,          /**< The Buffer data array is an invalid pointer object*/
	BufferEmpty = 16,            /**< Empty buffer object */
	ShapeScreenBounds = 17,      /**< Shape is outside screen bounds, check x and y */
	IconScreenWidth = 18,        /**< Icon is greater than screen width , check width w value */
	BitmapDataEmpty = 19,        /**< Empty bitmap span object  */
	GenericError = 20,           /**< Generic Error */
	FontDataEmpty = 21,          /**< There is no data in selected font. */
	MemoryAError = 22            /**<  Memory allocation failure*/
};

bool isDebugMode(void);
void setDebugMode(bool mode);
void printMemoryUsage(void);
uint16_t LibraryVersion(void);
}

// GPIO	Abstractions , for portability purposes
#define DISPLAY_DC_INIT gpio_init(_display_DC)
#define DISPLAY_RST_INIT gpio_init(_display_RST)
#define DISPLAY_CS_INIT gpio_init(_display_CS)
#define DISPLAY_SCLK_INIT gpio_init(_display_SCLK)
#define DISPLAY_SDATA_INIT gpio_init(_display_SDATA)
#define DISPLAY_MISO_INIT gpio_init(_display_MISO)

#define DISPLAY_DC_DEINIT gpio_deinit(_display_DC)
#define DISPLAY_RST_DEINIT gpio_deinit(_display_RST)
#define DISPLAY_CS_DEINIT gpio_deinit(_display_CS)
#define DISPLAY_SCLK_DEINIT gpio_deinit(_display_SCLK)
#define DISPLAY_SDATA_DEINIT gpio_deinit(_display_SDATA)
#define DISPLAY_MISO_DEINIT gpio_deinit(_display_MISO)

#define DISPLAY_DC_SetHigh gpio_put(_display_DC, true)
#define DISPLAY_DC_SetLow gpio_put(_display_DC, false)
#define DISPLAY_RST_SetHigh gpio_put(_display_RST, true)
#define DISPLAY_RST_SetLow gpio_put(_display_RST, false)
#define DISPLAY_CS_SetHigh gpio_put(_display_CS, true)
#define DISPLAY_CS_SetLow gpio_put(_display_CS, false)
#define DISPLAY_SCLK_SetHigh gpio_put(_display_SCLK, true)
#define DISPLAY_SCLK_SetLow gpio_put(_display_SCLK, false)
#define DISPLAY_SDATA_SetHigh gpio_put(_display_SDATA, true)
#define DISPLAY_SDATA_SetLow gpio_put(_display_SDATA, false)

#define DISPLAY_DC_SetDigitalOutput gpio_set_dir(_display_DC, GPIO_OUT)
#define DISPLAY_RST_SetDigitalOutput gpio_set_dir(_display_RST, GPIO_OUT)
#define DISPLAY_CS_SetDigitalOutput gpio_set_dir(_display_CS, GPIO_OUT)
#define DISPLAY_SCLK_SetDigitalOutput gpio_set_dir(_display_SCLK, GPIO_OUT)
#define DISPLAY_SDATA_SetDigitalOutput gpio_set_dir(_display_SDATA, GPIO_OUT)

#define DISPLAY_SCLK_SPI_FUNC gpio_set_function(_display_SCLK, GPIO_FUNC_SPI)
#define DISPLAY_SDATA_SPI_FUNC gpio_set_function(_display_SDATA, GPIO_FUNC_SPI)
#define DISPLAY_SCLK_SPI_FUNC_OFF gpio_set_function(_display_SCLK, GPIO_FUNC_NULL)
#define DISPLAY_SDATA_SPI_FUNC_OFF gpio_set_function(_display_SDATA, GPIO_FUNC_NULL)

// Delays
#define MILLISEC_DELAY busy_wait_ms
#define MICROSEC_DELAY busy_wait_us