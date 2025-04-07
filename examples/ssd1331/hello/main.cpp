/*! 
	@file   main.cpp
	@brief  Hello World hardware SPI test, ssd1331 OLED
	@author Gavin Lyons.
	@note   See USER OPTIONS 0-2 in SETUP function and in myOLED constructor
	@test 
		-# 101 Hello World Hardware SPI
*/

// Section ::  libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16/ssd1331.hpp"

/// @cond
#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif
// Section :: setup device
// Set contrast values at startup, range 0-0xFF
SSD1331_OLED::Constrast_values_t userContrast = { 0x7D, 0x7D, 0x7D };        //normal mode
SSD1331_OLED::Dim_Constrast_values_t userDimContrast = { 0x1E, 0x1E, 0x1E }; // dim mode
SSD1331_OLED myOLED(SSD1331_OLED::COLORORDER_RGB, userContrast, userDimContrast);

//  Section ::  Function Headers 
void SetupHWSPI(void); // setup + user options for hardware SPI
void HelloWorld(void);
void EndTests(void);

//  *** Main ***
int main() 
{
	SetupHWSPI();
	HelloWorld();
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
		uint32_t _SCLK_FREQ =  8000 ; // Spi freq in KiloHertz , 1000 = 1Mhz
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

void HelloWorld(void) 
{
	myOLED.fillScreen(myOLED.C_BLACK);
	myOLED.setCursor(0,0);
	myOLED.setTextColor(myOLED.C_GREEN, myOLED.C_BLACK);
	myOLED.setFont(font_default);
	myOLED.print("Hello world");
	busy_wait_ms(7000);
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
