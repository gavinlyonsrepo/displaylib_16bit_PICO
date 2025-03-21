/*!
	@file     displaylib_16_Font.hpp
	@author   Gavin Lyons
	@brief    Library header file for  font data
			  This file contains the font definitions and data.
			  see main readme file for details. font data is in cpp file.
	@note 		Fonts 9 -12  optional , comment in define _DISPLAY_OPTIONAL_FONT_X to enable
*/

#pragma once

#include <span>
#include "pico/stdlib.h"
#include "displaylib_16_common.hpp"

#define _DISPLAY_OPTIONAL_FONT_9  /**< Comment in to enable optional font 9 */
#define _DISPLAY_OPTIONAL_FONT_10 /**< Comment in to enable optional font 10*/
#define _DISPLAY_OPTIONAL_FONT_11 /**< Comment in to enable optional font 11*/
#define _DISPLAY_OPTIONAL_FONT_12 /**< Comment in to enable optional font 12*/

extern const unsigned char *pFontDefaultptr;	 /**< Pointer to default font data  */
extern const unsigned char *pFontThickptr;		 /**< Pointer to thick font data  */
extern const unsigned char *pFontSevenSegptr;	 /**< Pointer to seven segment font data  */
extern const unsigned char *pFontWideptr;		 /**< Pointer to wide font data  */
extern const unsigned char *pFontTinyptr;		 /**< Pointer to tiny font data  */
extern const unsigned char *pFontHomeSpunptr;	 /**< Pointer to homespun font data  */
extern const uint8_t (*pFontBigNum16x32ptr)[64]; /**< Pointer to big numbers font data  */
extern const uint8_t (*pFontMedNum16x16ptr)[32]; /**< Pointer to medium numbers font data  */
#ifdef _DISPLAY_OPTIONAL_FONT_9
extern const uint8_t (*pFontArial16x24ptr)[48]; /**< Pointer to Arial Round font data */
#endif
#ifdef _DISPLAY_OPTIONAL_FONT_10
extern const uint8_t (*pFontArial16x16ptr)[32]; /**< Pointer to Arial bold font data */
#endif
#ifdef _DISPLAY_OPTIONAL_FONT_11
extern const uint8_t (*pFontMia8x16ptr)[16]; /**< Pointer to Mia font data */
#endif
#ifdef _DISPLAY_OPTIONAL_FONT_12
extern const uint8_t (*pFontDedica6x12ptr)[12]; /**< Pointer to Dedica font data */
#endif