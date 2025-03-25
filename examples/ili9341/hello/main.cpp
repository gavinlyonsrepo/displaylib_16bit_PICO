/*! 
	@file   main.cpp
	@brief  Hello World basic use case
	@author Gavin Lyons.
	@note   See USER OPTIONS 0-2 in SETUP function
	@test 
		-# 101 Hello World Hardware SPI
*/

// Section ::  libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16/ili9341.hpp"

/// @cond

// Section :: Globals 
ILI9341_TFT myTFT;

//  Section ::  Function Headers 

void Setup(void); // setup + user options for hardware SPI
void HelloWorld(void);
void EndTests(void);

//  Section ::  MAIN loop

int main() 
{
	Setup();
	HelloWorld();
	EndTests();
	return 0;
}
// *** End OF MAIN **


//  Section ::  Function Space 

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
// the chosen interface eg Spi0 CLK=18 DIN=19 MISO = 17)
	int8_t SDIN_TFT = 19; 
	int8_t SCLK_TFT = 18; 
	int8_t RST_TFT = 5;
	int8_t DC_TFT = 3;
	int8_t CS_TFT = 2;  
	myTFT.SetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
//**********************************************************

// ****** USER OPTION 2 Screen Setup ****** 
	uint16_t TFT_WIDTH = 240;// Screen width in pixels
	uint16_t TFT_HEIGHT = 320; // Screen height in pixels
	myTFT.SetupScreenSize(TFT_WIDTH , TFT_HEIGHT);
// ******************************************

	myTFT.ILI9341Initialize(); 
}


void HelloWorld(void) 
{
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
	myTFT.setFont(font_groTesk);
	myTFT.setCursor(0,15);
	myTFT.println(" Hello ");
	myTFT.print(" World!");
	busy_wait_ms(7000);
}

void EndTests(void)
{
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.PowerDown(); // Power down device
	printf("End\r\n");
}

// *************** EOF ****************

/// @endcond
