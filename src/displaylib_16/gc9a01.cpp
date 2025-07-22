/*!
	@file   gc9a01.cpp
	@author Gavin Lyons
	@brief  Source file. Contains driver methods for gc9a01 display 
*/

#include "../../include/displaylib_16/gc9a01.hpp"
 
/*!
	@brief Constructor for class GC9A01_TFT
*/
GC9A01_TFT :: GC9A01_TFT(){}

/*!
	@brief : Init Hardware SPI
*/
void GC9A01_TFT::TFTHWSPIInitialize(void)
{
	spi_init(_pspiInterface, _speedSPIKHz * 1000); // Initialize SPI port 
	// Initialize SPI pins : clock and data
	DISPLAY_SDATA_SPI_FUNC;
	DISPLAY_SCLK_SPI_FUNC;
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

/*!
	@brief: Call when powering down TFT
	@note  Will switch off SPI 
*/
void GC9A01_TFT ::TFTPowerDown(void)
{
	TFTenableDisplay(false);
	if (_resetPinOn == true) {
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
	@brief Method for Hardware Reset pin control
*/
void GC9A01_TFT ::TFTResetPIN() {
	if (_resetPinOn == false) return;
	DISPLAY_RST_SetDigitalOutput;
	DISPLAY_RST_SetHigh;
	MILLISEC_DELAY(5);
	DISPLAY_RST_SetLow;
	MILLISEC_DELAY(20);
	DISPLAY_RST_SetHigh;
	MILLISEC_DELAY(150);
}

/*!
	@brief  sets up TFT GPIO
	@param rst reset GPIO, optional pass -1 to disable, see note
	@param dc data or command GPIO.
	@param cs chip select GPIO 
	@param sclk Data clock GPIO  
	@param din Data to TFT GPIO 
	@note if -1 is passed for reset pin, software reset is used, if LCD has optional reset pin
*/
void GC9A01_TFT::TFTsetupGPIO(int8_t rst, int8_t dc, int8_t cs, int8_t sclk, int8_t din)
{
	_display_RST= rst;
	_display_DC = dc;
	_display_CS = cs;
	_display_SDATA = din;
	_display_SCLK = sclk;

	if (_display_RST == -1 ){
		_resetPinOn = false;
	}else{
		_resetPinOn = true;
		DISPLAY_RST_INIT;
	}
	DISPLAY_DC_INIT; 
	DISPLAY_CS_INIT; 
	DISPLAY_SDATA_INIT; 
	DISPLAY_SCLK_INIT; 
}

/*!
	@brief init routine for GC9A01 controller
*/
void GC9A01_TFT::TFTGC9A01Initialize() 
{
	if (_resetPinOn == true) 
	{
		TFTResetPIN();
	}else {
		TFTresetSWDisplay();
	}

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
		TFTHWSPIInitialize();
	}
		cmdInitSequence();
		TFTsetRotation(Degrees_0);
}


/*!
	@brief Toggle the invert mode
	@param invert true invert off , false invert on
*/
void GC9A01_TFT ::TFTchangeInvertMode(bool invert) {
	if(invert) {
		writeCommand(GC9A01_INVOFF);
	} else {
		writeCommand(GC9A01_INVON);
	}
}



/*!
	@brief: change rotation of display.
	@param mode display_rotate_e enum
	0 = Normal
	1=  90 rotate
	2 = 180 rotate
	3 =  270 rotate
*/
void GC9A01_TFT::TFTsetRotation(display_rotate_e mode) {
	uint8_t madctl = 0;
	switch (mode) {
		case Degrees_0 :
			madctl = (MADCTL_FLAGS_t::MX | MADCTL_FLAGS_t::BGR);
			_width =_widthStartTFT;
			_height = _heightStartTFT;
			break;
		case Degrees_90:
			madctl = (MADCTL_FLAGS_t::MV | MADCTL_FLAGS_t::BGR);
			_width  =_heightStartTFT;
			_height = _widthStartTFT;
			break;
		case Degrees_180:
			madctl =(MADCTL_FLAGS_t::MY | MADCTL_FLAGS_t::BGR);
			_width =_widthStartTFT;
			_height = _heightStartTFT;
			break;
		case Degrees_270:
			madctl = (MADCTL_FLAGS_t::MX | MADCTL_FLAGS_t::MY |MADCTL_FLAGS_t::MV | MADCTL_FLAGS_t::BGR);
			_width =_heightStartTFT;
			_height = _widthStartTFT;
			break;
	}
	writeCommand(GC9A01_MADCTL);
	writeData(madctl);
}

/*!
	@brief initialise the variables that define the size of the screen
	@param width_TFT width in pixels
	@param height_TFT height in pixels
	@note  The offsets can be adjusted for any issues with manufacture tolerance/defects
*/
void GC9A01_TFT  :: TFTInitScreenSize( uint16_t width_TFT, uint16_t height_TFT)
{

	_width = width_TFT;
	_height = height_TFT;
	_widthStartTFT = width_TFT;
	_heightStartTFT = height_TFT;
}

/*!
	@brief intialise HW SPI setup
	@param speed_Khz SPI baudrate in Khz , 1000 = 1 Mhz
	@param spiInterface Spi interface, spi0 spi1 etc
	@note method overload used , method 1 hardware SPI 
*/
void GC9A01_TFT  :: TFTInitSPIType(uint32_t speed_Khz,  spi_inst_t* spiInterface) 
{
	_pspiInterface = spiInterface;
	_speedSPIKHz = speed_Khz;
	_hardwareSPI = true;
}

/*!
	@brief intialise SW SPI set
	@param CommDelay SW SPI GPIO delay
	@note method overload used , method 2 software SPI 
*/
void GC9A01_TFT ::TFTInitSPIType(uint16_t CommDelay) 
{
	TFTSwSpiGpioDelaySet(CommDelay);
	_hardwareSPI = false;
}


/*!
	@brief Freq delay used in SW SPI getter, uS delay used in SW SPI method
	@return The  GPIO communications delay in uS
*/
uint16_t GC9A01_TFT::TFTSwSpiGpioDelayGet(void){return _SWSPIGPIODelay;}

/*!
	@brief Freq delay used in SW SPI setter, uS delay used in SW SPI method
	@param CommDelay The GPIO communications delay in uS
*/
void  GC9A01_TFT::TFTSwSpiGpioDelaySet(uint16_t CommDelay){_SWSPIGPIODelay = CommDelay;}


/*!
	@brief Command Initialization sequence for GC9A01 display
*/
void GC9A01_TFT::cmdInitSequence(void)
{
	writeCommand(GC9A01_INREGEN1);
	writeCommand(GC9A01_INREGEN2);

	// Undocumented in datasheet registers
	writeCommand(0xEB); 
	writeData(0x14);
	writeCommand(0x84); 
	writeData(0x60);
	writeCommand(0x85);
	writeData(0xF7);
	writeCommand(0x86);
	writeData(0xFC);
	writeCommand(0x87);
	writeData(0x28);
	writeCommand(0x8E);
	writeData(0x0F);
	writeCommand(0x8F);
	writeData(0xFC);
	writeCommand(0x88);
	writeData(0x0A);
	writeCommand(0x89);
	writeData(0x21);
	writeCommand(0x8A);
	writeData(0x00);
	writeCommand(0x8B);
	writeData(0x80);
	writeCommand(0x8C);
	writeData(0x01);
	writeCommand(0x8D);
	writeData(0x03);

	writeCommand(GC9A01_FUNCTION_CTRL);//0xb6
	writeData(0x00);
	writeData(0x00);
	writeCommand(GC9A01_MADCTL);
	writeData(MADCTL_FLAGS_t::MX | MADCTL_FLAGS_t::BGR); // BGR color filter panel
	writeCommand(GC9A01_COLMOD);
	writeData(0x05);  // 16 bits / pixel DBI

	writeCommand(0x90); // Undocumented in datasheet registers
	uint8_t seqReg90[] = {0x08, 0x08, 0x08, 0x08};
	spiWriteDataBuffer(seqReg90, sizeof(seqReg90));

	writeCommand(GC9A01_TEWC);
	writeData(0x01); // Tearing Effect width 
	writeCommand(0xBD); // Undocumented in datasheet register
	writeData(0x06);
	writeCommand(0xBC); // Undocumented in datasheet register
	writeData(0x00);
	writeCommand(0xFF); // Undocumented in datasheet register
	uint8_t seqRegFF[] = {0x60, 0x01, 0x04};
	spiWriteDataBuffer(seqRegFF, sizeof(seqRegFF));

	writeCommand(GC9A01_POWER2);
	writeData(0x48);
	writeCommand(GC9A01_POWER3);
	writeData(0x48);
	writeCommand(GC9A01_POWER4);
	writeData(0x25);

	 // Undocumented in datasheet register
	writeCommand(0xBE);
	writeData(0x11);
	writeCommand(0xE1);
	writeData(0x10);
	writeData(0x0E);
	writeCommand(0xDF);
	uint8_t seqRegDF[] = {0x21, 0x10, 0x02};
	spiWriteDataBuffer(seqRegDF, sizeof(seqRegDF));

	// gamma control sequence
	writeCommand(GC9A01_GAMMA1);
	uint8_t seqGamma1_3[] = {0x4b, 0x0f, 0x0A, 0x0B, 0x15, 0x30};
	spiWriteDataBuffer(seqGamma1_3, sizeof(seqGamma1_3));
	writeCommand(GC9A01_GAMMA2);
	uint8_t seqGamma2_4[] = {0x43, 0x70, 0x72, 0x36, 0x37, 0x6f};
	spiWriteDataBuffer(seqGamma2_4, sizeof(seqGamma2_4));
	writeCommand(GC9A01_GAMMA3);
	spiWriteDataBuffer(seqGamma1_3, sizeof(seqGamma1_3));
	writeCommand(GC9A01_GAMMA4 );
	spiWriteDataBuffer(seqGamma2_4, sizeof(seqGamma2_4));

	// Undocumented in datasheet register
	writeCommand(0xED);
	writeData(0x1B);
	writeData(0x0B);
	writeCommand(0xAC);
	writeData(0x47);
	writeCommand(0xAE);
	writeData(0x77);
	writeCommand(0xCD);
	writeData(0x63);
	writeCommand(0x70);
	uint8_t seqReg70[] = {0x07, 0x09, 0x04, 0x0C, 0x0D, 0x09, 0x07, 0x08, 0x03};
	spiWriteDataBuffer(seqReg70, sizeof(seqReg70));

	writeCommand(GC9A01_FRAMERATE);
	writeData(0x34); // 4 dot inversion DINV[3:0] : Set display inversion mode

	// Undocumented in datasheet registers
	static uint8_t seqReg60[] = {
		0x38, 0x0B, 0x76, 0x62,0x39, 0xF0, 0x76, 0x62};
	writeCommand(0x60); 
	spiWriteDataBuffer(seqReg60, sizeof(seqReg60));
	static  uint8_t seqReg61[] = {
		0x38, 0xF6, 0x76, 0x62,0x38, 0xF7, 0x76, 0x62};
	writeCommand(0x61); 
	spiWriteDataBuffer(seqReg61, sizeof(seqReg61));
	static  uint8_t seqReg62[] = {
		0x38, 0x0D, 0x71, 0xED, 0x76, 0x62,0x38, 0x0F, 0x71, 0xEF, 0x76, 0x62};
	writeCommand(0x62); 
	spiWriteDataBuffer(seqReg62, sizeof(seqReg62));
	static  uint8_t seqReg63[] = {
		0x38, 0x11, 0x71, 0xF1, 0x76, 0x62,0x38, 0x13, 0x71, 0xF3, 0x76, 0x62};
	writeCommand(0x63); // 0x63
	spiWriteDataBuffer(seqReg63, sizeof(seqReg63));
	static  uint8_t seqReg64[] = {
		0x3B, 0x29, 0xF1, 0x01, 0xF1, 0x00, 0x0A};
	writeCommand(0x64); 
	spiWriteDataBuffer(seqReg64, sizeof(seqReg64));
	static uint8_t seqReg66[] = {
		0x3C, 0x00, 0xCD, 0x67, 0x45, 0x45, 0x10, 0x00, 0x00, 0x00};
	writeCommand(0x66); 
	spiWriteDataBuffer(seqReg66, sizeof(seqReg66));
	static uint8_t seqReg67[] = {
		0x00, 0x3C, 0x00, 0x00, 0x00, 0x01, 0x54, 0x10, 0x32, 0x98};
	writeCommand(0x67); 
	spiWriteDataBuffer(seqReg67, sizeof(seqReg67));

	static uint8_t seqPorchCtrl[] = {
		0x08, 0x09, 0x14, 0x08};
	writeCommand(GC9A01_BLANK_PORCH_CTRL); // 
	spiWriteDataBuffer(seqPorchCtrl, sizeof(seqPorchCtrl));


	// Undocumented in datasheet registers
	static uint8_t seqReg74[] = {
		0x10, 0x85, 0x80, 0x00, 0x00, 0x4E, 0x00
	};
	writeCommand(0x74); 
	spiWriteDataBuffer(seqReg74, sizeof(seqReg74));
	writeCommand(0x98);
	writeData(0x3E);
	writeData(0x07);

	writeCommand(GC9A01_TEON);
	writeData(0x00);
	writeCommand(GC9A01_INVON);
	writeCommand(GC9A01_SLPOUT);
	MILLISEC_DELAY (_sleepDelay);
	TFTenableDisplay(true);
}


/*!
  @brief SPI displays set an address window rectangle for blitting pixels
  @param  x1 Top left corner x coordinate
  @param  y1  Top left corner y coordinate
  @param  w  Width of window
  @param  h  Height of window
  @note https://en.wikipedia.org/wiki/Bit_blit
 */
void GC9A01_TFT::setAddrWindow(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h)
{	
	//if drawing a single pixel we need do this to avoid a blank pixel for this device
	if (w - x1 == 1)  {w = x1;}
	if( h - y1 == 1)  {h = y1;}
	uint8_t x1Higher = (x1 >> 8) ;
	uint8_t x1Lower  = (x1 &  0xFF);
	uint8_t x2Higher = (w >> 8);
	uint8_t x2Lower  = (w &  0xFF);
	uint8_t seqCASET[]    {x1Higher ,x1Lower,x2Higher,x2Lower};
	uint8_t y1Higher = (y1 >> 8); 
	uint8_t y1Lower  = (y1 &  0xFF);
	uint8_t y2Higher = (h >> 8);
	uint8_t y2Lower  = (h &  0xFF);
	uint8_t seqRASET[]    {y1Higher,y1Lower,y2Higher,y2Lower};
	writeCommand(GC9A01_CASET); //Column address set
	spiWriteDataBuffer(seqCASET, sizeof(seqCASET));
	writeCommand(GC9A01_RASET); //Row address set
	spiWriteDataBuffer(seqRASET, sizeof(seqRASET));
	writeCommand(GC9A01_RAMWR); // Write to RAM*/
}

/*!
	@brief This method defines the Vertical Scrolling Area of the display where:
	@param topFixed describes the Top Fixed Area.
	@param scrollArea describes the Scrolling Area.
	@param bottomFixed describes the Bottom Fixed Area.
*/
void GC9A01_TFT::TFTsetScrollArea(uint16_t topFixed, uint16_t scrollArea, uint16_t bottomFixed) {

	writeCommand(GC9A01_VSCRDEF);
	writeData(topFixed >> 8);
	writeData(topFixed & 0xFF);
	writeData(scrollArea >> 8);
	writeData(scrollArea  & 0xFF);
	writeData(bottomFixed >> 8);
	writeData(bottomFixed & 0xFF);
}

/*!
	@brief This method is used together with the setScrollDefinition.
	@param vsp scrolling mode
*/
void GC9A01_TFT::TFTsetScrollStart(uint16_t vsp) {
	writeCommand(GC9A01_VSCRSADD);
	writeData(vsp >> 8);
	writeData(vsp & 0xFF);
}

/*! @brief Scroll Mode can be left ,by the Normal Display Mode ON cmd*/
void GC9A01_TFT::TFTScrollModeLeave(void) {writeCommand(GC9A01_NORON);}

/*!
	@brief Software reset command
*/
void GC9A01_TFT::TFTresetSWDisplay(void) 
{
  writeCommand(GC9A01_SWRESET);
  MILLISEC_DELAY(150);
}

/*!
	@brief enable /disable display mode
	@param enableDisplay true enable on false disable
	@note Temporarily blank the screen.
	Use Case: Screen blanking, brief off periods without resetting or reinitializing the display.
*/
void GC9A01_TFT::TFTenableDisplay(bool enableDisplay){
	if(enableDisplay) {
		writeCommand(GC9A01_DISPON);
		_displayOn = true;
	} else {
		writeCommand(GC9A01_DISPOFF);
		_displayOn = false;
	}
}

/*!
	@brief Set the power mode of the display
	@param mode The power state to set
	@details 
		Power states are based on the power control flow chart in the datasheet.
		FIG 89 5.10.2. Power Flow Chart	
*/
void GC9A01_TFT::TFTsetPowerMode(PowerState_e mode) {
	// If already in the desired state or off , skip
	if (_currentPowerState == mode || _displayOn != true) 
	{
		printf("Warning: TFTsetPowerMode: Display already in this state or off, nothing done\n");
		return;
	}
	
	// Always return to a known base state
	writeCommand(GC9A01_SLPOUT);
	sleep_ms(_sleepDelay);

	switch (mode) {
		case PowerState_e::NormalIdleOff:
			writeCommand(GC9A01_NORON);
			writeCommand(GC9A01_IDLEOFF);
			break;
		case PowerState_e::NormalIdleOn:
			writeCommand(GC9A01_NORON);
			writeCommand(GC9A01_IDLEON);
			break;
		case PowerState_e::PartialIdleOff:
			writeCommand(GC9A01_PTLON);
			writeCommand(GC9A01_IDLEOFF);
			break;
		case PowerState_e::PartialIdleOn:
			writeCommand(GC9A01_PTLON);
			writeCommand(GC9A01_IDLEON);
			break;
		case PowerState_e::SleepNormalIdleOff:
			writeCommand(GC9A01_NORON);
			writeCommand(GC9A01_IDLEOFF);
			writeCommand(GC9A01_SLPIN);
			sleep_ms(_sleepDelay);
			break;
		case PowerState_e::SleepNormalIdleOn:
			writeCommand(GC9A01_NORON);
			writeCommand(GC9A01_IDLEON);
			writeCommand(GC9A01_SLPIN);
			sleep_ms(_sleepDelay);
			break;
		case PowerState_e::SleepPartialIdleOff:
			writeCommand(GC9A01_PTLON);
			writeCommand(GC9A01_IDLEOFF);
			writeCommand(GC9A01_SLPIN);
			sleep_ms(_sleepDelay);
			break;
		case PowerState_e::SleepPartialIdleOn:
			writeCommand(GC9A01_PTLON);
			writeCommand(GC9A01_IDLEON);
			writeCommand(GC9A01_SLPIN);
			sleep_ms(_sleepDelay);
			break;
	}

	_currentPowerState = mode;
}

/*!
  @brief Set display brightness (0–255).
  @param level Brightness level, 0 = darkest, 255 = brightest
  @note This is a software brightness control, not hardware PWM, may not work on all displays.
*/
void GC9A01_TFT::TFTsetBrightness(uint8_t level)
{
	writeCommand(GC9A01_SETCTRL); // CTRL Display
	writeData(0x2C);    // Brightness registers are active, Display Dimming is on Backlight On
	writeCommand(GC9A01_SETBRIGHT);
	writeData(level);
}
//**************** EOF *****************
