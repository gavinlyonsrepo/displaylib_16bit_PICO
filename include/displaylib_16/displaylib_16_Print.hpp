/*!
	@file  displaylib_16_Print.hpp
	@brief Base class that provides print() and println()
	@note  Port of arduino built-in print class, G Lyons 2022.
*/

#pragma once

#include <cstring>			// Strlen
#include <math.h>				//isnan isinf etc
#include <string>				// for std::string &
#include "pico/stdio.h" // for size_t
#include "displaylib_16_common.hpp"

/*!
	@brief class that provides polymorphic print methods for printing data
*/
class Print
{
public:
	Print() : _ErrorFlag(0) {}

	/*! Base number type enum*/
	enum BaseNum : uint8_t
	{
		DEC = 10, /**< Format the passed integer in Decimal format */
		HEX = 16, /**< Format the passed integer in Hexadecimal format  */
		OCT = 8,	/**< Format the passed integer in Octal format  */
		BIN = 2		/**< Format the passed integer in Binary format  */
	};

	int getWriteError();
	void clearWriteError();
	/*! @brief define in the sub class */
	virtual size_t write(uint8_t) = 0;
	/*! @brief define in the sub class */
	virtual size_t write(const uint8_t *buffer, size_t size);
	size_t write(const char *str);
	size_t write(const char *buffer, size_t size);
	/*! @brief define in the sub class */
	virtual int availableForWrite();

	size_t print(const char[]);
	size_t print(char);
	size_t print(int, int = DEC);
	size_t print(unsigned int, int = DEC);
	size_t print(long, int = DEC);
	size_t print(unsigned long, int = DEC);
	size_t print(double, int = 2);
	size_t print(const std::string &);

	size_t println(const char[]);
	size_t println(char);
	size_t println(int, int = DEC);
	size_t println(unsigned int, int = DEC);
	size_t println(long, int = DEC);
	size_t println(unsigned long, int = DEC);
	size_t println(double, int = 2);
	size_t println(void);
	size_t println(const std::string &s);

protected:
	void setWriteError(int err = 0);

private:
	int _ErrorFlag; /**< class Error flag , zero no error*/
	size_t printNumber(unsigned long, uint8_t);
	size_t printFloat(double, uint8_t);
};
