/*!
	@file   st7789.hpp
	@author Gavin Lyons
	@brief  Library header file, Contains driver methods for st7789 display
*/

#pragma once

// Section Libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16_graphics.hpp"

/*! @brief Class to control ST7789 TFT basic functionality. */
class ST7789_TFT : public displaylib_16_graphics
{

public:
	ST7789_TFT();
	~ST7789_TFT(){};

	virtual void setAddrWindow(uint16_t, uint16_t, uint16_t, uint16_t) override;

	void setupGPIO(int8_t, int8_t, int8_t, int8_t, int8_t);
	void TFTInitScreenSize(uint16_t xOffset, uint16_t yOffset, uint16_t w, uint16_t h);
	void TFTST7789Initialize(void);
	void TFTInitSPIType(uint32_t baudrate, spi_inst_t *spi);
	void TFTInitSPIType(uint16_t CommDelay);
	void TFTPowerDown(void);

	void setRotation(display_rotate_e r);
	void TFTchangeInvertMode(bool m);
	void TFTpartialDisplay(bool m);
	void TFTenableDisplay(bool m);
	void TFTidleDisplay(bool m);
	void TFTsleepDisplay(bool m);
	void TFTNormalMode(void);

	uint16_t TFTSwSpiGpioDelayGet(void);
	void TFTSwSpiGpioDelaySet(uint16_t);

	void TFTresetSWDisplay(void);
    void setScrollDefinition(uint16_t th, uint16_t tb, bool sd);
	void TFTVerticalScroll(uint16_t vsp);

private:
	void TFTHWSPIInitialize(void);
	void TFTResetPIN(void);
	void cmd89(void);
	void AdjustWidthHeight(void);

	// Screen
	uint8_t _colstart = 0;          /**< Used to offset column in the event of defect at edge of screen */
	uint8_t _rowstart = 0;          /**< Used to offset row in the event of defect at edge of screen */
	uint8_t _colstart2 = 0;         /**< Offset from the right for ST7789*/
	uint8_t _rowstart2 = 0;         /**< Offset from the bottom for ST7789*/
	uint16_t _widthStartTFT = 240;  /**<  never change after first init */
	uint16_t _heightStartTFT = 320; /**< never change after first init */

	// ST7789 registers + Commands
	// ST7789 general purpose
	static constexpr uint8_t ST7789_NOP     = 0x00; /**< Non operation */
	static constexpr uint8_t ST7789_SWRESET = 0x01; /**< Soft Reset */
	// ST7789 Modes
	static constexpr uint8_t ST7789_SLPIN    = 0x10; /**< Sleep ON */
	static constexpr uint8_t ST7789_SLPOUT   = 0x11; /**< Sleep OFF */
	static constexpr uint8_t ST7789_PTLON    = 0x12; /**< Partial mode */
	static constexpr uint8_t ST7789_NORON    = 0x13; /**< Normal Display */
	static constexpr uint8_t ST7789_INVOFF   = 0x20; /**< Display invert off */
	static constexpr uint8_t ST7789_INVON    = 0x21; /**< Display Invert on */
	static constexpr uint8_t ST7789_DISPOFF  = 0x28; /**< Display off */
	static constexpr uint8_t ST7789_DISPON   = 0x29; /**< Display on */
	static constexpr uint8_t ST7789_IDLE_ON  = 0x39; /**< Idle Mode ON */
	static constexpr uint8_t ST7789_IDLE_OFF = 0x38; /**< Idle Mode OFF */
	// ST7789 Addressing
	static constexpr uint8_t ST7789_CASET    = 0x2A; /**< Column address set */
	static constexpr uint8_t ST7789_RASET    = 0x2B; /**<  Page address set */
	static constexpr uint8_t ST7789_RAMWR    = 0x2C; /**< Memory write */
	static constexpr uint8_t ST7789_RAMRD    = 0x2E; /**< Memory read */
	static constexpr uint8_t ST7789_PTLAR    = 0x30; /**< Partial Area */
	static constexpr uint8_t ST7789_VSCRDEF  = 0x33; /**< Vertical scroll def */
	static constexpr uint8_t ST7789_SRLBTT   = 0x28; /**< Scroll direction bottom to top */
	static constexpr uint8_t ST7789_SRLTTB   = 0x30; /**< Scroll direction top to bottom */
	static constexpr uint8_t ST7789_COLMOD   = 0x3A; /**< Interface Pixel Format */
	static constexpr uint8_t ST7789_MADCTL   = 0x36; /**< Memory Access Control */
	static constexpr uint8_t ST7789_VSCRSADD = 0x37; /**< Vertical Access Control */
	// Frame Rate Control
	static constexpr uint8_t ST7789_FRMCTR1 = 0xB1; /**< Normal */
	static constexpr uint8_t ST7789_FRMCTR2 = 0xB2; /**< idle */
	static constexpr uint8_t ST7789_FRMCTR3 = 0xB3; /**< Partial */
	static constexpr uint8_t ST7789_INVCTR  = 0xB4; /**< Display Inversion control */
	static constexpr uint8_t ST7789_DISSET5 = 0xB6; /**< Display Function set */
	static constexpr uint8_t ST7789_RDID1   = 0xDA; /**< read ID1 */
	static constexpr uint8_t ST7789_RDID2   = 0xDB; /**< read ID2 */
	static constexpr uint8_t ST7789_RDID3   = 0xDC; /**< read ID3 */
	static constexpr uint8_t ST7789_RDID4   = 0xDD; /**< read ID4 */
	// ST7789 color control
	static constexpr uint8_t ST7789_GMCTRP1 = 0xE0; /**< Positive Gamma Correction Setting */
	static constexpr uint8_t ST7789_GMCTRN1 = 0xE1; /**< Negative Gamma Correction Setting */
	// Memory Access Data Control  Register
	static constexpr uint8_t ST7789_MADCTL_MY  = 0x80; /**< Row Address Order */
	static constexpr uint8_t ST7789_MADCTL_MX  = 0x40; /**< Column Address Order */
	static constexpr uint8_t ST7789_MADCTL_MV  = 0x20; /**< Row/Column Order (MV) */
	static constexpr uint8_t ST7789_MADCTL_ML  = 0x10; /**< Vertical Refresh Order */
	static constexpr uint8_t ST7789_MADCTL_RGB = 0x00; /**< RGB order */
	static constexpr uint8_t ST7789_MADCTL_BGR = 0x08; /**< BGR order */
	static constexpr uint8_t ST7789_MADCTL_MH  = 0x04; /**< Horizontal Refresh Order */
}; // end of class

// ********************** EOF *********************
