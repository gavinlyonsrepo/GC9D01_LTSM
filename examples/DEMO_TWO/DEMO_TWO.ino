/*!
	@file   DEMO_TWO.ino
	@author Gavin Lyons
	@brief   Example file for GC9D01_LTSM arduino library. demo
	@note   See USER OPTIONS 1-2 in SETUP function
            dislib16_ADVANCED_GRAPHICS_ENABLE and
            dislib16_ADVANCED_SCREEN_BUFFER_ENABLE must be enabled for this example
            or it will not compile. 
            These settings are in file :display16_common_LTSM.hpp
            of graphics library. 
            Requires usable heap memory of (160x160x2) = 51,200 bytes for buffer
            This example is for 160x160 Dual gate.
	@test
		-# Test 408 gauge demo 
*/

// libraries
#include "GC9D01_LTSM.hpp"
// Fonts needed
#include "fonts_LTSM/FontDefault_LTSM.hpp"

/// @cond

#ifndef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is not defined. it is required for this example")
#endif
#ifndef dislib16_ADVANCED_GRAPHICS_ENABLE
#pragma message("gll: dislib16_ADVANCED_GRAPHICS_ENABLE is not defined. it is required for this demo")
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
  // NB set to use pixel mode for text                 
  myTFT.setTextCharPixelOrBuffer(true); 

  Serial.println("Start");
}

// MAIN loop
void loop(void) {
  arcGauge(200);
  EndTests();
}
// End OF MAIN


/*!
	@brief  Stop testing and shutdown the TFT
*/
void EndTests(void) {
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.clearBuffer(myTFT.C_BLACK);
  myTFT.destroyBuffer();
  myTFT.TFTPowerDown();
  Serial.println("End");
  while (1) {};
}

//demo function
void arcGauge(uint16_t countLimit) {
  myTFT.clearBuffer(myTFT.C_BLACK);
  myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_BLACK);
  printf("Arc Demo ends at : %u\n", countLimit);
  int16_t currentValue = 150;
  int16_t oldValue = 150;
  myTFT.setFont(FontDefault);
  char buffer[10];
  // Draw Gauge
  uint16_t count = 1;
  uint16_t x = 80;
  uint16_t y = 80;
  uint16_t radius = 60;
  const int16_t minValue = 1;
  const int16_t maxValue = 255;
  //myTFT.drawArc(centerX, centerY, radius, 10, 180.0f, 360.0f, myTFT.C_BLUE);
  myTFT.drawArc(x, y, radius, 11, 150.0f, 180.0f, myTFT.C_RED);
  myTFT.drawArc(x, y, radius, 11, 181.0f, 250.0f, myTFT.C_ORANGE);
  myTFT.drawArc(x, y, radius, 11, 251.0f, 320.0f, myTFT.C_YELLOW);
  myTFT.drawArc(x, y, radius, 11, 321.0f, 30.0f, myTFT.C_GREEN);
  drawGaugeMarkers(x, y, radius, 150.0f, 390.0f, 1.3f);
  drawGaugeMarkers(x, y, radius, 165.0f, 375.0f, 1.1f);
  myTFT.writeBuffer();
	// Initialize random generator , optional
	// randomSeed(analogRead(A0));
  while (count++ < countLimit) {
    int step = random(-10, 11);  // Arduino built-in RNG
    currentValue += step;
    currentValue = constrain(currentValue, minValue, maxValue);
    myTFT.setCursor(40, 125);
    if (oldValue != currentValue) {
      drawPointer(currentValue, oldValue, x, y, radius, myTFT.C_GREEN, myTFT.C_BLACK);
      oldValue = currentValue;
    }
    myTFT.print("Count: ");
    myTFT.print(count);
    myTFT.setCursor(40, 140);
    myTFT.print("Value: ");
    sprintf(buffer, "%03d", currentValue);
    myTFT.print(buffer);
    myTFT.writeBuffer();
  }
  myTFT.clearBuffer(myTFT.C_BLACK);
}

void drawGaugeMarkers(uint16_t centerX, uint16_t centerY, uint16_t radius, int startAngle, int endAngle, float scaleFactor) {
  float angleRad, innerX, innerY, outerX, outerY;
  int angle;
  // Loop through the specified angle range, drawing ticks every 30 degrees
  for (angle = startAngle; angle <= endAngle; angle += 30) {
    // Convert degrees to radians
    angleRad = angle * (PI / 180);
    // inner marker position
    innerX = cos(angleRad) * (radius / scaleFactor);
    innerY = sin(angleRad) * (radius / scaleFactor);
    // outer marker position
    outerX = cos(angleRad) * radius;
    outerY = sin(angleRad) * radius;
    // Draw marker line from inner to outer radius
    myTFT.drawLine(1 + centerX + innerX, 1 + centerY + innerY,
                   1 + centerX + outerX, 1 + centerY + outerY,
                   myTFT.C_WHITE);
  }
}

void drawPointer(int16_t &currentValue, int16_t &oldValue, uint16_t x, uint16_t y, uint16_t r, uint16_t colour, uint16_t bcolour) {
  uint16_t i;
  if (currentValue > oldValue) {
    // Incrementally move the pointer from oldValue to currentValue
    for (i = oldValue; i <= currentValue; i++) {
      // Erase the previous pointer position
      drawPointerHelper(i - 1, x, y, r, bcolour);
      // Draw the pointer at the new position
      drawPointerHelper(i, x, y, r, colour);
      // Introduce a small delay for smooth animation
      if ((currentValue - oldValue) < 128) {
        delay(20);
      } else {
        delay(30);  // Slower update for small changes
      }
    }
  } else {
    // Decrementally move the pointer from oldValue to currentValue
    for (i = oldValue; i >= currentValue; i--) {
      // Erase the previous pointer position
      drawPointerHelper(i + 1, x, y, r, bcolour);
      // Draw the pointer at the new position
      drawPointerHelper(i, x, y, r, colour);
      // Adjust delay based on the difference to control animation speed
      if ((oldValue - currentValue) >= 128) {
        delay(20);  // Faster update for large changes
      } else {
        delay(30);  // Slower update for small changes
      }
    }
  }
}

void drawPointerHelper(int16_t value, uint16_t centerX, uint16_t centerY, uint16_t radius, uint16_t pointerColor) {
  float angleRad, pointerX, pointerY;
  int16_t minValue = 0;
  int16_t maxValue = 255;
  int startAngle = 150;
  int endAngle = 240;
  if (value > maxValue) value = maxValue;
  if (value < minValue) value = minValue;
  // Map value to an angle in radians based on the gauge range
  angleRad = (((float)(uint16_t)(value - minValue) / (float)(uint16_t)(maxValue - minValue) * endAngle)
              + startAngle)
             * (PI / 180);
  // Calculate pointer tip coordinates
  pointerX = cos(angleRad) * (radius / 1.35);
  pointerY = sin(angleRad) * (radius / 1.35);
  // Draw the pointer line from the center to the calculated tip position
  myTFT.drawLine(centerX, centerY, 1 + centerX + pointerX, 1 + centerY + pointerY, pointerColor);
  myTFT.fillCircle(centerX, centerY, 2, pointerColor);
}
/// @endcond
