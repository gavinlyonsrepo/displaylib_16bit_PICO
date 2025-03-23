/*!
	@file main.cpp
	@brief Library test file, tests text & fonts.
	@author Gavin Lyons.
	@note See USER OPTIONS 0-2 in SETUP function, output to serial port 38400 baud
	@details For this test file to work fully macrodislib16_ADVANCED_FONTS_ENABLE
 
			at top of  file displaylib_16_Font.hpp MUST be commented in.
	@test
	-# Test 701 Print out all fonts with writeCharString
	-# Test 702 Print out all fonts with print
	-# Test 703 Misc print class tests(string object, println invert, wrap, base nums etc)
	-# Test 808 Error check text functions (results to serial port, 38400 baud)
*/


// Section ::  libraries
#include <vector> // for error checking test
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

void Setup(void);
void Test701(void);
void Test702(void);
void Test703(void);
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
	stdio_init_all(); // optional for messages, Initialize serial port,  38400 baud
	MILLISEC_DELAY(TEST_DELAY1);
	printf("\nTFT Start\r\n");

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
	uint16_t OFFSET_COL = 0;	   // 2, These offsets can be adjusted for any issues->
	uint16_t OFFSET_ROW = 0;	   // 3, with screen manufacture tolerance/defects
	uint16_t TFT_WIDTH = 240;  // Screen width in pixels
	uint16_t TFT_HEIGHT = 320; // Screen height in pixels
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW, TFT_WIDTH, TFT_HEIGHT);
	// ******************************************

	myTFT.TFTST7789Initialize(); 
}


void Test701(void) {

	printf("Test 701: Print out all fonts with writeCharString\n");
	char teststr1[] = "Default ";
	char teststr2[] = "GLL ";
	char teststr3[] = "Pico ";
	char teststr4[] = "Sinclair ";
	char teststr5[] = "Orla ";
	char teststr6[] = "Retro ";
	char teststr7[] = "Mega";
	char teststr8[] = "Arial b";
	char teststr9[] = "Hall ";
	char teststr10[] = "Arial R";
	char teststr11[] = "GroTesk";

	myTFT.fillScreen(myTFT.C_BLACK);

	myTFT.setFont(font_default);
	myTFT.writeCharString(5, 45, teststr1);
	myTFT.setFont(font_gll);
	myTFT.writeCharString(5, 55, teststr2);
	myTFT.setFont(font_pico);
	myTFT.writeCharString(5, 70, teststr3);
	myTFT.setFont(font_sinclairS);
	myTFT.writeCharString(5, 130, teststr4);
	myTFT.setFont(font_orla);
	myTFT.writeCharString(5, 170, teststr5);
	myTFT.setFont(font_retro);
	myTFT.writeCharString(5, 200, teststr6);
	DisplayReset();

	myTFT.setFont(font_mega);
	myTFT.writeCharString(5, 52, teststr7);
	myTFT.setFont(font_arialBold);
	myTFT.writeCharString(5, 80, teststr8);
	myTFT.setFont(font_hallfetica);
	myTFT.writeCharString(5, 120, teststr9);
	myTFT.setFont(font_arialRound);
	myTFT.writeCharString(5, 200, teststr10);
	DisplayReset();

	myTFT.setFont(font_groTesk);
	myTFT.writeCharString(5, 55, teststr11);

	#ifdef dislib16_ADVANCED_FONTS_ENABLE

	char teststr12[] = "16";
	char teststr13[] = "7";
	char teststr14[] = "inco";
	char teststr15[] = "GB";
	char teststr16[] = "Mint";
	myTFT.setFont(font_sixteenSeg);
	myTFT.writeCharString(5,120, teststr12);
	DisplayReset();

	myTFT.setFont(font_sevenSeg);
	myTFT.writeCharString(5, 55, teststr13);
	myTFT.setFont(font_inconsola);
	myTFT.writeCharString(5, 120, teststr14);
	DisplayReset();
	
	myTFT.setFont(font_groTeskBig);
	myTFT.writeCharString(5, 55, teststr15);
	myTFT.setFont(font_mint);
	myTFT.writeCharString(5, 120, teststr16);
	DisplayReset();
	#endif
}

void Test702(void)
{
	printf("Test 702: Print class methods\n");

	// Test Fonts default +  + pico+ sinclair + retro
	myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);

	myTFT.setCursor(5,55);
	myTFT.setFont(font_default);
	myTFT.print("Default ");
	myTFT.print(-43);

	myTFT.setCursor(5,75);
	myTFT.setFont(font_gll);
	myTFT.print("GLL ");
	myTFT.print(123.284,1); // print 123.3

	myTFT.setCursor(5,125);
	myTFT.setFont(font_pico);
	myTFT.print("pico ");
	myTFT.print(747);

	myTFT.setCursor(5,170);
	myTFT.setFont(font_sinclairS);
	myTFT.print("sinclair ");
	myTFT.print(456);

	myTFT.setCursor(5,200);
	myTFT.setFont(font_retro);
	myTFT.print("retro  ");
	myTFT.print(-3.14);
	DisplayReset();

	// Test font  mega Arial bold and Hallf.
	myTFT.setCursor(5,55);
	myTFT.setFont(font_mega);
	myTFT.println("mega ");
	myTFT.print(61);

	myTFT.setCursor(5,100);
	myTFT.setFont(font_arialBold);
	myTFT.println("A bold");
	myTFT.print(12.08);

	myTFT.setCursor(5,160);
	myTFT.setFont(font_hallfetica);
	myTFT.println("hall f");
	myTFT.print(1.48);
	DisplayReset();

	// Test Font orla + arial round

	myTFT.setFont(font_orla);
	myTFT.setCursor(5,55);
	myTFT.println(-7.16);
	myTFT.print("Orla");

	myTFT.setCursor(5,120);
	myTFT.setFont(font_arialRound);
	myTFT.println(-8.16);
	myTFT.print("a rnd");
	DisplayReset();

	// Test font grotesk + sixteen segment
	myTFT.setCursor(5,75);
	myTFT.setFont(font_groTesk);
	myTFT.println("GROTESK");
	myTFT.print(1.78);

#ifdef dislib16_ADVANCED_FONTS_ENABLE

	myTFT.setCursor(0,180);
	myTFT.setFont(font_sixteenSeg);
	myTFT.print(1245);
	DisplayReset();
	
	// Test font seven segment 
	myTFT.setCursor(5,55);
	myTFT.setFont(font_sevenSeg);
	myTFT.println(12);
	DisplayReset();

	myTFT.setFont(font_mint);
	myTFT.println("MINT");;
	myTFT.print("9`C");
#endif
	DisplayReset();
}

void Test703(void)
{
	printf("Test 703: Misc print class(string object, println invert, wrap, base nums etc)\n");
	//Inverted print fonts 1-6
	myTFT.setTextColor(myTFT.C_RED, myTFT.C_YELLOW);
	myTFT.setFont(font_default);

	myTFT.setFont(font_sinclairS);
	myTFT.setCursor(5,55);
	myTFT.print(-49);

	myTFT.setFont(font_mega);
	myTFT.setCursor(5,90);
	myTFT.print(112.09);
	myTFT.setCursor(5,120);
	myTFT.print("ABCD");

	myTFT.setFont(font_gll);
	myTFT.setCursor(5,200);
	myTFT.print("ER");

	DisplayReset();

	// Inverted print fonts 7-12
	myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_RED);

	myTFT.setFont(font_arialBold);
	myTFT.setCursor(5,55);
	myTFT.print("INVERT");
	myTFT.setCursor(5,85);
	myTFT.print(-94.982, 2);

	myTFT.setFont(font_hallfetica);
	myTFT.setCursor(5,120);
	myTFT.print("INVERT");
	myTFT.setCursor(5,200);
	myTFT.print(123456);
	DisplayReset();

	// Test print with DEC BIN OCT HEX
	myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);
	uint8_t numPos = 47;
	myTFT.setFont(font_mega);
	myTFT.setCursor(5,55);
	myTFT.print(numPos , myTFT.DEC); // 47
	myTFT.setCursor(5,75);
	myTFT.print(numPos , myTFT.BIN); // 10111
	myTFT.setCursor(5,120);
	myTFT.print(numPos , myTFT.OCT); // 57
	myTFT.setCursor(5,160);
	myTFT.print(numPos , myTFT.HEX); // 2F

	DisplayReset();

	// Test print a string object with print
	myTFT.setFont(font_mega);
	std::string timeInfo = "12:45";
	std::string newLine = "new l";
	myTFT.setCursor(5, 55);
	myTFT.print(timeInfo);
	// print a new line with println
	myTFT.setCursor(5,120);
	myTFT.println(newLine); // print a new line feed with println
	myTFT.print(newLine);

	//text wrap with print
	myTFT.setCursor(5,70);
	myTFT.print("12345678901234567890ABCDEFGHIJ");
	DisplayReset();
	
}


void DisplayReset(void)
{
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
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
	printf("TFT :: Tests Over");
}

// *************** EOF ****************

/// @endcond
