/*! 
	@file   main.cpp
	@brief  Demo hardware SPI test, ssd1331 OLED
	@author Gavin Lyons.
	@note   See USER OPTIONS 0-2 in SETUP function and in myOLED constructor
			If dislib16_ADVANCED_GRAPHICS_ENABLE  is not enabled it will not compile
	@test 
		-# 401 Radar demo
*/

// Section ::  libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16/ssd1331.hpp"

/// @cond

#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif
#ifndef dislib16_ADVANCED_GRAPHICS_ENABLE 
	#pragma message("gll: dislib16_ADVANCED_GRAPHICS_ENABLE is not defined. it is required for this demo")
#endif
// Section :: setup device
// Set contrast values at startup, range 0-0xFF
SSD1331_OLED::Constrast_values_t userContrast = { 0x7A, 0x7A, 0x7A };        //normal mode
SSD1331_OLED::Dim_Constrast_values_t userDimContrast = { 0x1E, 0x1E, 0x1E }; // dim mode
SSD1331_OLED myOLED(SSD1331_OLED::COLORORDER_RGB, userContrast, userDimContrast);

//  Section ::  Function Headers 
void SetupHWSPI(void); // setup + user options for hardware SPI
void EndTests(void);
void DemoRadar(uint16_t sweeps);

//  *** Main ***
int main() 
{
	SetupHWSPI();
	DemoRadar(15);
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
	myOLED.InitScreenSize(OLED_WIDTH , OLED_HEIGHT);
	// ******************************************

	myOLED.SSD1331Initialize();
	busy_wait_ms(100);
}

void DemoRadar(uint16_t sweeps)
{
	printf("Demo Radar: Draw line at angle function, ends at: %u\n", sweeps);
	myOLED.setFont(font_default);
	myOLED.setTextColor(myOLED.C_BLACK, myOLED.C_TAN);
	myOLED.fillScreen(myOLED.C_TAN);
	const int centerX = 32;  
	const int centerY = 32;
	const int radarRadius = 30;
	const int lineLength = 28;
	int targetX = 24;
	int targetY = 20;
	// Draw radar circles
	myOLED.fillCircle(centerX, centerY, radarRadius, myOLED.C_BLACK);
	myOLED.drawCircle(centerX, centerY, radarRadius, myOLED.C_GREEN);
	myOLED.drawCircle(centerX, centerY, radarRadius - 1, myOLED.C_GREEN);
	uint16_t prevAngle = 0;
	uint16_t hitCount = 0;
	// Sweep loop
	for (uint16_t j = 0; j < sweeps; j++)
	{
		for (uint16_t angle = 0; angle < 360; angle += 2) 
		{
			// Erase previous line
			myOLED.drawLineAngle(centerX, centerY, prevAngle, 0, lineLength, -90, myOLED.C_BLACK);
			// Draw new radar line
			myOLED.drawLineAngle(centerX, centerY, angle, 0, lineLength, -90, myOLED.C_GREEN);
			if (j != 0){myOLED.fillCircle(targetX, targetY, 2, myOLED.C_RED);}
			// Check if the radar line hits the target
			if (angle == 320)
			{
				myOLED.fillCircle(targetX, targetY, 2, myOLED.C_WHITE); // Flash effect
				busy_wait_ms(50);
				hitCount++;
			}
			// Update angle display
			if ((angle % 2) == 0) 
			{
				myOLED.fillRectBuffer(71, 5, 28, 10, myOLED.C_TAN);
				myOLED.setCursor(71, 5);
				myOLED.print(angle);
			}
			// Display hit count
			myOLED.fillRectBuffer(71, 55, 40, 10, myOLED.C_TAN);
			myOLED.setCursor(71, 55);
			myOLED.print(hitCount);
			prevAngle = angle;
			busy_wait_ms(15);
		}
		printf("%u\r", j);
	}
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
