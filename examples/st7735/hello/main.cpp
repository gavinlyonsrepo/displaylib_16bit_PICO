/*!
	@file   main.cpp
	@author Gavin Lyons
	@brief  Example cpp file for st7735 library. Tests  Hello World
	@note   See USER OPTIONS 0-3 in SETUP function
	@test
		-# Test100 write out Hello world
*/

// Section ::  libraries 
#include "pico/time.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16/st7735.hpp"

/// @cond


// Section :: Globals 
ST7735_TFT myTFT;

//  Section ::  Function Headers 
void Setup(void);  // setup + user options
void Test100(void);  
void EndTests(void);

//  Section ::  MAIN loop
int main(void) 
{
	Setup();
	Test100();
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
	@brief print out hello world on TFT
*/
void Test100(void) {
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
	myTFT.setCursor(5,5);
	myTFT.setFont(font_default);
	myTFT.print("Hello World");
	MILLISEC_DELAY(5000);
	myTFT.fillScreen(myTFT.C_BLACK);
	MILLISEC_DELAY(1000);
}

/*!
	@brief  Stop testing and shutdown the TFT
*/
void EndTests(void)
{
	myTFT.TFTPowerDown(); 
	printf("TFT :: Tests Over");
}

// *************** EOF ****************

/// @endcond
