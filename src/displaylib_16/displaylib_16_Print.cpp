/*!
	@file  displaylib_16_Print.cpp
	@brief Base class that provides print() and println()
	@note  Port of arduino built-in print class, G Lyons 2022.
*/

#include "../../include/displaylib_16/displaylib_16_Print.hpp"

/**
 * @brief Writes a string to the output.
 * @param str Pointer to the character array.
 * @return The number of bytes written. Returns 0 if @p str is nullptr.
 */
size_t Print::write(const char *str) 
{
	if (str == nullptr) 
	{
		setWriteError(5); // rdlib::CharArrayNullptr
		return 0;
	}
	clearWriteError();
	return write(reinterpret_cast<const uint8_t *>(str), strlen(str));
}

/**
 * @brief Writes a buffer of a specified size to the output.
 * @param buffer Pointer to the data buffer.
 * @param size Number of bytes to write.
 * @return The number of bytes written.
 */
size_t Print::write(const char *buffer, size_t size) 
{
	return write(reinterpret_cast<const uint8_t *>(buffer), size);
}

/**
 * @brief Gets the number of bytes available for writing.
 * default to zero, meaning "a single write may block"
 * should be overriden by subclasses with buffering
 * @return The maximum number of bytes that can be written.
 */
int Print::availableForWrite() { return 255; }

/*! 
	@brief  gets the error flag status, zero no error 
	@return the errorFlag value
	*/
int Print::getWriteError() { return _ErrorFlag; }

/*! @brief clears the error flag by setting it to zero */
void Print::clearWriteError() { setWriteError(0); }

/*! sets error flag  to value 
 *  @param error error to set errorFlag*/
void Print::setWriteError(int error) { _ErrorFlag = error; }

// = print =

/*! 
	@brief default implementation: may be overridden 
	@param buffer of ints
	@param size
	@return total number of characters printed
*/
size_t Print::write(const uint8_t *buffer, size_t size)
{
	size_t n = 0;
	while (size--) {
	if (write(*buffer++)) n++;
	else break;
	}
	return n;
}

/*!
	@brief print an array
	@param str An array of characters
	@return passes string to write
*/
size_t Print::print(const char str[])
{
	return write(str);
}

/*!
	@brief print an character
	@param c the character to print
	@return passes character to write
*/
size_t Print::print(char c)
{
	return write(c);
}

/*!
	@brief print an integer of base number system
	@param n integer to print
	@param base the base number system to use (BIN HEX OCT DEC)
	@return calls print(long)
*/
size_t Print::print(int n, int base)
{
	return print((long) n, base);
}

/*!
	@brief print an unsigned integer of base number system
	@param n integer to print
	@param base the base number system to use (BIN HEX OCT DEC)
	@return calls print( unsigned long)
*/
size_t Print::print(unsigned int n, int base)
{
	return print((unsigned long) n, base);
}

/*!
	@brief print an long  integer of base number system
	@param n integer to print
	@param base the base number system to use (BIN HEX OCT RDL_DEC)
	@return formats negative number and passes to PrintNumber or write
*/
size_t Print::print(long n, int base)
{
	if (base == 0)
	{
		return write(n);
	} else if (base == 10)
	{
		if (n < 0)
		{
			int t = print('-');
			n = -n;
			return printNumber(n, 10) + t;
		}
		return printNumber(n, 10);
	} else
	{
		return printNumber(n, base);
	}
}

/*!
	@brief print an unsigned long integer of base number system
	@param n integer to print
	@param base the base number system to use (BIN HEX OCT RDL_DEC)
	@return  passes to PrintNumber or write
*/
size_t Print::print(unsigned long n, int base)
{
	if (base == 0) 
		return write(n);
	else 
		return printNumber(n, base);
}

/*!
	@brief print an double 
	@param n integer to print
	@param digits number of digits to print
	@details print(3.7468,2) = will print 3.75 on display
	@return passes to printFLoat
*/
size_t Print::print(double n, int digits)
{
	return printFloat(n, digits);
}

/*!
	@brief print an C++ string object
	@param s string object
	@return converts to c style string and passes to write
*/
size_t Print::print(const std::string &s) {
	return write(s.c_str(), s.length());
}

// = println =

/*!
	@brief Goto to new line
*/
size_t Print::println(void)
{
	return write("\r\n");
}

/*!
	@brief Prints a character array followed by a newline.
	@param c Pointer to a null-terminated character array.
	@return Number of characters printed, including the newline.
*/
size_t Print::println(const char c[])
{
	size_t n = print(c);
	n += println();
	return n;
}

/*!
	@brief print an character followed by new line
	@param c the character to print
	@return Number of characters printed, including the newline.
*/
size_t Print::println(char c)
{
	size_t n = print(c);
	n += println();
	return n;
}

/*!
	@brief print an integer of base number system followed by new line
	@param num integer to print
	@param base the base number system to use (BIN HEX OCT RDL_DEC)
	@return Number of characters printed, including the newline.
*/
size_t Print::println(int num, int base)
{
	size_t n = print(num, base);
	n += println();
	return n;
}

/*!
	@brief print an unsigned integer of base number system followed by new line
	@param num integer to print
	@param base the base number system to use (BIN HEX OCT RDL_DEC)
	@return Number of characters printed, including the newline.
*/
size_t Print::println(unsigned int num, int base)
{
	size_t n = print(num, base);
	n += println();
	return n;
}

/*!
	@brief print an long  integer of base number system followed by new line
	@param num integer to print
	@param base the base number system to use (BIN HEX OCT RDL_DEC)
	@return Number of characters printed, including the newline.
*/
size_t Print::println(long num, int base)
{
	size_t n = print(num, base);
	n += println();
	return n;
}

/*!
	@brief print an unsigned long integer of base number system followed by new line
	@param num integer to print
	@param base the base number system to use (BIN HEX OCT RDL_DEC)
	@return Number of characters printed, including the newline.
*/
size_t Print::println(unsigned long num, int base)
{
	size_t n = print(num, base);
	n += println();
	return n;
}

/*!
	@brief print an double 
	@param num float to print followed by new line
	@param digits number of digits to print
	@details print(3.7468,2) = will print 3.75 on display
	@return Number of characters printed, including the newline.
*/
size_t Print::println(double num, int digits)
{
	size_t n = print(num, digits);
	n += println();
	return n;
}

/*!
	@brief print an C++ string object followed by new line
	@param s string object
	@return Number of characters printed, including the newline.
*/
size_t Print::println(const std::string &s) {
	size_t n = print(s);
	n += println();
	return n;
}



// ==== Private Methods ====

/*!
	@brief Used internally to parse and print number
	@param n Number to parse
	@param base to print to Binary hexadecimal etc
	@return passes the formatted string to write
 */
size_t Print::printNumber(unsigned long n, uint8_t base)
{
	char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
	char *str = &buf[sizeof(buf) - 1];

	*str = '\0';

	// prevent crash if called with base == 1
	if (base < 2) base = 10;

	do {
	char c = n % base;
	n /= base;

	*--str = c < 10 ? c + '0' : c + 'A' - 10;
	} while(n);

	return write(str);
}

/*!
	@brief Used internally to parse float
	@param number The floating point number
	@param digits Number of precision digits to print
	@return total number of characters printed 
 */
size_t Print::printFloat(double number, uint8_t digits)
{
	size_t n = 0;

	if (std::isnan(number)) return print("nan");
	if (std::isinf(number)) return print("inf");
	if (number > 4294967040.0) return print ("ovf");  // constant determined empirically
	if (number <-4294967040.0) return print ("ovf");  // constant determined empirically

	// Handle negative numbers
	if (number < 0.0)
	{
	 n += print('-');
	 number = -number;
	}

	// Round correctly so that print(1.999, 2) prints as "2.00"
	double rounding = 0.5;
	for (uint8_t i=0; i<digits; ++i)
	rounding /= 10.0;

	number += rounding;

	// Extract the integer part of the number and print it
	unsigned long int_part = (unsigned long)number;
	double remainder = number - (double)int_part;
	n += print(int_part);

	// Print the decimal point, but only if there are digits beyond
	if (digits > 0) {
	n += print('.');
	}

	// Extract digits from the remainder one at a time
	while (digits-- > 0)
	{
	remainder *= 10.0;
	unsigned int toPrint = (unsigned int)(remainder);
	n += print(toPrint);
	remainder -= toPrint;
	}

	return n;
}
