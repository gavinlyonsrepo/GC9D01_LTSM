/*! 
	@file    TEXT.ino
	@brief   Example file for GC9D01_LTSM display Library test file, tests : Fonts & text.
	@author  Gavin Lyons.
	@note    See USER OPTIONS 1-2 in SETUP function
           This example is for 160x160 Dual gate.
	@test 
	-# Test 701 Print out all fonts with writeCharString
	-# Test 702 Print out all fonts with print
	-# Test 703 Misc print class tests(string object, println invert, wrap, base nums etc)
*/

// libraries
#include "GC9D01_LTSM.hpp"
// Included Fonts
#include <fonts_LTSM/FontArialBold_LTSM.hpp>
#include <fonts_LTSM/FontArialRound_LTSM.hpp>
#include <fonts_LTSM/FontDefault_LTSM.hpp>
#include <fonts_LTSM/FontGll_LTSM.hpp>
#include <fonts_LTSM/FontGroTeskBig_LTSM.hpp>
#include <fonts_LTSM/FontGroTesk_LTSM.hpp>
#include <fonts_LTSM/FontHallfetica_LTSM.hpp>
#include <fonts_LTSM/FontInconsola_LTSM.hpp>
#include <fonts_LTSM/FontMega_LTSM.hpp>
#include <fonts_LTSM/FontMint_LTSM.hpp>
#include <fonts_LTSM/FontOrla_LTSM.hpp>
#include <fonts_LTSM/FontPico_LTSM.hpp>
#include <fonts_LTSM/FontRetro_LTSM.hpp>
#include <fonts_LTSM/FontSevenSeg_LTSM.hpp>
#include <fonts_LTSM/FontSinclairS_LTSM.hpp>
#include <fonts_LTSM/FontSixteenSeg_LTSM.hpp>

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
  uint16_t OFFSET_X = 0;  // Screen X offset in pixels
  uint16_t OFFSET_Y = 0;  // Screen Y offset in pixels
  // Display type, 4 choice,  see readme
  GC9D01_LTSM::Resolution_e DisplayType = GC9D01_LTSM::Resolution_e::RGB160x160_DualGate;
  // Pixel Draw mode type, 4 choices , see readme.  
  GC9D01_LTSM::PixelFixMode_e FixType = GC9D01_LTSM::PixelFixMode_e::Both;
  myTFT.TFTInitScreenSize(TFT_WIDTH, TFT_HEIGHT, DisplayType, FixType, OFFSET_X, OFFSET_Y);
  // ===
  myTFT.TFTGC9D01Initialize();
  Serial.println("Start");
}

// MAIN loop
void loop(void) {
  Test701();
  Test702();
  Test703();
  EndTests();
}
// End OF MAIN


//   Function Space
void EndTests(void) {
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.TFTPowerDown();  // Power down device
  Serial.println("End");
  while (1) {};
}

void Test701(void) {

  Serial.println("Test 701: Print out some fonts with writeCharString");
  char teststr1[] = "Default ";
  char teststr2[] = "GLL ";
  char teststr3[] = "Pico ";
  char teststr4[] = "Sinclair ";
  char teststr5[] = "Orla ";
  char teststr6[] = "Retro ";
  char teststr7[] = "Mega";
  char teststr8[] = "Arial b";
  char teststr9[] = "Hall ";

  myTFT.fillScreen(myTFT.C_BLACK);
  // Start at ~20px Y offset and use ~20px spacing
  int startY = 20;
  int spacing = 20;
  int x = 30;
  int y = startY;

  myTFT.setFont(FontDefault);
  myTFT.writeCharString(x, y, teststr1);
  y += spacing;
  myTFT.setFont(FontGll);
  myTFT.writeCharString(x, y, teststr2);
  y += spacing;
  myTFT.setFont(FontPico);
  myTFT.writeCharString(x, y, teststr3);
  y += spacing;
  myTFT.setFont(FontSinclairS);
  myTFT.writeCharString(x, y, teststr4);
  y += spacing;
  myTFT.setFont(FontOrla);
  myTFT.writeCharString(x, y, teststr5);
  y += spacing;
  myTFT.setFont(FontRetro);
  myTFT.writeCharString(x, y, teststr6);

  DisplayReset();

  y = startY;
  myTFT.setFont(FontMega);
  myTFT.writeCharString(x, y, teststr7);
  y += spacing;
  myTFT.setFont(FontArialBold);
  myTFT.writeCharString(x, y, teststr8);
  y += spacing;
  myTFT.setFont(FontHallfetica);
  myTFT.writeCharString(x, y, teststr9);
  y += spacing;
  myTFT.setFont(FontArialRound);
  DisplayReset();
}

void Test702(void) {
  Serial.println("Test 702: Print class methods");

  // Test Fonts default + pico + sinclair + retro
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);

  int x = 25;
  int y = 20;
  int spacing = 25;

  // --- First Block ---
  myTFT.setCursor(x, y);
  myTFT.setFont(FontDefault);
  myTFT.print("Default ");
  myTFT.print(-43);
  y += spacing;
  myTFT.setCursor(x, y);
  myTFT.setFont(FontGll);
  myTFT.print("GLL ");
  myTFT.print(123.284, 1);  // print 123.3
  y += spacing;
  myTFT.setCursor(x, y);
  myTFT.setFont(FontPico);
  myTFT.print("pico ");
  myTFT.print(747);
  y += spacing;
  myTFT.setCursor(x, y);
  myTFT.setFont(FontSinclairS);
  myTFT.print("sinclair ");
  myTFT.print(456);
  y += spacing;
  myTFT.setCursor(x, y);
  myTFT.setFont(FontRetro);
  myTFT.print("retro ");
  myTFT.print(-3.14);

  DisplayReset();

  // --- Second Block ---
  y = 20;
  myTFT.setCursor(x, y);
  myTFT.setFont(FontMega);
  myTFT.print("mega");
  y += spacing;
  myTFT.setCursor(x, y);
  myTFT.setFont(FontArialBold);
  myTFT.print("A bold");
  y += spacing;
  myTFT.setCursor(x, y);
  myTFT.setFont(FontHallfetica);
  myTFT.print("hall f");

  DisplayReset();

  // --- Third Block ---
  y = 20;
  myTFT.setFont(FontOrla);
  myTFT.setCursor(x, y);
  myTFT.print(-7.1);
  myTFT.print(" Orla");
  y += spacing;
  myTFT.setCursor(x, y + spacing);
  myTFT.setFont(FontArialRound);
  myTFT.print(-8.2 );
  myTFT.print(" a rnd");

  DisplayReset();

  // --- 4th Block ---
  y = 20;
  myTFT.setCursor(x, y);
  myTFT.setFont(FontGroTesk);
  myTFT.println("GROTESK");
  myTFT.print(1.78);

  DisplayReset();

  // --- 5th block ---
  y= 20;
  myTFT.setCursor(x, y);
  myTFT.setFont(FontGroTeskBig);
  myTFT.println("GB");
  myTFT.print(1.78);
  DisplayReset();

  // --- 6th block ---
  myTFT.setCursor(x, y);
  myTFT.setFont(FontSevenSeg);
  myTFT.print(7);
  DisplayReset();
  // --- 7th block ----
  myTFT.setCursor(x, y);
  myTFT.setFont(FontSixteenSeg);
  myTFT.print(16);
  DisplayReset();
}

void Test703(void) {
  Serial.println("Test 703: Misc print class(println invert, wrap, base nums etc)");
  // --- Inverted print fonts ---
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.setTextColor(myTFT.C_RED, myTFT.C_YELLOW);
  myTFT.setFont(FontSinclairS);
  myTFT.setCursor(30, 30);
  myTFT.print(-49);
  myTFT.setFont(FontMega);
  myTFT.setCursor(10, 60);
  myTFT.print(12.09);
  myTFT.setCursor(10, 90);
  myTFT.print("ABCD");

  DisplayReset();

  // --- Inverted print ---
  myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_RED);
  myTFT.setFont(FontArialBold);
  myTFT.setCursor(40, 40);
  myTFT.print("INVERT");
  myTFT.setCursor(40, 80);
  myTFT.print(-94.982, 2);
  myTFT.setFont(FontHallfetica);
  myTFT.setCursor(40, 100);
  myTFT.print("INVERT");
  myTFT.setCursor(40, 140);
  myTFT.print(123456);

  DisplayReset();

  // --- Print in different number bases ---
  myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);
  uint8_t numPos = 47;
  myTFT.setFont(FontMega);
  myTFT.setCursor(40, 50);
  myTFT.print(numPos, DEC);  // 47
  myTFT.setCursor(40, 70);
  myTFT.print(numPos, BIN);  // 101111
  myTFT.setCursor(40, 90);
  myTFT.print(numPos, OCT);  // 57
  myTFT.setCursor(40, 110);
  myTFT.print(numPos, HEX);  // 2F

  DisplayReset();
}

void DisplayReset(void) {
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}
/// @endcond
