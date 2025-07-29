# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.18)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(displaylib_16 C CXX ASM)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# turn on all compiler warnings 
add_compile_options(-Wall -Wextra )
# print memory information
string(APPEND CMAKE_EXE_LINKER_FLAGS "-Wl,--print-memory-usage")

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
  examples/st7735/hello/main.cpp
  #examples/st7735/graphics/main.cpp
  #examples/st7735/functions_fps/main.cpp
  #examples/st7735/text/main.cpp
  #examples/st7735/bmp_data/main.cpp
  #examples/st7735/demos/main.cpp
  #examples/st7735/frame_buffer/main.cpp

  #examples/st7789/hello/main.cpp
  #examples/st7789/graphics/main.cpp
  #examples/st7789/functions_fps/main.cpp
  #examples/st7789/text/main.cpp
  #examples/st7789/bmp_data/main.cpp
  #examples/st7789/demos/main.cpp
 #examples/st7789/frame_buffer/main.cpp

  #examples/ili9341/hello/main.cpp
  #examples/ili9341/text_graphics_functions/main.cpp
  #examples/ili9341/bmp_data/main.cpp
  #examples/ili9341/demo/main.cpp
  #examples/ili9341/frame_buffer/main.cpp

  #examples/ssd1331/hello/main.cpp
  #examples/ssd1331/demo/main.cpp
  #examples/ssd1331/bitmap_functions/main.cpp
  #examples/ssd1331/frame_buffer/main.cpp

  #examples/gc9a01/hello/main.cpp
  #examples/gc9a01/frame_buffer/main.cpp
  #examples/gc9a01/demos/main.cpp
  #examples/gc9a01/bmp_data/main.cpp
  #examples/gc9a01/text_graphics_functions/main.cpp
  
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

add_library(pico_displaylib_16 INTERFACE)

target_sources(pico_displaylib_16 INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/src/displaylib_16/st7735.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/displaylib_16/st7789.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/displaylib_16/ili9341.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/displaylib_16/ssd1331.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/displaylib_16/gc9a01.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/displaylib_16/displaylib_16_graphics.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/displaylib_16/displaylib_16_Print.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/displaylib_16/displaylib_16_Font.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/displaylib_16/displaylib_16_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/displaylib_16/Bitmap_TEST_Data.cpp
)

target_include_directories(pico_displaylib_16 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

# Pull in pico libraries that we need
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_spi pico_displaylib_16)


# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

