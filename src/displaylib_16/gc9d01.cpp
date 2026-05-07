/*!
	@file   gc9d01.cpp
	@author Gavin Lyons
	@brief  Source file. Contains driver methods for gc9d01 display 
*/

#include "../../include/displaylib_16/gc9d01.hpp"
 
/*!
	@brief Constructor for class GC9D01_TFT
*/
GC9D01_TFT :: GC9D01_TFT(){}

/*!
	@brief : Init Hardware SPI
*/
void GC9D01_TFT::TFTHWSPIInitialize(void)
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
void GC9D01_TFT::TFTPowerDown(void)
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
void GC9D01_TFT ::TFTResetPIN() {
	if (_resetPinOn == false) return;
	DISPLAY_RST_SetDigitalOutput;
	DISPLAY_RST_SetHigh;
	MILLISEC_DELAY(5);
	DISPLAY_RST_SetLow;
	MILLISEC_DELAY(50);
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
void GC9D01_TFT::TFTsetupGPIO(int8_t rst, int8_t dc, int8_t cs, int8_t sclk, int8_t din)
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
	@brief intialise HW SPI setup
	@param speed_Khz SPI baudrate in Khz , 1000 = 1 Mhz
	@param spiInterface Spi interface, spi0 spi1 etc
	@note method overload used , method 1 hardware SPI 
*/
void GC9D01_TFT  :: TFTInitSPIType(uint32_t speed_Khz,  spi_inst_t* spiInterface) 
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
void GC9D01_TFT ::TFTInitSPIType(uint16_t CommDelay) 
{
	TFTSwSpiGpioDelaySet(CommDelay);
	_hardwareSPI = false;
}

/*!
	@brief init routine for GC9D01 controller
	@details Calls the appropriate init sequence based on resolution selected
*/
void GC9D01_TFT::TFTGC9D01Initialize() 
{
	if (_resetPinOn == true) 
	{
		TFTResetPIN();
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

	TFTinitByResolution();
}

/*!
	@brief   helper function for TFTGC9D01Initialize calls command sequnce
	@details Calls the appropriate init sequence based on resolution selected
*/
void GC9D01_TFT::TFTinitByResolution()
{
	// Mismatch guard: macro and Resolution_e must agree 
	bool _macroResolutionMismatch = false;
	#if defined(GC9D01_DUAL_INIT)
		if (_currentResolution != Resolution_e::RGB160x160_DualGate &&
		    _currentResolution != Resolution_e::RGB120x160_DualGate)
			_macroResolutionMismatch = true;
	#elif defined(GC9D01_SINGLE_INIT_40x160)
		if (_currentResolution != Resolution_e::RGB40x160_SingleGate &&
		    _currentResolution != Resolution_e::RGB80x160_SingleGate)
			_macroResolutionMismatch = true;
	#elif defined(GC9D01_SINGLE_INIT_50x160)
		if (_currentResolution != Resolution_e::RGB50x160_SingleGate)
			_macroResolutionMismatch = true;
	#elif defined(GC9D01_SINGLE_INIT_60x160)
		if (_currentResolution != Resolution_e::RGB60x160_SingleGate)
			_macroResolutionMismatch = true;
	#endif

	if (_macroResolutionMismatch) {
		if(DisLib16::isDebugMode())
			printf("Error: #define macro and Resolution_e mismatch in TFTInitScreenSize\n");
		while(1) { MILLISEC_DELAY(5000); }
	}
	// End mismatch guard
	// Select init sequence switch
	switch (_currentResolution)
	{
		case Resolution_e::RGB160x160_DualGate:
			#ifdef GC9D01_DUAL_INIT
				DualGatecmdInitSequence();
			#endif
		break;
		case Resolution_e::RGB120x160_DualGate:
			if(DisLib16::isDebugMode()){
				printf("Warning: No init for 120x160 DG available, fallback to 160x160\n");
			}
			#ifdef GC9D01_DUAL_INIT
				DualGatecmdInitSequence();
			#endif
		break;
		break;
		case Resolution_e::RGB40x160_SingleGate:
			#ifdef GC9D01_SINGLE_INIT_40x160
				SingleGatecmdInitSequence40x160();
			#endif
		break;
		case Resolution_e::RGB50x160_SingleGate:
			#ifdef GC9D01_SINGLE_INIT_50x160
				SingleGatecmdInitSequence50x160();
			#endif
		break;
		case Resolution_e::RGB60x160_SingleGate:
			#ifdef GC9D01_SINGLE_INIT_60x160
				SingleGatecmdInitSequence60x160();
			#endif
		break;
		case Resolution_e::RGB80x160_SingleGate:
			if(DisLib16::isDebugMode()){
				printf("Warning: No init for 80x160 SG available, fallback to 40x160 SG\n");
			}
			#ifdef GC9D01_SINGLE_INIT_40x160
				SingleGatecmdInitSequence40x160();
			#endif
		break;
		default:// Fallback should never get here. 
			while(1){
				if(DisLib16::isDebugMode()){
					printf("Error: unknown resolution (Resolution_e) chosen:\n");
				}
				MILLISEC_DELAY(5000);
			}
		break;
	}
}

/*!
	@brief Toggle the invert mode, inverted colours
	@param invert true = invert off , false = invert on
*/
void GC9D01_TFT ::TFTchangeInvertMode(bool invert) {
	if(invert) {
		writeCommand(GC9D01_INVOFF);
	} else {
		writeCommand(GC9D01_INVON);
	}
}

/*!
	@brief: change rotation of display.
	@param mode display_rotate_e enum
		0 = Normal
		1=  90 rotate
		2 = 180 rotate
		3 =  270 rotate
	@details 
			Offsets passed in by user in setup() via TFTInitScreenSize
			offsets     0,0,0,0 for 160x160 resolution
			offsets  -60,60,0,0 for 40x160  resolution
			offsets  -39,55,16,0 for 50x160  resolution(untested)
			offsets -40,50,10,0 for 60x160  resolution
*/
void GC9D01_TFT::TFTsetRotation(display_rotate_e mode) {
	uint8_t madctl =0;
	switch (mode) {
		case Degrees_0 : // 0x00
			_width =_widthStartTFT;
			_height = _heightStartTFT;
			_GC9D01_X_Offset = _GC9D01_X_Portrait;
			_GC9D01_Y_Offset = _GC9D01_Y_Portrait;
			break;
		case Degrees_90:
			madctl |= (MADCTL_FLAGS_t::MV | MADCTL_FLAGS_t::ML);
			_width  =_heightStartTFT;
			_height = _widthStartTFT;
			_GC9D01_X_Offset = _GC9D01_X_Landscape;
			_GC9D01_Y_Offset = _GC9D01_Y_Landscape;
			break;
		case Degrees_180:  
			madctl |= (MADCTL_FLAGS_t::MY | MADCTL_FLAGS_t::MX );
			_width =_widthStartTFT;
			_height = _heightStartTFT;
			_GC9D01_X_Offset = _GC9D01_X_Portrait;
			_GC9D01_Y_Offset = _GC9D01_Y_Portrait;
			break;
		case Degrees_270:  
			madctl |= (MADCTL_FLAGS_t::MV |MADCTL_FLAGS_t::MX |MADCTL_FLAGS_t::MY |MADCTL_FLAGS_t::ML );
			_width =_heightStartTFT;
			_height = _widthStartTFT;
			_GC9D01_X_Offset = _GC9D01_X_Landscape;
			_GC9D01_Y_Offset = _GC9D01_Y_Landscape;
			break;
	}
	writeCommand(GC9D01_MADCTL);
	writeData(madctl);
}

/*!
	@brief initialise the variables that define the size of the screen
	@param width_TFT width in pixels
	@param height_TFT height in pixels
	@param resolution Current Resolution see enum gc9d01_resolution_e for options
	@param pixelFixMode Current PixelFixMode see enum PixelFixMode_e for options
	@param XLstart landscape Column start offset based on resolution and display type
	@param YLstart landscape Row start offset based on resolution and display type
	@param XPstart portrait Column start offset based on resolution and display type
	@param YPstart portrait Row start offset based on resolution and display type
	@note  The offsets can be adjusted for any issues with manufacture tolerance/defects
*/
void GC9D01_TFT  :: TFTInitScreenSize( uint16_t width_TFT, uint16_t height_TFT, Resolution_e resolution, 
	PixelFixMode_e pixelFixMode, uint16_t XLstart, uint16_t YLstart, uint16_t XPstart, uint16_t YPstart)
{
	_width = width_TFT;
	_height = height_TFT;
	_widthStartTFT = width_TFT;
	_heightStartTFT = height_TFT;
	_currentResolution = resolution;
	_currentPixelFixMode = pixelFixMode;
	_GC9D01_X_Landscape  = XLstart;
	_GC9D01_Y_Landscape  = YLstart;
	_GC9D01_X_Portrait   = XPstart;
	_GC9D01_Y_Portrait   = YPstart;
	// Portrait is default rotation
	_GC9D01_X_Offset     = XPstart;
	_GC9D01_Y_Offset     = YPstart;
}

/*!
	@brief Freq delay used in SW SPI getter, uS delay used in SW SPI method
	@return The  GPIO communications delay in uS
*/
uint16_t GC9D01_TFT::TFTSwSpiGpioDelayGet(void){return _SWSPIGPIODelay;}

/*!
	@brief Freq delay used in SW SPI setter, uS delay used in SW SPI method
	@param CommDelay The GPIO communications delay in uS
*/
void  GC9D01_TFT::TFTSwSpiGpioDelaySet(uint16_t CommDelay){_SWSPIGPIODelay = CommDelay;}

/*!
  @brief SPI displays set an address window rectangle for blitting pixels
  @param  x1 Top left corner x coordinate
  @param  y1  Top left corner y coordinate
  @param  w  Width of window
  @param  h  Height of window
*/
void GC9D01_TFT::setAddrWindow(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h)
{	
	//if drawing a single pixel we need do this to avoid a blank pixel for this device
	const bool singlePixel =
		((w == x1 + 1) && (h == y1 + 1));
	if (singlePixel) {
		w = x1;
		h = y1;
	}
	// Apply offsets: not needed for normal 160x160 dual gate resolution
	if (_currentResolution != Resolution_e::RGB160x160_DualGate) {
		x1 += _GC9D01_X_Offset;
		y1 += _GC9D01_Y_Offset;
		w += _GC9D01_X_Offset;
		h += _GC9D01_Y_Offset;
	}

	uint8_t x1Higher = (x1 >> 8) ;
	uint8_t x1Lower  = (x1 & 0xFF);
	uint8_t x2Higher = (w >> 8);
	uint8_t x2Lower  = (w &  0xFF);
	uint8_t seqCASET[4]  =  {x1Higher ,x1Lower,x2Higher,x2Lower};
	uint8_t y1Higher = (y1 >> 8); 
	uint8_t y1Lower  = (y1 &  0xFF);
	uint8_t y2Higher = (h >> 8);
	uint8_t y2Lower  = (h &  0xFF);
	uint8_t seqRASET[4]  =  {y1Higher,y1Lower,y2Higher,y2Lower};
	writeCommand(GC9D01_CASET); //Column address set
	spiWriteDataBuffer(seqCASET, sizeof(seqCASET));
	writeCommand(GC9D01_RASET); //Row address set
	spiWriteDataBuffer(seqRASET, sizeof(seqRASET));
	writeCommand(GC9D01_RAMWR); //Write to RAM
}


/*!
	@brief This method defines the Vertical Scrolling Area of the display where:
	@param topFixed describes the Top Fixed Area.
	@param scrollArea describes the Scrolling Area.
	@param bottomFixed describes the Bottom Fixed Area.
*/
void GC9D01_TFT::TFTsetScrollArea(uint16_t topFixed, uint16_t scrollArea, uint16_t bottomFixed) {

	writeCommand(GC9D01_VSCRDEF);
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
void GC9D01_TFT::TFTsetScrollStart(uint16_t vsp) {
	writeCommand(GC9D01_VSCRSADD);
	writeData(vsp >> 8);
	writeData(vsp & 0xFF);
}

/*! @brief Scroll Mode can be left ,by the Normal Display Mode ON cmd*/
void GC9D01_TFT::TFTScrollModeLeave(void) {writeCommand(GC9D01_NORON);}

/*!
	@brief Software reset command
*/
void GC9D01_TFT::TFTresetSWDisplay(void) 
{
  writeCommand(GC9D01_SWRESET);
  MILLISEC_DELAY(150);
}

/*!
	@brief enable /disable display mode
	@param enableDisplay true enable on false disable
	@note Temporarily blank the screen.
	Use Case: Screen blanking, brief off periods without resetting or reinitializing the display.
*/
void GC9D01_TFT::TFTenableDisplay(bool enableDisplay){
	if(enableDisplay) {
		writeCommand(GC9D01_DISPON);
		_displayOn = true;
	} else {
		writeCommand(GC9D01_DISPOFF);
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
void GC9D01_TFT::TFTsetPowerMode(PowerState_e mode) {
	// If already in the desired state or off , skip
	if (_currentPowerState == mode || _displayOn != true) 
	{
		if(DisLib16::isDebugMode()){
			printf("Warning: TFTsetPowerMode: Display already in this state or off\n");
			return;
		}
	}
	
	// Always return to a known base state
	writeCommand(GC9D01_SLPOUT);
	MILLISEC_DELAY(_sleepDelay);

	switch (mode) {
		case PowerState_e::NormalIdleOff:
			writeCommand(GC9D01_NORON);
			writeCommand(GC9D01_IDLEOFF);
			break;
		case PowerState_e::NormalIdleOn:
			writeCommand(GC9D01_NORON);
			writeCommand(GC9D01_IDLEON);
			break;
		case PowerState_e::PartialIdleOff:
			writeCommand(GC9D01_PTLON);
			writeCommand(GC9D01_IDLEOFF);
			break;
		case PowerState_e::PartialIdleOn:
			writeCommand(GC9D01_PTLON);
			writeCommand(GC9D01_IDLEON);
			break;
		case PowerState_e::SleepNormalIdleOff:
			writeCommand(GC9D01_NORON);
			writeCommand(GC9D01_IDLEOFF);
			writeCommand(GC9D01_SLPIN);
			MILLISEC_DELAY(_sleepDelay);
			break;
		case PowerState_e::SleepNormalIdleOn:
			writeCommand(GC9D01_NORON);
			writeCommand(GC9D01_IDLEON);
			writeCommand(GC9D01_SLPIN);
			MILLISEC_DELAY(_sleepDelay);
			break;
		case PowerState_e::SleepPartialIdleOff:
			writeCommand(GC9D01_PTLON);
			writeCommand(GC9D01_IDLEOFF);
			writeCommand(GC9D01_SLPIN);
			MILLISEC_DELAY(_sleepDelay);
			break;
		case PowerState_e::SleepPartialIdleOn:
			writeCommand(GC9D01_PTLON);
			writeCommand(GC9D01_IDLEON);
			writeCommand(GC9D01_SLPIN);
			MILLISEC_DELAY(_sleepDelay);
			break;
	}
	_currentPowerState = mode;
}

/*!
  @brief Set display brightness (0–255).
  @param level Brightness level, 0 = darkest, 255 = brightest
  @note This is a software brightness control, not hardware PWM, may not work on all displays.
*/
void GC9D01_TFT::TFTsetBrightness(uint8_t level)
{
	writeCommand(GC9D01_SETCTRL);
	writeData(0x24); // Brightness registers are active, Display Dimming is on, Backlight Off
	writeCommand(GC9D01_SETBRIGHT);
	writeData(level);
}

/*!
	@brief Set the pixel padding fix mode
	@param mode the current pix fix padding mode
*/
void GC9D01_TFT::TFTsetPixelFixMode(PixelFixMode_e mode){
	_currentPixelFixMode = mode;
}

// ===  Init routine section ===

#ifdef GC9D01_DUAL_INIT
/*!
	@brief GC9D01 init command sequence for Dual Gate variant
		Dual Gate variants include 160x160 and 120x160 resolutions
*/
void GC9D01_TFT::DualGatecmdInitSequence(void)
{
	if(DisLib16::isDebugMode()){
		printf("DualGatecmdInitSequence init selected\n");
	}
	writeCommand(GC9D01_INREGEN1);
	writeCommand(GC9D01_INREGEN2); 

	// Enabling the internal register: Undocumented in datasheet registers
	constexpr uint8_t startCmd = 0x80;
	constexpr uint8_t endCmd   = 0x8F;
	for (uint8_t cmd = startCmd; cmd <= endCmd; ++cmd)
	{
		writeCommand(cmd);
		writeData(0xFF);
	}
	writeCommand(GC9D01_INVERSION);
	writeData(0x70); 
	
	// Change frame rate: Undocumented in datasheet registers
	writeCommand(0x74); 
	uint8_t seqReg1[] = {0x02, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00};
	spiWriteDataBuffer(seqReg1, sizeof(seqReg1));

	// Internal voltage regulation: Undocumented in datasheet registers
	writeCommand(0x98); 
	writeData(0x3E);
	writeCommand(0x99);
	writeData(0x3E);

	// Blanking Porch Control
	writeCommand(GC9D01_BLANK_PORCH_CTRL);
	uint8_t segBlankCTR[] = {0x0D, 0x0D, 0x00};
	spiWriteDataBuffer(segBlankCTR, sizeof(segBlankCTR));
	
	 // Timing Gap: Undocumented in datasheet registers
	writeCommand(0x60);
	uint8_t seqReg2[] = {0x38, 0x0F, 0x79, 0x67};
	spiWriteDataBuffer(seqReg2, sizeof(seqReg2));
	writeCommand(0x61);
	uint8_t seqReg3[] = {0x38, 0x11, 0x79, 0x67};
	spiWriteDataBuffer(seqReg3, sizeof(seqReg3));
	writeCommand(0x64);
	uint8_t seqReg4[] = {0x38, 0x17, 0x71, 0x5F, 0x79, 0x67};
	spiWriteDataBuffer(seqReg4, sizeof(seqReg4));
	writeCommand(0x65);
	uint8_t seqReg5[] = {0x38, 0x13, 0x71, 0x5B, 0x79, 0x67};
	spiWriteDataBuffer(seqReg5, sizeof(seqReg5));
	writeCommand(0x6A);
	writeData(0x00);
	writeData(0x00);
	writeCommand(0x6C);
	uint8_t segReg6[] = {0x22, 0x02, 0x22, 0x02, 0x22, 0x22, 0x50};
	spiWriteDataBuffer(segReg6, sizeof(segReg6));
	writeCommand(0x6E);
	uint8_t segReg7[] = {0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x0F, 0x0F, 0x0D, 0x0D,
						 0x0B, 0x0B, 0x09, 0x09, 0x00, 0x00, 0x00,
						 0x00, 0x0A, 0x0A, 0x0C, 0x0C, 0x0E, 0x0E,
						 0x10, 0x10, 0x00, 0x00, 0x02, 0x02, 0x04, 0x04};
	spiWriteDataBuffer(segReg7, sizeof(segReg7));

	writeCommand(GC9D01_DUAL_SINGLE);
	writeData(0x01);// Dual-Single Gate Select (BFh) 0=>Single gate

	 // Adjustments related to SOU : Undocumented in datasheet registers
	writeCommand(0xF9);
	writeData(0x40);
	//Voltage regulation vreg : Undocumented in datasheet registers
	writeCommand(0x9b);
	writeData(0x3b);
	writeCommand(0x93);
	uint8_t segReg8[] = {0x33, 0x7F, 0x00};
	spiWriteDataBuffer(segReg8, sizeof(segReg8));

	// Undocumented in datasheet registers
	writeCommand(0x7E);
	writeData(0x30);
	// VGH/VGL CLK adjustment 70，71h : Undocumented in datasheet registers
	writeCommand(0x70);
	uint8_t segReg9[] = {0x0D, 0x02, 0x08, 0x0D, 0x02, 0x08};
	spiWriteDataBuffer(segReg9, sizeof(segReg9));
	writeCommand(0x71);
	uint8_t segReg10[] = {0x0D, 0x02, 0x08};
	spiWriteDataBuffer(segReg10, sizeof(segReg10));
	// Internal voltage regulation : Undocumented in datasheet registers
	writeCommand(0x91);
	writeData(0x0E);
	writeData(0x09);

	writeCommand(GC9D01_VREG1A_CTRL);
	writeData(0x18);
	writeCommand(GC9D01_VREG1B_CTRL);
	writeData(0x18);
	writeCommand(GC9D01_VREG2A_CTRL);
	writeData(0x3c);

	writeCommand(GC9D01_GAMMA1); // 0xF0
	uint8_t seqGamma1[] = {0x13, 0x15, 0x04, 0x05, 0x01, 0x38};
	spiWriteDataBuffer(seqGamma1, sizeof(seqGamma1));
	writeCommand(GC9D01_GAMMA3); // 0xF2
	uint8_t seqGamma3[] = {0x13, 0x15, 0x04, 0x05, 0x01, 0x34};
	spiWriteDataBuffer(seqGamma3, sizeof(seqGamma3));
	writeCommand(GC9D01_GAMMA2); // 0xF1
	uint8_t seqGamma2[] = {0x4B, 0xB8, 0x7B, 0x34, 0x35, 0xEF};
	spiWriteDataBuffer(seqGamma2, sizeof(seqGamma2));
	writeCommand(GC9D01_GAMMA4); // 0xF3
	uint8_t seqGamma4[] = {0x47, 0xB4, 0x72, 0x34, 0x35, 0xDA};
	spiWriteDataBuffer(seqGamma4, sizeof(seqGamma4));

	writeCommand(GC9D01_COLMOD);
	writeData(0x55); // 16bit/pixel control
	writeCommand(GC9D01_FUNCTION_CTRL);
	writeData(0x00);
	writeData(0x00);
	writeCommand(GC9D01_IFACE);
	writeData(0xc0);
	writeCommand(GC9D01_SPI2DATA);
	writeData(0x00);
	
	writeCommand(GC9D01_MADCTL);
	writeData(0x00); 
	writeCommand(GC9D01_SLPOUT);
	MILLISEC_DELAY(200); // wait at least 120ms after sending Sleep Out cmd(4.2.4.)
	writeCommand(GC9D01_DISPON);
	writeCommand(GC9D01_CONTINUE);

}
#endif

#ifdef GC9D01_DUAL_INIT_120x160
/*!
	@brief GC9D01 init command sequence for dual Gate 120x160 variant
	@details does not exist yet, Default to DualGatecmdInitSequence
*/
void GC9D01_TFT::DualGatecmdInitSequence120x160(void)
{
	if(DisLib16::isDebugMode()){
		printf("GC9D01_DUAL_INIT_120x160 init selected\n");
		printf("Default to DualGatecmdInitSequence, 160x160\n")
	}
	DualGatecmdInitSequence();
}
#endif

#ifdef GC9D01_SINGLE_INIT_40x160
/*!
	@brief GC9D01 init command sequence for Single Gate variant
		Single Gate variants include 80x160 and 40x160 resolutions
*/
void GC9D01_TFT::SingleGatecmdInitSequence40x160(void)
{
	if(DisLib16::isDebugMode()){
		printf("SingleGatecmdInitSequence40x160 init selected\n");
	}
	writeCommand(0xFE);
	writeCommand(0xEF);

	// Internal registers 80h–8Fh (all 0xFF) — loop is already optimal
	for (uint8_t cmd = 0x80; cmd <= 0x8F; ++cmd) {
		writeCommand(cmd);
		writeData(0xFF);
	}

	writeCommand(GC9D01_COLMOD);
	writeData(0x05);
	writeCommand(0xEC);
	writeData(0x11);

	writeCommand(0x7E);
	writeData(0x7A);
	// Frame rate (0x74) — 7 bytes
	writeCommand(0x74);
	static const uint8_t FrameRatedata[] = {0x02, 0x0E, 0x00, 0x00, 0x28, 0x00, 0x00};
	spiWriteDataBuffer(const_cast<uint8_t*>(FrameRatedata), sizeof(FrameRatedata));

	writeCommand(0x98);
	writeData(0x3E);
	writeCommand(0x99);
	writeData(0x3E);

	// Porch B5h
	writeCommand(0xB5);
	static const uint8_t PorchData[] = {0x0E, 0x0E};
	spiWriteDataBuffer(const_cast<uint8_t*>(PorchData), sizeof(PorchData));

	// GIP / timing registers — grouped where multiple bytes
	writeCommand(0x60);
	static const uint8_t gap1data[]  = {0x38, 0x09, 0x6D, 0x67};
	spiWriteDataBuffer(const_cast<uint8_t*>(gap1data), sizeof(gap1data));
	writeCommand(0x63);
	static const uint8_t gap2data[] = {0x38, 0xAD, 0x6D, 0x67, 0x05};
	spiWriteDataBuffer(const_cast<uint8_t*>(gap2data), sizeof(gap2data));
	writeCommand(0x64);
	static const uint8_t gap3data[]  = {0x38, 0x0B, 0x70, 0xAB, 0x6D, 0x67};
	spiWriteDataBuffer(const_cast<uint8_t*>(gap3data), sizeof(gap3data));
	writeCommand(0x66);
	static const uint8_t gap4data[]  = {0x38, 0x0F, 0x70, 0xAF, 0x6D, 0x67};
	spiWriteDataBuffer(const_cast<uint8_t*>(gap4data), sizeof(gap4data));
	writeCommand(0x6A);
	static const uint8_t gap5data[]  = {0x00, 0x00};
	spiWriteDataBuffer(const_cast<uint8_t*>(gap5data), sizeof(gap5data));
	writeCommand(0x68);
	static const uint8_t gap6data[] = {0x3B, 0x08, 0x04, 0x00, 0x04, 0x64, 0x67};
	spiWriteDataBuffer(const_cast<uint8_t*>(gap6data), sizeof(gap6data));
	writeCommand(0x6C);
	static const uint8_t gap7data[] = {0x22, 0x02, 0x22, 0x02, 0x22, 0x22, 0x50};
	spiWriteDataBuffer(const_cast<uint8_t*>(gap7data), sizeof(gap7data));
	// Long 0x6E sequence — 32 bytes
	writeCommand(0x6E);
	{
		static const uint8_t gap8data[] = {
			0x00, 0x00, 0x00, 0x00, 0x07, 0x01, 0x13, 0x11,
			0x0B, 0x09, 0x16, 0x15, 0x1D, 0x1E, 0x00, 0x00,
			0x00, 0x00, 0x1E, 0x1D, 0x15, 0x16, 0x0A, 0x0C,
			0x12, 0x14, 0x02, 0x08, 0x00, 0x00, 0x00, 0x00
		};
		spiWriteDataBuffer(const_cast<uint8_t*>(gap8data), sizeof(gap8data));
	}

	// Voltage / adjustment block
	writeCommand(0xA9); writeData(0x1B);
	writeCommand(0xA8); writeData(0x6B);
	writeCommand(0xA8); writeData(0x6D);
	writeCommand(0xA7); writeData(0x40);
	writeCommand(0xAD); writeData(0x47);
	writeCommand(0xAF); writeData(0x73);
	writeCommand(0xAF); writeData(0x73);
	writeCommand(0xAC); writeData(0x44);
	writeCommand(0xA3); writeData(0x6C);
	writeCommand(0xCB); writeData(0x00);
	writeCommand(0xCD); writeData(0x22);
	writeCommand(0xC2); writeData(0x10);
	writeCommand(0xC5); writeData(0x00);
	writeCommand(0xC6); writeData(0x0E);
	writeCommand(0xC7); writeData(0x1F);
	writeCommand(0xC8); writeData(0x0E);

	// Gate select
	writeCommand(0xBF);
	writeData(0x00);

	writeCommand(0xF9);
	writeData(0x20);
	writeCommand(0x9B);
	writeData(0x3B);
	writeCommand(0x93);
	static const uint8_t data1[]  = {0x33, 0x7F, 0x00};
	spiWriteDataBuffer(const_cast<uint8_t*>(data1), sizeof(data1));
	writeCommand(0x70);
	static const uint8_t data2[] = {0x0E, 0x0F, 0x03, 0x0E, 0x0F, 0x03};
	spiWriteDataBuffer(const_cast<uint8_t*>(data2), sizeof(data2));
	writeCommand(0x71);
	static const uint8_t data3[]  = {0x0E, 0x16, 0x03};
	spiWriteDataBuffer(const_cast<uint8_t*>(data3), sizeof(data3));
	writeCommand(0x91);
	static const uint8_t data4[]  = {0x0E, 0x09};
	spiWriteDataBuffer(const_cast<uint8_t*>(data4), sizeof(data4));

	writeCommand(0xC3);
	writeData(0x2C);
	writeCommand(0xC4);
	writeData(0x1A);

	// Gamma curves — each 6 bytes
	writeCommand(0xF0);
	static const uint8_t gamma1[] = {0x51, 0x13, 0x0C, 0x06, 0x00, 0x2F};
	spiWriteDataBuffer(const_cast<uint8_t*>(gamma1), sizeof(gamma1));
	writeCommand(0xF2);
	static const uint8_t gamma2[] = {0x51, 0x13, 0x0C, 0x06, 0x00, 0x33};
	spiWriteDataBuffer(const_cast<uint8_t*>(gamma2), sizeof(gamma2));
	writeCommand(0xF1);
	static const uint8_t gamma3[] = {0x3C, 0x94, 0x4F, 0x33, 0x34, 0xCF};
	spiWriteDataBuffer(const_cast<uint8_t*>(gamma3), sizeof(gamma3));
	writeCommand(0xF3);
	static const uint8_t gamma4[]  = {0x4D, 0x94, 0x4F, 0x33, 0x34, 0xCF};
	spiWriteDataBuffer(const_cast<uint8_t*>(gamma4), sizeof(gamma4));

	// MADCTL
	writeCommand(GC9D01_MADCTL);
	writeData(0x00);
	// Sleep out + display on
	writeCommand(GC9D01_SLPOUT);
	MILLISEC_DELAY(200); 
	writeCommand(GC9D01_DISPON);
	MILLISEC_DELAY(50);
	writeCommand(GC9D01_CONTINUE); 
}
#endif

#ifdef GC9D01_SINGLE_INIT_50x160
/*!
	@brief GC9D01 init command sequence for Single Gate variant
		Single Gate variants include 50X160 display resolutions
*/
void GC9D01_TFT::SingleGatecmdInitSequence50x160(void)
{
	if(DisLib16::isDebugMode()){
		printf("SingleGatecmdInitSequence50x160 init selected\n");
	}
	writeCommand(GC9D01_INREGEN1);
	writeCommand(GC9D01_INREGEN2);

	// 0x80–0x8F block (mostly like 60x160 but 0x84=0x60)
	writeCommand(0x86); writeData(0xFF);
	writeCommand(0x87); writeData(0xFF);
	writeCommand(0x8E); writeData(0xFF);
	writeCommand(0x8F); writeData(0xFF);
	writeCommand(0x80); writeData(0x13);
	writeCommand(0x81); writeData(0x40);
	writeCommand(0x82); writeData(0x0A);
	writeCommand(0x83); writeData(0x0B);
	writeCommand(0x84); writeData(0x60);   // ← Unique
	writeCommand(0x85); writeData(0x80);
	writeCommand(0x89); writeData(0x10);
	writeCommand(0x8A); writeData(0x0F);
	writeCommand(0x8B); writeData(0x02);
	writeCommand(0x8C); writeData(0x59);
	writeCommand(0x8D); writeData(0x55);

	writeCommand(GC9D01_COLMOD);
	writeData(0x05);
	writeCommand(GC9D01_INVERSION);
	writeData(0x00);

	writeCommand(0x7E);
	writeData(0x30);
	// Frame rate 0x74
	writeCommand(0x74);
	static const uint8_t frameData[] = {0x05, 0x4D, 0x00, 0x00, 0x01, 0x00, 0x00};
	spiWriteDataBuffer(const_cast<uint8_t*>(frameData), sizeof(frameData));

	// Porch
	writeCommand(GC9D01_BLANK_PORCH_CTRL);
	static const uint8_t porchData[] = {0x0D, 0x0D};
	spiWriteDataBuffer(const_cast<uint8_t*>(porchData), sizeof(porchData));
	writeCommand(GC9D01_FUNCTION_CTRL);
	static const uint8_t b6Data[] = {0x00, 0x00};
	spiWriteDataBuffer(const_cast<uint8_t*>(b6Data), sizeof(b6Data));

	// GIP Timing
	writeCommand(0x60);
	static const uint8_t gip60[] = {0x38, 0x09, 0x1E, 0x7A};
	spiWriteDataBuffer(const_cast<uint8_t*>(gip60), sizeof(gip60));
	writeCommand(0x63);
	static const uint8_t gip63[] = {0x38, 0xAE, 0x1E, 0x7A};
	spiWriteDataBuffer(const_cast<uint8_t*>(gip63), sizeof(gip63));
	writeCommand(0x64);
	static const uint8_t gip64[] = {0x38, 0x0B, 0x70, 0xAB, 0x1E, 0x7A};
	spiWriteDataBuffer(const_cast<uint8_t*>(gip64), sizeof(gip64));
	writeCommand(0x66);
	static const uint8_t gip66[] = {0x38, 0x0F, 0x70, 0xAF, 0x1E, 0x7A};
	spiWriteDataBuffer(const_cast<uint8_t*>(gip66), sizeof(gip66));
	writeCommand(0x68);
	static const uint8_t gip68[] = {0x00, 0x08, 0x07, 0x00, 0x07, 0x55, 0x6A};
	spiWriteDataBuffer(const_cast<uint8_t*>(gip68), sizeof(gip68));
	writeCommand(0x6A);
	static const uint8_t gip6A[] = {0x00, 0x00};
	spiWriteDataBuffer(const_cast<uint8_t*>(gip6A), sizeof(gip6A));
	writeCommand(0x6C);
	static const uint8_t gip6C[] = {0x22, 0x02, 0x22, 0x02, 0x22, 0x22, 0x50};
	spiWriteDataBuffer(const_cast<uint8_t*>(gip6C), sizeof(gip6C));
	// 0x6E mapping (32 bytes)
	writeCommand(0x6E);
	static const uint8_t map6E[] = {
		0x00, 0x00, 0x00, 0x02, 0x14, 0x12, 0x0C, 0x0A,
		0x1E, 0x1D, 0x08, 0x00, 0x16, 0x15, 0x00, 0x00,
		0x00, 0x00, 0x15, 0x16, 0x00, 0x07, 0x1D, 0x1E,
		0x09, 0x0B, 0x11, 0x13, 0x01, 0x00, 0x00, 0x00
	};
	spiWriteDataBuffer(const_cast<uint8_t*>(map6E), sizeof(map6E));

	writeCommand(0x98); writeData(0x3E);
	writeCommand(0x99); writeData(0x3E);
	writeCommand(0x9B); writeData(0x3B);
	writeCommand(0x93);
	static const uint8_t data93[] = {0x33, 0x7F, 0x00};
	spiWriteDataBuffer(const_cast<uint8_t*>(data93), sizeof(data93));
	writeCommand(0x91);
	static const uint8_t data91[] = {0x0E, 0x09};
	spiWriteDataBuffer(const_cast<uint8_t*>(data91), sizeof(data91));
	writeCommand(0x70);
	static const uint8_t v70[] = {0x04, 0x02, 0x0D, 0x04, 0x02, 0x0D};
	spiWriteDataBuffer(const_cast<uint8_t*>(v70), sizeof(v70));
	writeCommand(0x71);
	static const uint8_t v71[] = {0x04, 0x02, 0x0D};
	spiWriteDataBuffer(const_cast<uint8_t*>(v71), sizeof(v71));

	writeCommand(GC9D01_VREG1A_CTRL); writeData(0x26);
	writeCommand(GC9D01_VREG1B_CTRL); writeData(0x26);
	writeCommand(GC9D01_VREG2A_CTRL); writeData(0x1C);

	// Gamma
	writeCommand(GC9D01_GAMMA1);
	static const uint8_t gmF0[] = {0x02, 0x03, 0x0A, 0x06, 0x00, 0x1A};
	spiWriteDataBuffer(const_cast<uint8_t*>(gmF0), sizeof(gmF0));
	writeCommand(GC9D01_GAMMA3);
	static const uint8_t gmF2[] = {0x02, 0x03, 0x0A, 0x06, 0x00, 0x1A};
	spiWriteDataBuffer(const_cast<uint8_t*>(gmF2), sizeof(gmF2));
	writeCommand(GC9D01_GAMMA2);
	static const uint8_t gmF1[] = {0x38, 0x78, 0x1B, 0x2E, 0x2F, 0xC8};
	spiWriteDataBuffer(const_cast<uint8_t*>(gmF1), sizeof(gmF1));
	writeCommand(GC9D01_GAMMA4);
	static const uint8_t gmF3[] = {0x38, 0x74, 0x12, 0x2E, 0x2F, 0xDF};
	spiWriteDataBuffer(const_cast<uint8_t*>(gmF3), sizeof(gmF3));

	writeCommand(GC9D01_DUAL_SINGLE); writeData(0x00);   // Single gate
	writeCommand(0xF9); writeData(0x40);

	// MADCTL with direction support
	writeCommand(GC9D01_MADCTL);
	writeData(0x00);
	// Sleep Out + Display On
	writeCommand(GC9D01_SLPOUT);
	MILLISEC_DELAY(200);
	writeCommand(GC9D01_DISPON);
	MILLISEC_DELAY(50);
	writeCommand(GC9D01_CONTINUE); 
}
#endif

#ifdef GC9D01_SINGLE_INIT_60x160
/*!
	@brief GC9D01 init command sequence for Single Gate variant
		Single Gate variants include 60X160 display resolutions
*/
void GC9D01_TFT::SingleGatecmdInitSequence60x160(void)
{
	if(DisLib16::isDebugMode()){
		printf("SingleGatecmdInitSequence60x160 init selected\n");
	}
	writeCommand(GC9D01_INREGEN1);
	writeCommand(GC9D01_INREGEN2);

	// Internal registers 0x80–0x8F
	writeCommand(0x80); writeData(0x13);
	writeCommand(0x81); writeData(0x40);
	writeCommand(0x82); writeData(0x0A);
	writeCommand(0x83); writeData(0x0B);
	writeCommand(0x84); writeData(0x20);
	writeCommand(0x85); writeData(0x80);
	writeCommand(0x86); writeData(0xFF);
	writeCommand(0x87); writeData(0xFF);
	writeCommand(0x89); writeData(0x10);
	writeCommand(0x8A); writeData(0x0F);
	writeCommand(0x8B); writeData(0x02);
	writeCommand(0x8C); writeData(0x59);
	writeCommand(0x8D); writeData(0x55);
	writeCommand(0x8E); writeData(0xFF);
	writeCommand(0x8F); writeData(0xFF);

	// Color mode: RGB565
	writeCommand(GC9D01_COLMOD);
	writeData(0x05);
	// Orientation MADCTL 
	writeCommand(GC9D01_MADCTL);
	writeData(0x00);
	writeCommand(GC9D01_INVERSION);
	writeData(0x00);
	writeCommand(0x7E);
	writeData(0x30);
	// Frame rate / clock (0x74)
	writeCommand(0x74);
	static const uint8_t frameRateData[] = {0x04, 0x4E, 0x00, 0x00, 0x00, 0x00, 0x00};
	spiWriteDataBuffer(const_cast<uint8_t*>(frameRateData), sizeof(frameRateData));
	// Porch (VFP / VBP)
	writeCommand(GC9D01_BLANK_PORCH_CTRL);
	static const uint8_t porchData[] = {0x0F, 0x0F};
	spiWriteDataBuffer(const_cast<uint8_t*>(porchData), sizeof(porchData));
	// GIP Timing registers
	writeCommand(0x61);
	static const uint8_t gip1[] = {0x58, 0x05, 0x6D, 0x67};
	spiWriteDataBuffer(const_cast<uint8_t*>(gip1), sizeof(gip1));
	writeCommand(0x62);
	static const uint8_t gip2[] = {0x58, 0x07, 0x6D, 0x67};
	spiWriteDataBuffer(const_cast<uint8_t*>(gip2), sizeof(gip2));
	writeCommand(0x63);
	static const uint8_t gip3[] = {0x58, 0x00, 0x6D, 0x67, 0x15};
	spiWriteDataBuffer(const_cast<uint8_t*>(gip3), sizeof(gip3));
	writeCommand(0x64);
	static const uint8_t gip4[] = {0x38, 0x0C, 0x70, 0xB4, 0x7F, 0x50};
	spiWriteDataBuffer(const_cast<uint8_t*>(gip4), sizeof(gip4));
	writeCommand(0x66);
	static const uint8_t gip5[] = {0x38, 0x08, 0x70, 0xB0, 0x7F, 0x50};
	spiWriteDataBuffer(const_cast<uint8_t*>(gip5), sizeof(gip5));
	writeCommand(0x6A);
	static const uint8_t clkInt[] = {0x00, 0x00};
	spiWriteDataBuffer(const_cast<uint8_t*>(clkInt), sizeof(clkInt));
	writeCommand(0x6C);
	static const uint8_t gip6[] = {0x22, 0x02, 0x22, 0x02, 0x22, 0x22, 0x50};
	spiWriteDataBuffer(const_cast<uint8_t*>(gip6), sizeof(gip6));

	// Long 0x6E mapping sequence (32 bytes)
	writeCommand(0x6E);
	static const uint8_t mappingData[] = {
		0x07, 0x00, 0x08, 0x05, 0x1D, 0x1E, 0x12, 0x14,
		0x0A, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, 0x09,
		0x13, 0x11, 0x1E, 0x1D, 0x04, 0x08, 0x00, 0x07
	};
	spiWriteDataBuffer(const_cast<uint8_t*>(mappingData), sizeof(mappingData));

	writeCommand(0x98); writeData(0x3E);
	writeCommand(0x99); writeData(0x3E);
	writeCommand(0x9B); writeData(0x3B);
	writeCommand(0x93);
	static const uint8_t data93[] = {0x33, 0x7F, 0x00};
	spiWriteDataBuffer(const_cast<uint8_t*>(data93), sizeof(data93));
	writeCommand(0x91);
	static const uint8_t data91[] = {0x0E, 0x09};
	spiWriteDataBuffer(const_cast<uint8_t*>(data91), sizeof(data91));
	writeCommand(0x70);
	static const uint8_t vgh1[] = {0x04, 0x05, 0x0D, 0x04, 0x05, 0x0D};
	spiWriteDataBuffer(const_cast<uint8_t*>(vgh1), sizeof(vgh1));
	writeCommand(0x71);
	static const uint8_t vgh2[] = {0x04, 0x05, 0x0D};
	spiWriteDataBuffer(const_cast<uint8_t*>(vgh2), sizeof(vgh2));
	writeCommand(GC9D01_VREG1A_CTRL); writeData(0x15);   // VBP
	writeCommand(GC9D01_VREG1B_CTRL); writeData(0x15);   // VBN
	writeCommand(GC9D01_VREG2A_CTRL); writeData(0x3E);   // VRH

	// GAMMA
	writeCommand(GC9D01_GAMMA1);
	static const uint8_t gammaF0[] = {0x54, 0x0D, 0x05, 0x02, 0x00, 0x32};
	spiWriteDataBuffer(const_cast<uint8_t*>(gammaF0), sizeof(gammaF0));
	writeCommand(GC9D01_GAMMA3);
	static const uint8_t gammaF2[] = {0x54, 0x0D, 0x05, 0x02, 0x00, 0x32};
	spiWriteDataBuffer(const_cast<uint8_t*>(gammaF2), sizeof(gammaF2));
	writeCommand(GC9D01_GAMMA2);
	static const uint8_t gammaF1[] = {0x4C, 0x94, 0x4F, 0x33, 0x34, 0xCF};
	spiWriteDataBuffer(const_cast<uint8_t*>(gammaF1), sizeof(gammaF1));
	writeCommand(GC9D01_GAMMA4);
	static const uint8_t gammaF3[] = {0x4C, 0x94, 0x4F, 0x33, 0x34, 0xCF};
	spiWriteDataBuffer(const_cast<uint8_t*>(gammaF3), sizeof(gammaF3));

	writeCommand(GC9D01_DUAL_SINGLE); writeData(0x00);   // Single gate mode
	writeCommand(0xF9); writeData(0x40); 

	// Sleep Out + Display On
	writeCommand(GC9D01_SLPOUT);
	MILLISEC_DELAY(200);

	writeCommand(GC9D01_DISPON);
	MILLISEC_DELAY(50);
	writeCommand(GC9D01_CONTINUE); 
}
#endif

#ifdef GC9D01_SINGLE_INIT_80x160
/*!
	@brief GC9D01 init command sequence for single Gate 80x160 variant
	@details does not exist yet, Default to SingleGatecmdInitSequence 40x160
*/
void GC9D01_TFT::SingleGatecmdInitSequence80x160(void)
{
	if(DisLib16::isDebugMode()){
		printf("GC9D01_SINGLE_INIT_80x160 init selected\n");
		printf("Default to SingleGatecmdInitSequence, 40x160\n")
	}
	SingleGatecmdInitSequence40x160();
}
#endif

//**************** EOF *****************
