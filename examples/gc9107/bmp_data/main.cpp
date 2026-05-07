/*!
	@file   main.cpp
	@author Gavin Lyons
	@brief  Example cpp file for gc9107 bitmap tests
	@details Setup for 60x160 display.
			 See USER OPTIONS in SETUP function
	@test
		-# Test 300 Sprite 
		-# Test 301 icons
		-# Test 302 bi-color small image
		-# Test 305 16 bit color image data from a data array
*/

// Section ::  libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16/gc9107.hpp"
#include "displaylib_16/Bitmap_TEST_Data.hpp" // bitmap data

/// @cond

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000
#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif

// Init display class object
GC9107_TFT myTFT;

//  Section ::  Function Headers

void Setup(void);	// setup + user options
void Test300(void); // sprite
void Test302(void); // bi-color small image
void Test305(void); // 16 bit color image data from a data array
void EndTests(void);

//  Section ::  MAIN

int main(void)
{
	Setup();
	// rotate the screen 90 degrees
	myTFT.TFTsetRotation(myTFT.Degrees_90);
	Test300();
	Test302();
	Test305();
	EndTests();
}
// *** End OF MAIN **

//  Section ::  Function Space

/*!
	@brief  setup + user options
*/
void Setup(void)
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
}

/*!
	@brief  "sprite demo" 32 x 32 with LBLUE background
*/
void Test300(void)
{
	// Test 300-A test 16-bit color Sprite 
	// Draw as sprite, without background , 32 X 32 .background color = ST7375_LBLUE
	// Green background screen
	myTFT.fillScreen(myTFT.C_GREEN);
	MILLISEC_DELAY(TEST_DELAY2);
	myTFT.drawSpriteData(0, 0, sSpriteTest16, 32, 32, myTFT.C_LBLUE, false);
	MILLISEC_DELAY(TEST_DELAY5);

	// Test 300-B test 16-bit color Sprite 
	// Draw as sprite, without background , 32 X 32 .background color = ST7375_LBLUE
	// Bitmap background screen
	myTFT.drawBitmap16Data(60, 0, sPosterImage, 80, 48);
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.drawSpriteData(60, 0, sSpriteTest16, 32, 32, myTFT.C_LBLUE, false);
	myTFT.drawSpriteData(90, 0, sSpriteTest16, 32, 32, myTFT.C_LBLUE, false);
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Test 302 bi-color small image 20x24
*/
void Test302(void)
{

	myTFT.fillScreen(myTFT.C_BLACK);
	char teststr1[] = "Test 302";
	myTFT.writeCharString(5, 10 , teststr1);
	myTFT.drawBitmap(0, 30 , 40, 16, myTFT.C_CYAN, myTFT.C_BLACK, sSunTextImage);
	myTFT.drawBitmap(50, 30 , 40, 16, myTFT.C_RED, myTFT.C_BLACK, sSunTextImage);
	myTFT.drawBitmap(90 , 30 , 40, 16, myTFT.C_YELLOW, myTFT.C_RED, sSunTextImage);
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Test305 16 bit color image data from a data array
*/
void Test305(void)
{
	char teststr1[] = "Test 305";
	myTFT.fillScreen(myTFT.C_WHITE);
	myTFT.writeCharString(0, 0, teststr1);
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.drawBitmap16Data(0, 0, sPosterImage, 80, 48);
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Stop testing and shutdown the TFT
*/
void EndTests(void)
{
	char teststr1[] = "Tests over";
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.writeCharString(0, 5, teststr1);
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTPowerDown();
	printf("TFT :: Tests Over\r\n");
}

/// @endcond
