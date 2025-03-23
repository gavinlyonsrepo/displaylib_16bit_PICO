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
	-# Test 703 print entire ASCII font 32 to 127, default font
	-# Test 704 font invert + test character draw using draw functions
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
bool Test808(void);
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
#ifdef dislib16_ADVANCED_FONTS_ENABLE
	Test808();
#endif
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


void Test701(void) {
	printf("Test 701: Print out some fonts with writeCharString\n");
	myTFT.fillScreen(myTFT.C_BLACK);
	
	struct FontText {
		const char *text;
		display_Font_name_e font;
		int x, y;
	};
	
	// First set of fonts
	FontText fontTests1[] = {
		{"Default", font_default, 5, 5}, {"GLL", font_gll, 5, 15}, {"Pico", font_pico, 5, 30},
		{"Sinclair", font_sinclairS, 5, 45}, {"Orla", font_orla, 5, 65}, {"Retro", font_retro, 5, 95},
		{"Mega", font_mega, 5, 2}, {"Arial b", font_arialBold, 5, 30}, {"Hall", font_hallfetica, 5, 60},
		{"Arial R", font_arialRound, 5, 90}, {"GroTesk", font_groTesk, 5, 5}
	};
	// Loop for first set
	size_t testCount1 = sizeof(fontTests1) / sizeof(FontText);
	for (size_t i = 0; i < testCount1; ++i) {
		myTFT.setFont(fontTests1[i].font);
		myTFT.writeCharString(fontTests1[i].x, fontTests1[i].y, const_cast<char*>(fontTests1[i].text));
		if (i == 5 || i == 9) {
			DisplayReset();
		}
	}
	DisplayReset();
#ifdef dislib16_ADVANCED_FONTS_ENABLE
	// Second set of fonts
	FontText fontTests2[] = {
		{"16", font_sixteenSeg, 5, 5}, {"7", font_sevenSeg, 5, 5}, {"inco", font_inconsola, 5, 5},
		{"GB", font_groTeskBig, 5, 5}, {"Mint", font_mint, 5, 5}
	};
	// Loop for second set
	size_t testCount2 = sizeof(fontTests2) / sizeof(FontText);
	for (size_t i = 0; i < testCount2; ++i) {
		myTFT.setFont(fontTests2[i].font);
		myTFT.writeCharString(fontTests2[i].x, fontTests1[i].y, const_cast<char*>(fontTests2[i].text));
		DisplayReset();
	
	}
#endif
	
}

void Test702(void) {
	printf("Test 702: Some of the Defined 16-bit Colors for text\n");
	myTFT.setFont(font_default);
	struct ColorText {
		const char *text;
		uint16_t color;
	};
	
	ColorText colorTexts[] = {
		{"WHITE", myTFT.C_WHITE}, {"BLUE", myTFT.C_BLUE}, {"RED", myTFT.C_RED},
		{"GREEN", myTFT.C_GREEN}, {"CYAN", myTFT.C_CYAN}, {"MAGENTA", myTFT.C_MAGENTA},
		{"YELLOW", myTFT.C_YELLOW}, {"GREY", myTFT.C_GREY}, {"TAN", myTFT.C_TAN},
		{"BROWN", myTFT.C_BROWN}, {"ORANGE", myTFT.C_ORANGE}, {"DGREEN", myTFT.C_DGREEN}
	};
	
	for (size_t i = 0; i < 12; ++i) {
		int x = (i < 6) ? 5 : 60;  // First 6 on the left, next 6 on the right
		int y = 5 + (i % 6) * 10;  // Vertical spacing
		myTFT.setTextColor(colorTexts[i].color, myTFT.C_BLACK);
		myTFT.writeCharString(x, y, const_cast<char*>(colorTexts[i].text));
	}
	
	DisplayReset();
	
}

void Test703(void) {
	printf("Test 703: Print entire ASCII font 32 to 127, default font\n");
	uint8_t row = 5;
	uint8_t col = 5;
	myTFT.setFont(font_default);
	myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
	for (char i = 32; i < 127; i++) {

		myTFT.writeChar(col, row, i);
		col += 10;
		if (col > 115) {
			row += 10;
			col = 5;
		}
	}
	DisplayReset();
}



void Test704(void) {
	printf("Test 704: font invert + test character draw using draw functions\n");
	myTFT.setTextColor(myTFT.C_RED, myTFT.C_YELLOW);
	myTFT.setFont(font_default);
	char teststr1[] = "TEST";
	myTFT.writeCharString(15, 5, teststr1);
	myTFT.setInvertFont(true);
	myTFT.writeCharString(15, 15, teststr1); //inverted yellow on red
	myTFT.setInvertFont(false);
	myTFT.writeCharString(15, 25, teststr1);
	myTFT.writeChar(0, 85, 'G');
	myTFT.writeChar(25, 85, 'L');
	DisplayReset();
}


void Test705(void)
{
	printf("Test 705: Print class methods\n");

	// Test Fonts default +  + pico+ sinclair + retro
	myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);

	myTFT.setCursor(5,5);
	myTFT.setFont(font_default);
	myTFT.print("Default ");
	myTFT.print(-43);

	myTFT.setCursor(5,15);
	myTFT.setFont(font_gll);
	myTFT.print("GLL ");
	myTFT.print(123.284,1); // print 123.3

	myTFT.setCursor(5,25);
	myTFT.setFont(font_pico);
	myTFT.print("pico ");
	myTFT.print(747);

	myTFT.setCursor(5,65);
	myTFT.setFont(font_sinclairS);
	myTFT.print("sinclair ");
	myTFT.print(456);

	myTFT.setCursor(5,85);
	myTFT.setFont(font_retro);
	myTFT.print("retro  ");
	myTFT.print(-3.14);
	DisplayReset();

	// Test font  mega Arial bold and Hallf.
	myTFT.setCursor(5,5);
	myTFT.setFont(font_mega);
	myTFT.println("mega ");
	myTFT.print(61);

	myTFT.setCursor(5,40);
	myTFT.setFont(font_arialBold);
	myTFT.println("A bold");
	myTFT.print(12.08);

	myTFT.setCursor(5,80);
	myTFT.setFont(font_hallfetica);
	myTFT.println("hall f");
	myTFT.print(1.48);
	DisplayReset();

	// Test Font orla + arial round

	myTFT.setFont(font_orla);
	myTFT.setCursor(5,5);
	myTFT.println(-7.16);
	myTFT.print("Orla");

	myTFT.setCursor(5,70);
	myTFT.setFont(font_arialRound);
	myTFT.println(-8.16);
	myTFT.print("a rnd");
	DisplayReset();

#ifdef dislib16_ADVANCED_FONTS_ENABLE
	// Test font grotesk + sixteen segment
	myTFT.setCursor(5,5);
	myTFT.setFont(font_groTesk);
	myTFT.println("GROTESK");
	myTFT.print(1.78);
	DisplayReset();

	myTFT.setCursor(5,5);
	myTFT.setFont(font_sixteenSeg);
	myTFT.print(124);
	DisplayReset();
	
	// Test font seven segment 
	myTFT.setCursor(5,5);
	myTFT.setFont(font_sevenSeg);
	myTFT.println(12);
	DisplayReset();

	// Test font mint
	myTFT.setFont(font_mint);
	myTFT.println("MINT");;
	myTFT.print("9`C");
	DisplayReset();
#endif

}

void Test706(void)
{
	printf("Test 706: Misc print class(string object, println invert, wrap, base nums etc)\n");
	//Inverted print fonts
	myTFT.setTextColor(myTFT.C_RED, myTFT.C_YELLOW);
	myTFT.setFont(font_default);

	myTFT.setFont(font_sinclairS);
	myTFT.setCursor(5,15);
	myTFT.print(-49);

	myTFT.setFont(font_default);
	myTFT.setCursor(5,35);
	myTFT.print(112.09);
	myTFT.setCursor(5,55);
	myTFT.print("ABCD");

	myTFT.setFont(font_gll);
	myTFT.setCursor(5,85);
	myTFT.print("ER");

	DisplayReset();

	// Inverted print fonts
	myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_RED);

	myTFT.setFont(font_arialBold);
	myTFT.setCursor(5,5);
	myTFT.print("INVERT");
	myTFT.setCursor(5,25);
	myTFT.print(-94.982, 2);

	myTFT.setFont(font_hallfetica);
	myTFT.setCursor(5,65);
	myTFT.print("INVERT");
	myTFT.setCursor(5,85);
	myTFT.print(123456);
	DisplayReset();


	// Test print with DEC BIN OCT HEX
	myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);
	uint8_t numPos = 47;
	myTFT.setFont(font_pico);
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
	myTFT.setFont(font_default);
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


void Test707(void)
{
	printf("Test 707: Misc draw functions tests wrap,  etc\n");

	//wrap
	char teststr0[] = "123456789";

	myTFT.setFont(font_default);
	myTFT.writeCharString(80, 5, teststr0);
	myTFT.setFont(font_mega);
	myTFT.writeCharString(80, 60, teststr0);
	DisplayReset();

	//invert + Numeric fonts  sprintf
	int myInt=931;
	char myStr[5];
	myTFT.setFont(font_sinclairS);
	sprintf(myStr, "%d", myInt);
	myTFT.writeCharString(5, 5, myStr);

	myTFT.setFont(font_retro);
	float  myPI = 3.171592;
	char myStr2[8];
	sprintf(myStr2, "%0.3f", myPI); // 3.172
	myTFT.writeCharString(5, 65, myStr2);

	DisplayReset();

	//invert + Numeric
	char teststr1[] = "12:81";
	char teststr2[] = "72:83";
	char teststr3[] = "9 rnd";
	char teststr4[] = "10 bold";

	myTFT.setFont(font_mega);
	myTFT.setTextColor(myTFT.C_RED, myTFT.C_BLACK);
	myTFT.writeChar(2, 2, '7');
	myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_BLACK);
	myTFT.writeCharString(2, 40, teststr1);

	myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);
	myTFT.writeChar(2, 75, '8');
	myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_RED);
	myTFT.writeCharString(2, 100, teststr2);

	DisplayReset();

	myTFT.setFont(font_arialRound);
	myTFT.setTextColor(myTFT.C_RED, myTFT.C_BLACK);
	myTFT.writeChar(2, 2, '9');
	myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_BLACK);
	myTFT.writeCharString(2, 26, teststr3);

	myTFT.setFont(font_arialBold);
	myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);
	myTFT.writeChar(2, 56, 'A');
	myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_RED);
	myTFT.writeCharString(2, 90, teststr4);

	DisplayReset();
}

#ifdef dislib16_ADVANCED_FONTS_ENABLE

bool Test808(void)
{
	// Unit testing
	printf("==== Test 808 Start Text Unit testing ====\n");
	myTFT.fillScreen(myTFT.C_BLACK);
	// Define the expected return values, note the numeric are 
	// from  print which returns num of characters it attempted to print
	std::vector<uint8_t> expectedErrors = 
	{
		3, DisLib16::CharFontASCIIRange, 3, DisLib16::CharFontASCIIRange, 5, DisLib16::Success, 
		0, DisLib16::CharArrayNullptr,
		DisLib16::Success, DisLib16::CharFontASCIIRange, DisLib16::CharFontASCIIRange,
		DisLib16::CharFontASCIIRange, DisLib16::CharScreenBounds, DisLib16::CharScreenBounds,
		DisLib16::CharScreenBounds, DisLib16::CharScreenBounds, DisLib16::CharArrayNullptr
	};
	
	// Vector to store return values
	std::vector<uint8_t> returnValues; 

	char testlowercase[] = "ZA[ab";
	char testNonNumExtend[] = "-;A";
	bool errorFlag = false;
	myTFT.setFont(font_gll);
	// (1) Print statement test, gll font lower case letters
	// We check print class error flag as print statement does not return errors it returns num of characters
	myTFT.setCursor(40,40);
	returnValues.push_back(myTFT.print("ABc")); // Print AB , return 3 (num of characters)
	returnValues.push_back(myTFT.getWriteError()); // return error
	returnValues.push_back(myTFT.print("abC"));  // print C , return 3 (num of characters)
	returnValues.push_back(myTFT.getWriteError()); // return error
	returnValues.push_back(myTFT.print("12345")); // print 12345 , return 5 (num of characters)
	returnValues.push_back(myTFT.getWriteError()); // return pass
	returnValues.push_back(myTFT.print(nullptr));  // return 0 (num of characters)
	returnValues.push_back(myTFT.getWriteError());  // return error
	DisplayReset();
	printf("========\r\n");
	// (2) writeChar + writeCharString
	// gll lower case + ] character out of font bounds
	returnValues.push_back(myTFT.writeChar(32, 0, '!')); //success
	returnValues.push_back(myTFT.writeCharString(5,  5, testlowercase)); //throw gll font error 2
	DisplayReset();
	// Numeric extended bounds ; , A errors
	myTFT.setFont(font_sixteenSeg);
	returnValues.push_back(myTFT.writeCharString(0, 0, testNonNumExtend)); //throw font error 2
	returnValues.push_back(myTFT.writeChar(32, 0, ',')); //throw error 2
	DisplayReset();
	printf("========\r\n");
	// screen out of bounds
	myTFT.setFont(font_default);
	returnValues.push_back(myTFT.writeChar(0, 400, 'e')); //throw error 1
	returnValues.push_back(myTFT.writeChar(400, 0, 'f')); //throw error 1
	DisplayReset();
	myTFT.setFont(font_orla);
	returnValues.push_back(myTFT.writeChar(0, 400, 'A')); //throw error 1
	returnValues.push_back(myTFT.writeChar(400, 0, 'B')); //throw error 1
	DisplayReset();
	
	returnValues.push_back(myTFT.writeCharString(5, 5, nullptr)); //throw error 
	
	//== SUMMARY SECTION===
	printf("\nUnit testing Summary.\n");
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
		printf("\nUnit testing Text has FAILED.\n");
	}else{
		printf("\nUnit testing Text has PASSED.\n");
	}
	printf("=== End of Test 808 ===\r\n");
	return !errorFlag;
}
#endif

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
	myTFT.setFont(font_default);
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
	myTFT.writeCharString(5, 50, teststr1);
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTPowerDown();
	printf("TFT :: Tests Over");
}

// *************** EOF ****************

/// @endcond
