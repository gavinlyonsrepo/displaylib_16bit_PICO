/*!
	@file   main.cpp
	@author Gavin Lyons
	@brief  Example cpp file for st7789 driver.bitmap tests + FPS bitmap test.
	@note   See USER OPTIONS 0-2 in SETUP function, Messages to serial port, baud 38400
	@test
		-# Test 300 Sprite 
		-# Test 301 "clock demo" , icons,
		-# Test 302 bi-color small image
		-# Test 303 bi-color image 128x128
		-# Test 304 16 bit color image from a data array
		-# Test 305 24 bit color image data from a data array
		-# Test 306 8 bit color image data from a data array
		-# Test 603 FPS bitmap results to serial port, 38400 baud
		-# Test 802 Error checking bitmap functions, results to serial port
*/

// Section ::  libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16/st7789.hpp"
#include "displaylib_16/Bitmap_TEST_Data.hpp"
#include <span>
#include <vector> // for error checking test

/// @cond

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000
#define CLOCK_DISPLAY_TIME 20 // seconds

// Section :: Globals
ST7789_TFT myTFT;
// Empty span For unit testing 
std::span<const uint8_t> emptyBitmap{};

//  Section ::  Function Headers
void Setup(void);	// setup + user options
void Test300(void); // sprite
void Test301(void); // "clock demo" , icons,
void Test302(void); // bi-color small image
void Test303(void); // bi-color full screen image 128x128
void Test304(void); // 16 bit color image from a data array
void Test305(void); // 24 bit color image data from a data array
void Test306(void); // 8 bit color image data from a data array
void Test603(void); // FPS test, results to serial port
void Test802(void); // bitmap error checking, results to serial port 
void EndTests(void);

//  Section ::  MAIN

int main(void)
{
	Setup();
	Test300();
	Test301();
	Test302();
	Test303();
	Test304();
	Test305();
	Test306();
	Test802();
	Test603();
	EndTests();
}
// *** End OF MAIN **

//  Section ::  Function Space

/*!
	@brief  setup + user options
*/
void Setup(void)
{
	stdio_init_all(); // Initialize chosen serial port
	MILLISEC_DELAY(TEST_DELAY1);
	printf("TFT :: Start\r\n");

	//*************** USER OPTION 0 SPI_SPEED + TYPE ***********
	bool bhardwareSPI = true; // true for hardware spi,

	if (bhardwareSPI == true)
	{								   // hw spi
		uint32_t TFT_SCLK_FREQ = 8000; // Spi freq in KiloHertz , 1000 = 1Mhz , max 62500
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
	// the chosen interface (eg Spi0 CLK=18 DIN=19)
	int8_t SDIN_TFT = 19;
	int8_t SCLK_TFT = 18;
	int8_t DC_TFT = 3;
	int8_t CS_TFT = 2;
	int8_t RST_TFT = 17;
	myTFT.setupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
	//**********************************************************

	// ****** USER OPTION 2 Screen Setup ******
	uint16_t OFFSET_COL = 0;	   // 2, These offsets can be adjusted for any issues->
	uint16_t OFFSET_ROW = 0;	   // 3, with manufacture tolerance/defects
	uint16_t TFT_WIDTH = 240;  // Screen width in pixels
	uint16_t TFT_HEIGHT = 320; // Screen height in pixels
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW, TFT_WIDTH, TFT_HEIGHT);
	// ******************************************

	myTFT.TFTST7789Initialize(); 
}

/*!
	@brief  "sprite demo" 32 x 32 with LBLUE background
*/
void Test300(void)
{
	// Test 300-A test 16-bit color Sprite 
	// Draw as sprite, without background , 32 X 32 .bakcground color = ST7375_LBLUE
	// Green bakcground screen
	myTFT.fillScreen(myTFT.C_GREEN);
	MILLISEC_DELAY(TEST_DELAY2);
	myTFT.drawSpriteData(55, 55, sSpriteTest16, 32, 32, myTFT.C_LBLUE, false);
	myTFT.drawSpriteData(100, 40, sSpriteTest16, 32, 32, myTFT.C_LBLUE, true);
	MILLISEC_DELAY(TEST_DELAY5);

	// Test 300-B test 16-bit color Sprite 
	// Draw as sprite, without background , 32 X 32 .bakcground color = ST7375_LBLUE
	// Bitmap background screen
	myTFT.drawBitmap24Data(50, 50, sPosterImage, 80, 48);
	MILLISEC_DELAY(TEST_DELAY2);

    myTFT.drawSpriteData(80, 50, sSpriteTest16, 32, 32, myTFT.C_LBLUE, false);
	myTFT.drawSpriteData(60, 80, sSpriteTest16, 32, 32, myTFT.C_LBLUE, false);
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}


/*! @brief  icons,*/
void Test301(void)
{
	myTFT.fillScreen(myTFT.C_BLACK);
	char teststr1[] = "Test 301";
	myTFT.drawText(90, 90, teststr1, myTFT.C_WHITE, myTFT.C_BLACK, 1);
	// TOP icons box
	myTFT.drawIcon(40, 40, 16, myTFT.C_BLACK, myTFT.C_WHITE, SignalIcon);
	myTFT.drawIcon(60, 40, 16, myTFT.C_BLACK, myTFT.C_WHITE, MsgIcon);
	myTFT.drawIcon(80, 40, 8, myTFT.C_BLACK, myTFT.C_WHITE, AlarmIcon);
	myTFT.drawIcon(110, 40, 16, myTFT.C_BLACK, myTFT.C_WHITE, BatIcon);
	// second box
	myTFT.drawIcon(35, 70, 12, myTFT.C_GREEN, myTFT.C_BLACK, powerIcon);
	myTFT.drawIcon(55, 70, 12, myTFT.C_RED, myTFT.C_YELLOW, speedIcon);
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
} // end of Test 301

/*!
	@brief  Test 302 bi-color small image 20x24
*/
void Test302(void)
{
	myTFT.fillScreen(myTFT.C_BLACK);
	char teststr1[] = "Test 302";
	myTFT.drawText(50, 50, teststr1, myTFT.C_WHITE, myTFT.C_BLACK, 1);
	myTFT.drawBitmap(80, 60, 40, 16, myTFT.C_CYAN, myTFT.C_BLACK, sSunTextImage);
	myTFT.drawBitmap(20, 100, 40, 16, myTFT.C_RED, myTFT.C_BLACK, sSunTextImage);
	myTFT.drawBitmap(30, 140, 40, 16, myTFT.C_YELLOW, myTFT.C_RED, sSunTextImage);
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Test303 bi-color full screen image 128x128
*/
void Test303(void)
{
	char teststr1[] = "Test 303";
	myTFT.drawText(50, 50, teststr1, myTFT.C_WHITE, myTFT.C_BLACK, 1);
	MILLISEC_DELAY(TEST_DELAY2);
	myTFT.drawBitmap(50, 65, 128, 128, myTFT.C_WHITE, myTFT.C_GREEN, sArrowImage);
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Test304 16 bit color image from a data array
*/
void Test304(void)
{
	char teststr1[] = "Test 304";
	myTFT.drawText(50, 50, teststr1, myTFT.C_WHITE, myTFT.C_BLACK, 1);
	MILLISEC_DELAY(TEST_DELAY2);
	myTFT.drawBitmap16Data(65, 65, sMotorImage, 128, 128);
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Test305 24 bit color image data from a data array
*/
void Test305(void)
{
	char teststr1[] = "Test 305";
	myTFT.drawText(50, 50, teststr1, myTFT.C_WHITE, myTFT.C_BLACK, 1);
	MILLISEC_DELAY(TEST_DELAY2);
	myTFT.drawBitmap24Data(65, 65, sPosterImage, 80, 48);
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Test306 8 bit color image data from a data array
*/
void Test306(void)
{
	char teststr1[] = "Test 306";
	myTFT.drawText(50, 50, teststr1, myTFT.C_WHITE, myTFT.C_BLACK, 1);
	MILLISEC_DELAY(TEST_DELAY2);
	myTFT.drawBitmap8Data(65, 65, sColorTestImage, 96,96);
	MILLISEC_DELAY(TEST_DELAY5);
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Test603 frame rate per second FPS ,results to serial port
*/
void Test603(void)
{
	// Values to count frame rate per second
	long previousMillis = 0;
	long lastFramerate = 0;
	long currentFramerate = 0;

	uint16_t count = 0;
	uint16_t seconds = 0;
	uint16_t fps = 0;
	char teststr1[] = "Test 601 FPS, Output Results to USB port";
	myTFT.drawText(20, 50, teststr1, myTFT.C_WHITE, myTFT.C_BLACK, 2);
	myTFT.setTextSize(2);
	MILLISEC_DELAY(TEST_DELAY5);
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
		myTFT.drawBitmap16Data(60, 120 ,sMotorImage, 128, 128);		//myTFT.fillScreen(randomColor );
		myTFT.setCursor(60, 90); 
		myTFT.print(fps);
		//   **
	} // end of while
	
	// Report results to usb
	printf("Seconds :: %u \n", seconds);
	printf("Count :: %u \n", count);
	printf("FPS :: %u \n", fps);
	// Print to screen
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setCursor(0, 50); 
	myTFT.setTextSize(2);
	myTFT.print("Seconds : ");
	myTFT.println(seconds);
	myTFT.print("Count : ");
	myTFT.println(count);
	myTFT.print("FPS : ");
	myTFT.print(fps);
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Bitmap error checking test
*/
void Test802(void)
{
	// === Setup tests ===
	// Define the expected return values
	std::vector<uint8_t> expectedErrors = 
	{
		DisLib16::Success, 
		DisLib16::BitmapScreenBounds, DisLib16::BitmapScreenBounds, DisLib16::BitmapDataEmpty, DisLib16::IconScreenWidth, //icon
		DisLib16::BitmapScreenBounds, DisLib16::BitmapScreenBounds, DisLib16::BitmapDataEmpty, DisLib16::BitmapSize, //sprite
		DisLib16::BitmapScreenBounds, DisLib16::BitmapScreenBounds, DisLib16::BitmapDataEmpty, DisLib16::BitmapHorizontalSize, //1-bit bitmap
		DisLib16::BitmapScreenBounds, DisLib16::BitmapScreenBounds, DisLib16::BitmapDataEmpty, DisLib16::BitmapSize,//8-bit bitmap
		DisLib16::BitmapScreenBounds, DisLib16::BitmapScreenBounds, DisLib16::BitmapDataEmpty, DisLib16::BitmapSize,//16-bit bitmap
		DisLib16::BitmapScreenBounds, DisLib16::BitmapScreenBounds, DisLib16::BitmapDataEmpty, DisLib16::BitmapSize//24-bit bitmap
	};
	// Vector to store return values
	std::vector<uint8_t> returnValues; 
	
	// test variables
	char testString5[] = "Error Check Test 802, results to usb";
	bool errorFlag = false;

	// === Tests===
	printf("=== START Error checking. Expecting errors ===\r\n");
	// Perform function calls and store return values
	myTFT.FontNum(myTFT.Font_Default);
	returnValues.push_back(myTFT.drawText(5, 55, testString5, myTFT.C_RED, myTFT.C_BLACK, 2)); 
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
	//drawIcon
	returnValues.push_back(myTFT.drawIcon(400, 40, 16, myTFT.C_BLACK, myTFT.C_WHITE, SignalIcon));
	returnValues.push_back(myTFT.drawIcon(400, 400, 16, myTFT.C_BLACK, myTFT.C_WHITE, SignalIcon));
	returnValues.push_back(myTFT.drawIcon(40, 40, 16, myTFT.C_BLACK, myTFT.C_WHITE, emptyBitmap));
	returnValues.push_back(myTFT.drawIcon(40, 40, 330, myTFT.C_BLACK, myTFT.C_WHITE, SignalIcon));
	//drawSpriteData
	returnValues.push_back(myTFT.drawSpriteData(400, 50, sSpriteTest16, 32, 32, myTFT.C_LBLUE, false));
	returnValues.push_back(myTFT.drawSpriteData(40, 400, sSpriteTest16, 32, 32, myTFT.C_LBLUE, false));
	returnValues.push_back(myTFT.drawSpriteData(40, 40, emptyBitmap, 32, 32, myTFT.C_LBLUE, false));
	returnValues.push_back(myTFT.drawSpriteData(40, 40, sSpriteTest16, 48, 32, myTFT.C_LBLUE, false));
	//drawBitmap
	returnValues.push_back(myTFT.drawBitmap(400, 65, 128, 128, myTFT.C_WHITE, myTFT.C_GREEN, sArrowImage));
	returnValues.push_back(myTFT.drawBitmap(50, 400, 128, 128, myTFT.C_WHITE, myTFT.C_GREEN, sArrowImage));
	returnValues.push_back(myTFT.drawBitmap(50, 65, 128, 128, myTFT.C_WHITE, myTFT.C_GREEN, emptyBitmap));
	returnValues.push_back(myTFT.drawBitmap(20, 20, 70, 128, myTFT.C_WHITE, myTFT.C_GREEN, sArrowImage));
	//drawBitmap8Data
	returnValues.push_back(myTFT.drawBitmap8Data(400, 50, sColorTestImage, 96, 96));
	returnValues.push_back(myTFT.drawBitmap8Data(40, 400, sColorTestImage, 96, 96));
	returnValues.push_back(myTFT.drawBitmap8Data(40, 40, emptyBitmap, 32, 32));
	returnValues.push_back(myTFT.drawBitmap8Data(40, 40, sColorTestImage, 128, 96));
	//drawBitmap16Data
	returnValues.push_back(myTFT.drawBitmap16Data(400, 50, sSpriteTest16, 32, 32));
	returnValues.push_back(myTFT.drawBitmap16Data(40, 400, sSpriteTest16, 32, 32));
	returnValues.push_back(myTFT.drawBitmap16Data(40, 40,  emptyBitmap, 32, 32));
	returnValues.push_back(myTFT.drawBitmap16Data(40, 40,  sSpriteTest16, 50, 32));
	//drawBitmap24Data
	returnValues.push_back(myTFT.drawBitmap24Data(400, 50, sPosterImage, 80, 48));
	returnValues.push_back(myTFT.drawBitmap24Data(40, 400, sPosterImage, 80, 48));
	returnValues.push_back(myTFT.drawBitmap24Data(40, 40,  emptyBitmap, 80, 48));
	returnValues.push_back(myTFT.drawBitmap24Data(40, 40,  sPosterImage, 80, 60));
	
	
	//== SUMMARY SECTION===
	printf("\nError Checking Summary.\n");
	// Check return values against expected errors
	for (size_t i = 0; i < returnValues.size(); ++i) {
		if (i >= expectedErrors.size() || returnValues[i] != expectedErrors[i]) {
			errorFlag = true;
			printf("Unexpected error code: %d at test case %zu (expected: %d)\n", 
				returnValues[i], i + 1, (i < expectedErrors.size() ? expectedErrors[i] : -1));
		}
	}

	// Print all expectedErrors for summary
	for (uint8_t value : expectedErrors ) 
	{
		printf("%d ", value);
	}
	printf("\n");
	// Print all returnValues for summary
	for (uint8_t value : returnValues) 
	{
		printf("%d ", value);
	}
	if (errorFlag == true ){
		printf("\nError Checking has FAILED.\n");
	}else{
		printf("\nError Checking has PASSED.\n");
	}
	printf("\n=== STOP Error checking. ===\r\n");
}

/*!
	@brief  Stop testing and shutdown the TFT
*/
void EndTests(void)
{
	char teststr1[] = "Tests over";
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.drawText(50, 50, teststr1, myTFT.C_GREEN, myTFT.C_BLACK, 2);
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTPowerDown();
	printf("TFT :: Tests Over\r\n");
}
// *************** EOF ****************

/// @endcond
