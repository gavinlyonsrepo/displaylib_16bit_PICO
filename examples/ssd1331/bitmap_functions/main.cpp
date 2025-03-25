/*! 
	@file   main.cpp
	@brief  Testing bitmap, functions hardware SPI test, ssd1331 OLED
	@author Gavin Lyons.
	@note   See USER OPTIONS 0-2 in SETUP function and in myOLED constructor
	@test 
		-# Test 300 Sprites, icons, small bitmap
		-# Test 303 24 bit color image bitmap from data array
		-# Test 500 RGB color OK?
		-# Test 501 change modes test -> Invert, display on/off and Sleep.
		-# Test 502 Rotate
		-# Test 503 windows command tests
		-# Test 504 Scroll
*/

// Section ::  libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16/ssd1331.hpp"
#include "displaylib_16/Bitmap_TEST_Data.hpp" // test data for bitmaps

/// @cond

// Section :: setup device
// Set contrast values at startup, range 0-0xFF
SSD1331_OLED::Constrast_values_t userContrast = { 0x7A, 0x7A, 0x7A };        //normal mode
SSD1331_OLED::Dim_Constrast_values_t userDimContrast = { 0x1E, 0x1E, 0x1E }; // dim mode
SSD1331_OLED myOLED(SSD1331_OLED::COLORORDER_RGB, userContrast, userDimContrast);

//  Test timing related defines
#define TEST_DELAY5 5000
#define TEST_DELAY2 2000
#define TEST_DELAY  1000

//  Section ::  Function Headers 
void SetupHWSPI(void); // setup + user options for hardware SPI
void Test300(void);
void Test303(void); // 24 color bitmap
void Test500(void);
void Test501(void);
void Test502(void);
void Test503(void);
void Test504(void); 
void colorBlock(void);
void EndTests(void);

//  *** Main ***
int main() 
{
	SetupHWSPI();
	Test300();
	Test303();
	Test500();
	Test501();
	Test502();
	Test503();
	Test504();
	EndTests();
	return 0;
}
// *** End of Main **


//  Section ::  Function Space 
// Hardware SPI setup
void SetupHWSPI(void)
{
	stdio_init_all(); // optional for error messages , Initialize chosen serial port, default 38400 baud
	MILLISEC_DELAY(1000);
	printf("Start\r\n");
	
	//*************** USER OPTION 0 SPI_SPEED + TYPE ***********
	bool bhardwareSPI = true; // true for hardware spi, false for software
	
	if (bhardwareSPI == true) { // hw spi
		uint32_t _SCLK_FREQ =  1000 ; // Spi freq in KiloHertz , 1000 = 1Mhz
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
	myOLED.InitScreenSize(OLED_WIDTH, OLED_HEIGHT);
	// ******************************************

	myOLED.SSD1331Initialize();
	busy_wait_ms(100);
}

void Test300(void){
	myOLED.fillScreen(myOLED.C_BLACK);
	myOLED.drawIcon(2, 2, 16, myOLED.C_BLACK, myOLED.C_WHITE, SignalIcon);
	myOLED.drawIcon(20, 2, 16, myOLED.C_BLACK, myOLED.C_WHITE, MsgIcon);
	myOLED.drawIcon(40, 2, 8, myOLED.C_BLACK, myOLED.C_WHITE, AlarmIcon);
	myOLED.drawIcon(80, 2, 16, myOLED.C_BLACK, myOLED.C_WHITE, BatIcon);
	myOLED.drawIcon(5, 20, 12, myOLED.C_GREEN, myOLED.C_BLACK, powerIcon);
	myOLED.drawIcon(20, 20, 12, myOLED.C_RED, myOLED.C_YELLOW, speedIcon);
	MILLISEC_DELAY(5000);

	myOLED.drawSpriteData(5, 5, sSpriteTest16, 32, 32, myOLED.C_LBLUE, false);
	myOLED.drawSpriteData(35, 35, sSpriteTest16, 32, 32, myOLED.C_LBLUE, true);
	MILLISEC_DELAY(5000);
	myOLED.fillScreen(myOLED.C_BLACK);

	myOLED.drawBitmap(0, 0, 40, 16, myOLED.C_CYAN, myOLED.C_BLACK, sSunTextImage);
	myOLED.drawBitmap(20, 30, 40, 16, myOLED.C_RED, myOLED.C_BLACK, sSunTextImage);
	myOLED.drawBitmap(30, 48, 40, 16, myOLED.C_YELLOW, myOLED.C_RED, sSunTextImage);
	MILLISEC_DELAY(5000);
	myOLED.fillScreen(myOLED.C_BLACK);

}

void Test303(void){
	myOLED.drawBitmap24Data(0, 0, sPosterImage, 80, 48);
	MILLISEC_DELAY(5000);
	myOLED.fillScreen(myOLED.C_BLACK);
}

void Test500(void)
{
	printf("Test 500: Color Test:: Red,green,blue,yellow,white, black background\n");
	printf("If wrong colour check color order setting in constructor\n");
	myOLED.fillScreen(myOLED.C_BLACK);
	myOLED.setTextColor(myOLED.C_GREEN,myOLED.C_BLACK);
	colorBlock();
	busy_wait_ms(TEST_DELAY5);

}

void colorBlock(void)
{
	myOLED.fillRect(0, 0, 60, 8, myOLED.C_RED);
	myOLED.fillRect(0, 8, 60, 8, myOLED.C_GREEN);
	myOLED.fillRect(0, 16, 60, 8, myOLED.C_BLUE);
	myOLED.fillRect(0, 24, 60, 8, myOLED.C_YELLOW);
	myOLED.fillRect(0, 32, 60, 8, myOLED.C_WHITE);
}

void Test501()
{
	printf("Test 501: Mode Tests\n");
	// Invert on and off
	myOLED.changeDisplayMode(myOLED.DISPLAY_INVERSE);
	printf("Test 501-1: Invert on \n");
	busy_wait_ms(TEST_DELAY5);
	myOLED.changeDisplayMode(myOLED.DISPLAY_NORMAL);
	printf("Test 501-2: Invert off \n");
	busy_wait_ms(TEST_DELAY5);

	// ALL pixels on and off
	myOLED.changeDisplayMode(myOLED.DISPLAY_ALL_OFF);
	printf("Test 501-3: All pixels off \n");
	busy_wait_ms(TEST_DELAY5);
	myOLED.changeDisplayMode(myOLED.DISPLAY_ALL_ON);
	printf("Test 501-4: All pixels on  \n");
	busy_wait_ms(TEST_DELAY5);
	myOLED.changeDisplayMode(myOLED.DISPLAY_NORMAL);
	busy_wait_ms(TEST_DELAY2);

	// sleep mode
	myOLED.sleepDisplay();
	printf("Test 501-5: Display off, sleep mode\n");
	busy_wait_ms(TEST_DELAY5);
	myOLED.enableDisplay();
	printf("Test 501-6: Turn Display back on\n");
	busy_wait_ms(TEST_DELAY5);

	// dim mode
	myOLED.DimDisplay();
	printf("Test 501-7: Display dim mode on\n");
	busy_wait_ms(TEST_DELAY5);
	myOLED.enableDisplay();
	printf("Test 501-8: Display dim mode off\n");
	busy_wait_ms(TEST_DELAY2);
}


void Test502(void)
{
	printf("Test 502: Window command Tests\n");
	printf("Test 502-1 line draw command OLED command (not using graphics class)\n");
	myOLED.DrawLineCmd(0, 0, 50, 50, myOLED.C_GREEN);
	busy_wait_ms(TEST_DELAY5);
	printf("Test 502-2 draw rect OLED command (not using graphics class)\n");
	myOLED.DrawRectCmd(0, 0, 80, 40, myOLED.C_YELLOW, false);
	busy_wait_ms(TEST_DELAY5);
	printf("Test 502-3 fill rect OLED command (not using graphics class)\n");
	myOLED.DrawRectCmd(0, 0, 95, 63, myOLED.C_CYAN, true);
	busy_wait_ms(TEST_DELAY5);
	printf("Test 502-4 Window clear command\n");
	myOLED.ClearWindowCmd(0, 0, 40, 40);
	busy_wait_ms(TEST_DELAY5);

	printf("Test 502-5 Window copy command\n");
	myOLED.fillScreen(myOLED.C_BLACK);
	myOLED.fillRect(0, 0, 20, 20, myOLED.C_GREEN);
	busy_wait_ms(TEST_DELAY);
	myOLED.CopyWindowCmd(0,0, 20, 20, 76,0);
	busy_wait_ms(TEST_DELAY5);

	printf("Test 502-6 Window Dim command\n");
	myOLED.fillScreen(myOLED.C_GREEN);
	busy_wait_ms(TEST_DELAY);
	myOLED.DimWindowCmd(0, 0, 40, 40);
	busy_wait_ms(TEST_DELAY5);
	
}

void Test503()
{
	printf("Test 503: Rotate\n");
	myOLED.setFont(font_default);
	myOLED.setTextColor(myOLED.C_RED,myOLED.C_BLACK);
	char teststr0[] = "Rot 0";
	char teststr1[] = "Rot 90";
	char teststr2[] = "Rot 180";
	char teststr3[] = "R 270";

	myOLED.fillScreen(myOLED.C_BLACK);
	myOLED.setRotation(myOLED.Degrees_0);
	myOLED.writeCharString(0, 0, teststr0);
	myOLED.writeCharString(0, 50, teststr0);
	busy_wait_ms(TEST_DELAY2);

	myOLED.fillScreen(myOLED.C_BLACK);
	myOLED.setRotation(myOLED.Degrees_90);
	myOLED.writeCharString(0, 0, teststr1);
	myOLED.writeCharString(0, 50, teststr1);
	busy_wait_ms(TEST_DELAY2);

	myOLED.fillScreen(myOLED.C_BLACK);
	myOLED.setRotation(myOLED.Degrees_180);
	myOLED.writeCharString(0, 0, teststr2);
	myOLED.writeCharString(0, 50, teststr2);
	busy_wait_ms(TEST_DELAY2);

	myOLED.fillScreen(myOLED.C_BLACK);
	myOLED.setRotation(myOLED.Degrees_270);
	myOLED.writeCharString(0, 0, teststr3);
	myOLED.writeCharString(0, 50, teststr3);
	busy_wait_ms(TEST_DELAY2);

	myOLED.setRotation(myOLED.Degrees_0);
	myOLED.fillScreen(myOLED.C_BLACK);
}


void Test504(void)
{
	printf("Test 504: Scroll\r\n");
	 // test horizontal scroll slow 
	printf("Test 504-1: Scroll test horizontal\r\n");
	colorBlock();
	uint8_t  horOffset = 95;
	uint8_t startRowAddr = 0; 
	uint8_t numOfRows = 62;
	uint8_t verOffset = 0; 
	myOLED.ScrollSetup(horOffset, startRowAddr,  numOfRows, verOffset , myOLED.SCROLL_FRAME_6);
	myOLED.Scroll(myOLED.SCROLL_ACTIVATE);
	busy_wait_ms(10000);
	myOLED.Scroll(myOLED.SCROLL_DEACTIVE);
	myOLED.fillScreen(myOLED.C_BLACK);

	// test horizontal scroll slow 
	printf("Test 504-2: Scroll test Vertical\r\n");
	colorBlock();
	horOffset = 0;
	numOfRows = 0;
	verOffset = 62; 
	myOLED.ScrollSetup(horOffset, startRowAddr,  numOfRows, verOffset , myOLED.SCROLL_FRAME_10);
	myOLED.Scroll(myOLED.SCROLL_ACTIVATE);
	busy_wait_ms(10000);
	myOLED.Scroll(myOLED.SCROLL_DEACTIVE);
	busy_wait_ms(5000);
	myOLED.fillScreen(myOLED.C_BLACK);
}


void EndTests(void)
{
	myOLED.fillScreen(myOLED.C_BLACK);
	busy_wait_ms(1000);
	myOLED.PowerDown(); // Power down device
	printf("End Test\n");
}

// *************** EOF ****************

/// @endcond
