/*!
	@file main.cpp
	@brief Library test file, st7735,  tests text & fonts.
	@author Gavin Lyons.
	@note See USER OPTIONS 0-3 in SETUP function
	@details For this test file to work fully the optional fonts 9-12 
		must be enabled which they are by default.
		User can disable these fonts by commenting out the following defines
		-# _font.hpp file_DISPLAY_OPTIONAL_FONT_9 for font 9
		-# _font.hpp file_DISPLAY_OPTIONAL_FONT_10 for font 10
		-# _font.hpp file_DISPLAY_OPTIONAL_FONT_11 for font 11
		-# _font.hpp file_DISPLAY_OPTIONAL_FONT_12 for font 12
	@test
	-# Test 701 print out fonts 1-12
	-# Test 702 defined 16-bit Colors, text
	-# Test 703 print entire ASCII font 0 to 254, default font
	-# Test 704 font sizes + character draw using draw functions
	-# Test 705 print method all fonts
	-# Test 706 Misc print class tests (string object, println invert, wrap, base nums etc)
	-# Test 707 Misc draw functions (Invert, wrap)
	-# Test 808 Error check text functions (results to serial port, 38400 baud)
*/


// Section ::  libraries
#include "pico/time.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16/st7735.hpp"
#include <vector> // for error checking test

/// @cond

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY1 1000	  // mS
#define TEST_DELAY2 2000	  // mS
#define TEST_DELAY5 5000	  // mS

// Section :: Globals
ST7735_TFT myTFT;

//  Section ::  Function Headers

void Setup(void);
void Test701(void);
void Test702(void);
void Test703(void);
void Test704(void);
void Test705(void);
void Test706(void);
void Test707(void);
void Test808(void);
void EndTests(void);
void DisplayReset(void);

//  Section ::  MAIN loop

int main(void)
{
	Setup();
	Test701();
	Test702();
	Test703();
	Test704();
	Test705();
	Test706();
	Test707();
	Test808();
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


/*! 
	@brief Test 701: Print out fonts 1-12 with drawtext 
*/
void Test701(void) {

	printf("Test 701: Print out fonts 1-12 with drawtext\r\n");
	char teststr1[] = "Default 1";
	char teststr2[] = "THICK 2";
	char teststr3[] = "Seven 3";
	char teststr4[] = "WIDE 4";
	char teststr5[] = "Tiny 5";
	char teststr6[] = "Home 6";
	char teststr7[] = "1234";
	char teststr8[] = "123456";
	char teststr9[] = "Arial R 9";
	char teststr10[] = "Arial B 10";
	char teststr11[] = "Mia 11";
	char teststr12[] = "Dedica 12";

	myTFT.fillScreen(myTFT.C_BLACK);

	myTFT.FontNum(myTFT.Font_Default);
	myTFT.drawText(5, 2, teststr1, myTFT.C_WHITE, myTFT.C_BLACK, 2);
	myTFT.FontNum(myTFT.Font_Thick);
	myTFT.drawText(5, 20, teststr2, myTFT.C_GREEN, myTFT.C_BLACK, 2);
	myTFT.FontNum(myTFT.Font_Seven_Seg);
	myTFT.drawText(5, 36, teststr3, myTFT.C_BLUE, myTFT.C_BLACK, 2);
	myTFT.FontNum(myTFT.Font_Wide);
	myTFT.drawText(5, 52, teststr4, myTFT.C_CYAN, myTFT.C_BLACK, 2);
	myTFT.FontNum(myTFT.Font_Tiny);
	myTFT.drawText(5, 68, teststr5, myTFT.C_RED, myTFT.C_BLACK, 2);
	myTFT.FontNum(myTFT.Font_HomeSpun);
	myTFT.drawText(5, 84, teststr6, myTFT.C_YELLOW, myTFT.C_BLACK, 2);
	DisplayReset();

	myTFT.FontNum(myTFT.Font_Bignum);
	myTFT.drawText(5, 2, teststr7, myTFT.C_WHITE, myTFT.C_BLACK);
	myTFT.FontNum(myTFT.Font_Mednum);
	myTFT.drawText(5, 33, teststr8, myTFT.C_GREEN, myTFT.C_BLACK);
	myTFT.FontNum(myTFT.Font_ArialRound);
	myTFT.drawText(5, 50, teststr9, myTFT.C_BLUE, myTFT.C_BLACK);
	myTFT.FontNum(myTFT.Font_ArialBold);
	myTFT.drawText(5, 74, teststr10, myTFT.C_CYAN, myTFT.C_BLACK);
	myTFT.FontNum(myTFT.Font_Mia);
	myTFT.drawText(5, 91, teststr11, myTFT.C_RED, myTFT.C_BLACK);
	myTFT.FontNum(myTFT.Font_Dedica);
	myTFT.drawText(5, 106, teststr12, myTFT.C_YELLOW, myTFT.C_BLACK);
	DisplayReset();

}

/*!
	@brief Test 702: Some of the Defined 16-bit Colors for text
*/
void Test702(void) {
	printf("Test 702: Some of the Defined 16-bit Colors for text\r\n");
	myTFT.FontNum(myTFT.Font_Default);
	char teststr1[] = "WHITE";
	char teststr2[] = "BLUE";
	char teststr3[] = "RED";
	char teststr4[] = "GREEN";
	char teststr5[] = "CYAN";
	char teststr6[] = "MAGENTA";
	char teststr7[] = "YELLOW";
	char teststr8[] = "GREY";
	char teststr9[] = "TAN";
	char teststr10[] = "BROWN";
	char teststr11[] = "ORANGE";
	char teststr12[] = "DGREEN";

	myTFT.drawText(5, 5, teststr1, myTFT.C_WHITE, myTFT.C_BLACK, 1);
	myTFT.drawText(5, 15, teststr2, myTFT.C_BLUE, myTFT.C_BLACK, 1);
	myTFT.drawText(5, 25, teststr3, myTFT.C_RED, myTFT.C_BLACK, 1);
	myTFT.drawText(5, 35, teststr4, myTFT.C_GREEN, myTFT.C_BLACK, 1);
	myTFT.drawText(5, 45, teststr5, myTFT.C_CYAN, myTFT.C_BLACK, 1);
	myTFT.drawText(5, 55, teststr6, myTFT.C_MAGENTA, myTFT.C_BLACK, 1);
	myTFT.drawText(60, 5, teststr7, myTFT.C_YELLOW, myTFT.C_BLACK, 1);
	myTFT.drawText(60, 15, teststr8, myTFT.C_GREY, myTFT.C_BLACK, 1);
	myTFT.drawText(60, 25, teststr9, myTFT.C_TAN, myTFT.C_BLACK, 1);
	myTFT.drawText(60, 35, teststr10 , myTFT.C_BROWN, myTFT.C_BLACK, 1);
	myTFT.drawText(60, 45, teststr11 , myTFT.C_ORANGE, myTFT.C_BLACK, 1);
	myTFT.drawText(60, 55, teststr12, myTFT.C_DGREEN, myTFT.C_BLACK, 1);

	DisplayReset();
}

/*!
	@brief Test 703: Print entire ASCII font 0 to 254, default font
*/
void Test703(void) {
	printf("Test 703: Print entire ASCII font 0 to 254, default font\r\n");
	uint8_t row = 5;
	uint8_t col = 5;
	for (char i = 0; i < 126; i++) {

		myTFT.drawChar(col, row, i, myTFT.C_GREEN, myTFT.C_BLACK, 1);
		col += 10;
		if (col > 115) {
			row += 10;
			col = 5;
		}
	}
	DisplayReset();

	row = 5;
	col = 5;
	for (unsigned char j = 127; j < 254; j++) {

		myTFT.drawChar(col, row, j, myTFT.C_GREEN, myTFT.C_BLACK, 1);
		col += 10;
		if (col > 115) {
			row += 10;
			col = 5;
		}
	}
	DisplayReset();
}


/*!
	@brief Test 704: font sizes (2-5) + character draw using draw functions
*/
void Test704(void) {
	printf("Test 704: font sizes (2-5) + character draw using draw functions\r\n");
	myTFT.FontNum(myTFT.Font_Default);
	char teststr1[] = "TEST";
	myTFT.drawText(0, 5, teststr1, myTFT.C_WHITE, myTFT.C_BLACK, 2);
	myTFT.drawText(0, 25, teststr1, myTFT.C_WHITE, myTFT.C_BLACK, 3);
	myTFT.drawText(0, 55, teststr1, myTFT.C_WHITE, myTFT.C_BLACK, 4);
	myTFT.drawChar(0, 85, 'G', myTFT.C_WHITE, myTFT.C_BLACK, 5);
	myTFT.drawChar(45, 85, 'L', myTFT.C_WHITE, myTFT.C_BLACK, 5);

	DisplayReset();
}

/*!
	@brief Test 705: Print class methods
*/
void Test705(void)
{
	printf("Test 705: Print class methods\r\n");

	// Test Fonts 1-6

	myTFT.setTextSize(1);
	myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);

	myTFT.setCursor(5,5);
	myTFT.FontNum(myTFT.Font_Default);
	myTFT.print("Default ");
	myTFT.print(-43);

	myTFT.setCursor(5,15);
	myTFT.FontNum(myTFT.Font_Thick);
	myTFT.print("THICK ");
	myTFT.print(123.284,1); // print 123.2

	myTFT.setCursor(5,25);
	myTFT.FontNum(myTFT.Font_Seven_Seg);
	myTFT.print("7 SEG ");
	myTFT.print(747);

	myTFT.setCursor(5,35);
	myTFT.FontNum(myTFT.Font_Wide);
	myTFT.print("WIDE ");
	myTFT.print(456);

	myTFT.setCursor(5,45);
	myTFT.FontNum(myTFT.Font_Tiny);
	myTFT.print("Tiny!  ");
	myTFT.print(-3.14);

	myTFT.setCursor(5,55);
	myTFT.FontNum(myTFT.Font_HomeSpun);
	myTFT.print("HomeSpun ");
	myTFT.print(6.019,2 );// print 6.02

	DisplayReset();

	// Test Font 7-12

	myTFT.FontNum(myTFT.Font_Bignum); // Test bignum font 7
	myTFT.setCursor(5,5);
	myTFT.println(-7.16);
	myTFT.print("10:05");

	myTFT.setCursor(5,70); // Test mednum font 8
	myTFT.FontNum(myTFT.Font_Mednum);
	myTFT.println(-8.16);
	myTFT.print("12:45");

	DisplayReset();

	myTFT.setCursor(5,5); // Test Arial round font 9
	myTFT.FontNum(myTFT.Font_ArialRound);
	myTFT.println("A rnd");
	myTFT.print(96.78);

	myTFT.setCursor(5,70); // Test Arial bold font 10
	myTFT.FontNum(myTFT.Font_ArialBold);
	myTFT.println("A bold");
	myTFT.print(12.08);

	DisplayReset();

	myTFT.setCursor(5,5); // Test 71 Mia
	myTFT.FontNum(myTFT.Font_Mia);
	myTFT.println("Mia Font");
	myTFT.print(1141.23);

	myTFT.setCursor(10,50); // Test 72 Dedica
	myTFT.FontNum(myTFT.Font_Dedica);
	myTFT.println("Dedica Font");
	myTFT.print(1241.23);

	DisplayReset();

}

/*!
	@brief Test 706: Misc print class(string object, println invert, wrap, base nums
*/
void Test706(void)
{
	printf("Test 706: Misc print class(string object, println invert, wrap, base nums etc)\r\n");
	//Inverted print fonts 1-6
	myTFT.setTextColor(myTFT.C_RED, myTFT.C_YELLOW);

	//size 1
	myTFT.setTextSize(1);
	myTFT.setCursor(5,5);
	myTFT.print("ABCD");

	//size 2
	myTFT.setTextSize(2);
	myTFT.setCursor(5,15);
	myTFT.FontNum(myTFT.Font_Seven_Seg);
	myTFT.print(-49);
	myTFT.setCursor(5,35);
	myTFT.FontNum(myTFT.Font_Default);
	myTFT.print(112.09);
	myTFT.setCursor(5,55);
	myTFT.print("ABCD");

	//size 3
	myTFT.setTextSize(3);
	myTFT.setCursor(5,85);
	myTFT.FontNum(myTFT.Font_Thick);
	myTFT.print("ER");
	DisplayReset();

	// Inverted print fonts 7-12
	myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_RED);
	myTFT.setCursor(5,5);
	myTFT.FontNum(myTFT.Font_ArialBold);
	myTFT.print("INVERT");
	myTFT.setCursor(5,25);
	myTFT.FontNum(myTFT.Font_Mednum);
	myTFT.print(-994.982, 2);
	myTFT.setCursor(5,45);
	myTFT.FontNum(myTFT.Font_Mia);
	myTFT.print("INVERT");
	myTFT.setCursor(5,85);
	myTFT.FontNum(myTFT.Font_Dedica);
	myTFT.print(12345678);
	DisplayReset();


	// Test print with DEC BIN OCT HEX
	myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);
	uint8_t numPos = 47;
	myTFT.FontNum(myTFT.Font_HomeSpun);
	myTFT.setTextSize(2);
	myTFT.setCursor(5,5);
	myTFT.print(numPos , myTFT.DEC); // 47
	myTFT.setCursor(5,25);
	myTFT.print(numPos , myTFT.BIN); // 10111
	myTFT.setCursor(5,45);
	myTFT.print(numPos , myTFT.OCT); // 57
	myTFT.setCursor(5,65);
	myTFT.print(numPos , myTFT.HEX); // 2F

	DisplayReset();

	// Test print a string object with print
	myTFT.FontNum(myTFT.Font_Dedica);
	std::string timeInfo = "12:45";
	std::string newLine = "new l";
	myTFT.setCursor(5, 5);
	myTFT.print(timeInfo);
	// print a new line with println
	myTFT.setCursor(5,40);
	myTFT.println(newLine); // print a new line feed with println
	myTFT.print(newLine);

	//text wrap with print
	myTFT.setCursor(5,70);
	myTFT.print("12345678901234567890ABCDEFGHIJ");
	DisplayReset();
}

/*!
	@brief Test 707: Misc draw functions tests invert, wrap
*/
void Test707(void)
{
	printf("Test 707: Misc functions tests invert, wrap\r\n");

	//wrap
	char teststr0[] = "123456789";

	myTFT.FontNum(myTFT.Font_Wide);
	myTFT.drawText(105, 5, teststr0, myTFT.C_WHITE, myTFT.C_BLACK, 2);
	myTFT.drawText(105, 75, teststr0, myTFT.C_WHITE, myTFT.C_BLACK, 1);
	DisplayReset();

	myTFT.FontNum(myTFT.Font_Bignum);
	myTFT.drawText(80, 10, teststr0, myTFT.C_WHITE, myTFT.C_BLACK);
	DisplayReset();

	//invert + Numeric fonts 1-6 sprintf
	int myInt=931;
	char myStr[5];
	myTFT.FontNum(myTFT.Font_Seven_Seg);
	sprintf(myStr, "%d", myInt);
	myTFT.drawText(5, 5, myStr, myTFT.C_BLUE, myTFT.C_BLACK, 3);

	myTFT.FontNum(myTFT.Font_HomeSpun);
	float  myPI = 3.171592;
	char myStr2[8];
	sprintf(myStr2, "%0.3f", myPI); // 3.172
	myTFT.drawText(5, 65, myStr2, myTFT.C_RED, myTFT.C_BLACK, 3);

	DisplayReset();

	//invert + Numeric fonts 7-12 sprintf
	// Note fonts 7 and 8 are extended numeric only
	// Note Fonts 9 and 10 are Alphanumeric
	char teststr1[] = "12:81";
	char teststr2[] = "72:83";
	char teststr3[] = "9 rnd";
	char teststr4[] = "10 bold";

	myTFT.FontNum(myTFT.Font_Bignum);
	myTFT.drawChar(2, 2, '7',  myTFT.C_RED, myTFT.C_BLACK);
	myTFT.drawText(2, 40, teststr1, myTFT.C_YELLOW, myTFT.C_RED);

	myTFT.FontNum(myTFT.Font_Mednum);
	myTFT.drawChar(2, 75, '8',  myTFT.C_WHITE, myTFT.C_BLACK);
	myTFT.drawText(2, 100, teststr2, myTFT.C_GREEN, myTFT.C_RED);

	DisplayReset();

	myTFT.FontNum(myTFT.Font_ArialRound);
	myTFT.drawChar(2, 2, '9',  myTFT.C_RED, myTFT.C_BLACK);
	myTFT.drawText(2, 26, teststr3, myTFT.C_YELLOW, myTFT.C_RED);

	myTFT.FontNum(myTFT.Font_ArialBold);
	myTFT.drawChar(2, 56, 'A',  myTFT.C_WHITE, myTFT.C_BLACK);
	myTFT.drawText(2, 90, teststr4, myTFT.C_GREEN, myTFT.C_RED);

	DisplayReset();
}

void Test808(void) {
	// === Setup tests ===
	// Define the expected return values
	std::vector<uint8_t> expectedErrors = 
	{
		DisLib16::Success, 
		DisLib16::WrongFont, DisLib16::WrongFont, DisLib16::WrongFont, DisLib16::WrongFont, 
		DisLib16::CharFontASCIIRange, DisLib16::CharFontASCIIRange, 
		DisLib16::CharFontASCIIRange, DisLib16::CharFontASCIIRange, 
		DisLib16::CharFontASCIIRange, DisLib16::CharFontASCIIRange, 
		DisLib16::CharFontASCIIRange, DisLib16::CharFontASCIIRange, 
		DisLib16::CharScreenBounds, DisLib16::CharScreenBounds, DisLib16::CharScreenBounds, 
		DisLib16::CharScreenBounds, DisLib16::CharScreenBounds ,DisLib16::CharScreenBounds, 
		DisLib16::CharArrayNullptr, DisLib16::CharArrayNullptr, 
		DisLib16::Success
	};
	// Vector to store return values
	std::vector<uint8_t> returnValues; 
	// test varibles
	char testString0[] = "123456789";
	char testLowerCase[] = "ABC]ab";
	char testNonNumExtend[] = "-./12:;,A";
	char testString5[] = "Error Check: Results to Serial port";
	bool errorFlag = false;

	// === Tests===
	printf("=== START Error checking. Expecting errors ===\r\n");
	// Perform function calls and store return values
	myTFT.FontNum(myTFT.Font_Default);
	returnValues.push_back(myTFT.drawText(5, 55, testString5, myTFT.C_RED, myTFT.C_BLACK, 2)); 
	DisplayReset();

	// wrong font
	myTFT.FontNum(myTFT.Font_Bignum);
	returnValues.push_back(myTFT.drawText(105, 55, testString0, myTFT.C_WHITE, myTFT.C_BLACK, 1));
	returnValues.push_back(myTFT.drawChar(105, 55, '1', myTFT.C_WHITE, myTFT.C_BLACK, 1));
	myTFT.FontNum(myTFT.Font_Default);
	returnValues.push_back(myTFT.drawText(80, 55, testString0, myTFT.C_WHITE, myTFT.C_BLACK));
	returnValues.push_back(myTFT.drawChar(105, 55, '1', myTFT.C_WHITE, myTFT.C_BLACK));
	DisplayReset();

	// character out of font range
	// wide & thick lower case + ]
	myTFT.FontNum(myTFT.Font_Wide);
	returnValues.push_back(myTFT.drawChar(5, 5, '[', myTFT.C_WHITE, myTFT.C_BLACK, 2));
	returnValues.push_back(myTFT.drawText(5, 5, testLowerCase, myTFT.C_WHITE, myTFT.C_BLACK, 2));
	myTFT.FontNum(myTFT.Font_Thick);
	returnValues.push_back(myTFT.drawChar(5, 5, '[', myTFT.C_WHITE, myTFT.C_BLACK, 2));
	returnValues.push_back(myTFT.drawText(5, 25, testLowerCase, myTFT.C_WHITE, myTFT.C_BLACK, 2));
	myTFT.FontNum(myTFT.Font_Bignum);
	returnValues.push_back(myTFT.drawChar(5, 5, ',', myTFT.C_WHITE, myTFT.C_BLACK));
	returnValues.push_back(myTFT.drawText(5, 45, testNonNumExtend, myTFT.C_WHITE, myTFT.C_BLACK));
	myTFT.FontNum(myTFT.Font_Mednum);
	returnValues.push_back(myTFT.drawChar(5, 5, ';', myTFT.C_WHITE, myTFT.C_BLACK));
	returnValues.push_back(myTFT.drawText(5, 75, testNonNumExtend, myTFT.C_WHITE, myTFT.C_BLACK));
	DisplayReset();

	// screen out of bounds
	myTFT.FontNum(myTFT.Font_Default);
	returnValues.push_back(myTFT.drawChar(0, 170, 'e', myTFT.C_WHITE, myTFT.C_BLACK, 1));
	returnValues.push_back(myTFT.drawChar(170, 0, 'f', myTFT.C_WHITE, myTFT.C_BLACK, 1));
	returnValues.push_back(myTFT.drawText(170, 75, testNonNumExtend, myTFT.C_WHITE, myTFT.C_BLACK, 1));
	myTFT.FontNum(myTFT.Font_ArialBold);
	returnValues.push_back(myTFT.drawChar(0, 170, 'A', myTFT.C_WHITE, myTFT.C_BLACK));
	returnValues.push_back(myTFT.drawChar(170, 0, 'B', myTFT.C_WHITE, myTFT.C_BLACK));
	returnValues.push_back(myTFT.drawText(170, 75, testString0, myTFT.C_WHITE, myTFT.C_BLACK));
	DisplayReset();

	// nullptr 
	myTFT.FontNum(myTFT.Font_Default);
	returnValues.push_back(myTFT.drawText(40, 75, nullptr, myTFT.C_WHITE, myTFT.C_BLACK, 1));
	myTFT.FontNum(myTFT.Font_ArialBold);
	returnValues.push_back(myTFT.drawText(40, 75, nullptr, myTFT.C_WHITE, myTFT.C_BLACK));
	DisplayReset();

	// font disabled 11_DISPLAY_OPTIONAL_FONT_11  returns ZERO by default, 6 if setup
	myTFT.FontNum(myTFT.Font_Mia);
	returnValues.push_back(myTFT.drawText(40, 75, testString0, myTFT.C_WHITE, myTFT.C_BLACK));
	
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

void DisplayReset(void)
{
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

// *************** EOF ****************

/// @endcond
