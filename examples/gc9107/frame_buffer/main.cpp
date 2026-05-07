/*!
	@file   main.cpp
	@author Gavin Lyons
	@brief  Example cpp file for gc9107 library. Tests frame buffer mode
	@details Setup for 60x160 display. This example is for testing the frame buffer 
			mode of the library, 
			see readme for more details on this mode and how to use it.
	@note   See USER OPTIONS in SETUP function
			dislib16_ADVANCED_SCREEN_BUFFER_ENABLE must be enabled for this example
			or it will not compile. Its in displaylib_16_graphics.hpp
	@test
		-# 1005 Frame buffer mode.
*/

// Section ::  libraries
#include "pico/time.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16/gc9107.hpp"

/// @cond

#ifndef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is not defined. it is required for this example")
#endif

// Init display class object
GC9107_TFT myTFT;

//  Section ::  Function Headers
bool Setup(void);
void Test1005(void);
void EndTests(void);
void ScreenReset(void);

//  Section ::  MAIN loop
int main(void)
{
	DisLib16::setDebugMode(true); // turn on debug mode  optional
	if (!Setup())
		return -1;
	myTFT.TFTsetRotation(myTFT.Degrees_90);
	Test1005();
	EndTests();
}
// *** End OF MAIN **

//  Section ::  Function Space
bool Setup(void)
{
	stdio_init_all(); // optional for error messages , Initialize chosen serial port, default 38400 baud
	MILLISEC_DELAY(1000);
	printf("TFT: Start\r\n");
// *** USER OPTION 0 SPI_SPEED + TYPE ***
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
	int8_t CS_TFT = 2 ;  
	int8_t RST_TFT = 4;
	myTFT.TFTsetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
// ***
// *** USER OPTION 2 Screen Setup ***
	uint16_t TFT_WIDTH = 60;   // width  size in pixels
	uint16_t TFT_HEIGHT = 160; // height size in pixels
	// GRAM memory base mapping resolution, see GC9107_TFT::GM_memory_base_e enum
	GC9107_TFT::GM_memory_base_e TFTmemoryBase = GC9107_TFT::GM_memory_base_e::MEMORY_BASE_GM_128x160;
	myTFT.TFTInitScreen(TFT_WIDTH , TFT_HEIGHT, TFTmemoryBase, GC9107_TFT::MADCTL_FLAGS_t::RGB);
// ***
// *** USER OPTION 3 Screen offsets in portrait***
	uint8_t X_Offset = 34;
	uint8_t Y_Offset = 0;
	myTFT.TFTsetPanelOffset(X_Offset, Y_Offset);
// ***
	myTFT.TFTGC9107Initialize();

	if (myTFT.setBuffer() != DisLib16::Success)
		return false;					  // set up buffer
	myTFT.setTextCharPixelOrBuffer(true); // set to use pixel mode for text
	DisLib16::printMemoryUsage(); // optional
	return true;
}

void EndTests(void)
{
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.clearBuffer(myTFT.C_BLACK);
	myTFT.destroyBuffer();
	myTFT.TFTPowerDown();
	printf("TFT: Tests Over\n");
}

void ScreenReset(void)
{
	myTFT.writeBuffer();
	MILLISEC_DELAY(2500);
	myTFT.clearBuffer(myTFT.C_BLACK);
}

void Test1005(void)
{
	printf("Test1005\r\n");
	myTFT.clearBuffer(myTFT.C_RED);
	myTFT.writeBuffer();
	MILLISEC_DELAY(1000);
	myTFT.clearBuffer(myTFT.C_GREEN);
	myTFT.writeBuffer();
	MILLISEC_DELAY(1000);
	myTFT.clearBuffer(myTFT.C_BLUE);
	myTFT.writeBuffer();
	MILLISEC_DELAY(1000);

	myTFT.clearBuffer(myTFT.C_BLACK);
	myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
	myTFT.setCursor(15, 15);
	myTFT.setFont(font_arialBold);
	myTFT.print("Buffer");
	myTFT.writeBuffer();
	MILLISEC_DELAY(5000);
}

/// @endcond
