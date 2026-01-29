/*! 
	@file    FUNCTIONS.ino
	@brief   Example file for GC9D01_LTSM display Library test file, tests : Functions.
	@author  Gavin Lyons.
	@note    See USER OPTIONS 1-2 in SETUP function
	@test 
	-# Test 500 RGB color OK?
	-# Test 501 Scroll test
	-# Test 502 Rotate
	-# Test 503 change modes test -> Invert, display on/off and Sleep.
*/

// libraries
#include "GC9D01_LTSM.hpp"
// Included Fonts
#include <fonts_LTSM/FontRetro_LTSM.hpp>

/// @cond

//  Test timing related defines
#define TEST_DELAY5 5000
#define TEST_DELAY2 2000
#define TEST_DELAY 1000

#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif

uint16_t TFT_WIDTH = 160;   // Screen width in pixels
uint16_t TFT_HEIGHT = 160;  // Screen height in pixels
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
  // Display type, multiple choice see readme
  GC9D01_LTSM::Resolution_e DisplayType = GC9D01_LTSM::Resolution_e::RGB160x160_DualGate;
  // Pixel Draw mode type, 4 choices , see readme.  
  GC9D01_LTSM::PixelFixMode_e FixType = GC9D01_LTSM::PixelFixMode_e::Both;
  myTFT.TFTInitScreenSize(TFT_WIDTH, TFT_HEIGHT, DisplayType, FixType);
  // ===
  myTFT.TFTGC9D01Initialize();
  Serial.println("Start");
}

// MAIN loop
void loop(void) {
  myTFT.setFont(FontRetro);
  myTFT.fillScreen(myTFT.C_RED);
  delay(TEST_DELAY);
  Test500();
  Test501();
  Test502();
  Test503();
  EndTests();
}
// End OF MAIN


//   Function Space
void EndTests(void) {
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.TFTPowerDown();  // Power down device
  Serial.println("End");
  while (1) {}; // wait here forever.
}

void Test500(void) {
  Serial.println("Test 500: Color Test:: Red,green,blue,yellow,white, black background");
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
  ColorBlock();
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}

void ColorBlock(void) {
  myTFT.fillRoundRect(8, 50, 24, 48, 8, myTFT.C_RED);
  myTFT.fillRoundRect(32, 50, 24, 48, 8, myTFT.C_GREEN);
  myTFT.fillRoundRect(56, 50, 24, 48, 8, myTFT.C_BLUE);
  myTFT.fillRoundRect(80, 50, 24, 48, 8, myTFT.C_YELLOW);
  myTFT.fillRoundRect(104, 50, 24, 48, 8, myTFT.C_WHITE);
}

void Test501(void) {
  Serial.println("Test 501: Scroll");
  myTFT.fillScreen(myTFT.C_BLACK);
  char teststr0[] = "Scroll";
  myTFT.writeCharString(25, 100, teststr0);

  const uint16_t screenH = 160;
  const uint16_t scrollArea = screenH - 0;  // no fixed areas
  myTFT.TFTsetScrollArea(0, scrollArea, 0);

  for (uint16_t y = 0; y < screenH; y++) {
    myTFT.TFTsetScrollStart(y);
    delay(20);
  }
  myTFT.TFTsetScrollStart(0);  // reset
  myTFT.TFTScrollModeLeave();  // leave scroll mode
  delay(TEST_DELAY);
  myTFT.fillScreen(myTFT.C_BLACK);
}

void Test502() {
  Serial.println("Test 502: Rotate");
  char teststr0[] = "R 0";    //normal
  char teststr1[] = "R 90";   // 90
  char teststr2[] = "R 180";  // 180
  char teststr3[] = "R 270";  // 270

  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.TFTsetRotation(myTFT.Degrees_0);
  myTFT.writeCharString(15, 80, teststr0);
  delay(TEST_DELAY2);

  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.TFTsetRotation(myTFT.Degrees_90);
  myTFT.writeCharString(25, 80, teststr1);
  delay(TEST_DELAY2);

  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.TFTsetRotation(myTFT.Degrees_180);
  myTFT.writeCharString(25, 80, teststr2);
  delay(TEST_DELAY2);

  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.TFTsetRotation(myTFT.Degrees_270);
  myTFT.writeCharString(25, 80, teststr3);
  delay(TEST_DELAY2);

  myTFT.TFTsetRotation(myTFT.Degrees_0);
  myTFT.fillScreen(myTFT.C_BLACK);
}

void Test503() {
  Serial.println("Test 503: Mode Tests");
  char teststr1[] = "Invert";
  char teststr2[] = "LCD on/off";
  char teststr3[] = "Sleep on/off";
  ColorBlock();
  myTFT.writeCharString(25, 100, teststr1);
  delay(TEST_DELAY2);

  // Invert on and off
  myTFT.TFTchangeInvertMode(false);
  Serial.println("Test 503-1: Invert on ");
  delay(TEST_DELAY5);
  myTFT.TFTchangeInvertMode(true);
  Serial.println("Test 503-1: Invert off ");
  delay(TEST_DELAY5);

  // Display on and off
  myTFT.TFTenableDisplay(false);
  Serial.println("Test 503-2: Display off");
  myTFT.writeCharString(25, 100, teststr2);
  delay(TEST_DELAY5);
  myTFT.TFTenableDisplay(true);
  Serial.println("Test 503-2: Turn Display back on");
  delay(TEST_DELAY5);

  // sleep mode
  myTFT.writeCharString(25, 100, teststr3);
  delay(TEST_DELAY5);
  myTFT.TFTsetPowerMode(GC9D01_LTSM::PowerState_e::SleepNormalIdleOff);
  Serial.println("Test 503-3: Power mode Sleep ON Idle Off");
  delay(TEST_DELAY5);
  delay(TEST_DELAY5);
  //back to sleep off - normal on - idle off
  myTFT.TFTsetPowerMode(GC9D01_LTSM::PowerState_e::NormalIdleOff);
  delay(TEST_DELAY5);
  Serial.println("Done!");
}


void DisplayReset(void) {
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}
/// @endcond
