/*!
	@file   60X160_SINGLE_GATE.ino
	@author Gavin Lyons
	@brief  Example file for GC9D01_LTSM arduino library. Test: print "Hi"
            This example will test 60x160 single gate 
	@note   See USER OPTIONS 1-2 in SETUP function, Default mode: no advanced buffer.
			Make sure to set relevant macro at top of 'GC9D01_LTSM.hpp', in 
			'Section User Options'. GC9D01_SINGLE_INIT_60x160
	@test
		-# Test 109 Print out 'Hi' 
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
  uint16_t TFT_WIDTH = 60;   // Screen width in pixels
  uint16_t TFT_HEIGHT = 160;  // Screen height in pixels
  uint16_t S_X_OFFSET = 0;   // Display landscape X offset in pixels 
  uint16_t S_Y_OFFSET = 0;  // Display landscape Y offset in pixels
  uint16_t D_X_OFFSET = 10; // Default display + X offset in pixels
  uint16_t D_Y_OFFSET = 0;  // Default display + Y offset in pixels
  // Display type, 4 choice's, see readme.
  GC9D01_LTSM::Resolution_e DisplayType = GC9D01_LTSM::Resolution_e::RGB60x160_SingleGate;
  // Pixel Draw mode type, 4 choices , see readme.  
  GC9D01_LTSM::PixelFixMode_e FixType = GC9D01_LTSM::PixelFixMode_e::Both;
  myTFT.TFTInitScreenSize(TFT_WIDTH, TFT_HEIGHT, DisplayType, FixType, S_X_OFFSET, S_Y_OFFSET, D_X_OFFSET, D_Y_OFFSET);
  // ===
  myTFT.TFTGC9D01Initialize();
  myTFT.TFTsetRotation(myTFT.Degrees_0);
  Serial.println("Start");
}

// main
void loop(void) {
  Test100();
  EndTests();
}

void Test100(void) {
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.setTextColor(myTFT.C_RED, myTFT.C_YELLOW);
  myTFT.setFont(FontArialBold);
  myTFT.setCursor(0, 0);
  myTFT.print("Hi!");
  delay(TEST_DELAY5);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLUE);
  delay(TEST_DELAY1);
  myTFT.fillScreen(myTFT.C_BLACK);
}

void EndTests(void) {
  myTFT.TFTPowerDown();
  Serial.println("End");
  while (1) {};
}
///@endcond
