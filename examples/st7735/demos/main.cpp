/*!
	@file   main.cpp
	@author Gavin Lyons
	@brief  Example cpp file for st7735 library. demo
	@note   See USER OPTIONS 0-3 in SETUP function
			If dislib16_ADVANCED_GRAPHICS_ENABLE  is not enabled it will not compile
	@test
		-# Test 408 gauge demo 
*/

// Section ::  libraries 
#include <random> // gauge
#include <algorithm> // For std::clamp
#include "pico/time.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16/st7735.hpp"

/// @cond


// Section :: Globals 
ST7735_TFT myTFT;

//  Section ::  Function Headers 
void Setup(void);  // setup + user options 
void EndTests(void);
// Demo 
void arcGauge(uint16_t count=100);
void drawPointerHelper(int16_t val, uint8_t x, uint8_t y, uint8_t r, uint16_t color);
void drawGaugeMarkers(uint8_t centerX, uint8_t centerY, uint8_t radius, int startAngle, int endAngle, float scaleFactor);
void drawPointer(int16_t &val, int16_t &oldVal , uint8_t x, uint8_t y, uint8_t r, uint16_t color, uint16_t bcolor);

//  Section ::  MAIN loop
int main(void) 
{
	#ifndef dislib16_ADVANCED_GRAPHICS_ENABLE 
		#pragma message("gll: dislib16_ADVANCED_GRAPHICS_ENABLE is not defined. it is required for this demo")
	#endif
	Setup();
	arcGauge(200);
	EndTests();
}
// *** End OF MAIN **

//  Section ::  Function Space 

/*!
	@brief setup the TFT :: user options 0-3
*/
void Setup(void)
{
	stdio_init_all(); // optional for error messages , Initialize chosen serial port, default 38400 baud
	MILLISEC_DELAY(1000);
	printf("TFT :: Start\r\n");
	
//*************** USER OPTION 0 SPI_SPEED + TYPE ***********
	bool bhardwareSPI = true; // true for hardware spi, false for software
	
	if (bhardwareSPI == true) { // hw spi
		uint32_t TFT_SCLK_FREQ =  8000 ; // Spi freq in KiloHertz , 1000 = 1Mhz
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
	uint8_t OFFSET_COL = 0;  // 2, These offsets can be adjusted for any issues->
	uint8_t OFFSET_ROW = 0; // 3, with screen manufacture tolerance/defects
	uint16_t TFT_WIDTH = 128;// Screen width in pixels
	uint16_t TFT_HEIGHT = 128; // Screen height in pixels
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT);
// ******************************************

// ******** USER OPTION 3 PCB_TYPE  **************************
	myTFT.TFTInitPCBType(myTFT.TFT_ST7735R_Red); // pass enum,4 choices,see README
//**********************************************************
}


/*!
	@brief  Stop testing and shutdown the TFT
*/
void EndTests(void)
{
	myTFT.TFTPowerDown(); 
	printf("TFT :: Tests Over");
}

//demo 4
void arcGauge(uint16_t countLimit)
{
	myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_BLACK);
	printf("Arc Demo ends at : %u\n", countLimit);
	int16_t currentValue = 150;
	int16_t oldValue = 150;
	myTFT.setFont(font_default);
	char buffer[10];
	// Draw Gauge
	uint16_t count = 1;
	uint8_t x = 64;
	uint8_t y = 64;
	uint8_t radius = 48;
	const int16_t minValue = 1;
	const int16_t maxValue = 255;
	//myTFT.drawArc(centerX, centerY, radius, 10, 180.0f, 360.0f, myTFT.C_BLUE);
	myTFT.drawArc(x, y, radius, 11,  150.0f, 180.0f, myTFT.C_RED);
	myTFT.drawArc(x, y, radius, 11,  181.0f, 250.0f, myTFT.C_ORANGE);
	myTFT.drawArc(x, y, radius, 11,  251.0f, 320.0f, myTFT.C_YELLOW);
	myTFT.drawArc(x, y, radius, 11,  321.0f, 30.0f, myTFT.C_GREEN);
	drawGaugeMarkers(x, y, radius, 150, 390, 1.3);
	drawGaugeMarkers(x, y, radius, 165, 375, 1.1);
	// Random number setup
	// Initialize random generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> stepDist(-10, 10); // Small step changes
	while(count++ < countLimit)
	{
		int step = stepDist(gen);
		currentValue += step;
		currentValue = std::clamp(currentValue, minValue, maxValue);
		myTFT.setCursor(10,100);
		if (oldValue != currentValue) {
			drawPointer(currentValue, oldValue,  x, y, radius, myTFT.C_GREEN, myTFT.C_BLACK);
			oldValue = currentValue;
		}
		myTFT.print(" Count :: ");
		myTFT.print(count);
		myTFT.setCursor(10,110);
		myTFT.print(" Value :: ");
		sprintf(buffer, "%03d", currentValue);
		myTFT.print(buffer);
	}
	myTFT.fillScreen(myTFT.C_BLACK);
	printf("Arc Gauge Demo Over \n");
}

void drawGaugeMarkers(uint8_t centerX, uint8_t centerY, uint8_t radius, int startAngle, int endAngle, float scaleFactor) 
{
	float angleRad, innerX, innerY, outerX, outerY;
	int angle;
	// Loop through the specified angle range, drawing ticks every 30 degrees
	for (angle = startAngle; angle <= endAngle; angle += 30) 
	{
		// Convert degrees to radians
		angleRad = angle * (std::numbers::pi / 180);
		// inner marker position
		innerX = cos(angleRad) * (radius / scaleFactor);
		innerY = sin(angleRad) * (radius / scaleFactor);
		// outer marker position
		outerX = cos(angleRad) * radius;
		outerY = sin(angleRad) * radius;
		// Draw marker line from inner to outer radius
		myTFT.drawLine(1 + centerX + innerX, 1 + centerY + innerY, 
					   1 + centerX + outerX, 1 + centerY + outerY, 
					   myTFT.C_WHITE);
	}
}

void drawPointer(int16_t &currentValue, int16_t &oldValue, uint8_t x, uint8_t y, uint8_t r, uint16_t colour, uint16_t bcolour) 
{
	uint8_t i;
	// If the current value is increasing
	if (currentValue > oldValue) 
	{
		// Incrementally move the pointer from oldValue to currentValue
		for (i = oldValue; i <= currentValue; i++) 
		{
			// Erase the previous pointer position
			drawPointerHelper(i - 1, x, y, r, bcolour);
			// Draw the pointer at the new position
			drawPointerHelper(i, x, y, r, colour);
			// Introduce a small delay for smooth animation
			if ((currentValue - oldValue) < 128) {
				busy_wait_ms(20);
			}else{
				busy_wait_ms(30); // Slower update for small changes
			}
		}
	}
	else // If the current value is decreasing
	{
		// Decrementally move the pointer from oldValue to currentValue
		for (i = oldValue; i >= currentValue; i--) 
		{
			// Erase the previous pointer position
			drawPointerHelper(i + 1, x, y, r, bcolour);
			// Draw the pointer at the new position
			drawPointerHelper(i, x, y, r, colour);
			// Adjust delay based on the difference to control animation speed
			if ((oldValue - currentValue) >= 128) {
				busy_wait_ms(20); // Faster update for large changes
			}else{
				busy_wait_ms(30); // Slower update for small changes
			}
		}
	}
}

void drawPointerHelper(int16_t value, uint8_t centerX, uint8_t centerY, uint8_t radius, uint16_t pointerColor) 
{
	float angleRad, pointerX, pointerY;
	int16_t minValue = 0;
	int16_t maxValue = 255;
	int startAngle = 150; // Gauge starting angle in degrees
	int endAngle = 240;   // Gauge ending angle in degrees
	// Ensure the value stays within the valid range
	if (value > maxValue) value = maxValue;
	if (value < minValue) value = minValue;
	// Map value to an angle in radians based on the gauge range
	angleRad = (((float)(uint16_t)(value - minValue) / (float)(uint16_t)(maxValue - minValue) * endAngle) 
				 + startAngle) * (std::numbers::pi / 180);
	// Calculate pointer tip coordinates
	pointerX = cos(angleRad) * (radius / 1.35);
	pointerY = sin(angleRad) * (radius / 1.35);
	// Draw the pointer line from the center to the calculated tip position
	myTFT.drawLine(centerX, centerY, 1 + centerX + pointerX, 1 + centerY + pointerY, pointerColor);
	// Draw a small circle to represent the pointer base
	myTFT.fillCircle(centerX, centerY, 2, pointerColor);
}
/// @endcond
