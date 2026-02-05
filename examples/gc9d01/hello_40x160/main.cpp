/*!
	@file   main.cpp
	@author Gavin Lyons
	@brief  Example cpp file for gc9d01 driver. Test Hello World.
	@note   See USER OPTIONS 0-2 in SETUP function
			This example is for 40x160 single gate.
			This example requires frame buffer mode to be on.(non-default)
			see displaylib_16_graphics.hpp USER OPTION 2.
			Untested in real hardware by author, third party contribution.
			Requires usable heap memory of (40x160x2) = 12,800 bytes for buffer
	@test
		-# Test 102 Print out Hello world on single gate 40x160
*/

// Section ::  libraries 
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16/gc9d01.hpp"

///@cond

// Section :: Defines   
//  Test timing related defines 
#define TEST_DELAY1 1000 // mS
#define TEST_DELAY2 2000 // mS
#define TEST_DELAY5 5000 // mS

#ifndef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is not defined. This example is for that mode")
#endif

// Section :: Globals 
GC9D01_TFT myTFT;

//  Section ::  Function Headers 

bool Setup(void);  // setup + user options
void Test102(void);  
void EndTests(void);

//  Section ::  MAIN loop

int main(void) 
{
	DisLib16::setDebugMode(true); // turn on debug mode
	if (!Setup())
		return -1;
	Test102();
	EndTests();
}
// *** End OF MAIN **

//  Section ::  Function Space 

bool Setup(void)
{
	stdio_init_all(); // optional for error messages , Initialize chosen serial port, default 38400 baud
	MILLISEC_DELAY(TEST_DELAY1);
	printf("TFT: Start\r\n");
//*** USER OPTION 0 SPI_SPEED + TYPE ***
	bool bhardwareSPI = true; // true for hardware spi, false for software
	
	if (bhardwareSPI == true) { // hw spi
		uint32_t TFT_SCLK_FREQ =  8000 ; // Spi freq in KiloHertz , 1000 = 1Mhz
		myTFT.TFTInitSPIType(TFT_SCLK_FREQ, spi0); 
	} else { // sw spi
		uint16_t SWSPICommDelay = 0; // optional SW SPI GPIO delay in uS
		myTFT.TFTInitSPIType(SWSPICommDelay);
	}
// ***
// *** USER OPTION 1 GPIO ***
// NOTE if using Hardware SPI clock and data pins will be tied to 
// the chosen interface eg Spi0 CLK=18 DIN=19)
	int8_t SDIN_TFT = 19; 
	int8_t SCLK_TFT = 18; 
	int8_t DC_TFT = 3;
	int8_t CS_TFT = 2;  
	int8_t RST_TFT = 4;
	myTFT.TFTsetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
// ***
// *** USER OPTION 2 Screen Setup ***
	uint16_t TFT_WIDTH  =   40; // Screen width in pixels
	uint16_t TFT_HEIGHT =  160; // Screen height in pixels
	uint16_t OFFSET_X   =  -60; // Screen X offset in pixels
	uint16_t OFFSET_Y   =   60; // Screen Y offset in pixels
	// Display type, 4 choice's, see readme.
	GC9D01_TFT::Resolution_e DisplayType = GC9D01_TFT::Resolution_e::RGB40x160_SingleGate;
	// Pixel Draw mode type, 4 choices , see readme.  
	GC9D01_TFT::PixelFixMode_e FixType = GC9D01_TFT::PixelFixMode_e::Off;
	myTFT.TFTInitScreenSize(TFT_WIDTH, TFT_HEIGHT, DisplayType, FixType, OFFSET_X, OFFSET_Y);
// ***
	myTFT.TFTGC9D01Initialize(); 
	if (myTFT.setBuffer() != DisLib16::Success)
		return false;					  // set up buffer
	myTFT.setTextCharPixelOrBuffer(true); // set to use pixel mode for text
	// Print memory usage, optional
	DisLib16::printMemoryUsage();
	return true;
}

void Test102(void) {
	printf("Version %u \n",DisLib16::LibraryVersion());
	myTFT.TFTsetRotation(displaylib_16_graphics::Degrees_270);
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.clearBuffer(myTFT.C_BLACK);
	myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
	myTFT.setCursor(10, 10);
	myTFT.setFont(font_arialBold);
	myTFT.print("Hello 270");
	myTFT.writeBuffer(); // update screen from buffer
	MILLISEC_DELAY(TEST_DELAY5);
}

void EndTests(void)
{
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.clearBuffer(myTFT.C_BLACK);
	myTFT.destroyBuffer();
	myTFT.TFTPowerDown(); 
	printf("TFT: End\n");
}
///@endcond
// *************** EOF ****************
