/*!
	@file    gc9107.hpp
	@author  Gavin Lyons, LionTron Systems
	@brief   PICO Library header file, Contains driver methods for gc9107 display
*/

#pragma once

// Section Libraries
#include "displaylib_16_graphics.hpp"

/*! @brief Class to control GC9107 TFT basic functionality. */
class GC9107_TFT : public displaylib_16_graphics
{

public:
	GC9107_TFT();
	~GC9107_TFT() {};

	/*!
	 * @brief GC9107 power states based on the power control flow chart.
	 * These states combine the modes: Normal/Partial, Idle ON/OFF, and Sleep IN/OUT.
	 * They are listed from highest to lowest power consumption.
	 * 	Power states are based on the power control flow chart in the datasheet.
		5.9.2. Power Flow Chart
	 */
	enum class PowerState_e : uint8_t
	{
		NormalIdleOff,		 /**< SLPOUT + NORON + IDMOFF */
		NormalIdleOn,		 /**< SLPOUT + NORON + IDMON */
		PartialIdleOff,		 /**< SLPOUT + PLTON + IDMOFF */
		PartialIdleOn,		 /**< SLPOUT + PLTON + IDMON */
		SleepNormalIdleOff,	 /**< SLPIN + NORON + IDMOFF */
		SleepNormalIdleOn,	 /**< SLPIN + NORON + IDMON */
		SleepPartialIdleOff, /**< SLPIN + PLTON + IDMOFF */
		SleepPartialIdleOn	 /**< SLPIN + PLTON + IDMON */
	};

	/*!
	 * @brief MADCTL bit flags for register MADCTL (0x36).
	 */
	struct MADCTL_FLAGS_t
	{
		static constexpr uint8_t MY = 0x80;	 /**< Row Address Order: bottom to top */
		static constexpr uint8_t MX = 0x40;	 /**< Column Address Order: right to left */
		static constexpr uint8_t MV = 0x20;	 /**< Row/Column Exchange (Transpose) */
		static constexpr uint8_t ML = 0x10;	 /**< Vertical Refresh Order: bottom to top */
		static constexpr uint8_t RGB = 0x00; /**< RGB color order */
		static constexpr uint8_t BGR = 0x08; /**< BGR color order */
	};

	/*! @brief Display data GRAM memory base mapping resolution options */
	enum class GM_memory_base_e : uint8_t
	{
		MEMORY_BASE_GM_128x128 = 1, /**< 128RGBx128 resolution (GM = “01”)*/
		MEMORY_BASE_GM_128x160 = 11 /**< 128RGBx160 resolution (GM = “11”)*/
	};

	virtual void setAddrWindow(uint16_t, uint16_t, uint16_t, uint16_t) override;

	void TFTsetupGPIO(int8_t rst, int8_t dc, int8_t cs, int8_t sclk, int8_t din);
	void TFTInitSPIType(uint32_t baudrate, spi_inst_t *spi);
	void TFTInitSPIType(uint16_t CommDelay);
	void TFTInitScreen(uint16_t w = 128, uint16_t h = 160,
						GM_memory_base_e memoryBase = GM_memory_base_e::MEMORY_BASE_GM_128x160,
						uint8_t colorOrder = MADCTL_FLAGS_t::RGB);
	void TFTsetPanelOffset(uint8_t colOffset, uint8_t rowOffset);
	void TFTGC9107Initialize(void);
	void TFTPowerDown(void);
	uint16_t TFTSwSpiGpioDelayGet(void);
	void TFTSwSpiGpioDelaySet(uint16_t);

	void TFTsetRotation(display_rotate_e r);
	void TFTchangeInvertMode(bool m);
	void TFTsetPowerMode(PowerState_e m);
	void TFTenableDisplay(bool m);
	void TFTsetScrollArea(uint16_t topFixed, uint16_t scrollArea, uint16_t bottomFixed);
	void TFTsetScrollStart(uint16_t vsp);
	void TFTScrollModeLeave(void);

private:
	void TFTHWSPIInitialize(void);
	void TFTResetPIN(void);
	void TFTInitSequence(void);
	void TFTGammaSet(void);

private:
	// Display

	PowerState_e _currentPowerState = PowerState_e::NormalIdleOff; /**< Hold current display power mode */
	GM_memory_base_e _memoryBase = GM_memory_base_e::MEMORY_BASE_GM_128x160; /**< GRAM memory base mapping resolution */
	uint8_t _colorOrder = MADCTL_FLAGS_t::RGB; /**< Color order , default RGB ,RGB or BGR*/
	bool _displayOn = false; /**< Enum to hold display on/off status */
	const uint16_t _sleepDelay = 120; /**< Sleep delay in ms, datasheet 6.2.9.*/
	// SPI
	bool _resetPinOn = true; /**< reset pin on module ? true:hw rst pin, false:sw rt*/
	// Screen & RAM Size 
	uint16_t _widthStartTFT = 128;  /**< never change after first init */
	uint16_t _heightStartTFT = 160; /**< never change after first init */
	uint16_t _RAM_WIDTH  = 128;     /**< Physical VRAM width limit of the GC9107 controller */
	uint16_t _RAM_HEIGHT = 160;    /**< Physical VRAM height limit of the GC9107 controller */
	// Screen Offsets
	uint8_t _colOffset = 0;     /**< Portrait col (X) dead-RAM offset – user supplied */
	uint8_t _rowOffset = 0;     /**< Portrait row (Y) dead-RAM offset – user supplied */
	uint8_t _xstart = 0;        /**< Applied offset for current rotation (X axis) */
	uint8_t _ystart = 0;        /**< Applied offset for current rotation (Y axis) */

	// GC9107 registers + Commands
	static constexpr uint8_t GC9107_SLPIN            = 0x10; /**< Enter Sleep Mode */
	static constexpr uint8_t GC9107_SLPOUT           = 0x11; /**< Sleep Out */
	static constexpr uint8_t GC9107_PTLON            = 0x12; /**< Partial Mode ON */
	static constexpr uint8_t GC9107_NORON            = 0x13; /**< Normal Display Mode ON */
	static constexpr uint8_t GC9107_INVOFF           = 0x20; /**< Display Inversion OFF */
	static constexpr uint8_t GC9107_INVON            = 0x21; /**< Display Inversion ON */
	static constexpr uint8_t GC9107_DISPOFF          = 0x28; /**< Display OFF */
	static constexpr uint8_t GC9107_DISPON           = 0x29; /**< Display ON */
	static constexpr uint8_t GC9107_CASET            = 0x2A; /**< Column Address Set */
	static constexpr uint8_t GC9107_RASET            = 0x2B; /**< Row/page Address Set */
	static constexpr uint8_t GC9107_RAMWR            = 0x2C; /**< Memory Write */
	static constexpr uint8_t GC9107_PTLAR            = 0x30; /**< Partial Area */
	static constexpr uint8_t GC9107_VSCRDEF          = 0x33; /**< Vertical Scrolling Definition */
	static constexpr uint8_t GC9107_TEOFF            = 0x34; /**< Tearing Effect Line OFF */
	static constexpr uint8_t GC9107_TEON             = 0x35; /**< Tearing Effect Line ON */
	static constexpr uint8_t GC9107_MADCTL           = 0x36; /**< Memory Access Control */
	static constexpr uint8_t GC9107_VSCRSADD         = 0x37; /**< Vertical Scrolling Start Address */
	static constexpr uint8_t GC9107_IDLEOFF          = 0x38; /**< Idle mode OFF */
	static constexpr uint8_t GC9107_IDLEON           = 0x39; /**< Idle mode ON */
	static constexpr uint8_t GC9107_COLMOD           = 0x3A; /**< Pixel Format Set */
	static constexpr uint8_t GC9107_TEARSET          = 0x44; /**< Set Tear Scanline */
	static constexpr uint8_t GC9107_GETLINE          = 0x45; /**< Get Scanline */
	static constexpr uint8_t GC9107_FRAME_RATE_SET   = 0xA8; /**< Frame Rate Set */
	static constexpr uint8_t GC9107_RGB_COMP_PRIN    = 0xAC; /**< Complement Principle of RGB 5, 6, 5 */
	static constexpr uint8_t GC9107_BLANK_PORCH_CTRL = 0xAD; /**< Blanking Porch Control */
	static constexpr uint8_t GC9107_INVERSION        = 0xCB; /**< Display Inversion Control */
	static constexpr uint8_t GC9107_AVDD_VCL_SET     = 0xE2; /**< Set AVDD VCL */
	static constexpr uint8_t GC9107_AVDD_VCL_CLK     = 0xE3; /**< AVDD Set VCL Clock */
	static constexpr uint8_t GC9107_VGH_VGL_CLK      = 0xEA; /**< VGH Set VGL Clock */
	static constexpr uint8_t GC9107_VREG_CTL         = 0xE7; /**< VREG control */
	static constexpr uint8_t GC9107_VGH_SET          = 0xE8; /**< Set VGH */
	static constexpr uint8_t GC9107_VGL_SET          = 0xE9; /**< Set VGL */
	static constexpr uint8_t GC9107_INREGEN2         = 0xEF; /**< Inter register enable 2 */
	static constexpr uint8_t GC9107_GAMMA1           = 0xF0; /**< Set gamma 1 */
	static constexpr uint8_t GC9107_GAMMA2           = 0xF1; /**< Set gamma 2 */
	static constexpr uint8_t GC9107_INREGEN1         = 0xFE; /**< Inter register enable 1 */
}; // end of class

// ********************** EOF *********************
