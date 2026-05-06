/*!
	@file   50X160_SINGLE_GATE.ino
	@author Gavin Lyons
	@brief  Example file for GC9D01_LTSM arduino library. Test: print "Hello World"
          This example will test 50x160 single gate 
	@note   See USER OPTIONS  in SETUP function, Default mode: no advanced buffer.
          Make sure to set relevant macro at top of 'GC9D01_LTSM.hpp', in 
          'Section User Options'. GC9D01_SINGLE_INIT_50x160
          This file is untested on hardware
	@test
		-# Test 103 Print out 'Hi!'  
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
  delay(2000);
  // === USER OPTION 1 SPI_SPEED + TYPE ===
  int8_t DC_TFT = 5;
  int8_t RST_TFT = 4;
  int8_t CS_TFT = 15;
  if (bhardwareSPI == true) {          // hw spi
    uint32_t TFT_SCLK_FREQ = 8000000;  // Spi freq in Hertz
    myTFT.TFTsetupGPIO_SPI(TFT_SCLK_FREQ, RST_TFT, DC_TFT, CS_TFT);
  } else {                        // sw spi
    uint16_t SWSPICommDelay = 0;  // optional SW SPI GPIO delay in uS
    int8_t SDIN_TFT = 26;
    int8_t SCLK_TFT = 27;
    myTFT.TFTsetupGPIO_SPI(SWSPICommDelay, RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
  }
  // ===
  // === USER OPTION 2 Screen Setup ===
  uint16_t TFT_WIDTH = 50;    // Screen width in pixels
  uint16_t TFT_HEIGHT = 160;  // Screen height in pixels
  uint16_t OFFSET_X_L = -39;  // Landscape Screen X offset in pixels
  uint16_t OFFSET_Y_L = 55;   // Landscape Screen Y offset in pixels
  uint16_t OFFSET_X_P = 16;   // Portrait Screen X offset in pixels
  uint16_t OFFSET_Y_P = 0;    // Portrait Screen Y offset in pixels
  // Display type, 6 choice's, see readme.
  GC9D01_LTSM::Resolution_e DisplayType = GC9D01_LTSM::Resolution_e::RGB50x160_SingleGate;
  // Pixel Draw mode type, 6 choices , see readme.
  GC9D01_LTSM::PixelFixMode_e FixType = GC9D01_LTSM::PixelFixMode_e::Both;
  myTFT.TFTInitScreenSize(TFT_WIDTH, TFT_HEIGHT, DisplayType, FixType, OFFSET_X_L, OFFSET_Y_L, OFFSET_X_P, OFFSET_Y_P);
  // ===
  myTFT.TFTGC9D01Initialize();
  myTFT.TFTsetRotation(myTFT.Degrees_0);
  Serial.println("Start");
}

// main
void loop(void) {
  Test103();
  EndTests();
}

void Test103(void) {
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
  myTFT.setFont(FontArialBold);
  myTFT.setCursor(0, 0);
  myTFT.print("Hi!");
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
