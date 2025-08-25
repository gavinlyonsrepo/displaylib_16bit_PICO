/*!
	@file     ili9341.cpp
	@author   Gavin Lyons
	@brief   Source for to manage hardware interface with ILI9341 display
*/

#include "../../include/displaylib_16/ili9341.hpp"

/*! @brief Constructor for class ILI9341_TFT */
ILI9341_TFT::ILI9341_TFT(){}

/*!
	@brief  sets up TFT GPIO 
	@param rst reset GPIO
	@param dc data or command GPIO.
	@param cs chip select GPIO
	@param sclk Data clock GPIO
	@param mosi Data to TFT GPIO
	@param miso Data from TFT GPIO, -1 if not used
*/
void ILI9341_TFT::SetupGPIO(int8_t rst, int8_t dc, int8_t cs, int8_t sclk, int8_t mosi, int8_t miso)
{
	if(rst != -1)
	{
		_display_RST= rst;
		_resetPinOn = true;
		DISPLAY_RST_INIT;
	}else{
		_resetPinOn  = false;
	}
	if (miso != -1)
	{
		_display_MISO = miso;
		_MISOPinOn = true;
		DISPLAY_MISO_INIT;
	}else{
		_MISOPinOn = false;
	}
	_display_DC = dc;
	_display_CS = cs;
	_display_SCLK = sclk;
	_display_SDATA = mosi;

	DISPLAY_DC_INIT; 
	DISPLAY_CS_INIT; 
	DISPLAY_SCLK_INIT; 
	DISPLAY_SDATA_INIT; 
}

/*!
	@brief initialise the variables that define the size of the screen
	@param width_TFT width in pixels
	@param height_TFT height in pixels
*/
void ILI9341_TFT::SetupScreenSize(uint16_t width_TFT, uint16_t height_TFT)
{
	_width = width_TFT;
	_height = height_TFT;
	_widthStartTFT = width_TFT;
	_heightStartTFT = height_TFT;
}

/*!
	@brief intialise  SPI, Hardware SPI
	@param speed_Khz SPI baudrate in Khz , 1000 = 1 Mhz
	@param spiInterface Spi interface, spi0 spi1 etc
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI
*/
void ILI9341_TFT::SetupSPI(uint32_t speed_Khz , spi_inst_t *spiInterface)
{
	_pspiInterface = spiInterface;
	_speedSPIKHz = speed_Khz;
	_hardwareSPI = true;
}

/*!
	@brief intialise SPI, Software SPI
	@param CommDelay uS GPIO delay used in software SPI
	@note overloaded 2 off, 1 for HW SPI , 1 for SW SPI
*/
void ILI9341_TFT::SetupSPI(uint16_t CommDelay)
{
	HighFreqDelaySet(CommDelay);
	_hardwareSPI = false;
}

/*!
	@brief init routine for ILI9341_TFT controller
*/
void ILI9341_TFT::ILI9341Initialize()
{
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
		if (_MISOPinOn == true)
		{
			DISPLAY_MISO_SetDigitalInput;
		}
	}else{
		spi_init(_pspiInterface, _speedSPIKHz * 1000); // Initialize SPI port 
		// Initialize SPI pins : clock and data
		DISPLAY_SDATA_SPI_FUNC;
		DISPLAY_SCLK_SPI_FUNC;
		if (_MISOPinOn == true)
		{
			DISPLAY_MISO_SPI_FUNC;
		}
		// Set SPI format
		if(_display_CS > -1) {
			spi_set_format( _pspiInterface,   // SPI instance
							8,      // Number of bits per transfer
							SPI_CPOL_0,      // Polarity (CPOL)
							SPI_CPHA_0,      // Phase (CPHA)
							SPI_MSB_FIRST);
		} else {
			spi_set_format( _pspiInterface,   // SPI instance
							8,      // Number of bits per transfer
							SPI_CPOL_1,      // Polarity (CPOL)
							SPI_CPHA_1,      // Phase (CPHA)
							SPI_MSB_FIRST);
		}
	}
	cmdInit();
}

// SPI related

/*!
	@brief Freq delay used in SW SPI getter, uS delay used in SW SPI method
	@return The  GPIO communications delay in uS
*/
uint16_t ILI9341_TFT::HighFreqDelayGet(void){return _SWSPIGPIODelay;}

/*!
	@brief Freq delay used in SW SPI setter, uS delay used in SW SPI method
	@param CommDelay The GPIO communications delay in uS
*/
void  ILI9341_TFT::HighFreqDelaySet(uint16_t CommDelay){_SWSPIGPIODelay = CommDelay;}

/*!
	@brief Call when powering down TFT
	@note Turns off Display Sets GPIO low and turns off SPI
*/
void  ILI9341_TFT::PowerDown(void)
{
	if (_resetPinOn == true)
	{
		DISPLAY_RST_SetLow;
		DISPLAY_RST_DEINIT;
	}
	DISPLAY_DC_SetLow;
	DISPLAY_CS_SetLow;
	DISPLAY_DC_DEINIT;
	DISPLAY_CS_DEINIT;
	if (_hardwareSPI == true) {
		DISPLAY_SCLK_SPI_FUNC_OFF;
		DISPLAY_SDATA_SPI_FUNC_OFF;
		if(_MISOPinOn == true)
		{
			DISPLAY_MISO_SPI_FUNC_OFF;
		}
		spi_deinit(_pspiInterface);
		DISPLAY_SCLK_DEINIT;
		DISPLAY_SDATA_DEINIT;
		if(_MISOPinOn == true)
		{
			DISPLAY_MISO_DEINIT;
		}
	}else{
		DISPLAY_SCLK_SetLow;
		DISPLAY_SDATA_SetLow;
		DISPLAY_SCLK_DEINIT;
		DISPLAY_SDATA_DEINIT;
		if(_MISOPinOn == true)
		{
			DISPLAY_MISO_DEINIT;
		}
	}
}

/*!
	@brief Command Initialization sequence for ILI9341 LCD TFT display
*/
void ILI9341_TFT::cmdInit(void)
{
	uint8_t seqPCA[]    {0x39,0x2C,0x00,0x34, 0x02};
	uint8_t seqPCB[]    {0x00,0xC1,0x30};
	uint8_t seqDTMCA[]  {0x85,0x00,0x78};
	uint8_t seqPWONCS[] {0x64,0x03,0x12,0x81};

	uint8_t seqDFUNCTR[] {0x08,0x82,0x27};
	uint8_t seqGammaP[] {0x0F,0x31,0x2B,0x0C,0x0E,0x08,0x4E,0xF1,0x37,0x07,0x10,0x03,0x0E,0x09,0x00};
	uint8_t seqGammaN[] {0x00,0x0E,0x14,0x03,0x11,0x07,0x31,0xC1,0x48,0x08,0x0F,0x0C,0x31,0x36,0x0F};

	// Section 0
	writeCommand(ILI9341_DISPOFF); // Display off
	busy_wait_ms(5);
	// Section 1
	writeCommand(ILI9341_PWCTRA); // Power control A
	spiWriteDataBuffer(seqPCA, sizeof(seqPCA));
	writeCommand(ILI9341_PWCTRB); // Power control B 
	spiWriteDataBuffer(seqPCB, sizeof(seqPCB));
	writeCommand(ILI9341_DTMCTRA); // Driver timing control A
	spiWriteDataBuffer(seqDTMCA, sizeof(seqDTMCA));
	writeCommand(ILI9341_DTMCTRC); // Driver timing control C
	writeData(0x00);
	writeData(0x00);
	writeCommand(ILI9341_PWONCS); // Power on sequence control
	spiWriteDataBuffer(seqPWONCS, sizeof(seqPWONCS));
	writeCommand(ILI9341_PURTCTR); // Pump ratio control (This has a subtle effect on colors/saturation.)
	writeData(0x20); // Try 0x00 or 0x30 if color's saturated or dim 
	// Section 2
	writeCommand(ILI9341_PWCTR1); // Power control 1
	writeData(0x23);
	writeCommand(ILI9341_PWCTR2); // Power control 2
	writeData(0x10);
	writeCommand(ILI9341_VMCTR1); // VCM control
	writeData(0x3E);
	writeData(0x28);
	writeCommand(ILI9341_VMCTR2); // VCM control2
	writeData(0x86);
	writeCommand(ILI9341_MADCTL); //  Memory Access Control
	writeData(0x48);
	writeCommand(ILI9341_VSCRSADD); // Vertical scroll zero
	writeData(0x00);
	writeCommand(ILI9341_PIXFMT);// COLMOD: Pixel Format Set
	writeData(0x55);
	writeCommand(ILI9341_FRMCTR1); // Frame Rate Ctrl (Normal Mode/Full Colors)
	writeData(0x00);
	writeData(0x18);
	writeCommand(ILI9341_DFUNCTR); // Display Function Control
	spiWriteDataBuffer(seqDFUNCTR, sizeof(seqDFUNCTR));
	writeCommand(ILI9341_EN3GAM); // 3 Gamma Function Disable
	writeData(0);
	// Section 3
	writeCommand(ILI9341_GAMMASET); // Gamma curve selected
	writeData(0x01);
	writeCommand(ILI9341_GMCTRP1); // Set Gamma P1
	spiWriteDataBuffer(seqGammaP, sizeof(seqGammaP));
	writeCommand(ILI9341_GMCTRN1); // Set Gamma N1
	spiWriteDataBuffer(seqGammaN, sizeof(seqGammaN));
	// Section 4
	writeCommand(ILI9341_SLPOUT); // Exit Sleep
	busy_wait_ms(150);
	writeCommand(ILI9341_DISPON); // Display on
	busy_wait_ms(150);
}

/*!
	@brief   Set origin of (0,0) and orientation of TFT display
	@param   rotation  The index for rotation, from 0-3 inclusive, enum
*/
void ILI9341_TFT::setRotation(display_rotate_e rotation) {
	uint8_t madctlData = 0;
	switch (rotation)
	{
	case Degrees_0 :
		madctlData = (MADCTL_MX | MADCTL_BGR);
		_width = _widthStartTFT ;
		_height = _heightStartTFT ;
	break;
	case Degrees_90:
		madctlData = (MADCTL_MV | MADCTL_BGR);
		_width = _heightStartTFT ;
		_height = _widthStartTFT ;
	break;
	case Degrees_180:
		madctlData = (MADCTL_MY | MADCTL_BGR);
		_width = _widthStartTFT ;
		_height = _heightStartTFT ;
	break;
	case Degrees_270:
		madctlData = (MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
		_width = _heightStartTFT ;
		_height = _widthStartTFT ;
	break;
}
	writeCommand(ILI9341_MADCTL);
	writeData(madctlData);
}

/*!
	@brief   Enable/Disable display color inversion
	@param   invert True to invert, False to have normal color
*/
void ILI9341_TFT::invertDisplay(bool invert) {
	if(invert) {
		writeCommand(ILI9341_INVON);
	} else {
		writeCommand(ILI9341_INVOFF);
	}
}

/*!
	@brief enable /disable display mode
	@param enableDisplay true enable on, false disable
*/
void ILI9341_TFT::EnableDisplay(bool enableDisplay){
	if(enableDisplay) {
		writeCommand(ILI9341_SLPOUT);
		busy_wait_ms(120);
		writeCommand(ILI9341_DISPON);
	} else {
		writeCommand(ILI9341_DISPOFF);
		writeCommand(ILI9341_SLPIN);
		busy_wait_ms(120);
	}
}

/*!
	@brief Scroll display memory
	@param pixelY How many pixels to scroll display by.
*/

void ILI9341_TFT::scrollTo(uint16_t pixelY) {
	writeCommand(ILI9341_VSCRSADD);
	writeData(pixelY >> 8);
	writeData(pixelY & 0xff);
}

/*!
	@brief To Return Display to normal mode
	@note used after scroll set for example
*/
void ILI9341_TFT::NormalMode(void){writeCommand(ILI9341_NORON);}

/*!
	@brief   Set the height of the Top and Bottom Scroll Margins
	@param   top The height of the Top scroll margin
	@param   bottom The height of the Bottom scroll margin
 */

void ILI9341_TFT::setScrollMargins(uint16_t top, uint16_t bottom) {
	// TFA+VSA+BFA must equal 320
	if (top + bottom <= _heightStartTFT ) 
	{
		uint16_t middle = _heightStartTFT  - (top + bottom);
		uint8_t dataBuffer[6];
		dataBuffer[0] = top >> 8;
		dataBuffer[1] = top & 0xff;
		dataBuffer[2] = middle >> 8;
		dataBuffer[3] = middle & 0xff;
		dataBuffer[4] = bottom >> 8;
		dataBuffer[5] = bottom & 0xff;
		writeCommand(ILI9341_VSCRDEF);
		spiWriteDataBuffer(dataBuffer, sizeof(dataBuffer));
	}
}

/*!
	@brief Set the address window - the rectangle we will write to RAM with
			the next chunk of SPI data writes. The ILI9341 will automatically wrap
			the data as each row is filled
	@param x0  TFT memory 'x' origin
	@param y0  TFT memory 'y' origin
	@param x1  Width of rectangle
	@param y1  Height of rectangle
*/
void ILI9341_TFT::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {

	uint8_t x0Higher = x0 >> 8;
	uint8_t x0Lower  = x0 &  0xFF;
	uint8_t y0Higher = y0 >> 8;
	uint8_t y0Lower  = y0 &  0xFF;
	uint8_t x1Higher = x1 >> 8;
	uint8_t x1Lower  = x1 &  0xFF;
	uint8_t y1Higher = y1 >> 8;
	uint8_t y1Lower  = y1 &  0xFF;
	uint8_t seqCASET[]    {x0Higher,x0Lower,x1Higher,x1Lower};
	uint8_t seqPASET[]    {y0Higher,y0Lower,y1Higher,y1Lower};
	writeCommand(ILI9341_CASET); //Column address set
	spiWriteDataBuffer(seqCASET, sizeof(seqCASET));
	writeCommand(ILI9341_PASET); //Row address set
	spiWriteDataBuffer(seqPASET, sizeof(seqPASET));
	writeCommand(ILI9341_RAMWR); // Write to RAM*/
}

/*!
	@brief: Method for Hardware Reset pin control
*/
void ILI9341_TFT::ResetPin() 
{
	if (_resetPinOn == true)
	{
		DISPLAY_RST_SetDigitalOutput;
		DISPLAY_RST_SetHigh;
		busy_wait_ms(5);
		DISPLAY_RST_SetLow;
		busy_wait_ms(20);
		DISPLAY_RST_SetHigh;
		busy_wait_ms(120);
	}else{
		writeCommand(ILI9341_SWRESET); // no hw reset pin, software reset.
		busy_wait_ms(120);
	}
}

/*!
	@brief  Read a single diagnostic byte from the ILI9341 controller.
			This function sends a diagnostic read command and retrieves
			the corresponding status or mode byte from the display controller.
	@param reg The diagnostic register to read
	@param index Optional index offset (usually zero).
	@return The diagnostic byte read from the specified register. Will
			return early in software SPI if MISO pin not set.
	@note This is primarily intended for debugging and testing.
 */
uint8_t ILI9341_TFT::readDiagByte(ILI9341_ReadRegister_e reg, uint8_t index) {
	uint8_t result = 0;
	uint8_t spiData ;
	if (_hardwareSPI) {
		DISPLAY_CS_SetLow;
		// 1) Set read index/preamble
		DISPLAY_DC_SetLow;
		spiData = 0xD9;
		spi_write_blocking(_pspiInterface, &spiData, 1);
		DISPLAY_DC_SetHigh;
		spiData = uint8_t(0x10 + index);
		spi_write_blocking(_pspiInterface, &spiData, 1);
		// 2) Issue the read command
		DISPLAY_DC_SetLow;
		spiData = reg;
		// 3) Read response byte 
		spi_write_blocking(_pspiInterface, &spiData, 1);
		DISPLAY_DC_SetHigh;
		uint8_t txData[] = {0x00};
		spi_write_read_blocking(_pspiInterface, txData, &result,1);
		DISPLAY_CS_SetHigh;
	} else {
		if (!_MISOPinOn) 	// check if MISO pin set 
		{
			if(DisLib16::isDebugMode())
				printf("Error: MISO not set for SW SPI, exiting");
			return result;
		}
		DISPLAY_CS_SetLow;
		// 1) Set read index
		DISPLAY_DC_SetLow;
		spiWriteSoftware(0xD9);
		DISPLAY_DC_SetHigh;
		spiWriteSoftware(uint8_t(0x10 + index));
		// 2) Issue the read command
		DISPLAY_DC_SetLow;
		spiWriteSoftware(reg);
		// 3) Read response byte
		DISPLAY_DC_SetHigh;
		result = spiReadSoftware();
		DISPLAY_CS_SetHigh;
	}
	return result;
}
