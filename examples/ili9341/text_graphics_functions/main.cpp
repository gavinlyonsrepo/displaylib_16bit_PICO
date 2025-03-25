/*! 
	@file   main.cpp
	@brief  Example cpp file for ili9341 display Library test file, tests Text,graphics & functions.
	@author Gavin Lyons.
	@details For this test file to work fully macro dislib16_ADVANCED_FONTS_ENABLE
			at top of  file displaylib_16_Font.hpp MUST be commented in.
			for graphics tests 905 plus to work 
			dislib16_ADVANCED_GRAPHICS_ENABLE must be commented in.
	@note   See USER OPTIONS 0-2 in SETUP function

	@test 
	-# Test 500 RGB color OK?
	-# Test 502 Rotate
	-# Test 503 change modes test -> Invert, display on/off and Sleep.
	-# Test 701 Print out all fonts with writeCharString
	-# Test 702 Print out all fonts with print
	-# Test 703 Misc print class tests(string object, println invert, wrap, base nums etc)
	-# Test 902 rectangles
	-# Test 903 Circle
	-# Test 904 Triangles
	-# Test 905 Polygons
	-# Test 906 Dot Gap
	-# Test 907 Quadrilateral
	-# Test 908 Ellipse
	-# Test 909 Draw Arc and draw simple arc
*/

// Section ::  libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16/ili9341.hpp"

/// @cond

//  Test timing related defines
#define TEST_DELAY5 5000
#define TEST_DELAY2 2000
#define TEST_DELAY  1000

uint16_t TFT_WIDTH = 240;// Screen width in pixels
uint16_t TFT_HEIGHT = 320; // Screen height in pixels
ILI9341_TFT myTFT;

//  Function Headers 
void Setup(void); // setup + user options for hardware SPI
void EndTests(void);
void DisplayReset(void);

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
	printf("Start\r\n");
//*************** USER OPTION 0 SPI_SPEED + TYPE ***********
	bool bhardwareSPI = true; // true for hardware spi, false for software
	
	if (bhardwareSPI == true) { // hw spi
		uint32_t TFT_SCLK_FREQ =  8000 ; // Spi freq in KiloHertz , 1000 = 1Mhz
		myTFT.SetupSPI(TFT_SCLK_FREQ, spi0); 
	} else { // sw spi
		uint16_t SWSPICommDelay = 0; // optional SW SPI GPIO delay in uS
		myTFT.SetupSPI(SWSPICommDelay);
	}
//*********************************************************
// ******** USER OPTION 1 GPIO *********
// NOTE if using Hardware SPI clock and data pins will be tied to 
// the chosen interface eg Spi0 CLK=18 DIN=19)
	int8_t SDIN_TFT = 19; 
	int8_t SCLK_TFT = 18; 
	int8_t RST_TFT = 4;
	int8_t DC_TFT = 3;
	int8_t CS_TFT = 2;  
	myTFT.SetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
//**********************************************************

// ****** USER OPTION 2 Screen Setup ****** 
	myTFT.SetupScreenSize(TFT_WIDTH , TFT_HEIGHT);
// ******************************************

	myTFT.ILI9341Initialize(); 
}

void EndTests(void)
{
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.PowerDown(); // Power down device
	printf("End\r\n");
}

void Test500(void)
{
	printf("Test 500: Color Test:: Red,green,blue,yellow,white, black background\n");
	myTFT.setFont(font_mega);
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setTextColor(myTFT.C_GREEN,myTFT.C_BLACK);
	myTFT.fillRoundRect(8, 10, 24, 60, 8, myTFT.C_RED);
	myTFT.fillRoundRect(32, 10, 24, 60, 8, myTFT.C_GREEN);
	myTFT.fillRoundRect(56, 10, 24, 60, 8, myTFT.C_BLUE);
	myTFT.fillRoundRect(80, 10, 24, 60, 8, myTFT.C_YELLOW);
	myTFT.fillRoundRect(104, 10, 24, 60, 8, myTFT.C_WHITE);
	busy_wait_ms(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test501(void)
{
	printf("Test 501: Scroll\n");
	myTFT.setFont(font_mega);
	myTFT.setCursor(20,120);
	myTFT.print("Scroll Test");
	myTFT.setScrollMargins(0, 0);
	for(uint16_t i =0 ; i < 100 ; i ++)
	{
		busy_wait_ms(50);
		myTFT.scrollTo(i);
	}
	busy_wait_ms(TEST_DELAY);
	myTFT.NormalMode();
}

void Test502()
{
	printf("Test 502: Rotate\n");
	myTFT.setFont(font_retro);
	char teststr0[] = "Rotate 0"; //normal
	char teststr1[] = "Rotate 90"; // 90
	char teststr2[] = "Rotate 180"; // 180
	char teststr3[] = "Rotate 270"; // 270

	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setRotation(myTFT.Degrees_0);
	myTFT.writeCharString(15, 15, teststr0);
	myTFT.writeCharString(15, 300, teststr0);
	busy_wait_ms(TEST_DELAY2);

	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setRotation(myTFT.Degrees_90);
	myTFT.writeCharString(15, 15, teststr1);
	myTFT.writeCharString(15, 220, teststr1);
	busy_wait_ms(TEST_DELAY2);

	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setRotation(myTFT.Degrees_180);
	myTFT.writeCharString(15, 15, teststr2);
	myTFT.writeCharString(15, 300, teststr2);
	busy_wait_ms(TEST_DELAY2);

	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setRotation(myTFT.Degrees_270);
	myTFT.writeCharString(15, 15, teststr3);
	myTFT.writeCharString(15, 220, teststr3);
	busy_wait_ms(TEST_DELAY2);

	myTFT.setRotation(myTFT.Degrees_0);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test503()
{
	printf("Test 503: Mode Tests\n");
	char teststr1[] = "Modes Test";
	myTFT.fillRoundRect(8, 10, 24, 60, 8, myTFT.C_RED);
	myTFT.fillRoundRect(32, 10, 24, 60, 8, myTFT.C_GREEN);
	myTFT.fillRoundRect(56, 10, 24, 60, 8, myTFT.C_BLUE);
	myTFT.fillRoundRect(80, 10, 24, 60, 8, myTFT.C_YELLOW);
	myTFT.fillRoundRect(104, 10, 24, 60, 8, myTFT.C_WHITE);
	myTFT.writeCharString(10, 80, teststr1);
	busy_wait_ms(TEST_DELAY2);

	// Invert on and off
	myTFT.invertDisplay(true);
	printf("Test 503-1: Invert on \n");
	busy_wait_ms(TEST_DELAY5);
	myTFT.invertDisplay(false);
	printf("Test 503-2: Invert off \n");
	busy_wait_ms(TEST_DELAY5);

	// Display on and off
	myTFT.EnableDisplay(false);
	printf("Test 503-3: Display off\n");
	busy_wait_ms(TEST_DELAY5);
	myTFT.EnableDisplay(true);
	printf("Test 503-4: Turn Display back on\n");
	busy_wait_ms(TEST_DELAY5);

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
	myTFT.setFont(font_groTesk);
	myTFT.writeCharString(5, 250, teststr11);
	DisplayReset();



	#ifdef dislib16_ADVANCED_FONTS_ENABLE

	char teststr12[] = "16";
	char teststr13[] = "7";
	char teststr14[] = "inco";
	char teststr15[] = "GB";
	char teststr16[] = "Mint";
	myTFT.setFont(font_sixteenSeg);
	myTFT.writeCharString(5,5, teststr12);

	myTFT.setFont(font_sevenSeg);
	myTFT.writeCharString(5, 85, teststr13);
	myTFT.setFont(font_inconsola);
	myTFT.writeCharString(5, 180, teststr14);
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

void Test902(void) {
	printf("Test 902: rectangles \n");
	DisLib16::Ret_Codes_e returnValue;
	myTFT.drawRectWH(25, 25, 20, 20, myTFT.C_RED);
	returnValue = myTFT.fillRectBuffer(85, 25, 20, 20, myTFT.C_YELLOW);
	if (returnValue != DisLib16::Success)
	{
		printf("Warning : Test TFTfillRectangle, An error occurred returnValue = %u \n", returnValue);
	}
	myTFT.fillRect(115, 25, 20, 20, myTFT.C_GREEN);
	myTFT.drawRoundRect(15, 100, 70, 70, 20, myTFT.C_CYAN);
	myTFT.fillRoundRect(120, 100, 50, 50, 10, myTFT.C_WHITE);

	busy_wait_ms(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test903(void) {
	printf("Test 903 & 904 : Triangles and circles\n");
	myTFT.drawCircle(40, 20, 15, myTFT.C_GREEN);
	myTFT.fillCircle(80, 20, 15, myTFT.C_YELLOW);
}

void Test904(void) {
	myTFT.drawTriangle(5, 80, 50, 40, 95, 80,myTFT.C_CYAN);
	myTFT.fillTriangle(55, 120, 100, 90, 127, 120, myTFT.C_RED);
	busy_wait_ms(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

#ifdef dislib16_ADVANCED_GRAPHICS_ENABLE
void Test905(void)
{
	printf("Test 905 : polygons\n");
	myTFT.fillScreen(myTFT.C_BLACK);
	//drawPolygon( x,  y, sides,  diameter,  rotation, color);
	// decagon
	myTFT.drawPolygon(50, 100, 10, 50, 0, false, myTFT.C_RED);
	// octagon
	myTFT.drawPolygon(150, 100, 8, 50, 0, false, myTFT.C_GREEN);
	// hexagon rotated
	myTFT.drawPolygon(50, 200, 6, 50, 45, false, myTFT.C_WHITE);
	// pentagon
	myTFT.drawPolygon(150, 200, 5, 50, 0, false, myTFT.C_TAN);
	busy_wait_ms(TEST_DELAY5);
	// decagon
	myTFT.drawPolygon(50, 100, 10, 50, 0, true, myTFT.C_RED);
	// octagon
	myTFT.drawPolygon(150, 100, 8, 50, 0, true, myTFT.C_GREEN);
	// hexagon rotated
	myTFT.drawPolygon(50, 200, 6, 50, 45, true, myTFT.C_WHITE);
	// pentagon
	myTFT.drawPolygon(150, 200, 5, 50, 0, true, myTFT.C_TAN);
	busy_wait_ms(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test906(void) {
	printf("Test 906 : mesh\n");
	myTFT.fillScreen(myTFT.C_BLACK);
	//drawDotGrid(x, y,  w,  h,  DotGridGap, color);
	myTFT.drawDotGrid(0, 0, 100,100, 5, myTFT.C_CYAN);
	myTFT.drawDotGrid(0, 200, 100,100, 10, myTFT.C_GREEN);
	busy_wait_ms(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test907() {
	printf("Test 907 : quadrilateral\n");
	// Define the four quadrilateral vertices for different shapes
	// First shape (quadrilateral )
	int16_t x0 = 10, y0 = 10; // First vertex
	int16_t x1 = 50, y1 = 10;
	int16_t x2 = 50, y2 = 50;
	int16_t x3 = 10, y3 = 50;
	// Second shape (Parallelogram )
	int16_t x4 = 180, y4 = 240;  // First vertex
	int16_t x5 = 230, y5 = 250;
	int16_t x6 = 220, y6 = 290;
	int16_t x7 = 170, y7 = 280;
	// Third shape (Trapezoid 1)
	int16_t x8 = 106, y8 = 107;  // First vertex
	int16_t x9 = 112, y9 = 128;
	int16_t x10 = 134, y10 = 128;
	int16_t x11 = 153, y11 = 107;
	// Fourth shape (Trapezoid 2)
	int16_t x12 = 16, y12 = 207;  // First vertex
	int16_t x13 = 29, y13 = 242;
	int16_t x14 = 85, y14 = 242;
	int16_t x15 = 46, y15 = 207;

	myTFT.drawQuadrilateral(x0, y0, x1, y1, x2, y2, x3, y3, myTFT.C_RED);
	myTFT.fillQuadrilateral(x4, y4, x5, y5, x6, y6, x7, y7, myTFT.C_GREEN);
	myTFT.fillQuadrilateral(x8, y8, x9, y9, x10, y10, x11, y11, myTFT.C_BLUE);
	myTFT.drawQuadrilateral(x12, y12, x13, y13, x14, y14, x15, y15, myTFT.C_YELLOW);
	busy_wait_ms(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test908(void) {
	printf("Test 908 : Ellipse\n");
	myTFT.fillScreen(myTFT.C_BLACK);
	// Define ellipses at different positions
	// Format: drawEllipse(centerX, centerY, semiMajorAxis, semiMinorAxis, fill, color)
	myTFT.drawEllipse(TFT_WIDTH / 4,TFT_HEIGHT / 4, 20, 40, false, myTFT.C_WHITE);  // Top-left
	myTFT.drawEllipse(3 * TFT_WIDTH / 4,TFT_HEIGHT / 4, 25, 50, false, myTFT.C_RED); // Top-right
	myTFT.drawEllipse(TFT_WIDTH / 4, 3 * TFT_HEIGHT / 4, 60, 30, false, myTFT.C_BLUE); // Bottom-left
	myTFT.drawEllipse(3 * TFT_WIDTH / 4, 3 * TFT_HEIGHT / 4, 50, 35, false, myTFT.C_YELLOW); // Bottom-right
	busy_wait_ms(TEST_DELAY5);
	myTFT.drawEllipse(TFT_WIDTH / 4,TFT_HEIGHT / 4, 20, 40, true, myTFT.C_WHITE);
	myTFT.drawEllipse(3 * TFT_WIDTH / 4,TFT_HEIGHT / 4, 25, 50, true, myTFT.C_RED);
	myTFT.drawEllipse(TFT_WIDTH / 4, 3 * TFT_HEIGHT / 4, 60, 30, true, myTFT.C_BLUE);
	myTFT.drawEllipse(3 * TFT_WIDTH / 4, 3 * TFT_HEIGHT / 4, 50, 35, true, myTFT.C_YELLOW);
	busy_wait_ms(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test909(void) {
	printf("Test 909 : Drawing Arc: drawArc and drawSimpleArc\n");
	myTFT.fillScreen(myTFT.C_BLACK);
	int16_t centerX = 120;  // X-coordinate of the circle center
	int16_t centerY = 160;  // Y-coordinate of the circle center
	int16_t radius = 50;    // Radius of the circle
	printf(" Angle offset default : %i \n", myTFT.getArcAngleOffset());
	//drawArc(cx,  cy,  radius, thickness, startAngle, endAngle, color);
	// _arcAngleOffset is zero default means 0 is positive Axis , Arc drawn counterclockwise
	// XXX 270 XXX
	// 180 XXX 000
	// XXX 090 XXX
	// Draw the full lower half (0° to 180°)
	myTFT.drawArc(centerX, centerY, radius, 7, 0.0f, 180.0f, myTFT.C_GREEN);
	busy_wait_ms(TEST_DELAY2);
	myTFT.fillScreen(myTFT.C_BLACK);
	// Draw the full upper half (180° to 360°)
	myTFT.drawArc(centerX, centerY, radius, 10, 180.0f, 360.0f, myTFT.C_BLUE);
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

	// drawSimpleArc :: Draw an arc with a radius of 50, from 0 to 300 degrees
	myTFT.drawSimpleArc(120, 160, 50, 0.0f, 300.0f, myTFT.C_GREEN);
	busy_wait_ms(TEST_DELAY2);
	myTFT.fillScreen(myTFT.C_BLACK);
}

#endif

void DisplayReset(void)
{
	busy_wait_ms(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}


/// @endcond
