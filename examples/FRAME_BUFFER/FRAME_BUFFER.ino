/*!
	@file   FRAME_BUFFER.ino
	@author Gavin Lyons
	@brief  Example file for GC9D01_LTSM arduino library : Advanced Frame buffer mode
          See USER OPTIONS 0-2 in SETUP function
            dislib16_ADVANCED_GRAPHICS_ENABLE and
            dislib16_ADVANCED_SCREEN_BUFFER_ENABLE must be enabled for this example
            or it will not compile. 
            These settings are in file :display16_common_LTSM.hpp
            of graphics library. 
            Requires usable heap memory of (160x160x2) = 51,200 bytes for buffer
            This example is for 160x160 Dual gate.
	@test
		-# Test 1500 Color + text
    -# Test 1504 Lines + shapes
    -# Test Bitmaps + rotate

*/

// libraries
#include "GC9D01_LTSM.hpp"
// Fonts needed
#include "fonts_LTSM/FontArialRound_LTSM.hpp"

// bitmap test data
#include "bitmap_test_data_LTSM/Bitmap_TEST_Data_16color2.hpp"
/// @cond

#ifndef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is not defined. it is required for this example")
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
  uint16_t OFFSET_X = 0;  // Screen X offset in pixels
  uint16_t OFFSET_Y = 0;  // Screen Y offset in pixels
  // Display type, 4 choice,  see readme
  GC9D01_LTSM::Resolution_e DisplayType = GC9D01_LTSM::Resolution_e::RGB160x160_DualGate;
  // Pixel Draw mode type, 4 choices , see readme.  
  GC9D01_LTSM::PixelFixMode_e FixType = GC9D01_LTSM::PixelFixMode_e::Both;
  myTFT.TFTInitScreenSize(TFT_WIDTH, TFT_HEIGHT, DisplayType, FixType, OFFSET_X, OFFSET_Y);
  // ===
  myTFT.TFTGC9D01Initialize();

  // set up buffer
  if (myTFT.setBuffer() != DisLib16::Success){
    Serial.println("buffer not set");
    while(1){};
  }
  // set to use pixel mode for text                 
  myTFT.setTextCharPixelOrBuffer(true); 
  Serial.println("Start");
}


// MAIN loop
void loop(void) {
  Test1500();
  Test1501();
  Test1502();
  Test1503();
  Test1504();
  Test1505();
  EndTests();
}
// End OF MAIN 

void EndTests(void) {
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.clearBuffer(myTFT.C_BLACK);
  myTFT.destroyBuffer();
  myTFT.TFTPowerDown();
  Serial.println("End");
  while (1) {};
}

void ScreenReset(void) {
  myTFT.writeBuffer();
  delay(2500);
  myTFT.clearBuffer(myTFT.C_BLACK);
}

void Test1500(void) {

  Serial.println("Test: Color Test");
  ColorBlock();
  myTFT.writeBuffer();
  delay(5000);
  myTFT.fillScreen(myTFT.C_BLACK);
  
  Serial.println("Test: clear buffer RGB");
  myTFT.clearBuffer(myTFT.C_RED);
  myTFT.writeBuffer();
  delay(1000);
  myTFT.clearBuffer(myTFT.C_GREEN);
  myTFT.writeBuffer();
  delay(1000);
  myTFT.clearBuffer(myTFT.C_BLUE);
  myTFT.writeBuffer();
  delay(1000);

  Serial.println("Test: text");
  myTFT.clearBuffer(myTFT.C_BLACK);
  myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
  myTFT.setCursor(15, 50);
  myTFT.setFont(FontArialRound);
  myTFT.print("Buffer Mode text test");
  myTFT.writeBuffer();
  delay(5000);
  myTFT.clearBuffer(myTFT.C_BLACK);
}

void ColorBlock(void) {
  myTFT.fillRoundRect(8, 50, 24, 48, 8, myTFT.C_RED);
  myTFT.fillRoundRect(32, 50, 24, 48, 8, myTFT.C_GREEN);
  myTFT.fillRoundRect(56, 50, 24, 48, 8, myTFT.C_BLUE);
  myTFT.fillRoundRect(80, 50, 24, 48, 8, myTFT.C_YELLOW);
  myTFT.fillRoundRect(104, 50, 24, 48, 8, myTFT.C_WHITE);
}

void Test1501(void)
{
	Serial.println("Test lines");
  delay(1000);
	myTFT.clearBuffer(myTFT.C_BLACK);

	myTFT.drawPixel(85, 5, myTFT.C_RED);
	myTFT.drawPixel(87, 7, myTFT.C_GREEN);
	myTFT.drawPixel(89, 9, myTFT.C_BLUE);
	// Horizontal lines (2)
	myTFT.drawFastHLine(10, 20, 140, myTFT.C_RED);
	myTFT.drawFastHLine(10, 40, 140, myTFT.C_RED);
	// Vertical lines (2)
	myTFT.drawFastVLine(20, 10, 140, myTFT.C_GREEN);
	myTFT.drawFastVLine(40, 10, 140, myTFT.C_GREEN);
	// Diagonal lines (2)
	myTFT.drawLine(10, 10, 150, 150, myTFT.C_BLUE);
	myTFT.drawLine(150, 10, 10, 150, myTFT.C_BLUE);
	// Shallow slope lines (2)
	myTFT.drawLine(10, 80, 150, 100, myTFT.C_YELLOW);
	myTFT.drawLine(10, 100, 150, 80, myTFT.C_YELLOW);
	// Steep slope lines (2)
	myTFT.drawLine(80, 10, 100, 150, myTFT.C_CYAN);
	myTFT.drawLine(100, 10, 80, 150, myTFT.C_CYAN);
  myTFT.writeBuffer();
  delay(5000);
  myTFT.clearBuffer(myTFT.C_BLACK);
}


void Test1502(void) {
  Serial.println("Test: rectangles");
  myTFT.drawRectWH(45, 25, 20, 20, myTFT.C_RED);
  myTFT.fillRect(110, 25, 20, 20, myTFT.C_GREEN);
  myTFT.drawRoundRect(15, 80, 70, 60, 20, myTFT.C_CYAN);
  myTFT.fillRoundRect(110, 80, 40, 50, 10, myTFT.C_WHITE);
  myTFT.writeBuffer();
  delay(5000);
  myTFT.clearBuffer(myTFT.C_BLACK);
}

void Test1503(void) {
  Serial.println("Test: Triangles and circles");
  myTFT.drawCircle(40, 40, 25, myTFT.C_GREEN);
  myTFT.fillCircle(80, 80, 25, myTFT.C_YELLOW);
  myTFT.writeBuffer();
  delay(5000);
  myTFT.clearBuffer(myTFT.C_BLACK);
}

void Test1504(void) {
  myTFT.drawTriangle(5, 80, 50, 40, 95, 80, myTFT.C_CYAN);
  myTFT.fillTriangle(55, 120, 100, 90, 127, 120, myTFT.C_RED);
  myTFT.writeBuffer();
  delay(5000);
  myTFT.clearBuffer(myTFT.C_BLACK);
}

void Test1505(void)
{
  myTFT.drawBitmap16Data(40, 60, PosterImage, 80, 48);
  myTFT.writeBuffer();
  delay(2000);
  myTFT.drawSpriteData(55, 55, SpriteTest16, 32, 32, myTFT.C_LBLUE, false);
  myTFT.drawSpriteData(90, 90, SpriteTest16, 32, 32, myTFT.C_LBLUE, true);
  myTFT.writeBuffer();
  delay(5000);

  myTFT.clearBuffer(myTFT.C_BLACK);
  myTFT.writeBuffer();
  myTFT.TFTsetRotation(myTFT.Degrees_90);
  myTFT.drawBitmap16Data(40, 60, PosterImage, 80, 48);
  myTFT.writeBuffer();
  delay(5000);

  myTFT.clearBuffer(myTFT.C_BLACK);
  myTFT.writeBuffer();
  myTFT.TFTsetRotation(myTFT.Degrees_180);
  myTFT.drawBitmap16Data(40, 60, PosterImage, 80, 48);
  myTFT.writeBuffer();
  delay(5000);

  myTFT.clearBuffer(myTFT.C_BLACK);
  myTFT.writeBuffer();
  myTFT.TFTsetRotation(myTFT.Degrees_270);
  myTFT.drawBitmap16Data(40, 60, PosterImage, 80, 48);
  myTFT.writeBuffer();
  delay(5000);
}
/// @endcond
