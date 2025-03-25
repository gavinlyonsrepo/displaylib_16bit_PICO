/*!
	@file   ili9341.hpp
	@author Gavin Lyons
	@brief  header file to manage hardware interface with ILI9341 display
	@todo   Add support for print diagnostics and MISO pin , xpt2046
	@todo   Add support for touch pad controller xpt2046 IC.
*/

#pragma once

// Section Libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16_graphics.hpp"

/*!@brief Class to interface with ILI9341 LCD display*/
class ILI9341_TFT : public displaylib_16_graphics
{
public:
	ILI9341_TFT();
	~ILI9341_TFT(){};

	//Functions
	virtual void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) override;
	//Init Setup related
	void SetupGPIO(int8_t RST, int8_t DC, int8_t CS, int8_t SCLK, int8_t MOSI);
	void SetupScreenSize(uint16_t w, uint16_t h);
	void SetupSPI(uint16_t CommDelay); // SW SPI
	void SetupSPI(uint32_t baudrate, spi_inst_t *spi); // HW SPI
	void ILI9341Initialize(void);
	// SPI related
	uint16_t HighFreqDelayGet(void);
	void HighFreqDelaySet(uint16_t);
	void PowerDown(void);
	void EnableDisplay(bool enableDisplay);
	// screen related
	void setRotation(display_rotate_e rotation);
	void invertDisplay(bool i);
	void scrollTo(uint16_t y);
	void setScrollMargins(uint16_t top, uint16_t bottom);
	void NormalMode(void);
	void ResetPin(void);
	//void PrintDiagnostic(void); // @todo not working

protected:

private:
	void cmdInit(void);

	// SPI
	bool _resetPinOn = true; /**< reset pin? true:hw rst pin, false:sw rt*/
	// Screen 
	uint16_t _widthStartTFT = 240;  /**< never change after first init */
	uint16_t _heightStartTFT = 320; /**< never change after first init */
	
	// Registers & command Set
	static constexpr uint8_t ILI9341_NOP      = 0x00; /**< No-op register */
	static constexpr uint8_t ILI9341_SWRESET  = 0x01; /**< Software reset register */
	static constexpr uint8_t ILI9341_RDDID    = 0x04; /**< Read display identification information */
	static constexpr uint8_t ILI9341_RDDST    = 0x09; /**< Read Display Status */

	static constexpr uint8_t ILI9341_SLPIN   = 0x10; /**< Enter Sleep Mode */
	static constexpr uint8_t ILI9341_SLPOUT  = 0x11; /**< Sleep Out */
	static constexpr uint8_t ILI9341_PTLON   = 0x12; /**< Partial Mode ON */
	static constexpr uint8_t ILI9341_NORON   = 0x13; /**< Normal Display Mode ON */

	static constexpr uint8_t ILI9341_INVOFF     = 0x20; /**< Display Inversion OFF */
	static constexpr uint8_t ILI9341_INVON      = 0x21; /**< Display Inversion ON */
	static constexpr uint8_t ILI9341_GAMMASET   = 0x26; /**< Gamma Set */
	static constexpr uint8_t ILI9341_DISPOFF    = 0x28; /**< Display OFF */
	static constexpr uint8_t ILI9341_DISPON     = 0x29; /**< Display ON */

	static constexpr uint8_t ILI9341_CASET  = 0x2A; /**< Column Address Set */
	static constexpr uint8_t ILI9341_PASET  = 0x2B; /**< Page Address Set */
	static constexpr uint8_t ILI9341_RAMWR  = 0x2C; /**< Memory Write */
	static constexpr uint8_t ILI9341_RAMRD  = 0x2E; /**< Memory Read */

	static constexpr uint8_t ILI9341_PTLAR     = 0x30; /**< Partial Area */
	static constexpr uint8_t ILI9341_VSCRDEF   = 0x33; /**< Vertical Scrolling Definition */
	static constexpr uint8_t ILI9341_MADCTL    = 0x36; /**< Memory Access Control */
	static constexpr uint8_t ILI9341_VSCRSADD  = 0x37; /**< Vertical Scrolling Start Address */
	static constexpr uint8_t ILI9341_PIXFMT    = 0x3A; /**< COLMOD: Pixel Format Set */

	static constexpr uint8_t ILI9341_FRMCTR1  = 0xB1; /**< Frame Rate Ctrl (Normal Mode/Full Colors)  */
	static constexpr uint8_t ILI9341_FRMCTR2  = 0xB2; /**< Frame Rate Ctrl (Idle Mode/8 colors) */
	static constexpr uint8_t ILI9341_FRMCTR3  = 0xB3; /**< Frame Rate control (In Partial Mode/Full Colors) */
	static constexpr uint8_t ILI9341_INVCTR   = 0xB4; /**< Display Inversion Control */
	static constexpr uint8_t ILI9341_DFUNCTR  = 0xB6; /**< Display Function Control */

	static constexpr uint8_t ILI9341_PWCTR1  = 0xC0; /**< Power Control 1 */
	static constexpr uint8_t ILI9341_PWCTR2  = 0xC1; /**< Power Control 2 */
	static constexpr uint8_t ILI9341_PWCTR3  = 0xC2; /**< Power Control 3 */
	static constexpr uint8_t ILI9341_PWCTR4  = 0xC3; /**< Power Control 4 */
	static constexpr uint8_t ILI9341_PWCTR5  = 0xC4; /**< Power Control 5 */

	static constexpr uint8_t ILI9341_VMCTR1  = 0xC5; /**< VCOM Control 1 */
	static constexpr uint8_t ILI9341_VMCTR2  = 0xC7; /**< VCOM Control 2 */

	static constexpr uint8_t ILI9341_RDID1  = 0xDA; /**< Read ID 1 */
	static constexpr uint8_t ILI9341_RDID2  = 0xDB; /**< Read ID 2 */
	static constexpr uint8_t ILI9341_RDID3  = 0xDC; /**< Read ID 3 */
	static constexpr uint8_t ILI9341_RDID4  = 0xDD; /**< Read ID 4 */

	static constexpr uint8_t ILI9341_EN3GAM   = 0xF2; /**< Enable 3 gamma control*/
	static constexpr uint8_t ILI9341_GMCTRP1  = 0xE0; /**< Positive Gamma Correction */
	static constexpr uint8_t ILI9341_GMCTRN1  = 0xE1; /**< Negative Gamma Correction */

	static constexpr uint8_t ILI9341_PWONCS   = 0xED; /**< Power on sequence control */
	static constexpr uint8_t ILI9341_PWCTRA   = 0xCB; /**< Power Control A */
	static constexpr uint8_t ILI9341_PWCTRB   = 0xCF; /**< Power Control B */
	static constexpr uint8_t ILI9341_DTMCTRA  = 0xE8; /**< Driver timing control A*/
	static constexpr uint8_t ILI9341_DTMCTRC  = 0xEA; /**< Driver timing control C*/
	static constexpr uint8_t ILI9341_PURTCTR  = 0xF7; /**< Pump ratio control*/

	static constexpr uint8_t MADCTL_MY   = 0x80; /**< Bottom to top */
	static constexpr uint8_t MADCTL_MX   = 0x40; /**< Right to left */
	static constexpr uint8_t MADCTL_MV   = 0x20; /**< Reverse Mode */
	static constexpr uint8_t MADCTL_ML   = 0x10; /**< LCD refresh Bottom to top */
	static constexpr uint8_t MADCTL_RGB  = 0x00; /**< Red-Green-Blue pixel order */
	static constexpr uint8_t MADCTL_BGR  = 0x08; /**< Blue-Green-Red pixel order */
	static constexpr uint8_t MADCTL_MH   = 0x04; /**< LCD refresh right to left */
};

