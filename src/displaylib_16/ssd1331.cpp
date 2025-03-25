/*!
	@file     ssd1331.cpp
	@author   Gavin Lyons
	@brief    Source header file for ssd1331 OLED library.
*/

#include "../../include/displaylib_16/ssd1331.hpp"


/*! @brief Constructor for class SSD1331_OLED
	@param colororder The enum value from color_order_e RGB or BGR color mode
	@param contrast Struct containing 3 color values for normal mode contrast
	@param dimContrast Struct containing 3 color values for dim mode contrast
 * */
SSD1331_OLED::SSD1331_OLED(color_order_e colororder, const Constrast_values_t& contrast, const Dim_Constrast_values_t& dimContrast)
    : _colorOrder(colororder), ContrastValues(contrast), DimContrastValues(dimContrast)
{
}

/*!
	@brief Call when powering down OLED, turns off display and  deinits SPI & GPIO.
*/
void  SSD1331_OLED ::PowerDown(void)
{
	sleepDisplay();

	DISPLAY_DC_SetLow;
	DISPLAY_RST_SetLow;
	DISPLAY_CS_SetLow;

	DISPLAY_DC_DEINIT;
	DISPLAY_RST_DEINIT;
	DISPLAY_CS_DEINIT;
	if (_hardwareSPI == true) {
		DISPLAY_SCLK_SPI_FUNC_OFF;
		DISPLAY_SDATA_SPI_FUNC_OFF;
		spi_deinit(_pspiInterface);
		DISPLAY_SCLK_DEINIT;
		DISPLAY_SDATA_DEINIT;
	}else{
		DISPLAY_SCLK_SetLow;
		DISPLAY_SDATA_SetLow;
		DISPLAY_SCLK_DEINIT;
		DISPLAY_SDATA_DEINIT;
	}
}

/*!
	@brief For Hardware Reset pin control
*/
void SSD1331_OLED::ResetPin() {
	const uint8_t OLEDResetDelay = 10; /**< Reset delay in mS*/
	DISPLAY_RST_SetDigitalOutput;
	DISPLAY_RST_SetHigh;
	MILLISEC_DELAY(OLEDResetDelay);
	DISPLAY_RST_SetLow;
	MILLISEC_DELAY(OLEDResetDelay);
	DISPLAY_RST_SetHigh;
	MILLISEC_DELAY(OLEDResetDelay);
}

/*!
	@brief  sets up OLED GPIO for SPI
	@param rst reset GPIO
	@param dc data or command GPIO.
	@param cs chip select GPIO
	@param sclk Data clock GPIO
	@param din Data to OLED GPIO
*/
void SSD1331_OLED::SetupGPIO(int8_t rst, int8_t dc, int8_t cs, int8_t sclk, int8_t din)
{
	_display_SDATA = din;
	_display_SCLK = sclk;
	_display_RST= rst;
	_display_DC = dc;
	_display_CS = cs;

	DISPLAY_SDATA_INIT; 
	DISPLAY_SCLK_INIT; 
	DISPLAY_RST_INIT;
	DISPLAY_DC_INIT; 
	DISPLAY_CS_INIT; 
}

/*!
	@brief init routine for SSD1331 controller
*/
void SSD1331_OLED::SSD1331Initialize() {
	ResetPin();
	DISPLAY_DC_SetDigitalOutput;
	DISPLAY_DC_SetLow;
	DISPLAY_CS_SetDigitalOutput;
	DISPLAY_CS_SetHigh;
if (_hardwareSPI == false)
{
	DISPLAY_SCLK_SetDigitalOutput;
	DISPLAY_SDATA_SetDigitalOutput;	
	DISPLAY_SCLK_SetLow;
	DISPLAY_SDATA_SetLow;
}else{
	HWSPIInitialize();
}
	cmdBegin();
	setRotation(Degrees_0);
}

/*!
	@brief : Init Hardware SPI
*/
void SSD1331_OLED::HWSPIInitialize(void)
{
	spi_init(_pspiInterface, _speedSPIKHz * 1000); // Initialize SPI port 
	// Initialize SPI pins : clock and data
	DISPLAY_SDATA_SPI_FUNC;
	DISPLAY_SCLK_SPI_FUNC;
	// Set SPI format
	spi_set_format( _pspiInterface,   // SPI instance
					8,      // Number of bits per transfer
					SPI_CPOL_0,      // Polarity (CPOL)
					SPI_CPHA_0,      // Phase (CPHA)
					SPI_MSB_FIRST);

}


/*!
	@brief This method setups up horizontal and vertical scrolling register _CMD_CONT_SCROLL_SET
	@param horOffset Set num of col as h scroll Range: 0-95 ( no h scroll = 0)
	@param startRowAddr Define start row address 0-63
	@param numOfRows Set number of rows to be horizontal scrolled (startRowAddr + numOfRows) <= 64 
	@param verOffset Set num of col as v scroll Range: 0-63 ( no v scroll = 0)
	@param scrollTimeInterval Set time interval between each scroll, enum
*/
void SSD1331_OLED::ScrollSetup(uint8_t horOffset,uint8_t startRowAddr, uint8_t numOfRows, uint8_t verOffset, scroll_time_interval_e scrollTimeInterval) {
	// Check user input
	if (horOffset > 95) horOffset = 94;
	if (startRowAddr > 63) startRowAddr= 62;
	if (startRowAddr + numOfRows > 64) {
		fprintf(stderr, " startRowAddr + numOfRows > 64, setting numOfRows = 1 ,  %u + %u  <= 64 \n", startRowAddr, numOfRows);
		numOfRows = 1;
	}
	if (verOffset > 63) verOffset= 62;
	// Send scroll command list
	writeCommand(_CMD_CONT_SCROLL_SET);
	writeCommand(horOffset);
	writeCommand(startRowAddr);
	writeCommand(numOfRows);
	writeCommand(verOffset);
	writeCommand(scrollTimeInterval);
}

/*!
	@brief: This method is used to turn scroll on or off
	@param scrollActivate Deactive or active scroll called after OLEDScrollSetup()
*/
void SSD1331_OLED::Scroll(scroll_control_e scrollActivate)  {
	
	switch (scrollActivate)
	{
		case SCROLL_ACTIVATE : writeCommand(_CMD_ACTIVE_SCROLL); break;
		case SCROLL_DEACTIVE : writeCommand(_CMD_DEACTIVE_SCROLL); break;
	}
}

/*!
	@brief change the display mode
	@param mode enum containing the display modes available
*/
void SSD1331_OLED::changeDisplayMode(display_mode_e mode) {
	switch (mode)
	{
		case DISPLAY_INVERSE: writeCommand(_CMD_INVERTDISPLAY); break;
		case DISPLAY_NORMAL:  writeCommand(_CMD_NORMALDISPLAY); break;
		case DISPLAY_ALL_ON:  writeCommand(_CMD_DISPLAYALLON);  break;
		case DISPLAY_ALL_OFF: writeCommand(_CMD_DISPLAYALLOFF); break;
	};
}

/*! @brief enable dim mode */
void SSD1331_OLED::DimDisplay(){
	writeCommand(_CMD_DIM_MODE_ON);
}

/*! @brief enable display mode */
void SSD1331_OLED::enableDisplay(){
	writeCommand(_CMD_DISPLAYON);
}

/*!@brief enable sleep mode*/
void SSD1331_OLED::sleepDisplay(){
	writeCommand(_CMD_DISPLAYOFF);
}

/*!
	@brief: change rotation of display.
	@param mode OLED_rotate_e enum
		0 = Normal
		1=  90 rotate
		2 = 180 rotate
		3 =  270 rotate
*/
void SSD1331_OLED::setRotation(display_rotate_e mode) {

	_displayRotate = mode;

	uint8_t rotateValue = 0;
	uint8_t DegreeZero = 0x72;
	uint8_t Degree90   = 0x71;
	uint8_t Degree180  = 0x60;
	uint8_t Degree270  = 0x63;
	// We must preserve setting A[2] in _CMD_SETREMAP setting register
	// This is set in constructor passing a by color_order_e. 
	// the pixel color order RGB mapping i.e RGB 0x72 BGR 0x76 A[2]
	uint8_t BitMask_ReMapCmdA2 = 0x04; 

	writeCommand(_CMD_SETREMAP);
	switch (mode) {
		case Degrees_0 :
			rotateValue  = (DegreeZero | (_colorOrder & BitMask_ReMapCmdA2)) ;
			_width =_widthStartOLED;
			_height = _heightStartOLED;
			break;
		case Degrees_90:
			rotateValue  = (Degree90 | (_colorOrder & BitMask_ReMapCmdA2)) ;
			_width  =_heightStartOLED;
			_height = _widthStartOLED;
			break;
		case Degrees_180:
			rotateValue  = (Degree180 | (_colorOrder & BitMask_ReMapCmdA2)) ;
			_width =_widthStartOLED;
			_height = _heightStartOLED;
			break;
		case Degrees_270:
			rotateValue  = (Degree270 | (_colorOrder & BitMask_ReMapCmdA2)) ;
			_width =_heightStartOLED;
			_height = _widthStartOLED;
			break;
	}
	writeCommand(rotateValue);
}

/*!
	@brief initialise the variables that define the size of the screen
	@param width_OLED width in pixels
	@param height_OLED height in pixels
*/
void SSD1331_OLED::InitScreenSize(uint16_t width_OLED, uint16_t height_OLED)
{
	_width = width_OLED;
	_height = height_OLED;
	_widthStartOLED = width_OLED;
	_heightStartOLED = height_OLED;
}

/*!
	@brief intialise HW SPI setup
	@param speed_Khz SPI baudrate in Khz , 1000 = 1 Mhz
	@param spiInterface Spi interface, spi0 spi1 etc
	@note method overload used , method 1 hardware SPI 
*/
void  SSD1331_OLED::InitSPIType(uint32_t speed_Khz, spi_inst_t *spiInterface)
{
	_pspiInterface = spiInterface;
	_speedSPIKHz = speed_Khz;
	_hardwareSPI = true;
}

/*!
	@brief intialise SW SPI set
	@param CommDelay SW SPI GPIO delay uS
	@note method overload used , method 2 software SPI 
*/
void  SSD1331_OLED::InitSPIType(uint16_t CommDelay)
{
	HighFreqDelaySet(CommDelay);
	_hardwareSPI = false;
}

/*!
	@brief Freq delay used in SW SPI getter, uS delay used in SW SPI method
	@return The  GPIO communications delay in uS
*/
uint16_t SSD1331_OLED::HighFreqDelayGet(void) const {return _SWSPIGPIODelay;}

/*!
	@brief Freq delay used in SW SPI setter, uS delay used in SW SPI method
	@param CommDelay The GPIO communications delay in uS
*/
void  SSD1331_OLED::HighFreqDelaySet(uint16_t CommDelay){_SWSPIGPIODelay = CommDelay;}

/*!
	@brief SPI displays set an address window rectangle for blitting pixels
	@param  x Top left corner x coordinate
	@param  y  Top left corner y coordinate
	@param  w  Width of window
	@param  h  Height of window
	@note virtual function overloads graphics library
*/
void SSD1331_OLED::setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
	if (_displayRotate & 1) {  // 90° or 270° rotation
		writeCommand(_CMD_SETROW);
		writeCommand(x);
		writeCommand(w);

		writeCommand(_CMD_SETCOLUMN);
		writeCommand(y);
		writeCommand(h);
		} else {  // 0° or 180° rotation
		writeCommand(_CMD_SETCOLUMN);
		writeCommand(x);
		writeCommand(w);

		writeCommand(_CMD_SETROW);
		writeCommand(y);
		writeCommand(h);
	}
}

/*!
	@brief Command Initialization sequence for SSD1331 display
*/
void SSD1331_OLED::cmdBegin(void)
{
	writeCommand(_CMD_DISPLAYOFF);
	ConfigueContrast();
	writeCommand(_CMD_MASTERCURRENT);
	writeCommand(0x06);
	writeCommand(_CMD_PRECHARGEA);
	writeCommand(0x64);
	writeCommand(_CMD_PRECHARGEB);
	writeCommand(0x78);
	writeCommand(_CMD_PRECHARGEC);
	writeCommand(0x64);
	ConfigueDimMode();

	writeCommand(_CMD_SETREMAP);
	switch (_colorOrder)
	{
		case COLORORDER_RGB: writeCommand(COLORORDER_RGB); break;
		case COLORORDER_BGR: writeCommand(COLORORDER_BGR); break;
		default: writeCommand(COLORORDER_RGB); break;
	};

	writeCommand(_CMD_STARTLINE);
	writeCommand(0x0);
	writeCommand(_CMD_DISPLAYOFFSET);
	writeCommand(0x0);
	writeCommand(_CMD_NORMALDISPLAY);
	writeCommand(_CMD_SETMULTIPLEX);
	writeCommand(0x3F);
	writeCommand(_CMD_SETMASTER);
	writeCommand(0x8E);
	writeCommand(_CMD_POWERMODE);
	writeCommand(0x0B); // disable power save mode
	writeCommand(_CMD_PRECHARGE);
	writeCommand(0x31);
	writeCommand(_CMD_CLOCKDIV);
	writeCommand(0xF0); // 7:4 = Oscillator Freq, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)
	writeCommand(_CMD_PRECHARGE_VOLT);
	writeCommand(0x3A);
	writeCommand(_CMD_V_VOLTAGE );
	writeCommand(0x3E);

	writeCommand(_CMD_DISPLAYON);
}

/*!
	@brief Command sequence for configure Dim mode contrast
*/
void SSD1331_OLED::ConfigueDimMode(void)
{
	writeCommand(_CMD_DIM_MODE_SET);
	writeCommand(0x00);
	writeCommand(DimContrastValues.Dim_ContrastColorA);
	writeCommand(DimContrastValues.Dim_ContrastColorB);
	writeCommand(DimContrastValues.Dim_ContrastColorC);
	writeCommand(0x0F);
}

/*!
	@brief Command sequence for configure  contrast
*/
void SSD1331_OLED::ConfigueContrast(void)
{
	writeCommand(_CMD_CONTRASTA);
	writeCommand(ContrastValues.ContrastColorA);
	writeCommand(_CMD_CONTRASTB);
	writeCommand(ContrastValues.ContrastColorB);
	writeCommand(_CMD_CONTRASTC);
	writeCommand(ContrastValues.ContrastColorC);
}

/*!
	@brief Clears a specific rectangular window on the OLED display.
	@param column1 Starting column (X coordinate) of the window
	@param row1 Starting row (Y coordinate) of the window
	@param column2 Ending column (X coordinate) of the window 
	@param row2 Ending row (Y coordinate) of the window
	@note The coordinates must be within the display bounds 
		Uses the SSD1331 `_CMD_CLEAR_WINDOW` command.
 */
void SSD1331_OLED::ClearWindowCmd(uint8_t column1 , uint8_t row1 , uint8_t column2 , uint8_t row2)
{
	if (_displayRotate % 2 == 1) // 90° or 270° rotations,
	{
		std::swap(column1, row1);
		std::swap(column2, row2);
	}
	writeCommand(_CMD_CLEAR_WINDOW);
	writeCommand(column1);
	writeCommand(row1);
	writeCommand(column2);
	writeCommand(row2);
	MICROSEC_DELAY(500);
}

/*!
	@brief Copies a specific rectangular window to a new position on the OLED display.
	@param column1 Starting column (X coordinate) of the source window (Range: 0-95).
	@param row1 Starting row (Y coordinate) of the source window (Range: 0-63).
	@param column2 Ending column (X coordinate) of the source window (Range: 0-95).
	@param row2 Ending row (Y coordinate) of the source window (Range: 0-63).
	@param column3 Destination column (X coordinate) where the copied window will be placed (Range: 0-95).
	@param row3 Destination row (Y coordinate) where the copied window will be placed (Range: 0-63).
	@note The coordinates must be within the display bounds (96x64 pixels).
			Uses the SSD1331 `_CMD_COPY_WINDOW` command.

 */
void SSD1331_OLED::CopyWindowCmd(uint8_t column1 , uint8_t row1 , uint8_t column2 , uint8_t row2, uint8_t column3 , uint8_t row3)
{
	if (_displayRotate % 2 == 1) // 90° or 270° rotations,
	{
		std::swap(column1, row1);
		std::swap(column2, row2);
		std::swap(column3, row3);
	}
	writeCommand(_CMD_COPY_WINDOW);
	writeCommand(column1);
	writeCommand(row1);
	writeCommand(column2);
	writeCommand(row2);
	writeCommand(column3);
	writeCommand(row3);
	MICROSEC_DELAY(500);
}


/*!
	@brief Dims a specific rectangular window on the OLED display.
	@param column1 Starting column (X coordinate) of the window to be dimmed
	@param row1 Starting row (Y coordinate) of the window to be dimmed
	@param column2 Ending column (X coordinate) of the window to be dimmed 
	@param row2 Ending row (Y coordinate) of the window to be dimmed
	@note The coordinates must be within the display bounds .
		Uses the SSD1331 `_CMD_DIM_WINDOW` command.
 */
void SSD1331_OLED::DimWindowCmd(uint8_t column1 , uint8_t row1 , uint8_t column2 , uint8_t row2)
{
	if (_displayRotate % 2 == 1) // 90° or 270° rotations,
	{
		std::swap(column1, row1);
		std::swap(column2, row2);
	}
	writeCommand(_CMD_DIM_WINDOW);
	writeCommand(column1);
	writeCommand(row1);
	writeCommand(column2);
	writeCommand(row2);
	MICROSEC_DELAY(500);
}

/*!
	@brief Draws a line between two points on the OLED display.
	@param c1 Starting column (X coordinate) of the line (Range: 0-95).
	@param r1 Starting row (Y coordinate) of the line (Range: 0-63).
	@param c2 Ending column (X coordinate) of the line (Range: 0-95).
	@param r2 Ending row (Y coordinate) of the line (Range: 0-63).
	@param color 16-bit RGB565 color value for the line.
	@note this uses built-in OLED command NOT the graphics library.
 */
void SSD1331_OLED::DrawLineCmd(uint8_t c1, uint8_t r1, uint8_t c2, uint8_t r2, uint16_t color)
{
	if (_displayRotate % 2 == 1) // 90° or 270° rotations,
	{
		std::swap(c1, r1);
		std::swap(c2, r2);
	}
	writeCommand(_CMD_DRAWLINE);
	writeCommand(c1);
	writeCommand(r1);
	writeCommand(c2);
	writeCommand(r2);
	WriteColor16(color);
	MICROSEC_DELAY(100);
}

/*!
	@brief Draws a rectangle on the OLED display, with optional filling.
		This function issues a command to draw a rectangle with the specified 
		coordinates and color. The rectangle can be either outlined or filled.
		The function also accounts for screen rotation when determining coordinates.
	@param c1 Starting column (X coordinate) of the rectangle.
	@param r1 Starting row (Y coordinate) of the rectangle.
	@param c2 Ending column (X coordinate) of the rectangle.
	@param r2 Ending row (Y coordinate) of the rectangle.
	@param color The 16-bit RGB565 color value for the rectangle.
	@param fill If true, the rectangle will be filled with the specified color.
            If false, only the border will be drawn.
    @note this uses built-in OLED command NOT the graphics library.
 */
void SSD1331_OLED::DrawRectCmd(uint8_t c1, uint8_t r1, uint8_t  c2, uint8_t r2, uint16_t color, bool fill) {

	if (_displayRotate % 2 == 1) // 90° or 270° rotations,
	{
		std::swap(c1, r1);
		std::swap(c2, r2);
	}
	//turn fill on or off
	if (fill)
	{
		writeCommand(_CMD_FILL);
		writeCommand(_CMD_ENABLE_FILL);
	}else{
		writeCommand(_CMD_FILL);
		writeCommand(_CMD_DISABLE_FILL);
	}
	// Rect coord
	writeCommand(_CMD_DRAWRECT);
	writeCommand(c1);
	writeCommand(r1);
	writeCommand(c2);
	writeCommand(r2);
	// Rect color
	WriteColor16(color);
	if (fill){
		WriteColor16(color);
	}else{
		writeCommand(0);
		writeCommand(0);
		writeCommand(0);
	}
	MICROSEC_DELAY(500);
}

/*!
	@brief Writes a 16-bit RGB565 color to the SSD1331 OLED display.
		This function extracts the red, green, and blue components from the 
		16-bit RGB565 color value and sends them as 6-bit values to the display.
		RRRRR GGGGGG BBBBB.
	@param colorWrite 16-bit RGB565 color value to be written to the display.
 */
void SSD1331_OLED::WriteColor16(uint16_t colorWrite)
{
	writeCommand(colorWrite >> 10 & 0x3E); // Extract and send 6-bit Red component
	writeCommand(colorWrite >> 5 & 0x3F);  // Extract and send 6-bit Green component
	writeCommand(colorWrite << 1 & 0x3E);  // Extract and send 6-bit Blue component
}

//**************** EOF *****************
