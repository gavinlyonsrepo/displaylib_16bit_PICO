/*!
	@file    displaylib_16_Font.hpp
	@author  Gavin Lyons
	@brief   Library header file for  font data
			This file contains the font definitions and data.
			see main readme file for details. font data is in cpp file.
	@details fonts 1-11 enabled by default, fonts 12-16 enabled by define 
		 	 dislib16_ADVANCED_FONTS_ENABLE dislib16_ADVANCED_FONTS_ENABLE see USER OPTION
*/

#pragma once

#include <span>
#include <cstdio>
#include "pico/stdlib.h"
#include "displaylib_16_common.hpp"

// ==========================================================
// ==========================================================
// ===== USER OPTION 3 turns on fonts 12-16 if commented in =
// default is off
//#define dislib16_ADVANCED_FONTS_ENABLE
// ==========================================================
// ==========================================================

///@cond
// ==========================================================
// ===============Font Extern declaration section ===========
// Font data is in the cpp file accessed thru extern.
extern const std::span<const uint8_t> pFontDefault; /**< 8x8 default font */
extern const std::span<const uint8_t> pFontGll; /**< 8x8 font (NO LOWERCASE letters) */
extern const std::span<const uint8_t> pFontSinclairS; /**< 8x8 Sinclair-style font */
extern const std::span<const uint8_t> pFontPico; /**< 8x12 Pico font */
extern const std::span<const uint8_t> pFontRetro; /**< 8x16 retro-style font */
extern const std::span<const uint8_t> pFontMega; /**< 16x16 mega font */
extern const std::span<const uint8_t> pFontArialBold; /**< 16x16 Arial Bold font */
extern const std::span<const uint8_t> pFontHallfetica; /**< 16x16 Helvetica-style font */
extern const std::span<const uint8_t> pFontArialRound; /**< 16x24 Arial Round font */
extern const std::span<const uint8_t> pFontOrla; /**< 16x24 Orla font */
extern const std::span<const uint8_t> pFontGroTesk; /**< 16x32 GroTesk font */
#ifdef dislib16_ADVANCED_FONTS_ENABLE

extern const std::span<const uint8_t> pFontInconsola; /**< 24x32 Inconsola font */
extern const std::span<const uint8_t> pFontMint; /**< 24x32 Mint font */
extern const std::span<const uint8_t> pFontSixteenSeg; /**< 32x48 Sixteen-segment font (NUMBERS ONLY + : . -) */
extern const std::span<const uint8_t> pFontSevenSeg; /**< 32x50 Seven-segment font (NUMBERS ONLY + : . -) */
extern const std::span<const uint8_t> pFontGroTeskBig; /**< 32x64 GroTesk font (NO LOWERCASE letters) */
#endif
// ==========================================================

///@endcond

/*! Enum to define current font type selected  */
enum display_Font_name_e : uint8_t
{
	font_default = 1,     /**< default 8by8 */
	font_pico = 2,        /**< Pico 8by12 */
	font_gll = 3,         /**< gll 8x8 (NO LOWERCASE) */
	font_sinclairS = 4,   /**< Sinclair 8by8 */
	font_retro = 5,       /**< Retro 8x16 */
	font_mega = 6,        /**< Mega 16by16 */
	font_arialBold = 7,   /**< Arial bold 16by16 */
	font_hallfetica = 8,  /**< Hallfetica 16by16 */
	font_orla = 9,        /**< Orla 16by24 */
	font_arialRound = 10, /**< ArialRound 16by24 */
	font_groTesk = 11     /**< GroTesk 16by32 */
	#ifdef dislib16_ADVANCED_FONTS_ENABLE
	,
	font_inconsola =12,   /**< Inconsola 24by32 */
	font_mint = 13,       /**< Mint 24by32 */
	font_sixteenSeg = 14, /**< Sixteen Segment 32by48 (NUMBERS ONLY + : . -) */
	font_sevenSeg = 15,   /**< Seven Segment 32by50 (NUMBERS ONLY + : . -) */
	font_groTeskBig = 16  /**< Grotesk big 32by64 (NO LOWERCASE) */
	#endif
};

/*! @brief Font class to hold font data object  */
class display_Fonts
{
	public:
		display_Fonts();
		~display_Fonts(){};
		
		DisLib16::Ret_Codes_e  setFont(display_Font_name_e);
		void setInvertFont(bool invertStatus);
		bool getInvertFont(void);

	protected:
		std::span<const uint8_t> _FontSelect = pFontDefault;  /**< Span to the active font,  Fonts Stored are Const */
		uint8_t _Font_X_Size = 0x08; /**< Width Size of a Font character */
		uint8_t _Font_Y_Size = 0x08; /**< Height Size of a Font character */
		uint8_t _FontOffset = 0x20; /**< Offset in the ASCII table 0x00 to 0xFF, where font begins */
		uint8_t _FontNumChars = 0x5F; /**< Number of characters in font (0x00 to 0xFE) -1 */
	private:
		bool _FontInverted = false; /**< display font inverted? , False = no invert , true = invert*/
};


