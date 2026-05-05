/*! 
	@file    GRAPHICS.ino
	@brief   Example file for GC9A01_LTSM display Library test file, tests : graphics.
	@author  Gavin Lyons.
	@details For graphics tests 905-909 to work, 
			     dislib16_ADVANCED_GRAPHICS_ENABLE must be commented in.
           this define is in display16_common_LTSM.hpp in graphics library
           see Readme.
	@note    See USER OPTIONS 1-2 in SETUP function
           This example is for 160x160 Dual gate.
	@test 
  -# Test 901 Lines and Pixels
	-# Test 902 rectangles
	-# Test 903 Circle
	-# Test 904 Triangles
	-# Test 905 Polygons
	-# Test 906 Dot Gap
	-# Test 907 Quadrilateral
	-# Test 908 Ellipse
	-# Test 909 Draw Arc
*/

// libraries
#include "GC9D01_LTSM.hpp"
// No Fonts needed for this test
 
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
    int8_t SDIN_TFT = 26;
    int8_t SCLK_TFT = 27;
    myTFT.TFTsetupGPIO_SPI(SWSPICommDelay, RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
  }
  // ===
  // === USER OPTION 2 Screen Setup ===
  uint16_t OFFSET_X_L = 0;  // Landscape Screen X offset in pixels
  uint16_t OFFSET_Y_L = 0;  // Landscape Screen Y offset in pixels
  uint16_t OFFSET_X_P = 0;  // Portrait Screen X offset in pixels
  uint16_t OFFSET_Y_P = 0;  // Portrait Screen Y offset in pixels
  // Display type, 6 choice,  see readme
  GC9D01_LTSM::Resolution_e DisplayType = GC9D01_LTSM::Resolution_e::RGB160x160_DualGate;
  // Pixel Draw mode type, 6 choices , see readme.  
  GC9D01_LTSM::PixelFixMode_e FixType = GC9D01_LTSM::PixelFixMode_e::Both;
  myTFT.TFTInitScreenSize(TFT_WIDTH, TFT_HEIGHT, DisplayType, FixType, OFFSET_X_L, OFFSET_Y_L, OFFSET_X_P, OFFSET_Y_P);
  // ===
  myTFT.TFTGC9D01Initialize();
  Serial.println("Start");
  myTFT.fillScreen(myTFT.C_BLACK);
}

// MAIN loop
void loop(void) {
  Test901();
  Test902();
  Test903();
  Test904();
#ifdef dislib16_ADVANCED_GRAPHICS_ENABLE
  Test905();
  Test906();
  Test907();
  Test908();
  Test909();
#endif
  EndTests();
}
// End OF MAIN


//   Function Space
void EndTests(void) {
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.TFTPowerDown();  // Power down device
  Serial.println("End");
  while(1) {};
}

void Test901(void)
{
	Serial.println("Test 901: lines");
  delay(TEST_DELAY);
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.drawPixel(85, 5, myTFT.C_RED);
	myTFT.drawPixel(87, 7, myTFT.C_GREEN);
	myTFT.drawPixel(89, 9, myTFT.C_BLUE);
	// Horizontal lines Fast (2)
	myTFT.drawFastHLine(10, 20, 140, myTFT.C_RED);
	myTFT.drawFastHLine(10, 40, 140, myTFT.C_RED);
	// Vertical lines (2)
	myTFT.drawFastVLine(20, 10, 140, myTFT.C_GREEN);
	myTFT.drawFastVLine(40, 10, 140, myTFT.C_GREEN);
	// Diagonal lines (2)
	myTFT.drawLine(10, 10, 150, 150, myTFT.C_BLUE);
	myTFT.drawLine(150, 10, 10, 150, myTFT.C_BLUE);
  // Horizontal lines drawLine(2)
  myTFT.drawLine(80, 10, 80, 150, myTFT.C_CYAN);  
  myTFT.drawLine(100, 10, 100, 150, myTFT.C_CYAN);
  delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}


void Test902(void) {
  Serial.println("Test 902: rectangles");
  DisLib16::Ret_Codes_e returnValue;
  myTFT.drawRectWH(45, 25, 20, 20, myTFT.C_RED);
  returnValue = myTFT.fillRectBuffer(85, 25, 20, 20, myTFT.C_YELLOW);
  if (returnValue != DisLib16::Success) {
    Serial.print("An error occurred returnValue : ");
    Serial.println(returnValue);
  }
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.fillRect(110, 25, 20, 20, myTFT.C_GREEN);
  myTFT.drawRoundRect(15, 80, 70, 60, 20, myTFT.C_CYAN);
  myTFT.fillRoundRect(110, 80, 40, 50, 10, myTFT.C_WHITE);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}

void Test903(void) {
  Serial.println("Test 903 & 904 : Triangles and circles");
  myTFT.drawCircle(40, 40, 25, myTFT.C_GREEN);
  myTFT.fillCircle(80, 80, 25, myTFT.C_YELLOW);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}

void Test904(void) {
  myTFT.drawTriangle(5, 80, 50, 40, 95, 80, myTFT.C_CYAN);
  myTFT.fillTriangle(55, 120, 100, 90, 127, 120, myTFT.C_RED);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}

#ifdef dislib16_ADVANCED_GRAPHICS_ENABLE
void Test905(void) {
  Serial.println("Test 905 : polygons");
  myTFT.fillScreen(myTFT.C_BLACK);
  //drawPolygon( x,  y, sides,  diameter,  rotation, color);
  // octagon draw
  myTFT.drawPolygon(80, 80, 8, 50, 0, false, myTFT.C_GREEN);
  delay(TEST_DELAY5);
  // octagon fill
  myTFT.drawPolygon(80, 80, 8, 50, 0, true, myTFT.C_GREEN);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}

void Test906(void) {
  Serial.println("Test 906 : mesh");
  myTFT.fillScreen(myTFT.C_BLACK);
  //drawDotGrid(x, y,  w,  h,  DotGridGap, color);
  myTFT.drawDotGrid(50, 50, 100, 100, 5, myTFT.C_CYAN);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}

void Test907() {
  Serial.println("Test 907 : quadrilateral");

  // First shape: Quadrilateral
  int16_t x0 = 30, y0 = 30;
  int16_t x1 = 80, y1 = 30;
  int16_t x2 = 80, y2 = 80;
  int16_t x3 = 30, y3 = 80;

  // Second shape: Parallelogram 
  int16_t x4 = 90,  y4 = 100;
  int16_t x5 = 130, y5 = 110;
  int16_t x6 = 120, y6 = 140;
  int16_t x7 = 80,  y7 = 130;

  // Third shape: Trapezoid 1
  int16_t x8  = 95,  y8  = 110;
  int16_t x9  = 100, y9  = 130;
  int16_t x10 = 140, y10 = 130;
  int16_t x11 = 150, y11 = 110;

  // Fourth shape: Trapezoid 2
  int16_t x12 = 40,  y12 = 120;
  int16_t x13 = 55,  y13 = 155;
  int16_t x14 = 100, y14 = 155;
  int16_t x15 = 70,  y15 = 120;

  myTFT.drawQuadrilateral(x0, y0, x1, y1, x2, y2, x3, y3, myTFT.C_RED);
  myTFT.fillQuadrilateral(x4, y4, x5, y5, x6, y6, x7, y7, myTFT.C_GREEN);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);

  myTFT.fillQuadrilateral(x8, y8, x9, y9, x10, y10, x11, y11, myTFT.C_BLUE);
  myTFT.drawQuadrilateral(x12, y12, x13, y13, x14, y14, x15, y15, myTFT.C_YELLOW);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}

void Test908(void) {
  Serial.println("Test 908 : Ellipse");
  myTFT.fillScreen(myTFT.C_BLACK);
  // Define ellipses at different positions
  // Format: drawEllipse(centerX, centerY, semiMajorAxis, semiMinorAxis, fill, color)
  // Draw unfilled ellipses
  myTFT.drawEllipse(60, 60, 18, 30, false, myTFT.C_WHITE);
  myTFT.drawEllipse(120, 60, 18, 30, false, myTFT.C_RED);
  delay(TEST_DELAY5);

  // Draw filled ellipses in same positions
  myTFT.drawEllipse(60, 60, 18, 30, true, myTFT.C_WHITE);
  myTFT.drawEllipse(120, 60, 18, 30, true, myTFT.C_RED);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}

void Test909(void) {
  Serial.println("Test 909 : Drawing Arc: drawArc");
  myTFT.fillScreen(myTFT.C_BLACK);
  int16_t centerX = 80;  // X-coordinate of the circle center
  int16_t centerY = 80;  // Y-coordinate of the circle center
  int16_t radius = 50;    // Radius of the circle
  Serial.print(" Angle offset default : ");
  Serial.println(myTFT.getArcAngleOffset());
  //drawArc(cx,  cy,  radius, thickness, startAngle, endAngle, color);
  // _arcAngleOffset is zero default means 0 is positive Axis , Arc drawn counterclockwise
  // XXX 270 XXX
  // 180 XXX 000
  // XXX 090 XXX
  // Draw the full lower half (0° to 180°)
  myTFT.drawArc(centerX, centerY, radius, 7, 0.0f, 180.0f, myTFT.C_GREEN);
  delay(TEST_DELAY2);
  myTFT.fillScreen(myTFT.C_BLACK);
  // Draw the full upper half (180° to 360°)
  myTFT.drawArc(centerX, centerY, radius, 10, 180.0f, 360.0f, myTFT.C_BLUE);
  delay(TEST_DELAY2);
  myTFT.fillScreen(myTFT.C_BLACK);
  // Draw the first quarter (0° to 90°)
  myTFT.drawArc(centerX, centerY, radius, 2, 0.0f, 90.0f, myTFT.C_RED);
  delay(TEST_DELAY);
  myTFT.fillScreen(myTFT.C_BLACK);
  // Draw the second quarter (90° to 180°)
  myTFT.drawArc(centerX, centerY, radius, 2, 90.0f, 180.0f, myTFT.C_YELLOW);
  delay(TEST_DELAY);
  myTFT.fillScreen(myTFT.C_BLACK);
  // Draw the third quarter (180° to 270°)
  myTFT.drawArc(centerX, centerY, radius, 2, 180.0f, 270.0f, myTFT.C_CYAN);
  delay(TEST_DELAY);
  myTFT.fillScreen(myTFT.C_BLACK);
  // Draw the fourth quarter (270° to 360°)
  myTFT.drawArc(centerX, centerY, radius, 2, 270.0f, 360.0f, myTFT.C_MAGENTA);
  delay(TEST_DELAY);
  myTFT.fillScreen(myTFT.C_BLACK);
}

#endif

void DisplayReset(void) {
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}
/// @endcond
