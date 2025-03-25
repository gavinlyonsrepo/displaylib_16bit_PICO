/*!
	@file    Bitmap_TEST_Data.hpp
	@author  Gavin Lyons
	@brief   Library header file for test bitmap data, holds bitmap data.
	@note    NOT part of core library only used for holding test data, 
			 can be omitted in project use or port.
*/

#pragma once

#include <array>
#include <span>
#include "pico/stdlib.h"

// extern declarations to bitmaps arrays,  data is in cpp file
extern const std::span<const uint8_t>  sArrowImage;      /**< bi-colour (128x128/8), 2048 bytes  */
extern const std::span<const uint8_t>  sSunTextImage;    /**< bi-colour (40x16/8), 80 bytes*/
extern const std::span<const uint8_t>  sMotorImage;      /**< 16 bit color 565, (128x128x2), 32768 bytes  */
extern const std::span<const uint8_t>  sPosterImage;     /**< 24 bit color, (80x48x3), 11520 bytes  */
extern const std::span<const uint8_t>  sSpriteTest16;    /**< 16 bit color (32x32x2), 2048 bytes background 0x7E5F/C_LBLUE*/
extern const std::span<const uint8_t>  sColorTestImage;  /**< 8 bit color (96x96x1), 9216 bytes */

// extern declarations to icon data arrays, data is in cpp file
extern const std::array< uint8_t, 12> powerIcon;      /**< Power icon, 12x8 */
extern const std::array< uint8_t, 12> speedIcon;      /**< Lighting symbol, 12x8 */
extern const std::array< uint8_t, 16> SignalIcon;     /**< Mobile icon, 16x8 */
extern const std::array< uint8_t, 16> MsgIcon;        /**< Message icon, 16x8 */
extern const std::array< uint8_t, 8>  AlarmIcon;      /**< Alarm icon, 8x8 */
extern const std::array< uint8_t, 16> BatIcon;        /**< Battery icon, 16x8 */
