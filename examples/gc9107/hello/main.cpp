/*!
	@file   main.cpp
	@author Gavin Lyons
	@brief  Example cpp file for gc9107 driver. Test Hello World.
	@details Setup for 60x160 display.
	@note   See USER OPTIONS 0-2 in SETUP function
	@test
		-# Test 101 Print out Hello world  
*/

// Section ::  libraries 
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16/gc9107.hpp"

///@cond

// Section :: Defines   
//  Test timing related defines 
#define TEST_DELAY1 1000 // mS
#define TEST_DELAY2 2000 // mS
#define TEST_DELAY5 5000 // mS
#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
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

void Setup(void);  // setup + user options
void Test100(void);  
void EndTests(void);

//  Section ::  MAIN loop

int main(void) 
{
	Setup();
	Test100();
	EndTests();
}
// *** End OF MAIN **

//  Section ::  Function Space 

void Setup(void)
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
	int8_t CS_TFT = 2 ;  
	int8_t RST_TFT = 4;
	myTFT.TFTsetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
// ***
// *** USER OPTION 2 Screen Setup ***
	myTFT.TFTInitScreen(TFT_VRAM_WIDTH , TFT_VRAM_HEIGHT, TFTmemoryBase, GC9107_TFT::MADCTL_FLAGS_t::RGB);
// ***
	myTFT.TFTGC9107Initialize(); 
}

void Test100(void) {
	printf("Version %u \n",DisLib16::LibraryVersion());
	myTFT.setFont(font_default);
	myTFT.fillScreen(myTFT.C_RED);
	myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
	myTFT.setCursor(X_OFFSET, 0);
	myTFT.print("Hello");
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLUE);
	MILLISEC_DELAY(TEST_DELAY1);
}

void EndTests(void)
{
	myTFT.TFTPowerDown(); 
	printf("TFT: End\n");
}
///@endcond
// *************** EOF ****************
