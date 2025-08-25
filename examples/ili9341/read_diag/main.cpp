/*! 
	@file   main.cpp
	@brief  Example file for ili9341 Library,  read diagnostics from Display
			Useful for diagnostics, debug and testing. outputs to console.
			at 38400 baud
	@author Gavin Lyons.
	@note   See USER OPTIONS 0-2 in SETUP function
	@details ILI9341 Read Diagnostics, Results from author's display :
		  -# Display Power Mode: 0x94
		  -# MADCTL Mode: 0x48
		  -# Pixel Format: 0x05
		  -# Image Format: 0x80
		  -# Self Diagnostic: 0xC0
	@test 
		-# Test 597 Read Diagnostics from display
*/

// Section ::  libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16/ili9341.hpp"

/// @cond

#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif

// Section :: Globals 
ILI9341_TFT myTFT;

//  Section ::  Function Headers 

void Setup(void);
void ReadDiags(void);
void EndTests(void);

//  Section ::  MAIN loop

int main() 
{
	Setup();
	ReadDiags();
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
	DisLib16::setDebugMode(true); // Enable debug mode for extra console messages
//*************** USER OPTION 0 SPI_SPEED + TYPE ***********
	bool bhardwareSPI = true; // true for hardware spi, false for software
	
	if (bhardwareSPI == true) { // hw spi
		uint32_t TFT_SCLK_FREQ =  1000 ; // Spi freq in KiloHertz , 1000 = 1Mhz
		myTFT.SetupSPI(TFT_SCLK_FREQ, spi0); 
	} else { // sw spi
		printf("Software SPI\r\n");
		uint16_t SWSPICommDelay = 0; // optional SW SPI GPIO delay in uS
		myTFT.SetupSPI(SWSPICommDelay);
	}
//*********************************************************
// ******** USER OPTION 1 GPIO *********
// NOTE if using Hardware SPI clock and data pins will be tied to 
// the chosen interface eg Spi0 CLK=18 DIN=19 MISO = 16)
	int8_t SDIN_TFT = 19; 
	int8_t SCLK_TFT = 18; 
	int8_t MISO_TFT = 16; // -1 if not used
	int8_t RST_TFT = 4;
	int8_t DC_TFT = 3;
	int8_t CS_TFT = 2;  
	myTFT.SetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT, MISO_TFT);
//**********************************************************

// ****** USER OPTION 2 Screen Setup ****** 
	uint16_t TFT_WIDTH = 240;// Screen width in pixels
	uint16_t TFT_HEIGHT = 320; // Screen height in pixels
	myTFT.SetupScreenSize(TFT_WIDTH , TFT_HEIGHT);
// ******************************************

	myTFT.ILI9341Initialize(); 
}

void ReadDiags(void) 
{
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
	myTFT.setFont(font_groTesk);
	myTFT.setCursor(0,15);
	myTFT.println(" Read");
	myTFT.print(" Diags");
	busy_wait_ms(1000);
	printf("= Read Diags =\n");
	uint8_t returnByte = 0xFF;
	// Read and print Display Power Mode
	returnByte = myTFT.readDiagByte(myTFT.ILI9341_RDMODE, 0);
	printf("Display Power Mode: 0x%02X\n", returnByte);
	busy_wait_ms(50);
	// Read and print MADCTL Mode
	returnByte = myTFT.readDiagByte(myTFT.ILI9341_RDMADCTL, 0);
	printf("MADCTL Mode: 0x%02X\n", returnByte);
	busy_wait_ms(50);
	// Read and print Pixel Format
	returnByte = myTFT.readDiagByte(myTFT.ILI9341_RDPIXFMT, 0);
	printf("Pixel Format: 0x%02X\n", returnByte);
	busy_wait_ms(50);
	// Read and print Image Format
	returnByte = myTFT.readDiagByte(myTFT.ILI9341_RDIMGFMT, 0);
	printf("Image Format: 0x%02X\n", returnByte);
	busy_wait_ms(50);
	// Read and print Self Diagnostic
	returnByte = myTFT.readDiagByte(myTFT.ILI9341_RDSELFDIAG, 0);
	printf("Self Diagnostic: 0x%02X\n", returnByte);
}

void EndTests(void)
{
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.PowerDown(); // Power down device
	printf("End\r\n");
}

// *************** EOF ****************

/// @endcond
