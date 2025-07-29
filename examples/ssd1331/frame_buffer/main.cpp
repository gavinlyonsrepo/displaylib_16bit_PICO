/*!
	@file   main.cpp
	@author Gavin Lyons
	@brief  Example cpp file for ssd1331 library. Tests  frame buffer mode
	@note   See USER OPTIONS 0-2 in SETUP function
			dislib16_ADVANCED_SCREEN_BUFFER_ENABLE must be enabled for this example
			or it will not compile 
	@test
		-# Frame buffer mode.
*/

// Section ::  libraries
#include "pico/time.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16/Bitmap_TEST_Data.hpp"
#include "displaylib_16/ssd1331.hpp"

/// @cond

#ifndef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is not defined. it is required for this example")
#endif
// Section :: Globals
// Set contrast values at startup, range 0-0xFF
SSD1331_OLED::Constrast_values_t userContrast = { 0x7D, 0x7D, 0x7D };        //normal mode
SSD1331_OLED::Dim_Constrast_values_t userDimContrast = { 0x1E, 0x1E, 0x1E }; // dim mode
SSD1331_OLED myOLED(SSD1331_OLED::COLORORDER_RGB, userContrast, userDimContrast);

//  Section ::  Function Headers
bool Setup(void);
void Test1(void);
void TestBitmap(void);
void EndTests(void);
void ScreenReset(void);
void colorBlock(void);
//  Section ::  MAIN loop
int main(void)
{
	DisLib16::setDebugMode(true); // turn on debug mode
	if (!Setup())
		return -1;
	Test1();
	TestBitmap();
	//TestBitmapFPS();
	//TestTextFPS();
	EndTests();
}
// *** End OF MAIN **

//  Section ::  Function Space
bool Setup(void)
{
	stdio_init_all(); // optional for error messages , Initialize chosen serial port, default 38400 baud
	MILLISEC_DELAY(1000);
	printf("Start\r\n");
	DisLib16::printMemoryUsage();
	//*************** USER OPTION 0 SPI_SPEED + TYPE ***********
	bool bhardwareSPI = false; // true for hardware spi, false for software
	
	if (bhardwareSPI == true) { // hw spi
		uint32_t _SCLK_FREQ =  8000 ; // Spi freq in KiloHertz , 1000 = 1Mhz
		myOLED.InitSPIType(_SCLK_FREQ, spi0); 
	} else { // sw spi
		uint16_t SWSPICommDelay = 0; // optional SW SPI GPIO delay in uS
		myOLED.InitSPIType(SWSPICommDelay);
	}
	//*******************************************
	// ******** USER OPTION 1 GPIO **************
	// NOTE if using Hardware SPI clock and data pins will be tied to 
	// the chosen interface eg Spi0 CLK=18 DIN=19)
	int8_t SDIN_OLED = 19; 
	int8_t SCLK_OLED = 18; 
	int8_t DC_OLED = 3;
	int8_t CS_OLED = 2 ;  
	int8_t RST_OLED = 4;
	myOLED.SetupGPIO(RST_OLED, DC_OLED, CS_OLED, SCLK_OLED, SDIN_OLED);
	//**********************************************************

	// ****** USER OPTION 2 Screen Setup ****** 
	uint16_t OLED_WIDTH = 96;// Screen width in pixels
	uint16_t OLED_HEIGHT = 64; // Screen height in pixels
	myOLED.InitScreenSize(OLED_WIDTH , OLED_HEIGHT);
	// ******************************************

	myOLED.SSD1331Initialize();
	if (myOLED.setBuffer() != DisLib16::Success)
		return false;					  // set up buffer
	myOLED.setTextCharPixelOrBuffer(true); // set to use pixel mode for text
	DisLib16::setDebugMode(true);
	DisLib16::printMemoryUsage();
	busy_wait_ms(100);
	return true;
}

void EndTests(void)
{
	myOLED.fillScreen(myOLED.C_BLACK);
	myOLED.clearBuffer(myOLED.C_BLACK);
	myOLED.destroyBuffer();
	myOLED.fillScreen(myOLED.C_BLACK);
	busy_wait_ms(1000);
	myOLED.PowerDown(); // Power down device
	printf("End Test\n");
}

void ScreenReset(void)
{
	myOLED.writeBuffer(1);
	MILLISEC_DELAY(2500);
	myOLED.clearBuffer(myOLED.C_BLACK);
}

void Test1(void)
{
	printf("Test1\r\n");
	colorBlock();
	MILLISEC_DELAY(2500);
	myOLED.fillScreen(myOLED.C_BLACK);

	myOLED.fillCircle(20, 20, 10, myOLED.C_RED);
	MILLISEC_DELAY(2500);
	myOLED.fillScreen(myOLED.C_BLACK);

	myOLED.clearBuffer(myOLED.C_RED);
	ScreenReset();

	myOLED.clearBuffer(myOLED.C_GREEN);
	ScreenReset();

	myOLED.clearBuffer(myOLED.C_BLUE);
	ScreenReset();

	myOLED.clearBuffer(myOLED.C_BLACK);
	myOLED.setTextColor(myOLED.C_GREEN, myOLED.C_BLACK);
	myOLED.setCursor(0, 0);
	myOLED.setFont(font_default);
	myOLED.print("Buffer Mode     Test");
	ScreenReset();
}

void TestBitmap(void)
{
	// Bitmap 1-bit
	printf("Bitmap 1-bit\n");
	myOLED.drawBitmap(10, 10, 40, 16, myOLED.C_CYAN, myOLED.C_BLACK, sSunTextImage);
	ScreenReset();
	// Bitmap 16-bit
	printf("Bitmap 16-bit\n");
	myOLED.drawBitmap16Data(10, 10, sPosterImage, 80, 48);
	ScreenReset();
}

void colorBlock(void)
{
	myOLED.fillRect(0, 0, 60, 8, myOLED.C_RED);
	myOLED.fillRect(0, 8, 60, 8, myOLED.C_GREEN);
	myOLED.fillRect(0, 16, 60, 8, myOLED.C_BLUE);
	myOLED.fillRect(0, 24, 60, 8, myOLED.C_YELLOW);
	myOLED.fillRect(0, 32, 60, 8, myOLED.C_WHITE);
}
/// @endcond
