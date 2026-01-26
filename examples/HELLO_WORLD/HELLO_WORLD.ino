/*!
	@file   HELLO_WORLD.ino
	@author Gavin Lyons
	@brief  Example file for GC9D01_LTSM arduino library. Test: print "Hello World"
	@note   See USER OPTIONS 1-2 in SETUP function, 
          "Hello" is printed in text row buffer mode
          "World" is printed in text draw pixel by pixel mode
	@test
		-# Test 101 Print out Hello world  
*/

// libraries
#include "GC9D01_LTSM.hpp"
// Fonts needed
#include <fonts_LTSM/FontGroTesk_LTSM.hpp>

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
  uint16_t TFT_WIDTH = 160;   // Screen width in pixels
  uint16_t TFT_HEIGHT = 160;  // Screen height in pixels
  // Display type, multiple choice see readme
  GC9D01_LTSM::Resolution_e DisplayType = GC9D01_LTSM::Resolution_e::RGB160x160_DualGate;
  myTFT.TFTInitScreenSize(TFT_WIDTH, TFT_HEIGHT, DisplayType);
  // ===
  myTFT.TFTGC9D01Initialize();

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
  myTFT.setFont(FontGroTesk);
  myTFT.setCursor(25, 40);
  myTFT.print("Hello");

  // Switch to pixel by pixel test 
  //================
  Serial.println(myTFT.getTextCharPixelOrBuffer());
  myTFT.setTextCharPixelOrBuffer(true);
  myTFT.setCursor(25, 80);
  myTFT.print("World");
  //===================
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
