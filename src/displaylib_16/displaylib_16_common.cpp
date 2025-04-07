/*!
	@file     displaylib_16_common.cpp
	@author   Gavin Lyons
	@brief    Library source file for  common data to all displays
*/

#include "../../include/displaylib_16/displaylib_16_common.hpp"

/*! namespace for common functions and data*/
namespace DisLib16
{
/*!
	@brief  get the library version number
	@return the library version number eg 120 = 1.2.0
*/
uint16_t LibraryVersion(void){return 210;}

bool _debugMode = false; /**< false default , true debug on  */

/*!
	@brief  get the debug mode
	@return true if debug mode is on
*/
bool isDebugMode(void)
{
	return _debugMode;	
}

/*!
	@brief  set the debug mode
	@param mode true to turn on debug mode
*/
void setDebugMode(bool mode)
{
	_debugMode = mode;	
}

/*!
	@brief  Print memory usage information
	@details This function prints the memory usage information, including the end of the BSS segment,
		the end of the heap, the stack pointer, heap usage, and the gap between the heap and stack.
		It also estimates the total used and free SRAM.
*/
void printMemoryUsage(void) {

	char* bss_end = &__bss_end__;
	char* heap_end = (char*)sbrk(0);
	char* stack_ptr;
	__asm volatile ("mov %0, sp" : "=r"(stack_ptr));

	std::size_t heap_usage = heap_end - bss_end;
	std::size_t heap_stack_gap = stack_ptr - heap_end;
	std::size_t total_used = heap_usage;
	std::size_t total_free = heap_stack_gap;

	printf("=== SRAM Usage ===\n");
	printf("BSS end          : %p\n", bss_end);
	printf("Heap end         : %p\n", heap_end);
	printf("Stack pointer    : %p\n", stack_ptr);
	printf("Heap usage       : %zu bytes\n", heap_usage);
	printf("Gap (heap→stack) : %zu bytes (available)\n", heap_stack_gap);
	printf("Estimated SRAM used : %zu bytes\n", total_used);
	printf("Estimated SRAM free : %zu bytes\n", total_free);
}

}