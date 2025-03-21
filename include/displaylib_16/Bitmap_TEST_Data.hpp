/*!
	@file    Bitmap_TEST_Data.hpp
	@author  Gavin Lyons
	@brief   Library header file for test bitmap, Data file to hold bitmap data.
	@note    NOT part of core library only used for holding test data, 
			 can be omitted in project use or port.
	 bitmaps in file
	-#  ArrowImage     : bi-colour (128x128/8), 2048 bytes 
	-#  SunTextImage   : bi-colour (40x16/8), 80 bytes
	-#  ColorTestImage : 8 bit color (96x96x1), 9216 bytes
	-#  motorImage     : 16 bit color 565, (128x128x2), 32768 bytes 
	-#  posterImage    : 24 bit color, (80x48x3), 11520 bytes 
	-#  SpriteTest16   : 16 bit color 565 (32x32x2), 2048 bytes background 0x7E5F/ST7735_LBLUE
*/

#pragma once

#include <array>
#include <span>
#include "pico/stdlib.h"

extern const std::span<const uint8_t>  sArrowImage;      /**< Pointer to bitmap data which is in cpp file */
extern const std::span<const uint8_t>  sSunTextImage;    /**< Pointer to bitmap data which is in cpp file */
extern const std::span<const uint8_t>  sMotorImage;      /**< Pointer to bitmap data which is in cpp file */
extern const std::span<const uint8_t>  sPosterImage;     /**< Pointer to bitmap data which is in cpp file */
extern const std::span<const uint8_t>  sSpriteTest16;    /**< Pointer to bitmap data which is in cpp file */
extern const std::span<const uint8_t>  sColorTestImage;  /**< Pointer to bitmap data which is in cpp file */

// Declare all icon data arrays
extern const std::array< uint8_t, 12> powerIcon;      /**< Power icon, 12x8 */
extern const std::array< uint8_t, 12> speedIcon;      /**< Lighting symbol, 12x8 */
extern const std::array< uint8_t, 16> SignalIcon;     /**< Mobile icon, 16x8 */
extern const std::array< uint8_t, 16> MsgIcon;        /**< Message icon, 16x8 */
extern const std::array< uint8_t, 8> AlarmIcon;       /**< Alarm icon, 8x8 */
extern const std::array< uint8_t, 16> BatIcon;        /**< Battery icon, 16x8 */

