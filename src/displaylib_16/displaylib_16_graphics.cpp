/*!
	@file     displaylib_16_graphics.cpp
	@author   Gavin Lyons
	@brief    Source file for  graphics library. This file handles the graphic methods
*/

#include "../../include/displaylib_16/displaylib_16_graphics.hpp"

/*!
	@brief Construct a new st7735 tft graphics::st7735 tft graphics object
 */
displaylib_16_graphics::displaylib_16_graphics(){}

/*!
	@brief Draw a pixel to screen
	@param x  Column co-ord
	@param y  row co-ord
	@param color 565 16-bit
*/
void displaylib_16_graphics ::drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	if ((x >= _width) || (y >= _height))
		return;
	setAddrWindow(x, y, x + 1, y + 1);
	uint8_t TransmitBuffer[2] {(uint8_t)(color >> 8), (uint8_t)(color & 0xFF)};
	spiWriteDataBuffer(TransmitBuffer, 2);
	//writeData(color >> 8);
	//writeData(color & 0xFF);
}

/*!
	@brief fills a rectangle starting from coordinates (x,y) with width of w and height of h.
	@param x x coordinate
	@param y y coordinate
	@param w width of the rectangle
	@param h height of the rectangle
	@param color color to fill  rectangle 565 16-bit
	@return
		-# Display_Success for success
		-# Display_ShapeScreenBounds out of screen bounds
	@note  uses spiWriteBuffer method
*/
DisLib16::Ret_Codes_e displaylib_16_graphics::fillRectBuffer(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
	// Check bounds
	if ((x >= _width) || (y >= _height))
	{
		printf("Error fillRectangle 2: Out of screen bounds\r\n");
		return DisLib16::ShapeScreenBounds;
	}
	if ((x + w - 1) >= _width)
		w = _width - x;
	if ((y + h - 1) >= _height)
		h = _height - y;

	// Convert color to bytes
	uint8_t hi = color >> 8;
	uint8_t lo = color;

	// Row buffer for one row of the rectangle
	uint8_t rowBuffer[w * 2]; // Each pixel is 2 bytes (16-bit color)
	// Fill the row buffer with the color
	for (uint32_t i = 0; i < w; i++)
	{
		rowBuffer[2 * i] = hi;
		rowBuffer[2 * i + 1] = lo;
	}
	// Draw the rectangle row by row
	for (uint16_t j = 0; j < h; j++)
	{
		// Set the address window for the current row
		setAddrWindow(x, y + j, x + w - 1, y + j);
		// Write the row buffer to the display
		spiWriteDataBuffer(rowBuffer, w * 2);
	}
	return DisLib16::Success;
}


/*!
	@brief Fills the whole screen with a given color.
	@param  color 565 16-bit
*/
void displaylib_16_graphics ::fillScreen(uint16_t color)
{
	fillRectBuffer(0, 0, _width, _height, color);
}

/*!
	@brief Draws a vertical line starting at (x,y) with height h.
	@param x The starting x coordinate
	@param y The starting y coordinate
	@param h The height of the line
	@param color The color of the line 565 16 Bit color
*/
void displaylib_16_graphics ::drawFastVLine(uint16_t x, uint16_t y, uint16_t h, uint16_t color)
{
	uint8_t hi, lo;
	if ((x >= _width) || (y >= _height))
		return;
	if ((y + h - 1) >= _height)
		h = _height - y;
	hi = color >> 8;
	lo = color;
	setAddrWindow(x, y, x, y + h - 1);
	DISPLAY_DC_SetHigh;

	DISPLAY_CS_SetLow;
	while (h--)
	{
		spiWrite(hi);
		spiWrite(lo);
	}
	DISPLAY_CS_SetHigh;
}

/*!
	@brief Draws a horizontal line starting at (x,y) with width w.
	@param x The starting x coordinate
	@param y The starting y coordinate
	@param w The width of the line
	@param color The color of the line 565 16 Bit color
*/
void displaylib_16_graphics ::drawFastHLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color)
{
	uint8_t hi, lo;
	if ((x >= _width) || (y >= _height))
		return;
	if ((x + w - 1) >= _width)
		w = _width - x;
	hi = color >> 8;
	lo = color;
	setAddrWindow(x, y, x + w - 1, y);
	DISPLAY_DC_SetHigh;
	DISPLAY_CS_SetLow;
	while (w--)
	{
		spiWrite(hi);
		spiWrite(lo);
	}
	DISPLAY_CS_SetHigh;
}

/*!
	@brief draws a circle where (x0,y0) are center coordinates an r is circle radius.
	@param x0 circle center x position
	@param y0 circle center y position
	@param r radius of circle
	@param color The color of the circle , 565 16 Bit color
*/
void displaylib_16_graphics ::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
	int16_t f, ddF_x, ddF_y, x, y;
	f = 1 - r, ddF_x = 1, ddF_y = -2 * r, x = 0, y = r;
	drawPixel(x0, y0 + r, color);
	drawPixel(x0, y0 - r, color);
	drawPixel(x0 + r, y0, color);
	drawPixel(x0 - r, y0, color);
	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		drawPixel(x0 + x, y0 + y, color);
		drawPixel(x0 - x, y0 + y, color);
		drawPixel(x0 + x, y0 - y, color);
		drawPixel(x0 - x, y0 - y, color);
		drawPixel(x0 + y, y0 + x, color);
		drawPixel(x0 - y, y0 + x, color);
		drawPixel(x0 + y, y0 - x, color);
		drawPixel(x0 - y, y0 - x, color);
	}
}

///@cond
/*! @brief Used internally by drawRoundRect */
void displaylib_16_graphics ::drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color)
{
	int16_t f, ddF_x, ddF_y, x, y;
	f = 1 - r, ddF_x = 1, ddF_y = -2 * r, x = 0, y = r;
	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		if (cornername & 0x4)
		{
			drawPixel(x0 + x, y0 + y, color);
			drawPixel(x0 + y, y0 + x, color);
		}
		if (cornername & 0x2)
		{
			drawPixel(x0 + x, y0 - y, color);
			drawPixel(x0 + y, y0 - x, color);
		}
		if (cornername & 0x8)
		{
			drawPixel(x0 - y, y0 + x, color);
			drawPixel(x0 - x, y0 + y, color);
		}
		if (cornername & 0x1)
		{
			drawPixel(x0 - y, y0 - x, color);
			drawPixel(x0 - x, y0 - y, color);
		}
	}
}

/*!
	@brief Used internally by fill circle fillRoundRect and fillcircle
*/
void displaylib_16_graphics ::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color)
{
	int16_t f, ddF_x, ddF_y, x, y;
	f = 1 - r, ddF_x = 1, ddF_y = -2 * r, x = 0, y = r;
	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if (cornername & 0x1)
		{
			drawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
			drawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
		}
		if (cornername & 0x2)
		{
			drawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
			drawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
		}
	}
}
///@endcond

/*!
	@brief fills a circle where (x0,y0) are center coordinates an r is circle radius.
	@param x0 circle center x position
	@param y0 circle center y position
	@param r radius of circle
	@param color color of the circle , 565 16 Bit color
*/
void displaylib_16_graphics ::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
	drawFastVLine(x0, y0 - r, 2 * r + 1, color);
	fillCircleHelper(x0, y0, r, 3, 0, color);
}

/*!
	@brief draws rectangle at (x,y) where h is height and w is width of the rectangle.
	@param x x start coordinate
	@param y y start coordinate
	@param w width of the rectangle
	@param h height of the rectangle
	@param color color to fill  rectangle 565 16-bit
*/
void displaylib_16_graphics ::drawRectWH(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
	drawFastHLine(x, y, w, color);
	drawFastHLine(x, y + h - 1, w, color);
	drawFastVLine(x, y, h, color);
	drawFastVLine(x + w - 1, y, h, color);
}

/*!
	@brief draws a line from (x0,y0) to (x1,y1).
	@param x0 x start coordinate
	@param y0 y start coordinate
	@param x1 x end coordinate
	@param y1 y end coordinate
	@param color color to fill  rectangle 565 16-bit
*/
void displaylib_16_graphics ::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
	int16_t steep, dx, dy, err, ystep;
	steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep)
	{
		swapint16t(x0, y0);
		swapint16t(x1, y1);
	}
	if (x0 > x1)
	{
		swapint16t(x0, x1);
		swapint16t(y0, y1);
	}
	dx = x1 - x0;
	dy = abs(y1 - y0);

	err = dx / 2;
	if (y0 < y1)
	{
		ystep = 1;
	}
	else
	{
		ystep = -1;
	}

	for (; x0 <= x1; x0++)
	{
		if (steep)
		{
			drawPixel(y0, x0, color);
		}
		else
		{
			drawPixel(x0, y0, color);
		}
		err -= dy;
		if (err < 0)
		{
			y0 += ystep;
			err += dx;
		}
	}
}

/*!
	@brief fills a rectangle at (x,y) where h is height and w is width of the rectangle.
	@param x x start coordinate
	@param y y start coordinate
	@param w width of the rectangle
	@param h height of the rectangle
	@param color color to fill  rectangle 565 16-bit
*/
void displaylib_16_graphics ::fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
	int16_t i;
	for (i = x; i < x + w; i++)
	{
		drawFastVLine(i, y, h, color);
	}
}

/*!
	@brief draws a rectangle with rounded edges
	@param x x start coordinate
	@param y y start coordinate
	@param w width of the rectangle
	@param h height of the rectangle
	@param r r: radius of the rounded edges
	@param color color to fill  rectangle 565 16-bit
*/
void displaylib_16_graphics ::drawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color)
{
	drawFastHLine(x + r, y, w - 2 * r, color);
	drawFastHLine(x + r, y + h - 1, w - 2 * r, color);
	drawFastVLine(x, y + r, h - 2 * r, color);
	drawFastVLine(x + w - 1, y + r, h - 2 * r, color);
	drawCircleHelper(x + r, y + r, r, 1, color);
	drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
	drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
	drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

/*!
	@brief Fills a rectangle with rounded edges
	@param x x start coordinate
	@param y y start coordinate
	@param w width of the rectangle
	@param h height of the rectangle
	@param r r: radius of the rounded edges
	@param color color to fill  rectangle 565 16-bit
*/
void displaylib_16_graphics ::fillRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color)
{
	fillRect(x + r, y, w - 2 * r, h, color);
	fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
	fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
}

/*!
	@brief draws a triangle of coordinates (x0,y0), (x1,y1) and (x2,y2).
	@param x0 x start coordinate point 1
	@param y0 y start coordinate point 1
	@param x1 x start coordinate point 2
	@param y1 y start coordinate point 2
	@param x2 x start coordinate point 3
	@param y2 y start coordinate point 3
	@param color color to draw triangle 565 16-bit
*/
void displaylib_16_graphics ::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
	drawLine(x0, y0, x1, y1, color);
	drawLine(x1, y1, x2, y2, color);
	drawLine(x2, y2, x0, y0, color);
}

/*!
	@brief Fills a triangle of coordinates (x0,y0), (x1,y1) and (x2,y2).
	@param x0 x start coordinate point 1
	@param y0 y start coordinate point 1
	@param x1 x start coordinate point 2
	@param y1 y start coordinate point 2
	@param x2 x start coordinate point 3
	@param y2 y start coordinate point 3
	@param color color to fill , 565 16-bit
*/
void displaylib_16_graphics ::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
	int16_t a, b, y, last, dx01, dy01, dx02, dy02, dx12, dy12, sa, sb;
	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1)
	{
		swapint16t(y0, y1);
		swapint16t(x0, x1);
	}
	if (y1 > y2)
	{
		swapint16t(y2, y1);
		swapint16t(x2, x1);
	}
	if (y0 > y1)
	{
		swapint16t(y0, y1);
		swapint16t(x0, x1);
	}
	if (y0 == y2)
	{
		a = b = x0;
		if (x1 < a)
			a = x1;
		else if (x1 > b)
			b = x1;
		if (x2 < a)
			a = x2;
		else if (x2 > b)
			b = x2;
		drawFastHLine(a, y0, b - a + 1, color);
		return;
	}
	dx01 = x1 - x0;
	dy01 = y1 - y0;
	dx02 = x2 - x0;
	dy02 = y2 - y0;
	dx12 = x2 - x1;
	dy12 = y2 - y1;
	sa = 0;
	sb = 0;
	if (y1 == y2)
		last = y1;
	else
		last = y1 - 1;
	for (y = y0; y <= last; y++)
	{
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		if (a > b)
			swapint16t(a, b);
		drawFastHLine(a, y, b - a + 1, color);
	}

	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for (; y <= y2; y++)
	{
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		if (a > b)
			swapint16t(a, b);
		drawFastHLine(a, y, b - a + 1, color);
	}
}

/*!
	@brief  writes a character on the display
	@param  x X coordinate
	@param  y Y coordinate
	@param  character The ASCII character
	@param color 565 16-bit foreground color
	@param bg 565 16-bit background color
	@param size 1-15
	@return
		-# Display_Success 
		-# Display_WrongFont = Wrong font This Function for font #1-6 only.
		-# Display_CharScreenBounds = X  Y Co-ordinates out of bounds.
		-# Display_CharFontASCIIRange = ASCII character not in fonts range.
	@note Function Overloaded 2 off , the other drawChar method is for fonts > 6
*/
DisLib16::Ret_Codes_e displaylib_16_graphics ::drawChar(uint16_t x, uint16_t y, uint8_t character, uint16_t color, uint16_t bg, uint8_t size)
{
	int8_t i, j;
	uint8_t line;
	// 0. Check size
	if (size == 0 || size >= 15)
		size = 1;
	// 1. Check for screen out of bounds
	if ((x >= _width) ||								  // Clip right
		(y >= _height) ||							  // Clip bottom
		((x + (_CurrentFontWidth + 1) * size - 1) < 0) || // Clip left
		((y + _CurrentFontheight * size - 1) < 0))		  // Clip top
	{
		printf("Error drawChar 3: Co-ordinates out of bounds\r\n");
		return DisLib16::CharScreenBounds;
	}

	// 2. Check for character out of font range bounds
	if (character < _CurrentFontoffset || character >= (_CurrentFontLength + _CurrentFontoffset))
	{
		printf("Error drawChar 4: Character = %u , Out of Font bounds %u <-> %u\r\n", character, _CurrentFontoffset,  (unsigned int)(_CurrentFontLength + _CurrentFontoffset));
		return DisLib16::CharFontASCIIRange;
	}

	for (i = 0; i < (_CurrentFontWidth + 1); i++)
	{

		if (i == _CurrentFontWidth)
		{
			line = 0x00;
		}
		else
		{
			switch (_FontNumber)
			{
			case Font_Default:
				line = pFontDefaultptr[(character - _CurrentFontoffset) * _CurrentFontWidth + i];
				break;
			case Font_Thick:
				line = pFontThickptr[(character - _CurrentFontoffset) * _CurrentFontWidth + i];
				break;
			case Font_Seven_Seg:
				line = pFontSevenSegptr[(character - _CurrentFontoffset) * _CurrentFontWidth + i];
				break;
			case Font_Wide:
				line = pFontWideptr[(character - _CurrentFontoffset) * _CurrentFontWidth + i];
				break;
			case Font_Tiny:
				line = pFontTinyptr[(character - _CurrentFontoffset) * _CurrentFontWidth + i];
				break;
			case Font_HomeSpun:
				line = pFontHomeSpunptr[(character - _CurrentFontoffset) * _CurrentFontWidth + i];
				break;
			default:
				printf("Error drawChar 5: Wrong font number set must be 1-6 : %u \r\n", _FontNumber);
				return DisLib16::WrongFont;
				break;
			}
		}
		for (j = 0; j < _CurrentFontheight; j++, line >>= 1)
		{
			if (line & 0x01)
			{
				if (size == 1)
					drawPixel(x + i, y + j, color);
				else
					fillRect(x + (i * size), y + (j * size), size, size, color);
			}
			else if (bg != color)
			{
				if (size == 1)
					drawPixel(x + i, y + j, bg);
				else
					fillRect(x + i * size, y + j * size, size, size, bg);
			}
		}
	}
	return DisLib16::Success;
}

/*!
	@brief turn on or off screen wrap of the text (fonts 1-6)
	@param w TRUE on
*/
void displaylib_16_graphics ::setTextWrap(bool w)
{
	_wrap = w;
}

/*!
	@brief Writes text string on the display
	@param x X coordinate
	@param y Y coordinate
	@param pText pointer to string/array
	@param color 565 16-bit
	@param bg background color
	@param size 1-x
	@return
		-# Display_Success=success
		-# Display_WrongFont=wrong font
		-# Display_CharScreenBounds=Co-ordinates out of bounds
		-# Display_CharArrayNullptr=Invalid pointer object
		-# if drawChar method error upstream it return that error code.
	@note for font #1-6 only
*/
DisLib16::Ret_Codes_e displaylib_16_graphics ::drawText(uint16_t x, uint16_t y, char *pText, uint16_t color, uint16_t bg, uint8_t size)
{

	// Check if correct font
	if (_FontNumber >= Font_Bignum)
	{
		printf("Error drawText 2: Wrong font number selected, must be 1-6\r\n");
		return DisLib16::WrongFont;
	}
	// Check for null pointer
	if (pText == nullptr)
	{
		printf("Error drawText 3: String array is not valid pointer object\r\n");
		return DisLib16::CharArrayNullptr;
	}
	// Out of screen bounds
	if ((x >= _width) || (y >= _height))
	{
		printf("Error drawText 4: Out of screen bounds\r\n");
		return DisLib16::CharScreenBounds;
	}
	uint8_t cursorX = x;
	uint8_t cursorY = y;
	DisLib16::Ret_Codes_e errorCode;
	while (*pText != '\0')
	{
		if (_wrap && ((cursorX + size * _CurrentFontWidth) > _width))
		{
			cursorX = 0;
			cursorY = cursorY + size * 7 + 3;
			if (cursorY > _height)
				cursorY = _height;
		}
		errorCode = drawChar(cursorX, cursorY, *pText, color, bg, size);
		if ( errorCode != DisLib16::Success)
		{
			printf("Error drawText 5: Method drawChar failed\r\n");
			return errorCode;
		}
		cursorX = cursorX + size * (_CurrentFontWidth + 1);

		if (cursorX > _width)
			cursorX = _width;
		pText++;
	}
	return DisLib16::Success;
}

/*!
	@brief: called by the print class after it converts the data to a character
	@param character character
	@return 
		-# 1=success
		-# -1=drawChar upstream function failed.
*/
size_t displaylib_16_graphics ::write(uint8_t character)
{
	if (_FontNumber < Font_Bignum)
	{
		switch (character)
		{
		case '\n':
			_cursorY += _textSize * _CurrentFontheight;
			_cursorX = 0;
			break;
		case '\r': /* skip */
			break;
		default:
			if (drawChar(_cursorX, _cursorY, character, _textcolor, _textbgcolor, _textSize) != 0)
			{
				printf("Error write_print method 1C: Method drawChar failed\r\n");
				return -1;
			}
			_cursorX += _textSize * (_CurrentFontWidth + 1);
			if (_wrap && (_cursorX > (_width - _textSize * (_CurrentFontWidth + 1))))
			{
				_cursorY += _textSize * _CurrentFontheight;
				_cursorX = 0;
			}
			break;
		}
	}
	else // for font numbers 7-12
	{
		switch (character)
		{
		case '\n':
			_cursorY += _CurrentFontheight;
			_cursorX = 0;
			break;
		case '\r': /* skip */
			break;
		default:
			if (drawChar(_cursorX, _cursorY, character, _textcolor, _textbgcolor) != 0)
			{
				printf("Error write_print method 2C: Method drawChar failed\r\n");
				return -1;
			}
			_cursorX += (_CurrentFontWidth);
			if (_wrap && (_cursorX > (_width - (_CurrentFontWidth + 1))))
			{
				_cursorY += _CurrentFontheight;
				_cursorX = 0;
			}
			break;
		} // end of switch
	}	  // end of else
	return 1;
}

/*!
	@brief   Set the font type
	@param FontNumber 1-12 enum OLED_FONT_TYPE_e
*/
void displaylib_16_graphics ::FontNum(Font_Type_e FontNumber)
{
	_FontNumber = FontNumber;
	switch (_FontNumber)
	{
	case Font_Default: // Norm default 5 by 8
		_CurrentFontWidth = Font_width_5;
		_CurrentFontoffset = Font_offset_none;
		_CurrentFontheight = Font_height_8;
		_CurrentFontLength = FontLenAll;
		break;
	case Font_Thick: // Thick 7 by 8 (NO LOWERCASE LETTERS)
		_CurrentFontWidth = Font_width_7;
		_CurrentFontoffset = Font_offset_space;
		_CurrentFontheight = Font_height_8;
		_CurrentFontLength = FontLenAlphaNumNoLCase;
		break;
	case Font_Seven_Seg: // Seven segment 4 by 8
		_CurrentFontWidth = Font_width_4;
		_CurrentFontoffset = Font_offset_space;
		_CurrentFontheight = Font_height_8;
		_CurrentFontLength = FontLenAlphaNum;
		break;
	case Font_Wide: // Wide  8 by 8 (NO LOWERCASE LETTERS)
		_CurrentFontWidth = Font_width_8;
		_CurrentFontoffset = Font_offset_space;
		_CurrentFontheight = Font_height_8;
		_CurrentFontLength = FontLenAlphaNumNoLCase;
		break;
	case Font_Tiny: // tiny 3 by 8
		_CurrentFontWidth = Font_width_3;
		_CurrentFontoffset = Font_offset_space;
		_CurrentFontheight = Font_height_8;
		_CurrentFontLength = FontLenAlphaNum;
		break;
	case Font_HomeSpun: // homespun 7 by 8
		_CurrentFontWidth = Font_width_7;
		_CurrentFontoffset = Font_offset_space;
		_CurrentFontheight = Font_height_8;
		_CurrentFontLength = FontLenAlphaNum;
		break;
	case Font_Bignum: // big nums 16 by 32 (NUMBERS + : only)
		_CurrentFontWidth = Font_width_16;
		_CurrentFontoffset = Font_offset_minus;
		_CurrentFontheight = Font_height_32;
		_CurrentFontLength = FontLenNumeric;
		break;
	case Font_Mednum: // med nums 16 by 16 (NUMBERS + : only)
		_CurrentFontWidth = Font_width_16;
		_CurrentFontoffset = Font_offset_minus;
		_CurrentFontheight = Font_height_16;
		_CurrentFontLength = FontLenNumeric;
		break;
	case Font_ArialRound: // Arial round 16 by 24
		_CurrentFontWidth = Font_width_16;
		_CurrentFontoffset = Font_offset_space;
		_CurrentFontheight = Font_height_24;
		_CurrentFontLength = FontLenAlphaNum;
		break;
	case Font_ArialBold: // Arial bold  16 by 16
		_CurrentFontWidth = Font_width_16;
		_CurrentFontoffset = Font_offset_space;
		_CurrentFontheight = Font_height_16;
		_CurrentFontLength = FontLenAlphaNum;
		break;
	case Font_Mia: // mia  8 by 16
		_CurrentFontWidth = Font_width_8;
		_CurrentFontoffset = Font_offset_space;
		_CurrentFontheight = Font_height_16;
		_CurrentFontLength = FontLenAlphaNum;
		break;
	case Font_Dedica: // dedica  6 by 12
		_CurrentFontWidth = Font_width_6;
		_CurrentFontoffset = Font_offset_space;
		_CurrentFontheight = Font_height_12;
		_CurrentFontLength = FontLenAlphaNum;
		break;
	default:
		_CurrentFontWidth = Font_width_5;
		_CurrentFontoffset = Font_offset_none;
		_CurrentFontheight = Font_height_8;
		_CurrentFontLength = FontLenAll;
		_FontNumber = Font_Default;
		break;
	}
}

/*!
	@brief Draws an custom Icon of X by 8 size to screen , where X = 0 to 127
	@param x X coordinate
	@param y Y coordinate
	@param w 0-MAX_Y possible values width of icon in pixels , height is fixed at 8 pixels
	@param color icon foreground colors ,is bi-color
	@param backcolor icon background colors ,is bi-color
	@param character  An array of unsigned chars containing icon data vertically addressed.
	@return
		-# Display_Success=success.
		-# Display_BitmapScreenBounds=Co-ordinates out of bounds.
		-# Display_BitmapNullptr=invalid pointer object.
		-# Display_IconScreenWidth=Icon width is greater than screen width
*/
DisLib16::Ret_Codes_e displaylib_16_graphics ::drawIcon(uint16_t x, uint16_t y, uint16_t w, uint16_t color, uint16_t backcolor, const unsigned char character[])
{
	// Out of screen bounds
	if ((x >= _width) || (y >= _height))
	{
		printf("Error drawIcon 2: Out of screen bounds\r\n");
		return DisLib16::BitmapScreenBounds;
	}
	// Check for null pointer
	if (character == nullptr)
	{
		printf("Error drawIcon 3: Icon array is not valid pointer object\r\n");
		return DisLib16::BitmapNullptr;
	}
	// Check w value
	if (w >= _width)
	{
		printf("Error drawIcon 4: Icon is greater than Screen width\r\n");
		return DisLib16::IconScreenWidth;
	}

	uint8_t value;
	for (uint8_t byte = 0; byte < w; byte++)
	{
		for (uint8_t mybit = 0; mybit < 8; mybit++)
		{
			value = !!(character[byte] & (1 << mybit));
			if (value)
			{
				drawPixel(x + byte, y + mybit, backcolor);
			}
			else
			{
				drawPixel(x + byte, y + mybit, color);
			}
			value = 0;
		}
	}
	return DisLib16::Success;
}

/*!
	@brief: Draws an bi-color bitmap to screen
	@param x X coordinate
	@param y Y coordinate
	@param w width of the bitmap in pixels
	@param h height of the bitmap in pixels
	@param color bitmap foreground colors ,is bi-color
	@param bgcolor bitmap background colors ,is bi-color
	@param pBmp  an array of uint8_t containing bitmap data horizontally addressed.
	@param sizeOfBitmap size of the bitmap
	@return
		-# Display_Success=success
		-# Display_BitmapNullptr=invalid pointer object
		-# Display_BitmapScreenBounds=Co-ordinates out of bounds,
		-# Display_BitmapHorizontalSize=bitmap wrong size
	@note A horizontal Bitmap's w must be divisible by 8. For a bitmap with w=88 & h=48.
		  Bitmap excepted size = (88/8) * 48 = 528 bytes.
*/
DisLib16::Ret_Codes_e displaylib_16_graphics::drawBitmap(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t bgcolor, uint8_t *pBmp, uint16_t sizeOfBitmap)
{
	int16_t byteWidth = (w + 7) / 8;
	uint8_t byte = 0;
	uint16_t mycolor = 0;

	// Validate size of the bitmap
	if (sizeOfBitmap != ((w / 8) * h))
	{
		printf("Error drawBitmap 4 : Horizontal Bitmap size is incorrect: Check Size =  (w/8 * h): %u  %i  %i \n", sizeOfBitmap, w, h);
		return DisLib16::BitmapHorizontalSize;
	}
	// Check for null pointer
	if (pBmp == nullptr)
	{
		printf("Error drawBitmap 1: Bitmap array is nullptr\r\n");
		return DisLib16::BitmapNullptr;
	}
	// Check bounds
	if ((x >= _width) || (y >= _height))
	{
		printf("Error drawBitmap 2: Out of screen bounds, check x & y\r\n");
		return DisLib16::BitmapScreenBounds;
	}
	if ((x + w - 1) >= _width)
		w = _width - x;
	if ((y + h - 1) >= _height)
		h = _height - y;

	// Buffer for one row of pixels (16-bit per pixel split into bytes)
	uint8_t rowBuffer[w * 2];
	// Draw row by row
	for (int16_t j = 0; j < h; j++)
	{
		// Process one row of pixels
		for (int16_t i = 0; i < w; i++)
		{
			if (i & 7)
				byte <<= 1;
			else
				byte = pBmp[j * byteWidth + i / 8];
			mycolor = (byte & 0x80) ? color : bgcolor;
			// Correct order: High byte first, low byte second
			rowBuffer[2 * i] = mycolor >> 8;      // High byte
			rowBuffer[2 * i + 1] = mycolor & 0xFF; // Low byte
		}
		// Set the address window for the current row
		setAddrWindow(x, y + j, x + w - 1, y + j);
		// Write the row to the display
		spiWriteDataBuffer(rowBuffer, w * 2);
	}
	return DisLib16::Success;
}

/*! 
	@brief: Draws a 16-bit color bitmap to the screen from a data array 
	@param x X coordinate 
	@param y Y coordinate 
	@param pBmp pointer to data array 
	@param w width of the bitmap in pixels 
	@param h height of the bitmap in pixels 
	@return
		-# Display_Success=success
		-# Display_BitmapNullptr=invalid pointer object
		-# Display_BitmapScreenBounds=Co-ordinates out of bounds
*/
DisLib16::Ret_Codes_e displaylib_16_graphics::drawBitmap16Data(uint16_t x, uint16_t y, uint8_t *pBmp, uint16_t w, uint16_t h)
{
	uint8_t j = 0;

	// 1. Check for null pointer
	if (pBmp == nullptr)
	{
		printf("Error drawBitmap16 1: Bitmap array is nullptr\r\n");
		return DisLib16::BitmapNullptr;
	}

	// Check bounds
	if ((x >= _width) || (y >= _height))
	{
		printf("Error drawBitmap16 2: Out of screen bounds\r\n");
		return DisLib16::BitmapScreenBounds;
	}
	if ((x + w - 1) >= _width)
		w = _width - x;
	if ((y + h - 1) >= _height)
		h = _height - y;

	// Process bitmap data row-by-row
	for (j = 0; j < h; j++)
	{
		setAddrWindow(x, y + j, x + w - 1, y + j); // Set the window for the current row
		spiWriteDataBuffer(pBmp, w * sizeof(uint16_t)); // Write one row of pixel data
		pBmp += w * 2; // Move to the next row in the bitmap
	}

	return DisLib16::Success;
}

/*!
	@brief Draws an 24 bit color bitmap to screen from a data array
	@param x X coordinate
	@param y Y coordinate
	@param pBmp pointer to data array
	@param w width of the bitmap in pixels
	@param h height of the bitmap in pixels
	@return
		-# Display_Success=success
		-# Display_BitmapNullptr=invalid pointer object
		-# Display_BitmapScreenBounds=Co-ordinates out of bounds,
	@note 24 bit color converted to 16 bit color
*/
DisLib16::Ret_Codes_e displaylib_16_graphics::drawBitmap24Data(uint16_t x, uint16_t y, uint8_t *pBmp, uint16_t w, uint16_t h)
{
	uint8_t i, j;
	uint16_t color, red, green, blue;
	// 1. Check for null pointer
	if (pBmp == nullptr)
	{
		printf("Error drawBitmap24 1: Bitmap array is nullptr\r\n");
		return DisLib16::BitmapNullptr;
	}
	// Check bounds
	if ((x >= _width) || (y >= _height))
	{
		printf("Error drawBitmap24 2: Out of screen bounds\r\n");
		return DisLib16::BitmapScreenBounds;
	}
	if ((x + w - 1) >= _width)
		w = _width - x;
	if ((y + h - 1) >= _height)
		h = _height - y;

	// Buffer for one row of pixels (16-bit per pixel split into bytes)
	uint8_t rowBuffer[w * 2];
	// Draw the bitmap row by row
	for (j = 0; j < h; j++)
	{
		for (i = 0; i < w; i++)
		{
			// Extract RGB values from 24-bit color data
			red = *pBmp++;
			green = *pBmp++;
			blue = *pBmp++;
			// Convert to 16-bit RGB565 format
			color = ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3);
			// Store high and low bytes of the color in the row buffer
			rowBuffer[2 * i] = color >> 8;      // High byte
			rowBuffer[2 * i + 1] = color & 0xFF; // Low byte
		}
		// Set the address window for the current row
		setAddrWindow(x, y + j, x + w - 1, y + j);
		// Write the row to the display
		spiWriteDataBuffer(rowBuffer, w * 2);
	}
	return DisLib16::Success;
}

/*!
	@brief writes a char (c) on the display
	@param x X coordinate
	@param y Y coordinate
	@param character The ASCII character
	@param color 565 16-bit
	@param bg background color
	@return
		-# Display_Success=success
		-# Display_WrongFont =wrong font
		-# Display_CharScreenBounds=Co-ordinates out of bounds,
		-# Display_CharFontASCIIRange=ASCII character not in fonts range,
		-# Display_FontNotEnabled=Font selected but not enabled in _font.hpp
	@note for font 7-12 only
*/
DisLib16::Ret_Codes_e displaylib_16_graphics ::drawChar(uint16_t x, uint16_t y, uint8_t character, uint16_t color, uint16_t bg)
{
	uint8_t FontSizeMod = 0;
	uint8_t i, j;
	uint8_t ctemp = 0, y0 = y;

	// 1. Check for screen out of bounds
	if ((x >= _width) ||					 // Clip right
		(y >= _height) ||				 // Clip bottom
		((x + _CurrentFontWidth + 1) < 0) || // Clip left
		((y + _CurrentFontheight) < 0))		 // Clip top
	{
		printf("Error drawChar 3B: Co-ordinates out of bounds\r\n");
		return DisLib16::CharScreenBounds;
	}

	// 2. Check for character out of font range bounds
	if (character < _CurrentFontoffset || character >= (_CurrentFontLength + _CurrentFontoffset))
	{
		printf("Error drawChar 4B: Character = %u. Out of Font bounds : %u <> %u\r\n", character, _CurrentFontoffset, (unsigned int)(_CurrentFontLength + _CurrentFontoffset));
		return DisLib16::CharFontASCIIRange;
	}

	// 3. Check for correct font and set FontSizeMod for fonts 7-12
	switch (_FontNumber)
	{
	case Font_Bignum:
	case Font_Mednum:
	case Font_ArialRound:
	case Font_ArialBold:
		FontSizeMod = 2;
		break;
	case Font_Mia:
	case Font_Dedica:
		FontSizeMod = 1;
		break;
	default:
		printf("Error drawChar 5B: Wrong font selected, Font must be > 7 : %u\r\n", _FontNumber);
		return DisLib16::WrongFont;
		break;
	}

	for (i = 0; i < _CurrentFontheight * FontSizeMod; i++)
	{
		switch (_FontNumber)
		{
		case Font_Bignum:
			ctemp = pFontBigNum16x32ptr[character - _CurrentFontoffset][i];
			break;
		case Font_Mednum:
			ctemp = pFontMedNum16x16ptr[character - _CurrentFontoffset][i];
			break;
#ifdef _DISPLAY_OPTIONAL_FONT_9
		case Font_ArialRound:
			ctemp = pFontArial16x24ptr[character - _CurrentFontoffset][i];
			break;
#endif
#ifdef _DISPLAY_OPTIONAL_FONT_10
		case Font_ArialBold:
			ctemp = pFontArial16x16ptr[character - _CurrentFontoffset][i];
			break;
#endif
#ifdef _DISPLAY_OPTIONAL_FONT_11
		case Font_Mia:
			ctemp = pFontMia8x16ptr[character - _CurrentFontoffset][i];
			break;
#endif
#ifdef _DISPLAY_OPTIONAL_FONT_12
		case Font_Dedica:
			ctemp = pFontDedica6x12ptr[character - _CurrentFontoffset][i];
			break;
#endif
		default:
			printf("Error drawChar 6B: Is the font you selected enabled in _font.hpp? : %u\r\n", _FontNumber);
			return DisLib16::FontNotEnabled;
			break;
		}
		for (j = 0; j < 8; j++)
		{
			if (ctemp & 0x80)
			{
				drawPixel(x, y, color);
			}
			else
			{
				drawPixel(x, y, bg);
			}

			ctemp <<= 1;
			y++;
			if ((y - y0) == _CurrentFontheight)
			{
				y = y0;
				x++;
				break;
			}
		}
	}
	return DisLib16::Success;
}

/*!
	@brief Writes text string (*ptext) on the display
	@param x X coordinate
	@param y Y coordinate
	@param pText pointer to string of ASCII character's
	@param color 565 16-bit
	@param bg background color
	@return
		-# Display_Success=success
		-# Display_WrongFont =wrong font
		-# Display_CharScreenBounds=Co-ordinates out of bounds
		-# Display_FontPtrNullptr=Invalid pointer object
		-# if drawChar method error upstream it return that error code.
	@note for font 7-12 only
*/
DisLib16::Ret_Codes_e displaylib_16_graphics ::drawText(uint16_t x, uint16_t y, char *pText, uint16_t color, uint16_t bg)
{
	// Check for correct font
	if (_FontNumber < Font_Bignum)
	{
		printf("Error drawText 2B: Wrong font selected, must be 7 to 12 \r\n");
		return DisLib16::WrongFont;
	}
	// Check for null pointer
	if (pText == nullptr)
	{
		printf("Error drawText 3B: String array is not valid pointer object\r\n");
		return DisLib16::CharArrayNullptr;
	}
	// Out of screen bounds
	if ((x >= _width) || (y >= _height))
	{
		printf("Error drawText 4B: Out of screen bounds\r\n");
		return DisLib16::CharScreenBounds;
	}
	DisLib16::Ret_Codes_e errorCode;
	while (*pText != '\0')
	{
		if (x > (_width - _CurrentFontWidth))
		{
			x = 0;
			y += _CurrentFontheight;
			if (y > (_height - _CurrentFontheight))
			{
				y = x = 0;
			}
		}
		errorCode = drawChar(x, y, *pText, color, bg);
		if (errorCode != DisLib16::Success)
		{
			printf("Error drawText 5B: drawChar method failed\r\n");
			return errorCode;
		}
		x += _CurrentFontWidth;
		pText++;
	}
	return DisLib16::Success;
}

/*!
	@brief: Convert: 24-bit color to 565 16-bit color
	@param r color red
	@param g color green
	@param b color blue
	@return 16 bit color value
	@note 3 byte to 2 byte,  Red Green Blue.
		  RRRR RRRR GGGG GGGG BBBB BBBB => 565 color mode => RRRRR GGGGGG BBBBB
*/
uint16_t displaylib_16_graphics::Color565(uint16_t r, uint16_t g, uint16_t b)
{
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

/*!
 * @brief pushColor
 * @param color
 * @note not in use currently.
 */
void displaylib_16_graphics ::pushColor(uint16_t color)
{
	uint8_t hi, lo;
	hi = color >> 8;
	lo = color;
	DISPLAY_DC_SetHigh;
	DISPLAY_CS_SetLow;
	spiWrite(hi);
	spiWrite(lo);
	DISPLAY_CS_SetHigh;
}

/*!
	@brief : Write an SPI command to TFT
	@param command byte to send
*/
void displaylib_16_graphics::writeCommand(uint8_t command)
{
	DISPLAY_DC_SetLow;
	DISPLAY_CS_SetLow;
	spiWrite(command);
	DISPLAY_CS_SetHigh;
}

/*!
	@brief Write an SPI data byte to device
	@param dataByte byte to send
*/
void displaylib_16_graphics ::writeData(uint8_t dataByte)
{
	DISPLAY_DC_SetHigh;
	DISPLAY_CS_SetLow;
	spiWrite(dataByte);
	DISPLAY_CS_SetHigh;
}

/*!
	@brief  Write byte to SPI
	@param spiData byte to write
*/
void displaylib_16_graphics::spiWrite(uint8_t spiData)
{
	if (_hardwareSPI == false)
	{
		spiWriteSoftware(spiData);
	}
	else
	{
		spi_write_blocking(_pspiInterface, &spiData, 1);
	}
}

/*!
	@brief Write a byte to SPI using software SPI
	@param spiData byte to send
	@note uses _SWSPIGPIODelay to slowdown software SPI if CPU frequency too fast
*/
void displaylib_16_graphics::spiWriteSoftware(uint8_t spiData)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		DISPLAY_SDATA_SetLow;
		if (spiData & 0x80)
			DISPLAY_SDATA_SetHigh;
		DISPLAY_SCLK_SetHigh;
		if (_SWSPIGPIODelay > 0)
			MICROSEC_DELAY(_SWSPIGPIODelay);
		spiData <<= 1;
		DISPLAY_SCLK_SetLow;
		if (_SWSPIGPIODelay > 0)
			MICROSEC_DELAY(_SWSPIGPIODelay);
	}
}

/*!
	@brief  Write a buffer to SPI, both Software and hardware SPI supported
	@param spiData to send
	@param len length of buffer
*/
void displaylib_16_graphics::spiWriteDataBuffer(uint8_t *spiData, uint32_t len)
{
	DISPLAY_DC_SetHigh;
	DISPLAY_CS_SetLow;
	if (_hardwareSPI == false)
	{
		for (uint32_t i = 0; i < len; i++)
		{
			spiWriteSoftware(spiData[i]);
		}
	}
	else
	{
		spi_write_blocking(_pspiInterface, spiData, len);
	}
	DISPLAY_CS_SetHigh;
}

/*!
	@brief Set the Cursor Position on screen
	@param x the x co-ord of the cursor position
	@param y the y co-ord of the cursor position
*/
void displaylib_16_graphics::setCursor(int16_t x, int16_t y)
{
	_cursorX = x;
	_cursorY = y;
}

/*!
	@brief Set the size of text, fonts 1-6
	@param s size of text. 1 2 3 etc
*/
void displaylib_16_graphics::setTextSize(uint8_t s){
	_textSize = (s > 0) ? s : 1;
}

/*!
	@brief Set text color
	@param c  text color , Color definitions 16-Bit Color Values R5G6B5
*/
void displaylib_16_graphics::setTextColor(uint16_t c){
	_textcolor = _textbgcolor = c;
}

/*!
	@brief Set text color foreground and background
	@param c text foreground color , Color definitions 16-Bit Color Values R5G6B5
	@param b text background color , Color definitions 16-Bit Color Values R5G6B5
*/
void displaylib_16_graphics::setTextColor(uint16_t c, uint16_t b){
	_textcolor = c;
	_textbgcolor = b;
}

/*!
	@brief: Draws an 16 bit color sprite bitmap to screen from a data array with transparent background
	@param x X coordinate
	@param y Y coordinate
	@param pBmp pointer to data array
	@param w width of the sprite in pixels
	@param h height of the sprite in pixels
	@param backgroundColor the background color of sprite (16 bit 565) this will be made transparent
	@note Experimental , does not use buffer or malloc, just draw pixel
	@return
		-# Display_Success=success
		-# Display_BitmapNullptr=invalid pointer object
		-# Display_BitmapScreenBounds=Co-ordinates out of bounds
*/
DisLib16::Ret_Codes_e displaylib_16_graphics::drawSpriteData(uint16_t x, uint16_t y, uint8_t *pBmp, uint16_t w, uint16_t h, uint16_t backgroundColor)
{
	uint8_t i, j;
	uint16_t colour;
	// 1. Check for null pointer
	if (pBmp == nullptr)
	{
		printf("Error drawSprite 1: Sprite array is nullptr\r\n");
		return DisLib16::BitmapNullptr;
	}
	// Check bounds
	if ((x >= _width) || (y >= _height))
	{
		printf("Error drawSprite 2: Sprite out of screen bounds\r\n");
		return DisLib16::BitmapScreenBounds;
	}
	if ((x + w - 1) >= _width)
		w = _width - x;
	if ((y + h - 1) >= _height)
		h = _height - y;

	for(j = 0; j < h; j++)
	{
		for(i = 0; i < w; i ++)
		{
			colour = (pBmp[0] << 8) | pBmp[1];
			pBmp += 2;
			if (colour != backgroundColor){
				drawPixel(x+i-1, y + j-1, colour);
			}
		}
	}
	return DisLib16::Success;
}

//**************** EOF *****************
