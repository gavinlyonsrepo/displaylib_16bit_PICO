/*!
	@file     ssd1331.hpp
	@author   Gavin Lyons
	@brief    Library header file for ssd1331 OLED library.
*/

#pragma once

// Section Libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "displaylib_16_graphics.hpp"


/*!
	@brief Class to control SSD1331 OLED basic functionality.
*/
class SSD1331_OLED : public displaylib_16_graphics
{

public:
	/*!
		 @brief Set the Set time interval between each scroll
	*/
	enum scroll_time_interval_e : uint8_t{
		SCROLL_FRAME_6,     /**< 6 frames */
		SCROLL_FRAME_10,    /**< 10 frames */
		SCROLL_FRAME_100,   /**< 100 frames */
		SCROLL_FRAME_200    /**< 200 frames */
	};
	/*!
		 @brief Set the Scroll mode on or off according to the setting done by OLEDScrollSetup()
	*/
	enum scroll_control_e: uint8_t{
		SCROLL_ACTIVATE, /**< This command activates the scrolling function */
		SCROLL_DEACTIVE  /**< This command deactivates the scrolling action */
	};
	/*!
		@brief Set the pixel color order RGB mapping 
		@details This command bit a[2](0x04) is made for flexible 
			layout of segment signals in OLED module to match filter
			design. If user color is inverted: try the other setting.
	*/
	enum color_order_e: uint8_t{
		COLORORDER_RGB = 0x72, /**< A[2]=0, normal order SA,SB,SC (e.g. RGB)*/
		COLORORDER_BGR = 0x76  /**< A[2]=1, reverse order SC,SB,SA (e.g. BGR)*/
	 };
	/*!
		@brief Set the display mode
	*/
	enum display_mode_e: uint8_t{
		DISPLAY_INVERSE,/**< The gray level of display data are swapped such that “GS0” <-> “GS63”, “GS1” <-> “GS62”,*/
		DISPLAY_NORMAL, /**< Reset the other effects and turn the data to ON at the corresponding gray level.*/
		DISPLAY_ALL_ON, /**< Forces the entire display to be at “GS63” regardless of the contents of the display data RAM.*/
		DISPLAY_ALL_OFF /**< Forces the entire display to be at gray level “GS0” regardless of the contents of the display data RAM */
	 };
	/*!
		 @brief Set contrast color values in normal mode
	*/
	struct Constrast_values_t {
		uint8_t ContrastColorA = 0x7D; /**< color A contrast normal mode */
		uint8_t ContrastColorB = 0x7D; /**< color B contrast normal mode */
		uint8_t ContrastColorC = 0x7D; /**< color C contrast normal mode */
	};
	/*!
		 @brief Set contrast color values in dim mode
	*/
	struct Dim_Constrast_values_t {
		uint8_t Dim_ContrastColorA = 0x1E; /**< color A contrast dim mode */
		uint8_t Dim_ContrastColorB = 0x1E; /**< color B contrast dim mode */
		uint8_t Dim_ContrastColorC = 0x1E; /**< color C contrast dim mode */
	};

	SSD1331_OLED(color_order_e, const Constrast_values_t& contrast, const Dim_Constrast_values_t& dimContrast);
	~SSD1331_OLED(){};

	// Functions
	// From graphics virtual
	virtual void setAddrWindow(uint16_t, uint16_t, uint16_t, uint16_t) override;
	//Setup related
	void InitSPIType(uint32_t baudrate, spi_inst_t *spi);
	void InitSPIType(uint16_t CommDelay);
	void SetupGPIO(int8_t, int8_t, int8_t, int8_t, int8_t);
	void InitScreenSize(uint16_t w, uint16_t h);
	void SSD1331Initialize(void);
	// SPI related
	void ResetPin(void);
	uint16_t HighFreqDelayGet(void) const;
	void HighFreqDelaySet(uint16_t);
	void PowerDown(void);

	// Screen related
	void setRotation(display_rotate_e r);
	void changeDisplayMode(display_mode_e);
	void DimDisplay(void);
	void enableDisplay(void);
	void sleepDisplay(void);
	void ClearWindowCmd(uint8_t c1, uint8_t r1, uint8_t c2 , uint8_t r2);
	void CopyWindowCmd(uint8_t c1, uint8_t r1, uint8_t c2 , uint8_t r2,uint8_t c3 , uint8_t r3);
	void DimWindowCmd(uint8_t c1, uint8_t r1, uint8_t c2 , uint8_t r2);
	void DrawLineCmd(uint8_t c1, uint8_t r1, uint8_t c2, uint8_t r2, uint16_t color);
	void DrawRectCmd(uint8_t c1, uint8_t r1, uint8_t  c2, uint8_t r2, uint16_t color, bool fill);
	void Scroll(scroll_control_e) ;
	void ScrollSetup(uint8_t ,uint8_t , uint8_t , uint8_t , scroll_time_interval_e );

private:

	void ConfigueDimMode(void);
	void ConfigueContrast(void);
	void DataCommandPin(void);
	void Clock_Data_ChipSelect_Pins(void);
	void HWSPIInitialize(void);
	void cmdBegin(void);
	void WriteColor16(uint16_t colorWrite);

	// Screen
	uint16_t _widthStartOLED = 96;  /**<  never change after first init */
	uint16_t _heightStartOLED = 64; /**< never change after first init */
	color_order_e _colorOrder = COLORORDER_RGB; /**< Enum to hold color order */
	display_rotate_e _displayRotate = Degrees_0; /**< Enum to hold rotation */
	Constrast_values_t ContrastValues  = { 0x7D, 0x7D, 0x7D }; /**< struct to hold normal mode contrast colors ABC */
	Dim_Constrast_values_t DimContrastValues{ 0x1E, 0x1E, 0x1E }; /**< struct to hold dim mode contrast colors ABC */

	// SSD1331 Commands & registers
	static constexpr uint8_t  _CMD_DRAWLINE = 0x21       ; /**< Draw line Command*/
	static constexpr uint8_t  _CMD_DRAWRECT = 0x22       ; /**< Draw rectangle command */
	static constexpr uint8_t  _CMD_COPY_WINDOW = 0x23    ; /**< Copy a window */
	static constexpr uint8_t  _CMD_DIM_WINDOW = 0x24     ; /**< Dim a window */
	static constexpr uint8_t  _CMD_CLEAR_WINDOW = 0x25   ; /**< Clear a window */
	static constexpr uint8_t  _CMD_FILL = 0x26           ; /**< Fill enable/disable used with Draw rect */
	static constexpr uint8_t  _CMD_DISABLE_FILL = 0x00   ; /**< used with _CMD_FILL to not fill rect*/
	static constexpr uint8_t  _CMD_ENABLE_FILL  = 0x01   ; /**< used with _CMD_FILL to fill rect*/

	static constexpr uint8_t  _CMD_CONT_SCROLL_SET =0x27 ; /**< Continuous Horizontal & Vertical Scrolling Setup*/
	static constexpr uint8_t  _CMD_DEACTIVE_SCROLL = 0x2E; /**< Deactivate scrolling*/
	static constexpr uint8_t  _CMD_ACTIVE_SCROLL = 0x2F  ; /**< Activate scrolling*/

	static constexpr uint8_t  _CMD_SETCOLUMN = 0x15      ; /**< Set column address */
	static constexpr uint8_t  _CMD_SETROW = 0x75         ; /**< Set row address */

	static constexpr uint8_t  _CMD_CONTRASTA = 0x81      ; /**< Set contrast for colour A */
	static constexpr uint8_t  _CMD_CONTRASTB = 0x82      ; /**< Set contrast for colour B */
	static constexpr uint8_t  _CMD_CONTRASTC = 0x83      ; /**< Set contrast for colour C */
	static constexpr uint8_t  _CMD_MASTERCURRENT = 0x87  ; /**< Master current control */
	static constexpr uint8_t  _CMD_PRECHARGEA = 0x8A     ; /**< Set second pre-charge speed for colour A */
	static constexpr uint8_t  _CMD_PRECHARGEB = 0x8B     ; /**< Set second pre-charge speed for colour B */
	static constexpr uint8_t  _CMD_PRECHARGEC = 0x8C     ; /**< Set second pre-charge speed for colour C */

	static constexpr uint8_t  _CMD_SETREMAP = 0xA0       ; /**< Set re-map & data format */
	static constexpr uint8_t  _CMD_STARTLINE = 0xA1      ; /**< Set display start line */
	static constexpr uint8_t  _CMD_DISPLAYOFFSET = 0xA2  ; /**< Set display offset */
	static constexpr uint8_t  _CMD_NORMALDISPLAY = 0xA4  ; /**< Set display to normal mode */
	static constexpr uint8_t  _CMD_DISPLAYALLON = 0xA5   ; /**< Set entire display ON */
	static constexpr uint8_t  _CMD_DISPLAYALLOFF = 0xA6  ; /**< Set entire display OFF */
	static constexpr uint8_t  _CMD_INVERTDISPLAY = 0xA7  ; /**< Invert display */
	static constexpr uint8_t  _CMD_SETMULTIPLEX = 0xA8   ; /**< Set multiplex ratio */
	static constexpr uint8_t  _CMD_DIM_MODE_SET =0xAB    ; /**< Configure settings for dim mode */
	static constexpr uint8_t  _CMD_DIM_MODE_ON = 0xAC    ; /**< Enable Dim mode */
	static constexpr uint8_t  _CMD_SETMASTER = 0xAD      ; /**< Set master configuration */
	static constexpr uint8_t  _CMD_DISPLAYOFF = 0xAE     ; /**< Display OFF (sleep mode) */
	static constexpr uint8_t  _CMD_DISPLAYON = 0xAF      ; /**< Normal Brightness Display ON */

	static constexpr uint8_t  _CMD_POWERMODE = 0xB0      ; /**< Power save mode */
	static constexpr uint8_t  _CMD_PRECHARGE = 0xB1      ; /**< Phase 1 and 2 period adjustment */
	static constexpr uint8_t  _CMD_CLOCKDIV  = 0xB3      ; /**< Set clk divide ratio/oscillator freq*/
	static constexpr uint8_t  _CMD_SET_GRAYSCALE = 0xB8  ; /**< Sets the gray scale table*/
	static constexpr uint8_t  _CMD_ENABLE_GRAYSCALE =0xB9; /**< Reloads preset linear gray scale table*/
	static constexpr uint8_t  _CMD_PRECHARGE_VOLT = 0xBB ; /**< Set pre-charge voltage */
	static constexpr uint8_t  _CMD_V_VOLTAGE   = 0xBE    ; /**< Set Vcomh voltage */

}; //end of SSD1331_OLED  class

// ********************** EOF *********************
