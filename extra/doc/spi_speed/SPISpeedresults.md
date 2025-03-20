# SPI Speed test results  St7735


**Dec 2024 V1.72**
1. 701 Bitmap + FPS Test :: HW SPI  8MHZ :: 22 fps
2. 701 Bitmap + FPS Test :: HW SPI  32MHZ :: 71 fps
3. 701 Bitmap + FPS Test :: SW SPI   ::  9 fps
4. 702 Functions + FPS Test :: HW SPI  8MHZ :: 26 fps 

**JAN 2024 v1.7.1**

1. 702 Functions + FPS Test :: HW SPI  8MHZ :: 28 fps 
2. 702 Functions + FPS Test :: SW SPI :: 18fps
1. 701 Bitmap + FPS Test :: HW SPI  8MHZ :: 22 fps 
2. 701 Bitmap + FPS Test :: SW SPI ::  9 fps 


**November 8 2022**

1. test11B function ::  ST7735_TFT_TESTS file
2. test :: graphics + text
3. 8 MHZ HW SPI
4. 33 fps 

**November 8 2022**

1. test11B function ::  ST7735_TFT_TESTS file
2. test :: graphics + text
3. SW SPI
4. 7 fps 

**November 10 2022**

1. testFPS function ::  ST7735_TFT_BMP_DATA file
2. test :: myTFT.drawBitmap16Data(0, 0, (uint8_t*)motorImage, 128, 128);
3. 8 MHZ HW SPI
4. 22 fps 

**November 10 2022**

1. testFPS function ::  ST7735_TFT_BMP_DATA file
2. test :: myTFT.drawBitmap16Data(0, 0, (uint8_t*)motorImage, 128, 128);
3. 20 MHZ HW SPI
4. 39 fps

**November 10 2022**

1. testFPS function ::  ST7735_TFT_BMP_DATA file
2. test :: myTFT.drawBitmap16Data(0, 0, (uint8_t*)motorImage, 128, 128);
3. 60 MHZ HW SPI
4. 64 fps

**November 10 2022**

1. testFPS function ::  ST7735_TFT_BMP_DATA file
2. test :: myTFT.drawBitmap24Data(0, 0, (uint8_t*)fruitBowl, 128, 128);
3. 20 MHZ HW SPI
4. 34 fps

**November 10 2022**

1. testFPS function ::  ST7735_TFT_BMP_DATA file
2. test :: myTFT.drawBitmap(0, 0, 128 , 128, ST7735_WHITE , ST7735_GREEN, ArrowImage);
3. 20 MHZ HW SPI
4. 43 FPS

# SPI Speed test results St7789

**DEC 2024 v1.0.2*

1. 702 Functions + FPS Test :: HW SPI  8MHZ :: 90 fps
2. 702 Functions + FPS Test :: SW SPI :: 6 fps
3. 701 Bitmap + FPS Test :: HW SPI  8MHZ :: 22 fps
4. 701 Bitmap + FPS Test :: SW SPI ::  9 fps
5. fillScreen test : 6 FPS (malloc or  row buffer method: same result)
