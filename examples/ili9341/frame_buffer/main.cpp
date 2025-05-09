/*!
	@file   main.cpp
	@author Gavin Lyons
	@brief  Example cpp file for ili9341 library. Tests  frame buffer mode
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
#include "displaylib_16/ili9341.hpp"

/// @cond

/* 2.1.0 April 25 test results
Bitmap FPS
Seconds :: 20 
Count :: 114 
FPS :: 6 
=== SRAM Usage ===
BSS end          : 200035DC
Heap end         : 2002A000
Stack pointer    : 20041FE0
Heap usage       : 158244 bytes
Gap (heap→stack) : 98272 bytes (available)
Estimated SRAM used : 158244 bytes
Estimated SRAM free : 98272 bytes
Text FPS
FPS : 6
Seconds : 85
*/

#ifndef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is not defined. it is required for this example")
#endif
// Section :: Globals
ILI9341_TFT myTFT;

//  Section ::  Function Headers
bool Setup(void);
void Test1(void);
void TestRotate(void);
void TestBitmap(void);
void TestBitmapFPS(void);
void TestTextFPS(void);
void EndTests(void);
void ScreenReset(void);

//  Section ::  MAIN loop
int main(void)
{
	DisLib16::setDebugMode(true); // turn on debug mode
	if (!Setup())
		return -1;
	Test1();
	TestRotate();
	TestBitmap();
	TestBitmapFPS();
	TestTextFPS();
	EndTests();
}
// *** End OF MAIN **

//  Section ::  Function Space
bool Setup(void)
{
	stdio_init_all(); // optional for error messages , Initialize chosen serial port, default 38400 baud
	MILLISEC_DELAY(1000);
	printf("\nTFT: Start\r\n");
	DisLib16::printMemoryUsage();
	//*************** USER OPTION 0 SPI_SPEED + TYPE ***********
	bool bhardwareSPI = true; // true for hardware spi, false for software

	if (bhardwareSPI == true)
	{								   // hw spi
		uint32_t TFT_SCLK_FREQ = 8000; // Spi freq in KiloHertz , 1000 = 1Mhz
		myTFT.SetupSPI(TFT_SCLK_FREQ, spi0);
	}
	else
	{								 // sw spi
		uint16_t SWSPICommDelay = 0; // optional SW SPI GPIO delay in uS
		myTFT.SetupSPI(SWSPICommDelay);
	}
	//*********************************************************
	// ******** USER OPTION 1 GPIO *********
	// NOTE if using Hardware SPI clock and data pins will be tied to 
	// the chosen interface eg Spi0 CLK=18 DIN=19 MISO = 17)
	int8_t SDIN_TFT = 19; 
	int8_t SCLK_TFT = 18; 
	int8_t RST_TFT = 4;
	int8_t DC_TFT = 3;
	int8_t CS_TFT = 2;  
	myTFT.SetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
	//**********************************************************

	// ****** USER OPTION 2 Screen Setup ******
	uint16_t TFT_WIDTH = 240;  // Screen width in pixels
	uint16_t TFT_HEIGHT = 320; // Screen height in pixels
	myTFT.SetupScreenSize(TFT_WIDTH, TFT_HEIGHT);
	// ******************************************

	if (myTFT.setBuffer() != DisLib16::Success)
		return false;					  // set up buffer
	myTFT.setTextCharPixelOrBuffer(true); // set to use pixel mode for text
	DisLib16::setDebugMode(true);
	DisLib16::printMemoryUsage();
	myTFT.ILI9341Initialize(); 
	return true;
}

void EndTests(void)
{
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.clearBuffer(myTFT.C_BLACK);
	myTFT.destroyBuffer();
	myTFT.PowerDown();
	printf("TFT: Tests Over\n");
}

void ScreenReset(void)
{
	myTFT.writeBuffer();
	MILLISEC_DELAY(2500);
	myTFT.clearBuffer(myTFT.C_BLACK);
}

void Test1(void)
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
	myTFT.setCursor(5, 5);
	myTFT.setFont(font_default);
	myTFT.print("Buffer Mode     Test");
	myTFT.writeBuffer();
	MILLISEC_DELAY(5000);
}

void TestRotate(void)
{
	printf("Rotate\r\n");
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.clearBuffer(myTFT.C_BLACK);
	const char *rotationLabels[] = {"Rotate 0", "Rotate 90", "Rotate 180", "Rotate 270"};
	const decltype(myTFT.Degrees_0) rotations[] = {
		myTFT.Degrees_0,
		myTFT.Degrees_90,
		myTFT.Degrees_180,
		myTFT.Degrees_270};
	for (int i = 0; i < 4; ++i)
	{
		myTFT.setRotation(rotations[i]);
		myTFT.setCursor(20, 20);
		myTFT.print(rotationLabels[i]);
		myTFT.setCursor(20, 100);
		myTFT.print("Bottom");
		ScreenReset();
	}
	myTFT.setRotation(myTFT.Degrees_0);
	myTFT.fillScreen(myTFT.C_BLACK);
	ScreenReset();
}

void TestBitmap(void)
{
	printf("Sprite\n");
	// Sprite
	for (int col = 0; col < 2; ++col) // 0: no flip, 1: flip
	{
		bool flip = (col == 1);
		int x = col * 32;

		for (int row = 0; row < 3; ++row)
		{
			int y = row * 32;
			myTFT.drawSpriteData(x, y, sSpriteTest16, 32, 32, myTFT.C_LBLUE, flip);
		}
	}
	ScreenReset();
	// Bitmap 1-bit
	printf("Bitmap 1-bit\n");
	myTFT.drawBitmap(0, 0, 128, 128, myTFT.C_WHITE, myTFT.C_GREEN, sArrowImage);
	ScreenReset();
	// Bitmap 8-bit
	printf("Bitmap 8-bit\n");
	myTFT.drawBitmap8Data(20, 20, sColorTestImage, 96, 96);
	ScreenReset();
	// Bitmap 16-bit
	printf("Bitmap 16-bit\n");
	myTFT.drawBitmap16Data(0, 0, sMotorImage, 128, 128);
	ScreenReset();
}

void TestBitmapFPS(void)
{
	printf("Bitmap FPS\n");
	// Values to count frame rate per second
	long previousMillis = 0;
	long lastFramerate = 0;
	long currentFramerate = 0;

	uint16_t count = 0;
	uint16_t seconds = 0;
	uint16_t fps = 0;

	char teststr1[] = "Test bitmap FPS, Output Results to USB Serial port";
	myTFT.writeCharString(5, 5, teststr1);
	MILLISEC_DELAY(5000);
	myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_RED);

	while (1)
	{
		unsigned long currentMillis = to_ms_since_boot(get_absolute_time());

		if (currentMillis - previousMillis >= 1000) // every second
		{
			fps = currentFramerate - lastFramerate;
			lastFramerate = currentFramerate;
			previousMillis = currentMillis;
			seconds++;
			if (seconds == 20)
				break;
		}
		currentFramerate++;
		count++;
		//  ** Code to test **
		myTFT.drawBitmap16Data(0, 0, sMotorImage, 128, 128);
		myTFT.setCursor(0, 0);
		myTFT.print(fps);
		myTFT.writeBuffer();
		//   **
	} // end of while
	// Report results to usb
	printf("Seconds :: %u \n", seconds);
	printf("Count :: %u \n", count);
	printf("FPS :: %u \n", fps);
	// Print to screen
	myTFT.clearBuffer(myTFT.C_BLACK);
	myTFT.setCursor(0, 0);
	myTFT.print("Seconds : ");
	myTFT.println(seconds);
	myTFT.print("Count : ");
	myTFT.println(count);
	myTFT.print("FPS : ");
	myTFT.print(fps);
	myTFT.writeBuffer();
	MILLISEC_DELAY(5000);
	myTFT.clearBuffer(myTFT.C_BLACK);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void TestTextFPS(void)
{
	printf("Text FPS\n");
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setFont(font_pico);
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
			//myTFT.clearBuffer(myTFT.C_BLACK);
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
		myTFT.setCursor(5, 5);
		myTFT.println("Secs:");
		myTFT.println(seconds);
		// print fps
		myTFT.println("FPS:");
		myTFT.println(fps);
		// print count
		myTFT.println("Count:");
		myTFT.println(count);
		myTFT.println("Testing!");
		myTFT.println(DisLib16::LibraryVersion());
		myTFT.writeBuffer();
		//  ****
	}
}
/// @endcond
