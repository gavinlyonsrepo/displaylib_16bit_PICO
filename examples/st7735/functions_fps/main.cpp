/*!
	@file   main.cpp
	@author Gavin Lyons
	@brief  Example cpp file for st7735 driver
			Function testing, rotate , modes, scroll etc.
			Also FPS frame rate per second test.
	@note  See USER OPTIONS 0-3 in SETUP function
	@test
		-# Test501  Color
		-# Test502  scroll
		-# Test503  Rotate
		-# Test504 change modes test -> Invert, display on/off and Sleep.
		-# Test602 FPS frame rate per second test 
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
void Test501(void);	// test colors
void Test502(void);	// scroll
void Test503(void);	// Rotate
void Test504(void);	// change modes test -> Invert, display on/off and Sleep.
void Test602(void); // FPS, frame rate per second
void EndTests(void);

//  Section ::  MAIN loop

int main(void)
{
	Setup();
	Test501();
	Test502();
	Test503();
	Test504();
	Test602();
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
	uint16_t TFT_WIDTH = 128;  // Screen width in pixels
	uint16_t TFT_HEIGHT = 128; // Screen height in pixels
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW, TFT_WIDTH, TFT_HEIGHT);
	// ******************************************

	// ******** USER OPTION 3 PCB_TYPE  **************************
	myTFT.TFTInitPCBType(myTFT.TFT_ST7735R_Red); // pass enum,4 choices,see README
	//**********************************************************
}

void Test501(void)
{
	myTFT.fillScreen(myTFT.C_BLACK);
	printf("Test 501: Color Test:: Red,green,blue,yellow,white, black background\r\n");
	myTFT.fillRoundRect(8, 10, 24, 60, 8, myTFT.C_RED);
	myTFT.fillRoundRect(32, 10, 24, 60, 8, myTFT.C_GREEN);
	myTFT.fillRoundRect(56, 10, 24, 60, 8, myTFT.C_BLUE);
	myTFT.fillRoundRect(80, 10, 24, 60, 8, myTFT.C_YELLOW);
	myTFT.fillRoundRect(104, 10, 24, 60, 8, myTFT.C_WHITE);
	printf("If colors are wrong user may have selected wrong PCB type in setup option 3\r\n");
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Vertical Scroll test
*/
void Test502(void)
{
	const uint8_t LINES = 10, LINE_SIZE = 10, LINE_OFFSET = 3, TOP_FIXED = 0, BOTTOM_FIXED = 0;
	char teststr1[] = "Scroll test";

	for (uint8_t i = 0; i < LINES; i++)
	{
		myTFT.drawText(0, LINE_OFFSET + i * LINE_SIZE, teststr1, myTFT.C_WHITE, myTFT.C_BLACK, 1);
	}
	myTFT.setScrollDefinition(TOP_FIXED, BOTTOM_FIXED, 1); // bottom-to-top
	uint8_t pos = LINE_OFFSET;
	for (uint8_t i = 0; i < LINES; i++)
	{
		for (uint8_t j = 0; j < LINE_SIZE; j++)
		{
			myTFT.TFTVerticalScroll(pos + TOP_FIXED);
			pos++;
			// check pos if necessary: must be < tftTFT_HEIGHT - TOP_FIXED - BOTTOM_FIXED
			MILLISEC_DELAY(5);
		}
		MILLISEC_DELAY(TEST_DELAY1);
	}
	myTFT.TFTchangeMode(myTFT.TFT_Normal_mode);
	myTFT.fillScreen(myTFT.C_BLACK);
}


/*!
	@brief  Rotate
*/
void Test503()
{
	char teststr0[] = "Rotate 0";	// normal
	char teststr1[] = "Rotate 90";	// 90
	char teststr2[] = "Rotate 180"; // 180
	char teststr3[] = "Rotate 270"; // 270

	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setRotation(myTFT.Degrees_0);
	myTFT.drawText(20, 20, teststr0, myTFT.C_GREEN, myTFT.C_BLACK, 1);
	MILLISEC_DELAY(TEST_DELAY2);

	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setRotation(myTFT.Degrees_90);
	myTFT.drawText(20, 20, teststr1, myTFT.C_GREEN, myTFT.C_BLACK, 1);
	MILLISEC_DELAY(TEST_DELAY2);

	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setRotation(myTFT.Degrees_180);
	myTFT.drawText(20, 20, teststr2, myTFT.C_GREEN, myTFT.C_BLACK, 1);
	MILLISEC_DELAY(TEST_DELAY2);

	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setRotation(myTFT.Degrees_270);
	myTFT.drawText(20, 20, teststr3, myTFT.C_GREEN, myTFT.C_BLACK, 1);
	MILLISEC_DELAY(TEST_DELAY2);

	myTFT.setRotation(myTFT.Degrees_0);
	myTFT.TFTchangeMode(myTFT.TFT_Normal_mode);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  change modes test -> Invert, display on/off and Sleep.
*/
void Test504()
{
	printf("Test 504: Mode Tests\r\n");
	char teststr1[] = "Modes Test";
	myTFT.fillRoundRect(8, 10, 24, 60, 8, myTFT.C_RED);
	myTFT.fillRoundRect(32, 10, 24, 60, 8, myTFT.C_GREEN);
	myTFT.fillRoundRect(56, 10, 24, 60, 8, myTFT.C_BLUE);
	myTFT.fillRoundRect(80, 10, 24, 60, 8, myTFT.C_YELLOW);
	myTFT.fillRoundRect(104, 10, 24, 60, 8, myTFT.C_WHITE);
	myTFT.drawText(10, 80, teststr1, myTFT.C_RED, myTFT.C_YELLOW, 1);
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTchangeMode(myTFT.TFT_Normal_mode);
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTchangeMode(myTFT.TFT_Invert_mode);
	printf("Test 504-2: Mode Invert\r\n");
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTchangeMode(myTFT.TFT_Normal_mode);
	printf("Test 504-3: Mode Normal\r\n");
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTchangeMode(myTFT.TFT_Display_off_mode);
	printf("Test 504-4: Mode Display off\r\n");
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTchangeMode(myTFT.TFT_Display_on_mode);
	printf("Test 504-5: Mode Display on\r\n");
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTchangeMode(myTFT.TFT_Sleep_mode);
	printf("Test 504-6: Mode Sleep on\r\n");
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTchangeMode(myTFT.TFT_Normal_mode);
	printf("Test 504-7 Mode Normal\r\n");
	MILLISEC_DELAY(TEST_DELAY2);
	myTFT.fillScreen(myTFT.C_BLACK);
}


/*!
	@brief   Frame rate per second test, FPS test. Optionally specify -> set bool bTestFPS
*/
void Test602(void)
{
	myTFT.FontNum(myTFT.Font_Default);
	// Values to count frame rate per second
	long previousMillis = 0;
	long lastFramerate = 0;
	long currentFramerate = 0;
	uint16_t count = 0;
	uint16_t seconds = 0;
	uint16_t fps = 0;
	uint16_t shapeColor = 0x2222;
	myTFT.setTextSize(1);

	while (1)
	{

		unsigned long currentMillis = to_ms_since_boot(get_absolute_time());

		if (currentMillis - previousMillis >= 1000) // every second
		{
			fps = currentFramerate - lastFramerate;
			lastFramerate = currentFramerate;
			previousMillis = currentMillis;
			seconds++;
			shapeColor = rand() % 60000;
			if (count >= 1500)
				return; // end if count gets to 1500
		}
		currentFramerate++;
		count++;

		//  ** Code to test **
		// print seconds
		myTFT.setCursor(5, 15);
		myTFT.print("Secs :");
		myTFT.setCursor(50, 15);
		myTFT.print(seconds);
		// print fps
		myTFT.setCursor(5, 25);
		myTFT.print("FPS :");
		myTFT.setCursor(50, 25);
		myTFT.print(fps);
		// print count
		myTFT.setCursor(5, 35);
		myTFT.print("Count :");
		myTFT.setCursor(50, 35);
		myTFT.print(count);
		// print some graphics
		myTFT.drawCircle(90, 90, 10, shapeColor);
		myTFT.drawRectWH(40, 80, 20, 20, shapeColor << 1);
		myTFT.drawRoundRect(5, 80, 20, 20, 2, shapeColor >> 1);
		//  ****
	}
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

// *************** EOF ****************

/// @endcond
