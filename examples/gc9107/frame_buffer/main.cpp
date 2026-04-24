/*!
	@file   main.cpp
	@author Gavin Lyons
	@brief  Example cpp file for gc9107 library. Tests frame buffer mode
	@details Setup for 60x160 display. This example is for testing the frame buffer 
			mode of the library, 
			see readme for more details on this mode and how to use it.
	@note   See USER OPTIONS 0-2 in SETUP function
			dislib16_ADVANCED_SCREEN_BUFFER_ENABLE must be enabled for this example
			or it will not compile. Its in displaylib_16_graphics.hpp
	@test
		-# 1001 Frame buffer mode.
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
// Screen Values Setup
// Size of VRAM (note 60x160 display has VRAM of 128x160)
uint16_t TFT_VRAM_WIDTH  = 128; // VRAM width  size in pixels
uint16_t TFT_VRAM_HEIGHT = 160; // VRAM height size in pixels
// Offsets for portrait and landscape rotations, see readme for values calculation
#define X_OFFSET 34 // Offset for portrait  rotation  0 and 180
#define Y_OFFSET 34 // Offset for landscape rotation 90 and 270
// Init display class object
GC9107_TFT myTFT;
// GRAM memory base mapping resolution, see GC9107_TFT::GM_memory_base_e enum
GC9107_TFT::GM_memory_base_e TFTmemoryBase = GC9107_TFT::GM_memory_base_e::MEMORY_BASE_GM_128x160;

//  Section ::  Function Headers
bool Setup(void);
void Test100(void);
void EndTests(void);
void ScreenReset(void);

//  Section ::  MAIN loop
int main(void)
{
	DisLib16::setDebugMode(true); // turn on debug mode
	if (!Setup())
		return -1;
	myTFT.TFTsetRotation(myTFT.Degrees_90);
	Test100();
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
	myTFT.TFTInitScreen(TFT_VRAM_WIDTH , TFT_VRAM_HEIGHT, TFTmemoryBase, GC9107_TFT::MADCTL_FLAGS_t::RGB);
// ***
	myTFT.TFTGC9107Initialize();

	if (myTFT.setBuffer() != DisLib16::Success)
		return false;					  // set up buffer
	myTFT.setTextCharPixelOrBuffer(true); // set to use pixel mode for text
	DisLib16::setDebugMode(true);
	DisLib16::printMemoryUsage();
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

void Test100(void)
{
	printf("Test1\r\n");
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
	myTFT.setCursor(15, 15+Y_OFFSET);
	myTFT.setFont(font_arialBold);
	myTFT.print("Buffer");
	myTFT.writeBuffer();
	MILLISEC_DELAY(5000);
}

/// @endcond
