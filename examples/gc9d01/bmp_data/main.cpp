/*!
	@file   main.cpp
	@author Gavin Lyons
	@brief  Example cpp file for gc9d01 bitmap tests + FPS bitmap test.
	@details  	See USER OPTIONS in SETUP function
				This example is for 160x160 Dual gate.
				This example requires dislib16_ADVANCED_SCREEN_BUFFER_ENABLE 
				to be on.(non-default)
				see displaylib_16_graphics.hpp USER OPTION 2
				Requires usable heap memory of (160x160x2) = 51,200 bytes for buffer
				dislib16_ADVANCED_GRAPHICS_ENABLE mode must also be on(default).
	@test
		-# Test 300 Sprite 
		-# Test 301 icons
		-# Test 302 bi-color small image
		-# Test 303 bi-color full screen image 128x128
		-# Test 304 16 bit color image from a data array
		-# Test 305 16 bit color image data from a data array
		-# Test 306 8 bit color image data from a data array
		-# Test 601 FPS bitmap results to serial port
		-# Test 802 Error checking bitmap functions, results to serial port
*/

// Section ::  libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16/gc9d01.hpp"
#include "displaylib_16/Bitmap_TEST_Data.hpp" // bitmap data
#include <vector> // for error checking test

/// @cond

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000

#ifndef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is not defined. it is required for this example")
#endif
#ifndef dislib16_ADVANCED_GRAPHICS_ENABLE
#pragma message("gll: dislib16_ADVANCED_GRAPHICS_ENABLE is not defined. it is required for this demo")
#endif

// Section :: Globals
GC9D01_TFT myTFT;
// Empty span For unit testing 
std::span<const uint8_t> emptyBitmap{};

//  Section ::  Function Headers

bool Setup(void);	// setup + user options
void Test300(void); // sprite
void Test301(void); // bi-color small image
void Test302(void); // bi-color small image
void Test303(void); // bi-color full screen image 128x128
void Test304(void); // 16 bit color image from a data array
void Test305(void); // 16 bit color image data from a data array
void Test306(void); // 8 bit color image data from a data array
void Test601(void); // FPS test optional , results to serial port
void Test802(void); // bitmap error checking, results to serial port 
void EndTests(void);
void DisplayReset(void); // helper function to reset display between tests

//  Section ::  MAIN

int main(void)
{
	DisLib16::setDebugMode(true); // turn on debug mode optional
	if (!Setup())
		return -1; // setup failed
	Test300();
	Test301();
	Test302();
	Test303();
	Test304();
	Test305();
	Test306();
	Test802();
	Test601();
	EndTests();
}
// *** End OF MAIN **

//  Section ::  Function Space

/*!
	@brief  setup + user options
*/
bool Setup(void)
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
	uint16_t TFT_WIDTH = 160;// Screen width in pixels
	uint16_t TFT_HEIGHT = 160; // Screen height in pixels
	uint16_t OFFSET_X   =   0; // Screen X offset in pixels
	uint16_t OFFSET_Y   =   0; // Screen Y offset in pixels
	// Display type, 4 choice's, see readme.
	GC9D01_TFT::Resolution_e DisplayType = GC9D01_TFT::Resolution_e::RGB160x160_DualGate;
	// Pixel Draw mode type, 4 choices , see readme.  
	GC9D01_TFT::PixelFixMode_e FixType = GC9D01_TFT::PixelFixMode_e::Off;
	myTFT.TFTInitScreenSize(TFT_WIDTH, TFT_HEIGHT, DisplayType, FixType, OFFSET_X, OFFSET_Y);
// ***
	myTFT.TFTGC9D01Initialize(); 
	if (myTFT.setBuffer() != DisLib16::Success)
		return false;					  // set up buffer
	myTFT.setTextCharPixelOrBuffer(true); // set to use pixel mode for text
	// Print memory usage, optional
	DisLib16::printMemoryUsage();
	return true;
}

/*!
	@brief  "sprite demo" 32 x 32 with LBLUE background
*/
void Test300(void)
{
	// Test 300-A test 16-bit color Sprite 
	// Draw as sprite, without background , 32 X 32 .background color = ST7375_LBLUE
	// Green background screen
	myTFT.clearBuffer(myTFT.C_GREEN);
	myTFT.writeBuffer();
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.drawSpriteData(55, 55, sSpriteTest16, 32, 32, myTFT.C_LBLUE, false);
	myTFT.drawSpriteData(90, 90, sSpriteTest16, 32, 32, myTFT.C_LBLUE, true);
	DisplayReset();

	// Test 300-B test 16-bit color Sprite 
	// Draw as sprite, without background , 32 X 32 .background color = ST7375_LBLUE
	// Bitmap background screen
	myTFT.drawBitmap16Data(60, 60, sPosterImage, 80, 48);
	myTFT.writeBuffer();
	MILLISEC_DELAY(TEST_DELAY5);

	myTFT.drawSpriteData(65, 10, sSpriteTest16, 32, 32, myTFT.C_LBLUE, false);
	myTFT.drawSpriteData(70, 60, sSpriteTest16, 32, 32, myTFT.C_LBLUE, false);
	DisplayReset();
}

/*!
	@brief  small 1-bit bitmap's
*/
void Test301(void)
{
	myTFT.drawBitmap(42, 40, 16, 8, myTFT.C_BLACK, myTFT.C_WHITE, SignalIcon);
	myTFT.drawBitmap(62, 40, 16, 8, myTFT.C_BLACK, myTFT.C_WHITE, MsgIcon);
	myTFT.drawBitmap(80, 40, 8, 8, myTFT.C_BLACK, myTFT.C_WHITE, AlarmIcon);
	myTFT.drawBitmap(100, 40, 16, 8, myTFT.C_BLACK, myTFT.C_WHITE, BatIcon);
	myTFT.drawBitmap(25, 60, 16, 8, myTFT.C_GREEN, myTFT.C_BLACK, powerIcon);
	myTFT.drawBitmap(45, 60, 16, 8, myTFT.C_RED, myTFT.C_YELLOW, speedIcon);
	myTFT.setCursor(60,100);
	myTFT.print("Test 301");
	DisplayReset();
} // end of Test 301

/*!
	@brief  Test 302 bi-color small image 20x24
*/
void Test302(void)
{
	char teststr1[] = "Test 302";
	myTFT.writeCharString(45, 45, teststr1);
	myTFT.drawBitmap(80, 60, 40, 16, myTFT.C_CYAN, myTFT.C_BLACK, sSunTextImage);
	myTFT.drawBitmap(20, 80, 40, 16, myTFT.C_RED, myTFT.C_BLACK, sSunTextImage);
	myTFT.drawBitmap(30, 100, 40, 16, myTFT.C_YELLOW, myTFT.C_RED, sSunTextImage);
	DisplayReset();
}

/*!
	@brief  Test303 bi-color full screen image 128x128
*/
void Test303(void)
{
	char teststr1[] = "Test 303";
	myTFT.writeCharString(45,10, teststr1);
	myTFT.drawBitmap(25, 25, 128, 128, myTFT.C_WHITE, myTFT.C_GREEN, sArrowImage);
	DisplayReset();
}

/*!
	@brief  Test304 16 bit color image from a data array
*/
void Test304(void)
{
	char teststr1[] = "Test 304";
	myTFT.writeCharString(45, 10, teststr1);
	myTFT.drawBitmap16Data(25, 25, sMotorImage, 128, 128);
	DisplayReset();
}

/*!
	@brief  Test305 16 bit color image data from a data array
*/
void Test305(void)
{
	char teststr1[] = "Test 305";
	myTFT.fillScreen(myTFT.C_WHITE);
	myTFT.writeCharString(45, 10, teststr1);
	myTFT.drawBitmap16Data(50, 50, sPosterImage, 80, 48);
	DisplayReset();
}

/*!
	@brief  Test306 8 bit color image data from a data array
*/
void Test306(void)
{
	char teststr1[] = "Test 306";
	myTFT.writeCharString(45, 10, teststr1);
	myTFT.drawBitmap8Data(50, 50, sColorTestImage, 96,96);
	DisplayReset();
}

/*!
	@brief  Test601 frame rate per second FPS ,results to serial port
*/
void Test601(void)
{
	printf("Frame rate Test 601, results:\r\n");
	// Values to count frame rate per second
	long previousMillis = 0;
	long lastFramerate = 0;
	long currentFramerate = 0;

	uint16_t count = 0;
	uint16_t seconds = 0;
	uint16_t fps = 0;

	char teststr1[] = "Test 601 FPS, Output Results to USB Serial port";
	myTFT.writeCharString(25, 75, teststr1);
	myTFT.writeBuffer();
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
		myTFT.drawBitmap16Data(30, 30, sMotorImage, 128, 128);
		myTFT.setCursor(45, 10); 
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
	myTFT.setCursor(55, 55); 
	myTFT.print("Seconds : ");
	myTFT.print(seconds);
	myTFT.setCursor(55, 75);
	myTFT.print("Count : ");
	myTFT.print(count);
	myTFT.setCursor(55, 95);
	myTFT.print("FPS : ");
	myTFT.print(fps);
	DisplayReset();
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
		DisLib16::BitmapScreenBounds, DisLib16::BitmapScreenBounds, DisLib16::BitmapDataEmpty, DisLib16::BitmapSize, //sprite
		DisLib16::BitmapScreenBounds, DisLib16::BitmapScreenBounds, DisLib16::BitmapDataEmpty, DisLib16::BitmapHorizontalSize, //1-bit bitmap
		DisLib16::BitmapScreenBounds, DisLib16::BitmapScreenBounds, DisLib16::BitmapDataEmpty, DisLib16::BitmapSize,//8-bit bitmap
		DisLib16::BitmapScreenBounds, DisLib16::BitmapScreenBounds, DisLib16::BitmapDataEmpty, DisLib16::BitmapSize,//16-bit bitmap
		DisLib16::BitmapScreenBounds, DisLib16::BitmapScreenBounds, DisLib16::BitmapDataEmpty, DisLib16::BitmapSize//16-bit bitmap
	};
	// Vector to store return values
	std::vector<uint8_t> returnValues; 
	// test variables
	char testString5[] = "Error Check Test 802, results to usb";
	bool errorFlag = false;

	// === Tests===
	printf("=== START Error checking. Expecting errors ===\r\n");
	// Perform function calls and store return values

	// Print message + sanity check for success
	myTFT.setFont(font_default);
	returnValues.push_back(myTFT.writeCharString(55, 75, testString5)); 
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
	//drawSpriteData
	returnValues.push_back(myTFT.drawSpriteData(250, 50, sSpriteTest16, 32, 32, myTFT.C_LBLUE, false));
	returnValues.push_back(myTFT.drawSpriteData(40, 250, sSpriteTest16, 32, 32, myTFT.C_LBLUE, false));
	returnValues.push_back(myTFT.drawSpriteData(40, 180, emptyBitmap, 32, 32, myTFT.C_LBLUE, false));
	returnValues.push_back(myTFT.drawSpriteData(40, 40, sSpriteTest16, 48, 32, myTFT.C_LBLUE, false));
	//drawBitmap
	returnValues.push_back(myTFT.drawBitmap(260, 65, 128, 128, myTFT.C_WHITE, myTFT.C_GREEN, sArrowImage));
	returnValues.push_back(myTFT.drawBitmap(50, 260, 128, 128, myTFT.C_WHITE, myTFT.C_GREEN, sArrowImage));
	returnValues.push_back(myTFT.drawBitmap(50, 65, 128, 128, myTFT.C_WHITE, myTFT.C_GREEN, emptyBitmap));
	returnValues.push_back(myTFT.drawBitmap(20, 20, 70, 128, myTFT.C_WHITE, myTFT.C_GREEN, sArrowImage));
	//drawBitmap8Data
	returnValues.push_back(myTFT.drawBitmap8Data(400, 50, sColorTestImage, 96, 96));
	returnValues.push_back(myTFT.drawBitmap8Data(40, 400, sColorTestImage, 96, 96));
	returnValues.push_back(myTFT.drawBitmap8Data(40, 40, emptyBitmap, 32, 32));
	returnValues.push_back(myTFT.drawBitmap8Data(40, 40, sColorTestImage, 128, 96));
	//drawBitmap16Data
	returnValues.push_back(myTFT.drawBitmap16Data(260, 50, sSpriteTest16, 32, 32));
	returnValues.push_back(myTFT.drawBitmap16Data(40, 260, sSpriteTest16, 32, 32));
	returnValues.push_back(myTFT.drawBitmap16Data(40, 180, emptyBitmap, 32, 32));
	returnValues.push_back(myTFT.drawBitmap16Data(40, 40,  sSpriteTest16, 50, 32));
	//drawBitmap16Data
	returnValues.push_back(myTFT.drawBitmap16Data(260, 50, sSpriteTest16, 32, 32));
	returnValues.push_back(myTFT.drawBitmap16Data(40, 260, sSpriteTest16, 32, 32));
	returnValues.push_back(myTFT.drawBitmap16Data(40, 180, emptyBitmap, 32, 32));
	returnValues.push_back(myTFT.drawBitmap16Data(40, 40,  sPosterImage, 80, 60));
	
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
	myTFT.writeCharString(10, 40, teststr1);
	DisplayReset();
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.clearBuffer(myTFT.C_BLACK);
	myTFT.destroyBuffer();
	myTFT.TFTPowerDown();
	printf("TFT :: Tests Over\r\n");
}

void DisplayReset(void)
{
	myTFT.writeBuffer();
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.clearBuffer(myTFT.C_BLACK);
}
// *************** EOF ****************

/// @endcond
