/*!
	@file     st7735.hpp
	@author   Gavin Lyons
	@brief    Library header file for st7735 display
*/

#pragma once

// Section Libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16_graphics.hpp"

/*! @brief Class to control ST7735 TFT basic functionality. */
class ST7735_TFT : public displaylib_16_graphics
{

public:
	ST7735_TFT();
	~ST7735_TFT(){};

	//  Enums
	/*! TFT display modes */
	enum TFT_modes_e : uint8_t
	{
		TFT_Normal_mode = 0, /**< In this mode, the display is able to show maximum 262*/
		TFT_Partial_mode,	 /**< In this mode part of the display is used with maximum 262*/
		TFT_Idle_mode,		 /**< In this mode, part of the display is used but with 8 colors.*/
		TFT_Sleep_mode,		 /**<  In this mode, the DC: DC converter, internal oscillator and panel driver circuit are stopped. Only the MCU interface and
							 memory works with VDDI power supply. Contents of the memory are safe.*/
		TFT_Invert_mode,	 /**< Invert display colors */
		TFT_Display_on_mode, /**< Display On mode*/
		TFT_Display_off_mode /**< In this mode, both VDD and VDDI are removed.*/
	};

	/*! TFT type type of PCB */
	enum TFT_PCBtype_e : uint8_t
	{
		TFT_ST7735R_Red = 0, /**<  ST7735R Red Tab  */
		TFT_ST7735R_Green,	 /**<  ST7735R Green Tab */
		TFT_ST7735S_Black,	 /**<  ST7735S Black Tab */
		TFT_ST7735B,		 /**<  ST7735B controller */
	};

	TFT_modes_e _currentMode = TFT_Normal_mode;                   /**< Enum to hold display mode */
	TFT_PCBtype_e TFT_PCBtype = TFT_ST7735R_Red;                  /**< Enum to hold TFT type  */
	display_rotate_e displayRotate = Degrees_0; /**< Enum to hold rotation */

	virtual void setAddrWindow(uint16_t, uint16_t, uint16_t, uint16_t) override;
	void setupGPIO(int8_t, int8_t, int8_t, int8_t, int8_t);
	void TFTInitScreenSize(uint8_t xOffset, uint8_t yOffset, uint16_t w, uint16_t h);
	void TFTInitPCBType(TFT_PCBtype_e);
	void TFTInitSPIType(uint32_t baudrate, spi_inst_t *spi);
	void TFTInitSPIType(uint16_t CommDelay);
	void TFTPowerDown(void);

	void setRotation(display_rotate_e r);
	void TFTchangeInvertMode(bool invertModeOn);
	void TFTchangeMode(TFT_modes_e m);
	void setScrollDefinition(uint8_t th, uint8_t tb, bool sd);
	void TFTVerticalScroll(uint8_t vsp);

	uint16_t TFTSwSpiGpioDelayGet(void);
	void TFTSwSpiGpioDelaySet(uint16_t);

private:
	void TFTResetPIN(void);
	void TFTSPIInitialize(void);
	void TFTST7735BInitialize(void);
	void TFTGreenTabInitialize(void);
	void TFTBlackTabInitialize(void);
	void TFTRedTabInitialize(void);
	void Rcmd1(void);
	void Rcmd2red(void);
	void Rcmd3(void);
	void Bcmd(void);
	void Rcmd2green(void);
	//screen 
	uint8_t _colstart = 0;        /**< Used to offset column in the event of defect at edge of screen */
	uint8_t _rowstart= 0;        /**< Used to offset row in the event of defect at edge of screen */
	uint16_t _widthStartTFT= 128; /**<  never change after first init */
	uint16_t _heightStartTFT= 128; /**< never change after first init */

	// ST7735 registers + Commands
	// ST7735 general purpose
	static constexpr uint8_t ST7735_NOP    = 0x00 ; /**< Non operation */
	static constexpr uint8_t ST7735_SWRESET= 0x01 ; /**< Soft Reset */
	static constexpr uint8_t ST7735_RDDID  = 0x04 ; /**< Read Display ID*/
	static constexpr uint8_t ST7735_RDDST  = 0x09 ; /**< Read Display Status*/

	// ST7735 Modes
	static constexpr uint8_t ST7735_SLPIN  = 0x10 ; /**< Sleep ON */
	static constexpr uint8_t ST7735_SLPOUT = 0x11 ; /**< Sleep OFF */
	static constexpr uint8_t ST7735_PTLON  = 0x12 ; /**< Partial mode */
	static constexpr uint8_t ST7735_NORON  = 0x13 ; /**< Normal Display */
	static constexpr uint8_t ST7735_INVOFF = 0x20 ; /**< Display invert off */
	static constexpr uint8_t ST7735_INVON  = 0x21 ; /**< Display Invert on */
	static constexpr uint8_t ST7735_DISPOFF= 0x28 ; /**< Display off */
	static constexpr uint8_t ST7735_DISPON = 0x29 ; /**< Display on */
	static constexpr uint8_t ST7735_TFT_Idle_modeON= 0x39 ; /**< Idle Mode ON */
	static constexpr uint8_t ST7735_TFT_Idle_modeOF = 0x38 ; /**< Idle Mode OFF */

	// ST7735 Addressing
	static constexpr uint8_t ST7735_CASET  = 0x2A ; /**< Column address set */
	static constexpr uint8_t ST7735_RASET  = 0x2B ; /**<  Page address set */
	static constexpr uint8_t ST7735_RAMWR  = 0x2C ; /**< Memory write */
	static constexpr uint8_t ST7735_RAMRD  = 0x2E ; /**< Memory read */
	static constexpr uint8_t ST7735_PTLAR  = 0x30 ; /**< Partial Area */
	static constexpr uint8_t ST7735_VSCRDEF= 0x33 ; /**< Vertical scroll def */
	static constexpr uint8_t ST7735_COLMOD = 0x3A ; /**< Interface Pixel Format */
	static constexpr uint8_t ST7735_MADCTL = 0x36 ; /**< Memory Access Control */
	static constexpr uint8_t ST7735_VSCRSADD= 0x37 ; /**< Vertical Access Control */

	// Frame Rate Control
	static constexpr uint8_t ST7735_FRMCTR1= 0xB1 ; /**< Normal */
	static constexpr uint8_t ST7735_FRMCTR2= 0xB2 ; /**< idle */
	static constexpr uint8_t ST7735_FRMCTR3= 0xB3 ; /**< Partial */

	static constexpr uint8_t ST7735_INVCTR = 0xB4 ; /**< Display Inversion control */
	static constexpr uint8_t ST7735_DISSET5= 0xB6 ; /**< Display Function set */

	// ST7735 Power_Control
	static constexpr uint8_t ST7735_PWCTR1 = 0xC0 ; /**< Power_Control VRH: Set the GVDD voltage */
	static constexpr uint8_t ST7735_PWCTR2 = 0xC1 ; /**< Power_Control BT: set VGH/ VGL voltage */
	static constexpr uint8_t ST7735_PWCTR3 = 0xC2 ; /**< Power_Control APA: adjust op amp & DCA: boost Voltage */
	static constexpr uint8_t ST7735_PWCTR4 = 0xC3 ; /**< Power_Control APB: adjust op amp & DCB: boost Voltage */
	static constexpr uint8_t ST7735_PWCTR5 = 0xC4 ; /**< Power_Control APC: adjust op amp & DCC: boost Voltage */
	static constexpr uint8_t ST7735_PWCTR6 = 0xFC ; /**< Power_Control In partial mode + Idle */

	static constexpr uint8_t ST7735_VMCTR1 = 0xC5 ; /**< VCOM_Control 1 */

	static constexpr uint8_t ST7735_RDID1  = 0xDA ; /**< read ID1 */
	static constexpr uint8_t ST7735_RDID2  = 0xDB ; /**< read ID2  */
	static constexpr uint8_t ST7735_RDID3  = 0xDC ; /**< read ID3  */
	static constexpr uint8_t ST7735_RDID4  = 0xDD ; /**< read ID4  */

	// ST7735 color control
	static constexpr uint8_t ST7735_GMCTRP1= 0xE0 ; /**< Positive Gamma Correction Setting */
	static constexpr uint8_t ST7735_GMCTRN1= 0xE1 ; /**< Negative Gamma Correction Setting */

	// Memory Access Data Control  Register
	static constexpr uint8_t ST7735_MADCTL_MY= 0x80 ; /**< Row Address Order */
	static constexpr uint8_t ST7735_MADCTL_MX= 0x40 ; /**< Column Address Order */
	static constexpr uint8_t ST7735_MADCTL_MV= 0x20 ; /**< Row/Column Order (MV) */
	static constexpr uint8_t ST7735_MADCTL_ML= 0x10 ; /**< Vertical Refresh Order */
	static constexpr uint8_t ST7735_MADCTL_RGB= 0x00 ; /**< RGB order */
	static constexpr uint8_t ST7735_MADCTL_BGR= 0x08 ; /**< BGR order */
	static constexpr uint8_t ST7735_MADCTL_MH= 0x04  ; /**< Horizontal Refresh Order */

}; // end of class

// ********************** EOF *********************
