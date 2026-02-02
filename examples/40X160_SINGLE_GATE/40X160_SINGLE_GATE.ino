/*!
	@file   HELLO_WORLD.ino
	@author Gavin Lyons
	@brief  Example file for GC9D01_LTSM arduino library. Test: print "Hello World"
          This example will test 40X160 single gate 
	@note   See USER OPTIONS 1-2 in SETUP function, Default mode: no advanced buffer.
  @details We demonstrate the two different text draw modes:
           "Hello" is printed in text local buffer mode, default. Much faster(5X) but creates 
           a local character buffer for each
           character which could cause problems with very large fonts used by MCU with low RAM.
           "World" is printed in text draw pixel by pixel mode. The "FixType" variable in setup
           will effect this draw mode. User can turn this off and it will be 2X faster but 
           missing pixels may result depending on Display manufacture/varient. 
	@test
		-# Test 101 Print out Hello world  
*/

// libraries
#include "GC9D01_LTSM.hpp"
// Fonts needed
#include <fonts_LTSM/FontArialBold_LTSM.hpp>

///@cond

//  Test timing related defines
#define TEST_DELAY1 1000  // mS
#define TEST_DELAY2 2000  // mS
#define TEST_DELAY5 5000  // mS
#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif

GC9D01_LTSM myTFT;
bool bhardwareSPI = true;  // true for hardware spi, false for software

void setup(void) {
  Serial.begin(38400);
  delay(1000);
  // === USER OPTION 1 SPI_SPEED + TYPE ===
  int8_t DC_TFT = 5;
  int8_t RST_TFT = 4;
  int8_t CS_TFT = 15;
  if (bhardwareSPI == true) {          // hw spi
    uint32_t TFT_SCLK_FREQ = 8000000;  // Spi freq in Hertz
    myTFT.TFTsetupGPIO_SPI(TFT_SCLK_FREQ, RST_TFT, DC_TFT, CS_TFT);
  } else {                        // sw spi
    uint16_t SWSPICommDelay = 0;  // optional SW SPI GPIO delay in uS
    int8_t SDIN_TFT = 13;
    int8_t SCLK_TFT = 12;
    myTFT.TFTsetupGPIO_SPI(SWSPICommDelay, RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
  }
  // ===
  // === USER OPTION 2 Screen Setup ===
  uint16_t TFT_WIDTH = 40;   // Screen width in pixels
  uint16_t TFT_HEIGHT = 160;  // Screen height in pixels
  uint16_t S_X_OFFSET = -60; // Display X offset in pixels 
  uint16_t S_Y_OFFSET = 60;  // Display Y offset in pixels
  // Display type, 4 choice's, see readme.
  GC9D01_LTSM::Resolution_e DisplayType = GC9D01_LTSM::Resolution_e::RGB40x160_SingleGate;
  // Pixel Draw mode type, 4 choices , see readme.  
  GC9D01_LTSM::PixelFixMode_e FixType = GC9D01_LTSM::PixelFixMode_e::Both;
  myTFT.TFTInitScreenSize(TFT_WIDTH, TFT_HEIGHT, DisplayType, FixType, S_X_OFFSET, S_Y_OFFSET);
  // ===
  myTFT.TFTGC9D01Initialize();
  myTFT.TFTsetRotation(myTFT.Degrees_270);
  Serial.println("Start");
}

// main
void loop(void) {
  Test100();
  EndTests();
}

void Test100(void) {
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
  myTFT.setFont(FontArialBold);
  myTFT.setCursor(10, 10);
  myTFT.print("Hello 270");

  // Switch to pixel by pixel mode test 
  //================
  Serial.println(myTFT.getTextCharPixelOrBuffer());
  myTFT.setTextCharPixelOrBuffer(true);
   //===================
  myTFT.setCursor(25, 80);
  myTFT.print("World");
  delay(TEST_DELAY5);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
  delay(TEST_DELAY1);
}

void EndTests(void) {
  myTFT.TFTPowerDown();
  Serial.println("End");
  while (1) {};
}
///@endcond
