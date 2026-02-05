/*! 
	@file   main.cpp
	@brief  Example cpp file for gc9d01 display Library test file, tests : Text,graphics & functions.
	@author Gavin Lyons.
	@details
			This example is for 160x160 Dual gate.
			For graphics after tests 905 to work 
			dislib16_ADVANCED_GRAPHICS_ENABLE to be on.
			Also this example requires dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
			to be on, (non-default).
			See displaylib_16_graphics.hpp USER OPTIONS 
			Requires usable heap memory of (160x160x2) = 51,200 bytes for buffer

	@test 
		-# Test 500 RGB color OK?
		-# Test 501 Scroll test
		-# Test 502 Rotate
		-# Test 503 change modes test -> Invert, display on/off and Sleep.
		-# Test 701 Print out some fonts with writeCharString
		-# Test 702 Print out some fonts with print
		-# Test 703 Misc print class tests(string object, println invert, wrap, base nums etc)
		-# Test 902 rectangles
		-# Test 903 Circle
		-# Test 904 Triangles
		-# Test 905 Polygons
		-# Test 906 Dot Gap
		-# Test 907 Quadrilateral
		-# Test 908 Ellipse
		-# Test 909 Draw Arc
*/

// Section ::  libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16/gc9d01.hpp"

/// @cond

//  Test timing related defines
#define TEST_DELAY5 5000
#define TEST_DELAY2 2000
#define TEST_DELAY  1000

#ifndef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is not defined. it is required for this example")
#endif
#ifndef dislib16_ADVANCED_GRAPHICS_ENABLE
#pragma message("gll: dislib16_ADVANCED_GRAPHICS_ENABLE is not defined. it is required for this demo")
#endif

uint16_t TFT_WIDTH = 160;// Screen width in pixels
uint16_t TFT_HEIGHT = 160; // Screen height in pixels
GC9D01_TFT myTFT;

//  Function Headers 
bool Setup(void); // setup + user options for hardware SPI
void EndTests(void);
void DisplayReset(void);
void ColorBlock(void);

void Test500(void);
void Test501(void);
void Test502(void);
void Test503(void);

void Test701(void);
void Test702(void);
void Test703(void);

void Test902(void);
void Test903(void);
void Test904(void);
#ifdef dislib16_ADVANCED_GRAPHICS_ENABLE
void Test905(void);
void Test906(void);
void Test907(void);
void Test908(void);
void Test909(void);
#endif

//  *** MAIN loop ***
int main() 
{
	DisLib16::setDebugMode(true); // turn on debug mode optional
	if (!Setup())
		return -1;

	Test500();
	Test501();
	Test502();
	Test503();

	Test701();
	Test702();
	Test703();

	Test902();
	Test903();
	Test904();
#ifdef dislib16_ADVANCED_GRAPHICS_ENABLE
	Test905();
	Test906();
	Test907();
	Test908();
	Test909();
#endif
	EndTests();
	return 0;
}
// *** End OF MAIN **


//   Function Space 

bool Setup(void)
{
	stdio_init_all(); // optional for error messages , Initialize chosen serial port, default 38400 baud
	MILLISEC_DELAY(1000);
	printf("TFT: Start\r\n");
//*** USER OPTION 0 SPI_SPEED + TYPE ***
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
	int8_t CS_TFT = 2;  
	int8_t RST_TFT = 4;
	myTFT.TFTsetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
// ***
// *** USER OPTION 2 Screen Setup ***
	uint16_t TFT_WIDTH  = 160; // Screen width in pixels
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

void EndTests(void)
{
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.clearBuffer(myTFT.C_BLACK);
	myTFT.destroyBuffer();
	myTFT.TFTPowerDown(); // Power down device
	printf("End\r\n");
}

void Test500(void)
{
	printf("Test 500: Color Test:: Red,green,blue,yellow,white, black background\n");
	myTFT.setFont(font_mega);
	myTFT.clearBuffer(myTFT.C_BLACK);
	myTFT.setTextColor(myTFT.C_GREEN,myTFT.C_BLACK);
	ColorBlock();
	myTFT.writeBuffer();
	busy_wait_ms(TEST_DELAY5);
	myTFT.clearBuffer(myTFT.C_BLACK);
}

void ColorBlock(void){
	myTFT.fillRoundRect(8, 50, 24, 48, 8, myTFT.C_RED);
	myTFT.fillRoundRect(32, 50, 24, 48, 8, myTFT.C_GREEN);
	myTFT.fillRoundRect(56, 50, 24, 48, 8, myTFT.C_BLUE);
	myTFT.fillRoundRect(80, 50, 24, 48, 8, myTFT.C_YELLOW);
	myTFT.fillRoundRect(104, 50, 24, 48, 8, myTFT.C_WHITE);
}

void Test501(void)
{
	printf("Test 501: Scroll\n");
	myTFT.clearBuffer(myTFT.C_BLACK);
	myTFT.setFont(font_retro);
	char teststr0[] = "Scroll Test";
	myTFT.writeCharString(25, 80, teststr0);
	myTFT.writeBuffer(); // update screen from buffer

	const uint16_t screenH = 160;
	const uint16_t scrollArea = screenH - 0; // no fixed areas
	myTFT.TFTsetScrollArea(0, scrollArea, 0);

	for (uint16_t y = 0; y < screenH; y++) {
		myTFT.TFTsetScrollStart(y);
		sleep_ms(20);
	}
	myTFT.TFTsetScrollStart(0); // reset
	myTFT.TFTScrollModeLeave(); // leave scroll mode
	myTFT.clearBuffer(myTFT.C_BLACK);
}

void Test502()
{
	printf("Test 502: Rotate\n");
	myTFT.setFont(font_retro);
	char teststr0[] = "Rotate 0"; //normal
	char teststr1[] = "Rotate 90"; // 90
	char teststr2[] = "Rotate 180"; // 180
	char teststr3[] = "Rotate 270"; // 270

	myTFT.clearBuffer(myTFT.C_BLACK);
	myTFT.TFTsetRotation(myTFT.Degrees_0);
	myTFT.writeCharString(15, 80, teststr0);
	myTFT.writeBuffer();
	busy_wait_ms(TEST_DELAY2);

	myTFT.clearBuffer(myTFT.C_BLACK);
	myTFT.TFTsetRotation(myTFT.Degrees_90);
	myTFT.writeCharString(15, 80, teststr1);
	myTFT.writeBuffer();
	busy_wait_ms(TEST_DELAY2);

	myTFT.clearBuffer(myTFT.C_BLACK);
	myTFT.TFTsetRotation(myTFT.Degrees_180);
	myTFT.writeCharString(15, 80, teststr2);
	myTFT.writeBuffer();
	busy_wait_ms(TEST_DELAY2);

	myTFT.clearBuffer(myTFT.C_BLACK);
	myTFT.TFTsetRotation(myTFT.Degrees_270);
	myTFT.writeCharString(15, 80, teststr3);
	myTFT.writeBuffer();
	busy_wait_ms(TEST_DELAY2);

	myTFT.TFTsetRotation(myTFT.Degrees_0);
	myTFT.clearBuffer(myTFT.C_BLACK);
}

void Test503()
{
	printf("Test 503: Mode Tests\n");
	char teststr1[] = "Invert";
	char teststr2[] = "Display on/off";
	char teststr3[] = "Sleep   on/off";
	ColorBlock();
	myTFT.writeCharString(25, 100, teststr1);
	myTFT.writeBuffer();
	busy_wait_ms(TEST_DELAY2);

	// Invert on and off
	myTFT.TFTchangeInvertMode(false);
	printf("Test 503-1: Invert on \n");
	busy_wait_ms(TEST_DELAY5);
	myTFT.TFTchangeInvertMode(true);
	printf("Test 503-2: Invert off \n");
	busy_wait_ms(TEST_DELAY5);

	// Display on and off
	myTFT.TFTenableDisplay(false);
	printf("Test 503-3: Display off\n");
	myTFT.writeCharString(10, 100, teststr2);
	myTFT.writeBuffer();
	busy_wait_ms(TEST_DELAY5);
	myTFT.TFTenableDisplay(true);
	printf("Test 503-4: Turn Display back on\n");
	busy_wait_ms(TEST_DELAY5);
	// modes
	myTFT.writeCharString(10, 100, teststr3);
	myTFT.writeBuffer();
	busy_wait_ms(TEST_DELAY5); 
	// 1. sleep mode
	myTFT.TFTsetPowerMode(GC9D01_TFT::PowerState_e::SleepNormalIdleOff);
	printf("Test 503-7: Power mode Sleep ON Idle Off\n");
	busy_wait_ms(TEST_DELAY5);
	busy_wait_ms(TEST_DELAY5);
	//back to sleep off - normal on - idle off
	myTFT.TFTsetPowerMode(GC9D01_TFT::PowerState_e::NormalIdleOff);	
	busy_wait_ms(TEST_DELAY5);
}



void Test701(void) {

	printf("Test 701: Print out some fonts with writeCharString\n");
	char teststr1[] = "Default ";
	char teststr2[] = "GLL ";
	char teststr3[] = "Pico ";
	char teststr4[] = "Sinclair ";
	char teststr5[] = "Orla ";
	char teststr6[] = "Retro ";
	char teststr7[] = "Mega";


	myTFT.clearBuffer(myTFT.C_BLACK);
	// Start at ~20px Y offset and use ~20px spacing
	int startY = 20;
	int spacing = 20;
	int x = 30;
	int y = startY;

	myTFT.setFont(font_default);
	myTFT.writeCharString(x, y, teststr1);
	y += spacing;
	myTFT.setFont(font_gll);
	myTFT.writeCharString(x, y, teststr2);
	y += spacing;
	myTFT.setFont(font_pico);
	myTFT.writeCharString(x, y, teststr3);
	y += spacing;
	myTFT.setFont(font_sinclairS);
	myTFT.writeCharString(x, y, teststr4);
	y += spacing;
	myTFT.setFont(font_orla);
	myTFT.writeCharString(x, y, teststr5);
	y += spacing;
	myTFT.setFont(font_retro);
	myTFT.writeCharString(x, y, teststr6);
	y += spacing;
	myTFT.setFont(font_mega);
	myTFT.writeCharString(x, y, teststr7);
	y += spacing;

	DisplayReset();
}

void Test702(void)
{
	printf("Test 702: Print class methods\n");

	// Test Fonts default +  + pico+ sinclair + retro
	myTFT.clearBuffer(myTFT.C_BLACK);
	myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);

	int x = 25;
	int y = 20;
	int spacing = 25;

	// --- First Block ---
	myTFT.setCursor(x, y);
	myTFT.setFont(font_default);
	myTFT.print("Default ");
	myTFT.print(-43);
	y += spacing;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_gll);
	myTFT.print("GLL ");
	myTFT.print(123.284, 1); // print 123.3
	y += spacing;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_pico);
	myTFT.print("pico ");
	myTFT.print(747);
	y += spacing;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_sinclairS);
	myTFT.print("sinclair ");
	myTFT.print(456);
	y += spacing;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_retro);
	myTFT.print("retro ");
	myTFT.print(-3.14);

	DisplayReset();

	// --- Second Block ---
	y = 20;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_mega);
	myTFT.print("mega");
	y += spacing;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_arialBold);
	myTFT.print("A bold");
	y += spacing;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_hallfetica);
	myTFT.print(1.48);

	DisplayReset();

	// --- Third Block ---
	y = 20;
	myTFT.setFont(font_orla);
	myTFT.setCursor(x, y);
	myTFT.print("Orla");
	y += spacing;
	myTFT.setCursor(x, y + spacing);
	myTFT.setFont(font_arialRound);
	myTFT.print(-8.16);

	DisplayReset();

	// --- Final Block ---
	y = 20;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_groTesk);
	myTFT.println("GROTESK");
	myTFT.print(1.78);
	DisplayReset();
}

void Test703(void)
{
	printf("Test 703: Misc print class(string object, println invert, wrap, base nums etc)\n");
	// --- Inverted print fonts 1–6 ---
	myTFT.clearBuffer(myTFT.C_BLACK);
	myTFT.setTextColor(myTFT.C_RED, myTFT.C_YELLOW);
	myTFT.setFont(font_sinclairS);
	myTFT.setCursor(30, 30);
	myTFT.print(-49);
	myTFT.setFont(font_mega);
	myTFT.setCursor(10, 60);
	myTFT.print(112.09);
	myTFT.setCursor(10, 90);
	myTFT.print("ABCD");

	DisplayReset();

	// --- Inverted print ---
	myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_RED);
	myTFT.setFont(font_arialBold);
	myTFT.setCursor(40, 40);
	myTFT.print("INVERT");
	myTFT.setCursor(40, 80);
	myTFT.print(-94.982, 2);
	myTFT.setFont(font_hallfetica);
	myTFT.setCursor(40, 100);
	myTFT.print("INVERT");
	myTFT.setCursor(40, 140);
	myTFT.print(123456);

	DisplayReset();

	// --- Print in different number bases ---
	myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);
	uint8_t numPos = 47;
	myTFT.setFont(font_mega);
	myTFT.setCursor(40, 50);
	myTFT.print(numPos, myTFT.DEC); // 47
	myTFT.setCursor(40, 70);
	myTFT.print(numPos, myTFT.BIN); // 101111
	myTFT.setCursor(40, 90);
	myTFT.print(numPos, myTFT.OCT); // 57
	myTFT.setCursor(40, 110);
	myTFT.print(numPos, myTFT.HEX); // 2F

	DisplayReset();

	// --- Test std::string print and wrapping ---
	myTFT.setFont(font_mega);
	std::string timeInfo = "12:45";
	std::string newLine = "new l";
	myTFT.setCursor(60, 60);
	myTFT.print(timeInfo);
	myTFT.setCursor(60, 60);
	myTFT.println(newLine); // newline
	myTFT.print(newLine);
	myTFT.setCursor(10, 100);
	myTFT.print("1234567890ABCDEFGHIJ"); // wrap long text

	DisplayReset();
}

void Test902(void) {
	printf("Test 902: rectangles \n");
	DisLib16::Ret_Codes_e returnValue;
	myTFT.fillScreen(myTFT.C_BLACK);
	// Note fillRectBuffer() function does not write to global buffer, 
	// it writes directly to the screen, 
	// with local buffer. so no need to call writeBuffer() after it.
	// Note also that fillRectBuffer is wrapped by fillscreen() function.
	returnValue = myTFT.fillRectBuffer(85, 25, 20, 20, myTFT.C_YELLOW);
	if (returnValue != DisLib16::Success)
	{
		printf("Warning : Test TFTfillRectangle, An error occurred returnValue = %u \n", returnValue);
	}
	busy_wait_ms(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);

	myTFT.drawRectWH(45, 25, 20, 20, myTFT.C_RED);
	myTFT.fillRect(110, 25, 20, 20, myTFT.C_GREEN);
	myTFT.drawRoundRect(15, 80, 70, 60, 20, myTFT.C_CYAN);
	myTFT.fillRoundRect(110, 80, 40, 50, 10, myTFT.C_WHITE);

	DisplayReset();
}

void Test903(void) {
	printf("Test 903 & 904 : Triangles and circles\n");
	myTFT.drawCircle(40, 40, 25, myTFT.C_GREEN);
	myTFT.fillCircle(80, 80, 25, myTFT.C_YELLOW);
	DisplayReset();
}

void Test904(void) {
	myTFT.drawTriangle(5, 80, 50, 40, 95, 80, myTFT.C_CYAN);
	myTFT.fillTriangle(55, 120, 100, 90, 127, 120, myTFT.C_RED);
	DisplayReset();
}

#ifdef dislib16_ADVANCED_GRAPHICS_ENABLE
void Test905(void)
{
	printf("Test 905 : polygons\n");
	myTFT.clearBuffer(myTFT.C_BLACK);
	//drawPolygon( x,  y, sides,  diameter,  rotation, color);
	// octagon
	myTFT.drawPolygon(80, 80, 8, 50, 0, false, myTFT.C_GREEN);
	busy_wait_ms(TEST_DELAY5);
	// octagon
	myTFT.drawPolygon(80, 80, 8, 50, 0, true, myTFT.C_GREEN);
	DisplayReset();
}

void Test906(void) {
	printf("Test 906 : mesh\n");
	myTFT.clearBuffer(myTFT.C_BLACK);
	//drawDotGrid(x, y,  w,  h,  DotGridGap, color);
	myTFT.drawDotGrid(50, 50, 100,100, 5, myTFT.C_CYAN);
	DisplayReset();
}

void Test907() {
	printf("Test 907 : quadrilateral\n");
	// Define the four quadrilateral vertices for different shapes
	// First shape: Quadrilateral
	int16_t x0 = 30, y0 = 30;
	int16_t x1 = 80, y1 = 30;
	int16_t x2 = 80, y2 = 80;
	int16_t x3 = 30, y3 = 80;

	// Second shape: Parallelogram 
	int16_t x4 = 90,  y4 = 100;
	int16_t x5 = 130, y5 = 110;
	int16_t x6 = 120, y6 = 140;
	int16_t x7 = 80,  y7 = 130;

	// Third shape: Trapezoid 1
	int16_t x8  = 95,  y8  = 110;
	int16_t x9  = 100, y9  = 130;
	int16_t x10 = 140, y10 = 130;
	int16_t x11 = 150, y11 = 110;

	// Fourth shape: Trapezoid 2
	int16_t x12 = 40,  y12 = 120;
	int16_t x13 = 55,  y13 = 155;
	int16_t x14 = 100, y14 = 155;
	int16_t x15 = 70,  y15 = 120;
	myTFT.drawQuadrilateral(x0, y0, x1, y1, x2, y2, x3, y3, myTFT.C_RED);
	myTFT.fillQuadrilateral(x4, y4, x5, y5, x6, y6, x7, y7, myTFT.C_GREEN);
	DisplayReset();

	myTFT.fillQuadrilateral(x8, y8, x9, y9, x10, y10, x11, y11, myTFT.C_BLUE);
	myTFT.drawQuadrilateral(x12, y12, x13, y13, x14, y14, x15, y15, myTFT.C_YELLOW);
	DisplayReset();
}

void Test908(void) {
	printf("Test 908 : Ellipse\n");
	myTFT.clearBuffer(myTFT.C_BLACK);
	// Define ellipses at different positions
	// Format: drawEllipse(centerX, centerY, semiMajorAxis, semiMinorAxis, fill, color)
	// Draw unfilled ellipses
	myTFT.drawEllipse(60, 60, 18, 30, false, myTFT.C_WHITE);
	myTFT.drawEllipse(120, 60, 18, 30, false, myTFT.C_RED);
	DisplayReset();

	// Draw filled ellipses in same positions
	myTFT.drawEllipse(60, 60, 18, 30, true, myTFT.C_WHITE);
	myTFT.drawEllipse(120, 60, 18, 30, true, myTFT.C_RED);
	DisplayReset();
}

void Test909(void) {
	printf("Test 909 : Drawing Arc: drawArc\n");
	myTFT.clearBuffer(myTFT.C_BLACK);
	int16_t centerX = 80;  // X-coordinate of the circle center
	int16_t centerY = 80;  // Y-coordinate of the circle center
	int16_t radius = 50;    // Radius of the circle
	printf(" Angle offset default : %i \n", myTFT.getArcAngleOffset());
	//drawArc(cx,  cy,  radius, thickness, startAngle, endAngle, color);
	// _arcAngleOffset is zero default means 0 is positive Axis , Arc drawn counterclockwise
	// XXX 270 XXX
	// 180 XXX 000
	// XXX 090 XXX
	// Draw the full lower half (0° to 180°)
	myTFT.drawArc(centerX, centerY, radius, 7, 0.0f, 180.0f, myTFT.C_GREEN);
	// Draw the full upper half (180° to 360°)
	myTFT.drawArc(centerX, centerY, radius, 10, 180.0f, 360.0f, myTFT.C_BLUE);
	DisplayReset();
	// Draw the first quarter (0° to 90°)
	myTFT.drawArc(centerX, centerY, radius, 2, 0.0f, 90.0f, myTFT.C_RED);
	// Draw the second quarter (90° to 180°)
	myTFT.drawArc(centerX, centerY, radius, 2, 90.0f, 180.0f, myTFT.C_YELLOW);
	// Draw the third quarter (180° to 270°)
	myTFT.drawArc(centerX, centerY, radius, 2, 180.0f, 270.0f, myTFT.C_CYAN);
	// Draw the fourth quarter (270° to 360°)
	myTFT.drawArc(centerX, centerY, radius, 2, 270.0f, 360.0f, myTFT.C_MAGENTA);
	DisplayReset();
}

#endif

void DisplayReset(void)
{
	myTFT.writeBuffer();
	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.clearBuffer(myTFT.C_BLACK);
}


/// @endcond
