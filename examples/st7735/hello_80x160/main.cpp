/*!
	@file     main.cpp
	@author   Gavin Lyons
	@brief    Example cpp file for st7735 library. Tests Hello World
	@details  This example file is for 0.96 inch 80x160 pixel display 
			  See USER OPTIONS in SETUP function
			  Note PCB choice and offsets
	@test
		-# Test 101 write out color block and Hello world on 80x160 display
*/

// Section ::  libraries
#include "pico/time.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16/st7735.hpp"

/// @cond

#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif

// Section :: Globals
ST7735_TFT myTFT;

//  Section ::  Function Headers
void Setup(void); // setup + user options
void Test101(void);
void ColorTest(void);
void EndTests(void);

//  Section ::  MAIN loop
int main(void)
{
	Setup();
	myTFT.setRotation(myTFT.Degrees_0);
	ColorTest();
	Test101();
	EndTests();
}
// *** End OF MAIN **

//  Section ::  Function Space

void Setup(void)
{
	stdio_init_all(); // optional for error messages , Initialize chosen serial port, default 38400 baud
	MILLISEC_DELAY(1000);
	printf("TFT Start\r\n");

	//*************** USER OPTION 0 SPI_SPEED + TYPE ***********
	bool bhardwareSPI = true; // true for hardware spi, false for software

	if (bhardwareSPI == true)
	{								   // hw spi
		uint32_t TFT_SCLK_FREQ = 8000; // Spi freq in KiloHertz , 1000 = 1Mhz
		myTFT.TFTInitSPIType(TFT_SCLK_FREQ, spi0);
	}
	else
	{								 // sw spi
		uint16_t SWSPICommDelay = 0; // optional SW SPI GPIO delay in uS
		myTFT.TFTInitSPIType(SWSPICommDelay);
	}
	//*********************************************************
	// ******** USER OPTION 1 GPIO *********
	// NOTE if using Hardware SPI clock and data pins will be tied to
	// the chosen interface eg Spi0 CLK=18 DIN=19)
	int8_t SDIN_TFT = 19;
	int8_t SCLK_TFT = 18;
	int8_t DC_TFT = 3;
	int8_t CS_TFT = 2;
	int8_t RST_TFT = 4;
	myTFT.setupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
	//**********************************************************

	// ****** USER OPTION 2 Screen Setup ******
	uint8_t OFFSET_COL = 26;   // buydisplay suggests 24
	uint8_t OFFSET_ROW = 1;	   // buydisplay suggests  0
	uint16_t TFT_WIDTH = 80;   // Screen width in pixels
	uint16_t TFT_HEIGHT = 160; // Screen height in pixels
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW, TFT_WIDTH, TFT_HEIGHT);
	// ******************************************

	// ******** USER OPTION 3 PCB_TYPE  **************************
	myTFT.TFTInitPCBType(myTFT.TFT_ST7735S_80160); // pass enum, multi choice,see README
	//**********************************************************
}

void Test101(void)
{
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
	myTFT.setCursor(0, 0);
	myTFT.setFont(font_default);
	myTFT.print("Hello");
	MILLISEC_DELAY(5000);
	myTFT.fillScreen(myTFT.C_BLACK);
	MILLISEC_DELAY(1000);
}

// Colour Test :: Red, Green, Blue, Yellow, White — black background
// If colours are wrong check: PCB type (option 3), TFTchangeInvertMode()
void ColorTest(void)
{
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.fillRoundRect(8,  12, 64, 24, 8, myTFT.C_RED);
	myTFT.fillRoundRect(8,  40, 64, 24, 8, myTFT.C_GREEN);
	myTFT.fillRoundRect(8,  68, 64, 24, 8, myTFT.C_BLUE);
	myTFT.fillRoundRect(8,  96, 64, 24, 8, myTFT.C_YELLOW);
	myTFT.fillRoundRect(8, 124, 64, 24, 8, myTFT.C_WHITE);
	MILLISEC_DELAY(5000);
	myTFT.fillScreen(myTFT.C_BLACK);
	MILLISEC_DELAY(1000);
}

void EndTests(void)
{
	myTFT.TFTPowerDown();
	printf("TFT: Tests Over");
}

/// @endcond
