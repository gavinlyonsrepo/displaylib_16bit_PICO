/*!
	@file   gc9a01.hpp
	@author Gavin Lyons
	@brief  Library header file, Contains driver methods for gc9a01 display
	@todo gc9a01 :: investigate brightness mode, not working?
	@todo gc9a01 :: investigate idle mode on , some lines on screen missing?
*/

#pragma once

// Section Libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16_graphics.hpp"

/*! @brief Class to control GC9A01 TFT basic functionality. */
class GC9A01_TFT : public displaylib_16_graphics
{

public:
	GC9A01_TFT();
	~GC9A01_TFT() {};

	/*!
	 * @brief GC9A01 power states based on the power control flow chart.
	 * These states combine the modes: Normal/Partial, Idle ON/OFF, and Sleep IN/OUT.
	 * They are listed from highest to lowest power consumption.
	 * 	Power states are based on the power control flow chart in the datasheet.
		FIG 89 5.10.2. Power Flow Chart	
	 */
	enum class PowerState_e : uint8_t{
		NormalIdleOff,  	/**< SLPOUT + NORON + IDMOFF */
		NormalIdleOn,		/**< SLPOUT + NORON + IDMON */
		PartialIdleOff,		/**< SLPOUT + PLTON + IDMOFF */
		PartialIdleOn,		/**< SLPOUT + PLTON + IDMON */
		SleepNormalIdleOff,	/**< SLPIN + NORON + IDMOFF */
		SleepNormalIdleOn,	/**< SLPIN + NORON + IDMON */
		SleepPartialIdleOff,/**< SLPIN + PLTON + IDMOFF */
		SleepPartialIdleOn	/**< SLPIN + PLTON + IDMON */
	};
	
	virtual void setAddrWindow(uint16_t, uint16_t, uint16_t, uint16_t) override;

	void TFTsetupGPIO(int8_t rst, int8_t dc, int8_t cs, int8_t sclk, int8_t din);
	void TFTInitScreenSize(uint16_t w, uint16_t h);
	void TFTGC9A01Initialize(void);
	void TFTInitSPIType(uint32_t baudrate, spi_inst_t *spi);
	void TFTInitSPIType(uint16_t CommDelay);
	void TFTPowerDown(void);
	uint16_t TFTSwSpiGpioDelayGet(void);
	void TFTSwSpiGpioDelaySet(uint16_t);
	void TFTresetSWDisplay(void);

	void TFTsetRotation(display_rotate_e r);
	void TFTchangeInvertMode(bool m);
	void TFTsetPowerMode(PowerState_e m);
	void TFTenableDisplay(bool m);
	void TFTsetBrightness(uint8_t level);
	void TFTsetScrollArea(uint16_t topFixed, uint16_t scrollArea, uint16_t bottomFixed);
	void TFTsetScrollStart(uint16_t vsp);
	void TFTScrollModeLeave(void);

private:
	void TFTHWSPIInitialize(void);
	void TFTResetPIN(void);
	void cmdInitSequence(void);
	
	// Display 
	PowerState_e _currentPowerState = PowerState_e::NormalIdleOff; /**< Enum to hold display mode */
	bool _displayOn = false; /**< Enum to hold display on/off status */
	const uint16_t _sleepDelay = 120; /**< Sleep delay in ms */
	// SPI
	bool _resetPinOn = true; /**< reset pin? true:hw rst pin, false:sw rt*/
	// Screen
	uint8_t _colstart = 0;			/**< Used to offset column in the event of defect at edge of screen */
	uint8_t _rowstart = 0;			/**< Used to offset row in the event of defect at edge of screen */
	uint16_t _widthStartTFT = 240;	/**<  never change after first init */
	uint16_t _heightStartTFT = 240; /**< never change after first init */

	/*!
	 * @brief MADCTL bit flags for register GC9A01_MADCTL (0x36).
	 */
	struct MADCTL_FLAGS_t
	{
		static constexpr uint8_t MY = 0x80;	 /**< Row Address Order: bottom to top */
		static constexpr uint8_t MX = 0x40;	 /**< Column Address Order: right to left */
		static constexpr uint8_t MV = 0x20;	 /**< Row/Column Exchange (Transpose) */
		static constexpr uint8_t ML = 0x10;	 /**< Vertical Refresh Order: bottom to top */
		static constexpr uint8_t RGB = 0x00; /**< RGB color order */
		static constexpr uint8_t BGR = 0x08; /**< BGR color order */
		static constexpr uint8_t MH = 0x04;	 /**< Horizontal Refresh Order: right to left */
	};

	// GC9A01 registers + Commands
	static constexpr uint8_t GC9A01_SWRESET = 0x01; /**< Software Reset */
	static constexpr uint8_t GC9A01_RDDID = 0x04;	/**< Read display identification information */
	static constexpr uint8_t GC9A01_RDDST = 0x09;	/**< Read Display Status */

	static constexpr uint8_t GC9A01_SLPIN = 0x10;	/**< Enter Sleep Mode */
	static constexpr uint8_t GC9A01_SLPOUT = 0x11;	/**< Sleep Out */
	static constexpr uint8_t GC9A01_PTLON = 0x12;	/**< Partial Mode ON */
	static constexpr uint8_t GC9A01_NORON = 0x13;	/**< Normal Display Mode ON */
	static constexpr uint8_t GC9A01_DISPOFF = 0x28; /**< Display OFF */
	static constexpr uint8_t GC9A01_DISPON = 0x29;	/**< Display ON */
	static constexpr uint8_t GC9A01_IDLEOFF = 0x38; /**< Idle mode OFF */
	static constexpr uint8_t GC9A01_IDLEON = 0x39;	/**< Idle mode ON */

	static constexpr uint8_t GC9A01_INVOFF = 0x20;	  /**< Display Inversion OFF */
	static constexpr uint8_t GC9A01_INVON = 0x21;	  /**< Display Inversion ON */
	static constexpr uint8_t GC9A01_CASET = 0x2A;	  /**< Column Address Set */
	static constexpr uint8_t GC9A01_RASET = 0x2B;	  /**< Row Address Set */
	static constexpr uint8_t GC9A01_RAMWR = 0x2C;	  /**< Memory Write */
	static constexpr uint8_t GC9A01_PTLAR = 0x30;	  /**< Partial Area */
	static constexpr uint8_t GC9A01_VSCRDEF = 0x33;	  /**< Vertical Scrolling Definition */
	static constexpr uint8_t GC9A01_TEOFF = 0x34;	  /**< Tearing Effect Line OFF */
	static constexpr uint8_t GC9A01_TEON = 0x35;	  /**< Tearing Effect Line ON */
	static constexpr uint8_t GC9A01_MADCTL = 0x36;	  /**< Memory Access Control */
	static constexpr uint8_t GC9A01_VSCRSADD = 0x37;  /**< Vertical Scrolling Start Address */
	static constexpr uint8_t GC9A01_COLMOD = 0x3A;	  /**< Pixel Format Set */
	static constexpr uint8_t GC9A01_CONTINUE = 0x3C;  /**< Write Memory Continue */
	static constexpr uint8_t GC9A01_TEARSET = 0x44;	  /**< Set Tear Scanline */
	static constexpr uint8_t GC9A01_GETLINE = 0x45;	  /**< Get Scanline */
	static constexpr uint8_t GC9A01_SETBRIGHT = 0x51; /**< Write Display Brightness */
	static constexpr uint8_t GC9A01_SETCTRL = 0x53;	  /**< Write CTRL Display */

	static constexpr uint8_t GC9A01_TEWC = 0xBA;			 /**< Tearing effect width control */
	static constexpr uint8_t GC9A01_BLANK_PORCH_CTRL = 0xB5; /**< Blanking Porch Control (B5h) */
	static constexpr uint8_t GC9A01_FUNCTION_CTRL = 0xB6;	 /**< Display Function Control */

	static constexpr uint8_t GC9A01_POWER1 = 0xC1; /**< Power Control 1 */
	static constexpr uint8_t GC9A01_POWER2 = 0xC3; /**< Power Control 2 */
	static constexpr uint8_t GC9A01_POWER3 = 0xC4; /**< Power Control 3 */
	static constexpr uint8_t GC9A01_POWER4 = 0xC9; /**< Power Control 4 */
	static constexpr uint8_t GC9A01_POWER7 = 0xA7; /**< Power Control 7 */

	static constexpr uint8_t GC9A01_RDID1 = 0xDA; /**< Read ID 1 */
	static constexpr uint8_t GC9A01_RDID2 = 0xDB; /**< Read ID 2 */
	static constexpr uint8_t GC9A01_RDID3 = 0xDC; /**< Read ID 3 */

	static constexpr uint8_t GC9A01_FRAMERATE = 0xE8; /**< Frame rate control */
	static constexpr uint8_t GC9A01_SPI2DATA = 0xE9;  /**< SPI 2DATA control */
	static constexpr uint8_t GC9A01_INREGEN2 = 0xEF;  /**< Inter register enable 2 */

	static constexpr uint8_t GC9A01_GAMMA1 = 0xF0;	 /**< Set gamma 1 */
	static constexpr uint8_t GC9A01_GAMMA2 = 0xF1;	 /**< Set gamma 2 */
	static constexpr uint8_t GC9A01_GAMMA3 = 0xF2;	 /**< Set gamma 3 */
	static constexpr uint8_t GC9A01_GAMMA4 = 0xF3;	 /**< Set gamma 4 */
	static constexpr uint8_t GC9A01_IFACE = 0xF6;	 /**< Interface control */
	static constexpr uint8_t GC9A01_INREGEN1 = 0xFE; /**< Inter register enable 1 */
}; // end of class

// ********************** EOF *********************
