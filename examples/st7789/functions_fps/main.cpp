/*!
	@file   main.cpp
	@author Gavin Lyons
	@brief  Example cpp file for st7789 driver.
			Function testing, rotate , modes, scroll etc.
			Also FPS frame rate per second test.
	@note  See USER OPTIONS 0-2 in SETUP function
	@test
		-# Test500 Scroll test 
		-# Test501 Color
		-# Test503 Rotate
		-# Test504 change modes test -> Invert, display on/off and Sleep.
		-# Test602 FPS frame rate per second test for text
		-# Test603 FPS frame rate per second test for graphics
*/

// Section ::  libraries
#include "pico/time.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16/st7789.hpp"

/// @cond

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY1 1000	  // mS
#define TEST_DELAY2 2000	  // mS
#define TEST_DELAY5 5000	  // mS

// Section :: Globals
ST7789_TFT myTFT;

//  Section ::  Function Headers

void Setup(void);	// setup + user options
void Test500(void);
void Test501(void);	// test colors
void Test503(void);	// Rotate
void Test504(void);	// change modes test -> Invert, display on/off and Sleep.
void Test602(void); // FPS, frame rate per second text
void Test603(void); // FPS, frame rate per second graphics
void EndTests(void);

//  Section ::  MAIN loop

int main(void)
{
	Setup();
	Test500();
	Test501();
	Test503();
	Test504();
	Test602();
	Test603();
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
	printf("TFT Start\r\n");

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
	int8_t RST_TFT = 4;
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
	myTFT.fillScreen(myTFT.C_BLACK);
}
void Test500(void)
{
	printf("Test 500: Scroll\r\n");
	myTFT.setFont(font_default);
	myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
	const uint8_t LINES = 10, LINE_SIZE = 10, LINE_OFFSET = 3, TOP_FIXED = 0, BOTTOM_FIXED = 0;
	char teststr1[] = "Scroll test";
	
	for (uint8_t i = 0; i < LINES; i++)
	{
	myTFT.writeCharString(5, LINE_OFFSET+i*LINE_SIZE,teststr1);
	}
	myTFT.setScrollDefinition(TOP_FIXED,BOTTOM_FIXED,1);  // bottom-to-top
	uint8_t pos = LINE_OFFSET;
	for (uint8_t i = 0; i < LINES; i++) 
	{
		for (uint8_t j = 0; j < LINE_SIZE; j++) 
		{
			myTFT.TFTVerticalScroll(pos + TOP_FIXED);
			pos++;
			// check pos if necessary: must be < tftTFT_HEIGHT - TOP_FIXED - BOTTOM_FIXED 
		}
	MILLISEC_DELAY(TEST_DELAY1);
	}
	myTFT.TFTNormalMode();
	myTFT.fillScreen(myTFT.C_BLACK);

}

void Test501(void)
{
	myTFT.fillScreen(myTFT.C_BLACK);
	printf("Test 501: Color Test:: Red,green,blue,yellow,white, black background\r\n");
	myTFT.fillRoundRect(8, 60, 24, 60, 8, myTFT.C_RED);
	myTFT.fillRoundRect(32, 60, 24, 60, 8, myTFT.C_GREEN);
	myTFT.fillRoundRect(56, 60, 24, 60, 8, myTFT.C_BLUE);
	myTFT.fillRoundRect(80, 60, 24, 60, 8, myTFT.C_YELLOW);
	myTFT.fillRoundRect(104, 60, 24, 60, 8, myTFT.C_WHITE);
	MILLISEC_DELAY(TEST_DELAY5);
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
	myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
	myTFT.setRotation(myTFT.Degrees_0);
	myTFT.writeCharString(40, 40, teststr0);
	MILLISEC_DELAY(TEST_DELAY2);

	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setRotation(myTFT.Degrees_90);
	myTFT.writeCharString(40, 40, teststr1);
	MILLISEC_DELAY(TEST_DELAY2);

	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setRotation(myTFT.Degrees_180);
	myTFT.writeCharString(40, 40, teststr2);
	MILLISEC_DELAY(TEST_DELAY2);

	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setRotation(myTFT.Degrees_270);
	myTFT.writeCharString(40, 40, teststr3);
	MILLISEC_DELAY(TEST_DELAY2);

	myTFT.setRotation(myTFT.Degrees_0);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  change modes test -> Invert, display on/off and Sleep.
*/
void Test504()
{
	printf("Test 504: Mode Tests\r\n");
	char teststr1[] = "Modes Test";
	myTFT.fillRoundRect(32, 60, 24, 60, 8, myTFT.C_RED);
	myTFT.fillRoundRect(56, 60, 24, 60, 8, myTFT.C_GREEN);
	myTFT.fillRoundRect(80, 60, 24, 60, 8, myTFT.C_BLUE);
	myTFT.fillRoundRect(104, 60, 24, 60, 8, myTFT.C_YELLOW);
	myTFT.fillRoundRect(124, 60, 24, 60, 8, myTFT.C_WHITE);
	myTFT.writeCharString(40, 140, teststr1);
	MILLISEC_DELAY(TEST_DELAY2);
	
	// Invert on and off
	myTFT.TFTchangeInvertMode(true);
	printf("Test 504-2: Mode Invert\r\n");
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTchangeInvertMode(false);
	MILLISEC_DELAY(TEST_DELAY5);
	
	// Display on and off
	myTFT.TFTenableDisplay(false);
	printf("Test 504-3: Mode Display off\r\n");
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTenableDisplay(true);
	MILLISEC_DELAY(TEST_DELAY5);
	
	// Sleep on and off
	myTFT.TFTsleepDisplay(true);
	printf("Test 504-4: Mode Sleep on\r\n");
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTsleepDisplay(false);
	MILLISEC_DELAY(TEST_DELAY2);
	myTFT.fillScreen(myTFT.C_BLACK);
}


/*!
	@brief   Frame rate per second test, FPS test. text
*/
void Test602(void)
{
	myTFT.setFont(font_arialRound);
	// Values to count frame rate per second
	long previousMillis = 0;
	long lastFramerate = 0;
	long currentFramerate = 0;
	uint16_t count = 0;
	uint16_t seconds = 0;
	uint16_t fps = 0;
	while (1)
	{
		unsigned long currentMillis = to_ms_since_boot(get_absolute_time());
		if (currentMillis - previousMillis >= 1000) // every second
		{
			fps = currentFramerate - lastFramerate;
			lastFramerate = currentFramerate;
			previousMillis = currentMillis;
			seconds++;
			if (count >= 500){
				printf("FPS : %u\n", fps);
				printf("Seconds : %u\n", seconds);
				return; // end if count gets to this
			}
		}
		currentFramerate++;
		count++;
		//  ** Code to test **
		// print seconds
		myTFT.setCursor(5, 55);
		myTFT.print("Secs :");
		myTFT.setCursor(120, 55);
		myTFT.print(seconds);
		// print fps
		myTFT.setCursor(5, 85);
		myTFT.print("FPS :");
		myTFT.setCursor(120, 85);
		myTFT.print(fps);
		// print count
		myTFT.setCursor(5, 125);
		myTFT.print("Count:");
		myTFT.setCursor(120, 125);
		myTFT.print(count);

		myTFT.setCursor(5, 165);
		myTFT.print("Testing!");
		//  ****
	}
}

/*!
	@brief   Frame rate per second test, FPS test. graphics
*/
void Test603(void)
{
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setFont(font_arialRound);
	// Values to count frame rate per second
	long previousMillis = 0;
	long lastFramerate = 0;
	long currentFramerate = 0;
	uint16_t count = 0;
	uint16_t seconds = 0;
	uint16_t fps = 0;
	uint16_t shapeColor = 0x2222;
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
			if (count >= 1000){
				printf("FPS : %u\n", fps);
				printf("Seconds : %u\n", seconds);
				return; // end if count gets to this
			}
		}
		currentFramerate++;
		count++;

		//  ** Code to test **
		// print fps
		myTFT.setCursor(5, 45);
		myTFT.print(fps);
		// print some graphics
		myTFT.drawCircle(90, 210, 10, shapeColor);
		myTFT.drawRectWH(40, 200, 20, 20, shapeColor << 1);
		myTFT.drawRoundRect(5, 200, 20, 20, 2, shapeColor >> 1);
		myTFT.fillCircle(90, 210, 10, shapeColor);
		myTFT.fillRect(40, 200, 20, 20, shapeColor << 1);
		myTFT.fillRoundRect(5, 200, 20, 20, 2, shapeColor >> 1);
		myTFT.fillTriangle(55, 120, 100, 90, 127, 120, shapeColor-100);
		myTFT.fillTriangle(55, 120, 100, 160, 160, 160, shapeColor);
		//  ****
	}
}

/*!
	@brief  Stop testing and shutdown the TFT
*/
void EndTests(void)
{
	myTFT.setFont(font_groTesk);
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
	myTFT.setCursor(15,50);
	myTFT.print("Tests over");
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTPowerDown();
	printf("Tests Over");
}

// *************** EOF ****************

/// @endcond
