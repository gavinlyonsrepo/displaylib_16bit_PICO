/*!
	@file    main.cpp
	@author  Gavin Lyons
	@brief   Example cpp file for gc9107 driver. Test Hello World.
	@details Setup for 60x160 display.
	@note    See USER OPTIONS in SETUP function
	@test
		-# Test 103 Print out Hello world  at 90 degrees
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

// Init display class object
GC9107_TFT myTFT;

//  Section ::  Function Headers 

void Setup(void);  // setup + user options
void Test103(void);  
void EndTests(void);

//  Section ::  MAIN loop

int main(void) 
{
	Setup();
	Test103();
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
	myTFT.TFTsetRotation(myTFT.Degrees_90);
}

void Test103(void) {
	printf("Version %u \n",DisLib16::LibraryVersion());
	myTFT.setFont(font_default);
	myTFT.fillScreen(myTFT.C_RED);
	myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
	myTFT.setCursor(0, 0);
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
