/*!
	@file   main.cpp
	@author Gavin Lyons
	@brief  Example cpp file for st7735 driver, Graphics methods testing
	@note   See USER OPTIONS 0-3 in SETUP function
	@test
		-# Test901  pixels and lines
		-# Test902  rectangles
		-# Test903  Circle
		-# Test904  Triangles
*/

// Section ::  libraries
#include "pico/time.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16/st7735.hpp"

/// @cond

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY1 1000	  // mS
#define TEST_DELAY2 2000	  // mS
#define TEST_DELAY5 5000	  // mS

// Section :: Globals
ST7735_TFT myTFT;

//  Section ::  Function Headers

void Setup(void);	// setup + user options
void Test901(void);	// pixels and lines
void Test902(void);	// rectangles
void Test903(void);	// Circle
void Test904(void);	// Triangles
void EndTests(void);

//  Section ::  MAIN loop
int main(void)
{
	Setup();
	Test901();
	Test902();
	Test903();
	Test904();
	EndTests();
	return 0;
}
// *** End OF MAIN **

//  Section ::  Function Space
void Setup(void)
{
	stdio_init_all(); // optional for error messages , Initialize chosen serial port, default 38400 baud
	MILLISEC_DELAY(TEST_DELAY1);
	printf("TFT :: Start\r\n");

	//*************** USER OPTION 0 SPI_SPEED + TYPE ***********
	bool bhardwareSPI = true; // true for hardware spi,

	if (bhardwareSPI == true)
	{								// hw spi
		uint32_t TFT_SCLK_FREQ = 8000; // Spi freq in KiloHertz , 1000 = 1Mhz
		myTFT.TFTInitSPIType(TFT_SCLK_FREQ, spi0);
	}
	else
	{								 // sw spi
		uint16_t SWSPICommDelay = 0; // optional SW SPI GPIO delay in uS
		myTFT.TFTInitSPIType(SWSPICommDelay);
	}
	//**********************************************************

	// ******** USER OPTION 1 GPIO *********
	// NOTE if using Hardware SPI clock and data pins will be tied to
	// the chosen interface eg Spi0 CLK=18 DIN=19)
	int8_t SDIN_TFT = 19;
	int8_t SCLK_TFT = 18;
	int8_t DC_TFT = 3;
	int8_t CS_TFT = 2;
	int8_t RST_TFT = 17;
	myTFT.setupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
	//**********************************************************

	// ****** USER OPTION 2 Screen Setup ******
	uint8_t OFFSET_COL = 0;	   // 2, These offsets can be adjusted for any issues->
	uint8_t OFFSET_ROW = 0;	   // 3, with screen manufacture tolerance/defects
	uint16_t TFT_WIDTH = 128;  // Screen width in pixels
	uint16_t TFT_HEIGHT = 128; // Screen height in pixels
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW, TFT_WIDTH, TFT_HEIGHT);
	// ******************************************

	// ******** USER OPTION 3 PCB_TYPE  **************************
	myTFT.TFTInitPCBType(myTFT.TFT_ST7735R_Red); // pass enum,4 choices,see README
	//**********************************************************
}


void Test901(void)
{
	MILLISEC_DELAY(TEST_DELAY1);
	myTFT.fillScreen(myTFT.C_BLACK);

	myTFT.drawPixel(85, 5, myTFT.C_WHITE);
	myTFT.drawPixel(87, 7, myTFT.C_WHITE);
	myTFT.drawPixel(89, 9, myTFT.C_WHITE);
	myTFT.drawLine(10, 10, 30, 30, myTFT.C_RED);
	myTFT.drawFastVLine(40, 40, 40, myTFT.C_GREEN);
	myTFT.drawFastHLine(60, 60, 40, myTFT.C_YELLOW);

	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test902(void)
{
	myTFT.drawRectWH(5, 5, 20, 20, myTFT.C_RED);
	if(myTFT.fillRectBuffer(45, 5, 20, 20, myTFT.C_YELLOW) != 0) //uses spiwrite
	{
		printf("Error Test902 1: Error in the fillRectangle function\r\n");
	}
	myTFT.fillRect(85, 5, 20, 20, myTFT.C_GREEN);
	myTFT.drawRoundRect(15, 60, 50, 50, 5, myTFT.C_CYAN);
	myTFT.fillRoundRect(70, 60, 50, 50, 10, myTFT.C_WHITE);

	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test903(void)
{
	myTFT.drawCircle(40, 20, 15, myTFT.C_GREEN);
	myTFT.fillCircle(80, 20, 15, myTFT.C_YELLOW);
}

/*!
	@brief  triangles
*/
void Test904(void)
{
	myTFT.drawTriangle(5, 80, 50, 40, 95, 80, myTFT.C_CYAN);
	myTFT.fillTriangle(55, 120, 100, 90, 127, 120, myTFT.C_RED);

	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}


void EndTests(void)
{
	myTFT.TFTPowerDown(); 
	printf("TFT :: Tests Over");
}
/// @endcond
