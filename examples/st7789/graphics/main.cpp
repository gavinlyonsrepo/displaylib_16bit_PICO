/*!
	@file     main.cpp
	@author   Gavin Lyons
	@brief Example cpp file for st7789 library. Graphics methods testing
	@note  See USER OPTIONS 0-2 in SETUP function
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
#include "displaylib_16/st7789.hpp"

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY1 1000	  // mS
#define TEST_DELAY2 2000	  // mS
#define TEST_DELAY5 5000	  // mS

// Section :: Globals
ST7789_TFT myTFT;

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

/*!
	@brief setup + user options
*/
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
	uint16_t TFT_WIDTH = 240;  // Screen width in pixels
	uint16_t TFT_HEIGHT = 320; // Screen height in pixels
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW, TFT_WIDTH, TFT_HEIGHT);
	// ******************************************

	myTFT.TFTST7789Initialize();
}


/*!
	@brief   pixels and lines
*/
void Test901(void)
{
	MILLISEC_DELAY(TEST_DELAY1);
	myTFT.fillScreen(myTFT.C_BLACK);

	myTFT.drawPixel(85, 55, myTFT.C_WHITE);
	myTFT.drawPixel(87, 59, myTFT.C_WHITE);
	myTFT.drawPixel(110, 79, myTFT.C_WHITE);
	myTFT.drawLine(80, 80, 40, 40, myTFT.C_RED);
	myTFT.drawFastVLine(100, 100, 100, myTFT.C_GREEN);
	myTFT.drawFastHLine(160, 60, 70, myTFT.C_YELLOW);

	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  rectangles
*/
void Test902(void)
{
	myTFT.drawRectWH(65, 65, 20, 20, myTFT.C_RED);
	if(myTFT.fillRectBuffer(105, 75, 20, 20, myTFT.C_YELLOW) != 0) //uses spiwrite
	{
		printf("Error Test902 1: Error in the fillRectangle function\r\n");
	}
	myTFT.fillRect(160, 55, 20, 20, myTFT.C_GREEN);
	myTFT.drawRoundRect(15, 160, 50, 50, 5, myTFT.C_CYAN);
	myTFT.fillRoundRect(70, 160, 50, 50, 10, myTFT.C_WHITE);

	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  circles
*/
void Test903(void)
{
	myTFT.drawCircle(90, 160, 15, myTFT.C_GREEN);
	myTFT.fillCircle(140, 80, 15, myTFT.C_YELLOW);
}

/*!
	@brief  triangles
*/
void Test904(void)
{
	myTFT.drawTriangle(35, 80, 85, 40, 115, 80, myTFT.C_CYAN);
	myTFT.fillTriangle(55, 120, 100, 90, 127, 120, myTFT.C_RED);

	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}


/*!
	@brief  Stop testing and shutdown the TFT
*/
void EndTests(void)
{
	char teststr1[] = "Tests over";
	myTFT.FontNum(myTFT.Font_Default);
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.drawText(5, 50, teststr1, myTFT.C_GREEN, myTFT.C_BLACK, 2);
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTPowerDown();
	printf("TFT :: Tests Over");
}