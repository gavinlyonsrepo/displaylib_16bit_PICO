/*!
	@file   main.cpp
	@author Gavin Lyons
	@brief  Example cpp file for st7789 driver. Demo.
	@note   See USER OPTIONS 0-2 in SETUP function
	@test
		-# Test 401 Gauge Demo 
*/

// Section ::  libraries 
#include "pico/time.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16/st7789.hpp"

///@cond

// Demo 
// Gauge properties
const int GAUGE_WIDTH = 20;
const int GAUGE_HEIGHT = 140;
const int GAUGE_X_START = 20;
const int GAUGE_Y_START = 40;
const int GAUGE_SPACING = 70;
void updateGauges(float t);
void drawGauge(int x, int y, uint16_t color, float value);
void drawGaugeDemo(uint16_t  countLimit = 50);
uint16_t generateColor(uint8_t value);
int mapValue(int x, int in_min, int in_max, int out_min, int out_max);

ST7789_TFT myTFT;

void Setup(void);  // setup + user options 
void EndTests(void);

//  Section ::  MAIN loop

int main(void) 
{
	Setup();
	drawGaugeDemo(200);
	EndTests();
}
// *** End OF MAIN **

//  Section ::  Function Space 

void Setup(void)
{
	stdio_init_all(); // optional for error messages , Initialize chosen serial port, default 38400 baud
	MILLISEC_DELAY(500);
	printf("TFT: Start\r\n");
	
//*************** USER OPTION 0 SPI_SPEED + TYPE ***********
	bool bhardwareSPI = true; // true for hardware spi, false for software
	
	if (bhardwareSPI == true) { // hw spi
		uint32_t TFT_SCLK_FREQ =  16000 ; // Spi freq in KiloHertz , 1000 = 1Mhz
		myTFT.TFTInitSPIType(TFT_SCLK_FREQ, spi0); 
	} else { // sw spi
		uint16_t SWSPICommDelay = 0; // optional SW SPI GPIO delay in uS
		myTFT.TFTInitSPIType(SWSPICommDelay);
	}
//*********************************************************
// ******** USER OPTION 1 GPIO *********
// NOTE if using Hardware SPI clock and data pins will be tied to 
// the chosen interface eg Spi0 CLK=18 DIN=19)
	int8_t SDIN_TFT = 19; 
	int8_t SCLK_TFT = 18; 
	int8_t DC_TFT = 3;
	int8_t CS_TFT = 2 ;  
	int8_t RST_TFT = 4;
	myTFT.setupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
//**********************************************************

// ****** USER OPTION 2 Screen Setup ****** 
	uint16_t OFFSET_COL = 0;  // 2, These offsets can be adjusted for any issues->
	uint16_t OFFSET_ROW = 0; // 3, with screen manufacture tolerance/defects
	uint16_t TFT_WIDTH = 240;// Screen width in pixels
	uint16_t TFT_HEIGHT = 320; // Screen height in pixels
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT);
// ******************************************

	myTFT.TFTST7789Initialize(); 
}

void EndTests(void)
{
	myTFT.TFTPowerDown(); 
	printf("TFT: End\n");
}

void drawGaugeDemo(uint16_t countLimit)
{
	printf("Demo ends at : %u\n", countLimit);
	myTFT.setFont(font_default);
	myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_BLACK);
	myTFT.fillScreen(myTFT.C_BLACK);
	float t = 0;
	while (countLimit-- > 1 ) 
	{
		updateGauges(t);
		t += 0.1;
		printf("%u\r", countLimit);
		busy_wait_ms(200);
	}
	myTFT.fillScreen(myTFT.C_BLACK);
	printf("Demo Over \n");
}

void drawGauge(int x, int y, uint16_t color, float value) {
	// Draw border
	myTFT.drawRectWH(x - 2, y - 2, GAUGE_WIDTH + 4, GAUGE_HEIGHT + 4, myTFT.C_WHITE);
	// Clear inside
	myTFT.fillRectBuffer(x, y, GAUGE_WIDTH, GAUGE_HEIGHT, myTFT.C_BLACK);
	// Draw fill
	int fillHeight = static_cast<int>(GAUGE_HEIGHT * value);
	if (fillHeight == 0) fillHeight =1;
	myTFT.fillRect(x, y + (GAUGE_HEIGHT - fillHeight), GAUGE_WIDTH, fillHeight, color);
}

void updateGauges(float t) {
	// Calculate values
	float val1 = (std::sin(t) + 1) / 2;
	float val2 = (std::cos(t) + 1) / 2;
	float val3 = ((std::sin(t) * std::cos(t)) + 1) / 2;
	// Convert float (0 to 1) to integer (1 to 127) range for color mapping
	uint8_t mappedVal1 = mapValue(static_cast<int>(val1 * 100), 0, 100, 1, 127);
	uint8_t mappedVal2 = mapValue(static_cast<int>(val2 * 100), 0, 100, 1, 127);
	uint8_t mappedVal3 = mapValue(static_cast<int>(val3 * 100), 0, 100, 1, 127);
	// Generate dynamic colors
	uint16_t color1 = generateColor(mappedVal1);
	uint16_t color2 = generateColor(mappedVal2);
	uint16_t color3 = generateColor(mappedVal3);

	// Draw gauges with dynamic colors
	drawGauge(GAUGE_X_START, GAUGE_Y_START, color1, val1);
	drawGauge(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START, color2, val2);
	drawGauge(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START, color3, val3);

	char buffer[6]; // To store formatted text
	sprintf(buffer, "%.2f", val1);
	myTFT.fillRectBuffer(GAUGE_X_START, GAUGE_Y_START + GAUGE_HEIGHT + 10, 40, 10, myTFT.C_BLACK); // Clear previous text
	myTFT.setCursor(GAUGE_X_START, GAUGE_Y_START + GAUGE_HEIGHT + 10);
	myTFT.print(buffer);
	sprintf(buffer, "%.2f", val2);
	myTFT.fillRectBuffer(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10, 40, 10, myTFT.C_BLACK);
	myTFT.setCursor(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10);
	myTFT.print(buffer);
	sprintf(buffer, "%.2f", val3);
	myTFT.fillRectBuffer(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10, 40, 10,  myTFT.C_BLACK);
	myTFT.setCursor(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10);
	myTFT.print(buffer);
}
/*!
	@brief This function is designed to return a color in the 16-bit 
		RGB format based on the input value, creating a smooth transition 
		between colors in different stages. 
	@details The gradient is calculated by 
		cycling through different color transitions as val increases, 
		allowing for smooth visual effects such as changing hues 
		or creating rainbow-like effects on an LCD. 1-127 will
		This will create a gradient color between blue and red based on the value.
*/
uint16_t generateColor(uint8_t value)
{
	uint8_t red = 0;
	uint8_t green = 0;
	uint8_t blue = 0;
	uint8_t segment = value / 32;
	switch(segment) 
	{
		case 0: red = 0; green = 2 * (value % 32); blue = 31; break;
		case 1: red = 0; green = 63; blue = 31 - (value % 32); break;
		case 2: red = value % 32; green = 63; blue = 0; break;
		case 3: red = 31; green = 63 - 2 * (value % 32); blue = 0; break;
	}
	return (red << 11) + (green << 5) + blue;
}

/**
 * @brief Maps a value from one range to another using linear interpolation.
 * This function takes an input value within a specified range and maps it
 * to a corresponding value in a different output range using std::lerp.
 * @param x The input value to map.
 * @param in_min The lower bound of the input range.
 * @param in_max The upper bound of the input range.
 * @param out_min The lower bound of the output range.
 * @param out_max The upper bound of the output range.
 * @return The mapped value in the output range.
 */
int mapValue(int x, int in_min, int in_max, int out_min, int out_max) 
{
	return static_cast<int>(std::lerp(out_min, out_max, (x - in_min) / static_cast<double>(in_max - in_min)));
}

///@endcond
// *************** EOF ****************
