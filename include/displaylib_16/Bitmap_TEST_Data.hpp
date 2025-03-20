/*!
	@file     Bitmap_TEST_Data.hpp
	@author   Gavin Lyons
	@brief    Library header file for test bitmap, Data file to hold bitmap data.
    @note    Not part of core library only used for holding test data, should be omitted
    in project use or port.
     bitmaps in file
    -#  ArrowImage     : bi-colour 2048 bytes  (128x128)
    -#  SunTextImage   : bi-colour (40x16)
    -#  motorImage     : 16 bit color 565,  32768 bytes (128x128)
    -#  posterImage    : 24 bit color, 11520 bytes (80x48)
    -#  SpriteTest16   : 16 bit color 565 2048 bytes (32x32) with background 0x7E5F or ST7735_LBLUE
*/

#pragma once

#include "pico/stdlib.h"

extern const uint8_t * pArrowImage;    /**< Pointer to bitmap data which is in cpp file */
extern const uint8_t * pSunTextImage;  /**< Pointer to bitmap data which is in cpp file */
extern const uint8_t * pMotorImage;    /**< Pointer to bitmap data which is in cpp file */
extern const uint8_t * pPosterImage;   /**< Pointer to bitmap data which is in cpp file */
extern const uint8_t * pSpriteTest16;  /**< Pointer to bitmap data which is in cpp file */

// All icon data vertically addressed
/**< power icon, 12x8 */
const unsigned char powerIcon[12] = {0xff, 0xe7, 0xc3, 0x99, 0xa5, 0xad, 0xad, 0xa5, 0x99, 0xc3, 0xe7, 0xff};
/**< lighting symbol, 12x8 */
const unsigned char speedIcon[12] = {0xff, 0xff, 0xf7, 0xb3, 0xd1, 0xc0, 0xe0, 0xf4, 0xf6, 0xfe, 0xff, 0xff};
/**< Mobile icon  16x8px */
const unsigned char SignalIcon[16] = {0x03, 0x05, 0x09, 0xff, 0x09, 0x05, 0xf3, 0x00, 0xf8, 0x00, 0xfc, 0x00, 0xfe, 0x00, 0xff, 0x00};
/**< Message icon  16x8px */
const unsigned char MsgIcon[16] = {0x00, 0x00, 0x00, 0xff, 0x85, 0x89, 0x91, 0x91, 0x91, 0x91, 0x89, 0x85, 0xff, 0x00, 0x00, 0x00};
/**< Alarm icon  8x8px */
const unsigned char AlarmIcon[8] = {0x83, 0xbd, 0x42, 0x4a, 0x52, 0x52, 0xbd, 0x83};
/**< Battery Icon  16x8px */
const unsigned char BatIcon[16] = {0x00, 0x00, 0x7e, 0x42, 0x81, 0xbd, 0xbd, 0x81, 0xbd, 0xbd, 0x81, 0xbd, 0xbd, 0x81, 0xff, 0x00};
