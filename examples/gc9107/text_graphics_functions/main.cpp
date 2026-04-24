/*! 
	@file   main.cpp
	@brief  Example cpp file for gc9107 display Library test file, tests : Text,graphics & functions.
	@author Gavin Lyons.
	@details
			Setup for 60x160 display.
			For graphics after tests 905 to work 
			dislib16_ADVANCED_GRAPHICS_ENABLE must be commented in.
			Its in displaylib_16_graphics.hpp
	@note   See USER OPTIONS 0-2 in SETUP function

	@test 
	-# Test 500 RGB color OK?
	-# Test 501 Scroll test
	-# Test 502 Rotate
	-# Test 503 change modes test -> Invert, display on/off and Sleep.
	-# Test 701 Print out all fonts with writeCharString
	-# Test 702 Print out all fonts with print
	-# Test 703 Misc print class tests(string object, println invert, base nums etc)
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
#include "displaylib_16/gc9107.hpp"

/// @cond

//  Test timing related defines
#define TEST_DELAY5 5000
#define TEST_DELAY2 2000
#define TEST_DELAY  1000
#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif

// Screen Values Setup
// Size of VRAM (note 60x160 display has VRAM of 128x160)
uint16_t TFT_VRAM_WIDTH  = 128; // VRAM width  size in pixels
uint16_t TFT_VRAM_HEIGHT = 160; // VRAM height size in pixels
// Offsets for portrait and landscape rotations, see readme for values calculation
#define X_OFFSET 34 // Offset for portrait  rotation  0 and 180
#define Y_OFFSET 34 // Offset for landscape rotation 90 and 270
// Init display class object
GC9107_TFT myTFT;
// GRAM memory base mapping resolution, see GC9107_TFT::GM_memory_base_e enum
GC9107_TFT::GM_memory_base_e TFTmemoryBase = GC9107_TFT::GM_memory_base_e::MEMORY_BASE_GM_128x160;

//  Function Headers 
void Setup(void); // setup + user options for hardware SPI
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
	Setup();
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

void Setup(void)
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
	int8_t CS_TFT = 2 ;  
	int8_t RST_TFT = 4;
	myTFT.TFTsetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
// ***
// *** USER OPTION 2 Screen Setup ***
	myTFT.TFTInitScreen(TFT_VRAM_WIDTH , TFT_VRAM_HEIGHT, TFTmemoryBase, GC9107_TFT::MADCTL_FLAGS_t::RGB);
// ***
	myTFT.TFTGC9107Initialize(); 
}

void EndTests(void)
{
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.TFTPowerDown(); // Power down device
	printf("End\r\n");
}

void Test500(void)
{
	printf("Test 500: Color Test:: Red,green,blue,yellow,white, black background\n");
	myTFT.setFont(font_mega);
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setTextColor(myTFT.C_GREEN,myTFT.C_BLACK);
	ColorBlock();
	busy_wait_ms(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void ColorBlock(void)
{
	myTFT.fillRoundRect(X_OFFSET    , 100, 12, 60, 4, myTFT.C_RED);
	myTFT.fillRoundRect(X_OFFSET + 12, 100, 12, 60, 4, myTFT.C_GREEN);
	myTFT.fillRoundRect(X_OFFSET + 24, 100, 12, 60, 4, myTFT.C_BLUE);
	myTFT.fillRoundRect(X_OFFSET + 36, 100, 12, 60, 4, myTFT.C_YELLOW);
	myTFT.fillRoundRect(X_OFFSET + 48, 100, 12, 60, 4, myTFT.C_WHITE);
}

void Test501(void)
{
	printf("Test 501: Scroll\n");
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setFont(font_default);
	char teststr0[] = "Scroll";
	myTFT.writeCharString(X_OFFSET, 100, teststr0);

	const uint16_t screenH = 160;
	const uint16_t scrollArea = screenH - 0; // no fixed areas
	myTFT.TFTsetScrollArea(0, scrollArea, 0);

	for (uint16_t y = 0; y < screenH; y++) {
		myTFT.TFTsetScrollStart(y);
		sleep_ms(20);
	}
	myTFT.TFTsetScrollStart(0); // reset
	myTFT.TFTScrollModeLeave(); // leave scroll mode
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test502()
{
	printf("Test 502: Rotate\n");
	myTFT.setFont(font_retro);
	char teststr0[] = "R 0"; //normal
	char teststr1[] = "R 90"; // 90
	char teststr2[] = "R 180"; // 180
	char teststr3[] = "R 270"; // 270

	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.TFTsetRotation(myTFT.Degrees_0);
	myTFT.writeCharString(X_OFFSET, 0, teststr0);
	myTFT.writeCharString(X_OFFSET, 140, teststr0);
	busy_wait_ms(TEST_DELAY2);

	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.TFTsetRotation(myTFT.Degrees_90);
	myTFT.writeCharString(0,   Y_OFFSET, teststr1);
	myTFT.writeCharString(0,   68, teststr1);
	busy_wait_ms(TEST_DELAY2);

	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.TFTsetRotation(myTFT.Degrees_180);
	myTFT.writeCharString(X_OFFSET,    0, teststr2);
	myTFT.writeCharString(X_OFFSET,  140, teststr2);
	busy_wait_ms(TEST_DELAY2);

	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.TFTsetRotation(myTFT.Degrees_270);
	myTFT.writeCharString(0, Y_OFFSET, teststr3);
	myTFT.writeCharString(0, 68, teststr3);
	busy_wait_ms(TEST_DELAY2);

	myTFT.TFTsetRotation(myTFT.Degrees_0);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test503()
{
	printf("Test 503: Mode Tests\n");
	char teststr1[] = "Invert";
	char teststr2[] = "Display";
	char teststr3[] = "Sleep  ";
	ColorBlock();
	myTFT.writeCharString(X_OFFSET, 32, teststr1);
	busy_wait_ms(TEST_DELAY2);

	// Invert on and off
	myTFT.TFTchangeInvertMode(false);
	printf("Test 503-1: Invert on \n");
	busy_wait_ms(TEST_DELAY5);
	myTFT.TFTchangeInvertMode(true);
	printf("Test 503-2: Invert off \n");
	busy_wait_ms(TEST_DELAY5);

	// Display on and off
	myTFT.writeCharString(X_OFFSET, 32, teststr2);
	busy_wait_ms(TEST_DELAY2);
	myTFT.TFTenableDisplay(false);
	printf("Test 503-3: Display off\n");
	busy_wait_ms(TEST_DELAY2);
	myTFT.TFTenableDisplay(true);
	printf("Test 503-4: Turn Display back on\n");
	busy_wait_ms(TEST_DELAY2);
	// sleep mode on and off
	myTFT.writeCharString(X_OFFSET, 32, teststr3);
	busy_wait_ms(TEST_DELAY2); 
	// 1. sleep mode
	myTFT.TFTsetPowerMode(GC9107_TFT::PowerState_e::SleepNormalIdleOff);
	printf("Test 503-7: Power mode Sleep ON Idle Off\n");
	busy_wait_ms(TEST_DELAY5);
	//back to sleep off - normal on - idle off
	myTFT.TFTsetPowerMode(GC9107_TFT::PowerState_e::NormalIdleOff);	
	busy_wait_ms(TEST_DELAY2);
}



void Test701(void) {

	printf("Test 701: Print out all fonts with writeCharString\n");
	char teststr1[] = "Default";
	char teststr2[] = "GLL";
	char teststr3[] = "Pico";
	char teststr6[] = "Retro";
	myTFT.fillScreen(myTFT.C_BLACK);
	// Start at ~20px Y offset and use ~20px spacing
	int startY = 0;
	int spacing = 20;
	int x = X_OFFSET + 5;
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
	myTFT.setFont(font_retro);
	myTFT.writeCharString(x, y, teststr6);
	DisplayReset();
}

void Test702(void)
{
	printf("Test 702: Print class methods\n");
	// Test Fonts default +  + pico+ sinclair + retro
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);
	int x = X_OFFSET + 5;
	int y = 0;
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
	myTFT.print("pico");
	myTFT.print(747);
	y += spacing;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_retro);
	myTFT.print("-3.14");
	DisplayReset();
	// --- Second Block ---
	y = 0;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_mega);
	myTFT.print("meg");
	DisplayReset();
	// --- Third Block ---
	y = 0;
	myTFT.setFont(font_orla);
	myTFT.setCursor(x, y);
	myTFT.print("OA");
	y += spacing;
	myTFT.setCursor(x, y + spacing);
	myTFT.setFont(font_arialRound);
	myTFT.print(-8);
	DisplayReset();
	// --- Final Block ---
	y = 0;
	myTFT.setCursor(x, y);
	myTFT.setFont(font_groTesk);
	myTFT.println("GRO");
	DisplayReset();
}

void Test703(void)
{
	printf("Test 703: Misc print class(string object, println invert, base nums etc)\n");
	// --- Inverted print fonts 1–6 ---
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setTextColor(myTFT.C_RED, myTFT.C_YELLOW);
	myTFT.setFont(font_sinclairS);
	myTFT.setCursor(X_OFFSET, 20);
	myTFT.print(-49);
	DisplayReset();
	// --- Inverted print ---
	myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_RED);
	myTFT.setFont(font_default);
	myTFT.setCursor(X_OFFSET, 40);
	myTFT.print("INV");
	DisplayReset();
	// --- Test std::string print and wrapping ---
	myTFT.setFont(font_default);
	std::string timeInfo = "12:45";
	myTFT.setCursor(X_OFFSET, 60);
	myTFT.print(timeInfo);
	DisplayReset();
}

void Test902(void) {
	printf("Test 902: rectangles \n");
	DisLib16::Ret_Codes_e returnValue;
	myTFT.drawRectWH(X_OFFSET + 5, 5, 20, 20, myTFT.C_RED);
	returnValue = myTFT.fillRectBuffer(X_OFFSET + 5, 30, 20, 20, myTFT.C_YELLOW);
	if (returnValue != DisLib16::Success)
	{
		printf("Warning : Test TFTfillRectangle, An error occurred returnValue = %u \n", returnValue);
	}
	myTFT.fillRect(X_OFFSET + 5, 55, 20, 20, myTFT.C_GREEN);
	myTFT.drawRoundRect(X_OFFSET + 5, 80, 50, 50, 8, myTFT.C_CYAN);
	myTFT.fillRoundRect(X_OFFSET + 5, 135, 40, 20, 6, myTFT.C_WHITE);

	busy_wait_ms(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test903(void) {
	printf("Test 903 : circles\n");
	myTFT.drawCircle(X_OFFSET + 29, 55,  15, myTFT.C_GREEN);   // top circle
	myTFT.fillCircle(X_OFFSET + 29, 105, 15, myTFT.C_YELLOW);  // bottom circle
	busy_wait_ms(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test904(void) {
	printf("Test 904 : Triangles \n");
	// Triangle 1: pointing down, upper half
	myTFT.drawTriangle(
		X_OFFSET +  5, 20,   // top-left
		X_OFFSET + 55, 20,   // top-right
		X_OFFSET + 29, 70,   // bottom-centre
		myTFT.C_CYAN);

	// Triangle 2: pointing up, lower half
	myTFT.fillTriangle(
		X_OFFSET +  5, 145,  // bottom-left
		X_OFFSET + 55, 145,  // bottom-right
		X_OFFSET + 29, 90,   // top-centre
		myTFT.C_RED);

	busy_wait_ms(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

#ifdef dislib16_ADVANCED_GRAPHICS_ENABLE
void Test905(void)
{
	printf("Test 905 : polygons\n");
	myTFT.fillScreen(myTFT.C_BLACK);
	//drawPolygon( x,  y, sides,  diameter,  rotation, color);
	// octagon
	myTFT.drawPolygon(X_OFFSET+30, 80, 8, 20, 0, false, myTFT.C_GREEN);
	busy_wait_ms(TEST_DELAY5);
	// octagon
	myTFT.drawPolygon(X_OFFSET+30, 80, 8, 20, 0, true, myTFT.C_GREEN);
	busy_wait_ms(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test906(void) {
	printf("Test 906 : mesh\n");
	myTFT.fillScreen(myTFT.C_BLACK);
	// drawDotGrid(x, y, w, h, gap, color)
	myTFT.drawDotGrid(X_OFFSET + 1, 5, X_OFFSET + 59, 155, 5, myTFT.C_CYAN);
	busy_wait_ms(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test907(void) {
	printf("Test 907 : quadrilateral\n");
	// Shape 1: Rectangle (top)
	int16_t x0 = X_OFFSET +  5, y0 =  5;
	int16_t x1 = X_OFFSET + 50, y1 =  5;
	int16_t x2 = X_OFFSET + 50, y2 = 40;
	int16_t x3 = X_OFFSET +  5, y3 = 40;
	// Shape 2: Parallelogram (upper-middle)
	int16_t x4 = X_OFFSET + 20, y4 =  50;
	int16_t x5 = X_OFFSET + 55, y5 =  50;
	int16_t x6 = X_OFFSET + 40, y6 =  80;
	int16_t x7 = X_OFFSET +  5, y7 =  80;
	// Shape 3: Trapezoid wide-top (lower-middle)
	int16_t x8  = X_OFFSET +  5, y8  =  95;
	int16_t x9  = X_OFFSET + 55, y9  =  95;
	int16_t x10 = X_OFFSET + 45, y10 = 125;
	int16_t x11 = X_OFFSET + 15, y11 = 125;
	// Shape 4: Irregular quad (bottom)
	int16_t x12 = X_OFFSET +  5, y12 = 135;
	int16_t x13 = X_OFFSET + 50, y13 = 140;
	int16_t x14 = X_OFFSET + 45, y14 = 158;
	int16_t x15 = X_OFFSET + 10, y15 = 155;
	myTFT.drawQuadrilateral(x0,  y0,  x1,  y1,  x2,  y2,  x3,  y3,  myTFT.C_RED);
	myTFT.fillQuadrilateral(x4,  y4,  x5,  y5,  x6,  y6,  x7,  y7,  myTFT.C_GREEN);
	myTFT.fillQuadrilateral(x8,  y8,  x9,  y9,  x10, y10, x11, y11, myTFT.C_BLUE);
	myTFT.drawQuadrilateral(x12, y12, x13, y13, x14, y14, x15, y15, myTFT.C_YELLOW);
	busy_wait_ms(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test908(void) {
	printf("Test 908 : Ellipse\n");
	myTFT.fillScreen(myTFT.C_BLACK);
	// === Unfilled pass ===
	myTFT.drawEllipse(X_OFFSET + 15,  55, 12, 25, false, myTFT.C_WHITE);  // top-left
	myTFT.drawEllipse(X_OFFSET + 45,  55, 12, 25, false, myTFT.C_RED);    // top-right
	myTFT.drawEllipse(X_OFFSET + 15, 120, 14, 12, false, myTFT.C_BLUE);   // bottom-left
	myTFT.drawEllipse(X_OFFSET + 45, 120, 14, 12, false, myTFT.C_YELLOW); // bottom-right
	busy_wait_ms(TEST_DELAY5);
	// === Filled pass - same positions ===
	myTFT.drawEllipse(X_OFFSET + 15,  55, 12, 25, true, myTFT.C_WHITE);
	myTFT.drawEllipse(X_OFFSET + 45,  55, 12, 25, true, myTFT.C_RED);
	myTFT.drawEllipse(X_OFFSET + 15, 120, 14, 12, true, myTFT.C_BLUE);
	myTFT.drawEllipse(X_OFFSET + 45, 120, 14, 12, true, myTFT.C_YELLOW);
	busy_wait_ms(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test909(void) {
	printf("Test 909 : Drawing Arc: drawArc\n");
	myTFT.fillScreen(myTFT.C_BLACK);
	int16_t centerX = X_OFFSET+40;  // X-coordinate of the circle center
	int16_t centerY = 80;  // Y-coordinate of the circle center
	int16_t radius = 20;    // Radius of the circle
	printf(" Angle offset default : %i \n", myTFT.getArcAngleOffset());
	//drawArc(cx,  cy,  radius, thickness, startAngle, endAngle, color);
	// _arcAngleOffset is zero default means 0 is positive Axis , Arc drawn counterclockwise
	// XXX 270 XXX
	// 180 XXX 000
	// XXX 090 XXX
	// Draw the full lower half (0° to 180°)
	myTFT.drawArc(centerX, centerY, radius, 4, 0.0f, 180.0f, myTFT.C_GREEN);
	busy_wait_ms(TEST_DELAY2);
	myTFT.fillScreen(myTFT.C_BLACK);
	// Draw the full upper half (180° to 360°)
	myTFT.drawArc(centerX, centerY, radius, 6, 180.0f, 360.0f, myTFT.C_BLUE);
	busy_wait_ms(TEST_DELAY2);
	myTFT.fillScreen(myTFT.C_BLACK);
	// Draw the first quarter (0° to 90°)
	myTFT.drawArc(centerX, centerY, radius, 2, 0.0f, 90.0f, myTFT.C_RED);
	busy_wait_ms(TEST_DELAY);
	myTFT.fillScreen(myTFT.C_BLACK);
	// Draw the second quarter (90° to 180°)
	myTFT.drawArc(centerX, centerY, radius, 2, 90.0f, 180.0f, myTFT.C_YELLOW);
	busy_wait_ms(TEST_DELAY);
	myTFT.fillScreen(myTFT.C_BLACK);
	// Draw the third quarter (180° to 270°)
	myTFT.drawArc(centerX, centerY, radius, 2, 180.0f, 270.0f, myTFT.C_CYAN);
	busy_wait_ms(TEST_DELAY);
	myTFT.fillScreen(myTFT.C_BLACK);
	// Draw the fourth quarter (270° to 360°)
	myTFT.drawArc(centerX, centerY, radius, 2, 270.0f, 360.0f, myTFT.C_MAGENTA);
	busy_wait_ms(TEST_DELAY);
	myTFT.fillScreen(myTFT.C_BLACK);
}

#endif

void DisplayReset(void)
{
	busy_wait_ms(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}


/// @endcond
