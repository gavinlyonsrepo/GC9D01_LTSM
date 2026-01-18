/*!
	@file   GC9D01_LTSM.cpp
	@author Gavin Lyons
	@brief  Source file. Contains driver methods for GC9D01_LTSM display 
*/

#include "GC9D01_LTSM.hpp"
 
/*!
	@brief Constructor for class GC9D01_LTSM
*/
GC9D01_LTSM :: GC9D01_LTSM(){}

/*!
	@brief : Init Hardware SPI
*/
void GC9D01_LTSM::TFTHWSPIInitialize(void){
	SPI.begin();
}

/*!
	@brief: Call when powering down TFT
	@note  Will switch off SPI 
*/
void GC9D01_LTSM ::TFTPowerDown(void)
{
	TFTenableDisplay(false);
	if (_resetPinOn == true) {
		DISPLAY16_RST_SetLow;
	}
	DISPLAY16_DC_SetLow;
	DISPLAY16_CS_SetLow;

	if (_hardwareSPI == true) {
		SPI.end();
	}else{
		DISPLAY16_SCLK_SetLow;
		DISPLAY16_SDATA_SetLow;
	}
}

/*!
	@brief Method for Hardware Reset pin control
*/
void GC9D01_LTSM ::TFTResetPIN() {
	if (_resetPinOn == false) return;
	DISPLAY16_RST_SetDigitalOutput;
	DISPLAY16_RST_SetHigh;
	MILLISEC_DELAY(5);
	DISPLAY16_RST_SetLow;
	MILLISEC_DELAY(50);
	DISPLAY16_RST_SetHigh;
	MILLISEC_DELAY(150);
}

/*!
	@brief sets up TFT GPIO
	@param CommDelay SW SPI GPIO delay
	@param rst reset GPIO, optional pass -1 to disable, see note
	@param dc data or command GPIO.
	@param cs chip select GPIO 
	@param sclk Data clock GPIO  
	@param din Data to TFT GPIO 
	@details if -1 is passed for reset pin, software reset is used, if LCD has optional reset pin
			Overloaded one of two this one if for software SPI
*/
void GC9D01_LTSM::TFTsetupGPIO_SPI(uint16_t CommDelay, int8_t rst, int8_t dc, int8_t cs, int8_t sclk, int8_t din)
{
	TFTSwSpiGpioDelaySet(CommDelay);
	_hardwareSPI = false;
	_display_RST= rst;
	_display_DC = dc;
	_display_CS = cs;
	_display_SDATA = din;
	_display_SCLK = sclk;

	if (_display_RST == -1 ){
		_resetPinOn = false;
	}else{
		_resetPinOn = true;
	}
}

/*!
	@brief sets up TFT GPIO
	@param speed_hz SPI baudrate in hz 
	@param rst reset GPIO, optional pass -1 to disable, see note
	@param dc data or command GPIO.
	@param cs chip select GPIO 
	@param sclk Data clock GPIO  
	@param din Data to TFT GPIO 
	@details if -1 is passed for reset pin, software reset is used, if LCD has optional reset pin
			Overloaded one of two this one if for hardware  SPI
*/
void GC9D01_LTSM::TFTsetupGPIO_SPI(uint32_t speed_hz, int8_t rst, int8_t dc, int8_t cs)
{
	_speedSPIHz = speed_hz;
	_hardwareSPI = true;
	_display_RST= rst;
	_display_DC = dc;
	_display_CS = cs;

	if (_display_RST == -1 ){
		_resetPinOn = false;
	}else{
		_resetPinOn = true;
	}
}

/*!
	@brief init routine for GC9D01 controller
*/
void GC9D01_LTSM::TFTGC9D01Initialize() 
{
	if (_resetPinOn == true) 
	{
		TFTResetPIN();
	}else {
		TFTresetSWDisplay();
	}
	DISPLAY16_DC_SetDigitalOutput;
	DISPLAY16_DC_SetLow;
	DISPLAY16_CS_SetDigitalOutput;
	DISPLAY16_CS_SetHigh;
	if (_hardwareSPI == false)
	{
		DISPLAY16_SCLK_SetDigitalOutput;
		DISPLAY16_SDATA_SetDigitalOutput;
		DISPLAY16_SCLK_SetLow;
		DISPLAY16_SDATA_SetLow;
	}else{
		TFTHWSPIInitialize();
	}
		cmdInitSequence();
		//TFTsetRotation(Degrees_0); // optional
}

/*!
	@brief Toggle the invert mode, inverted colours
	@param invert true = invert off , false = invert on
*/
void GC9D01_LTSM ::TFTchangeInvertMode(bool invert) {
	if(invert) {
		writeCommand(GC9D01_INVOFF);
	} else {
		writeCommand(GC9D01_INVON);
	}
}

/*!
	@brief: change rotation of display.
	@param mode display_rotate_e enum
	0 = Normal
	1=  90 rotate
	2 = 180 rotate
	3 =  270 rotate
*/
void GC9D01_LTSM::TFTsetRotation(display_rotate_e mode) {
	uint8_t madctl = MADCTL_FLAGS_t::BGR;
	switch (mode) {
		case Degrees_0 : // 0x00
			_width =_widthStartTFT;
			_height = _heightStartTFT;
			break;
		case Degrees_90: // MX + MV 0x06
			madctl |= (MADCTL_FLAGS_t::MX | MADCTL_FLAGS_t::MV); 
			_width  =_heightStartTFT;
			_height = _widthStartTFT;
			break;
		case Degrees_180:  //  MY + MX  0x0C
			madctl |= (MADCTL_FLAGS_t::MY | MADCTL_FLAGS_t::MX );
			_width =_widthStartTFT;
			_height = _heightStartTFT;
			break;
		case Degrees_270:  // MY + MV 0x0A
			madctl |= (MADCTL_FLAGS_t::MY |MADCTL_FLAGS_t::MV); 
			_width =_heightStartTFT;
			_height = _widthStartTFT;
			break;
	}
	writeCommand(GC9D01_MADCTL);
	writeData(madctl);
}

/*!
	@brief initialise the variables that define the size of the screen
	@param width_TFT width in pixels
	@param height_TFT height in pixels
	@param resolution Current Resolution see enum gc9d01_resolution_e for options 
	@note  The offsets can be adjusted for any issues with manufacture tolerance/defects
*/
void GC9D01_LTSM  :: TFTInitScreenSize( uint16_t width_TFT, uint16_t height_TFT, Resolution_e resolution)
{
	_width = width_TFT;
	_height = height_TFT;
	_widthStartTFT = width_TFT;
	_heightStartTFT = height_TFT;
	_currentResolution = resolution;
}

/*!
	@brief Freq delay used in SW SPI getter, uS delay used in SW SPI method
	@return The  GPIO communications delay in uS
*/
uint16_t GC9D01_LTSM::TFTSwSpiGpioDelayGet(void){return _SWSPIGPIODelay;}

/*!
	@brief Freq delay used in SW SPI setter, uS delay used in SW SPI method
	@param CommDelay The GPIO communications delay in uS
*/
void  GC9D01_LTSM::TFTSwSpiGpioDelaySet(uint16_t CommDelay){_SWSPIGPIODelay = CommDelay;}


/*!
	@brief Command Initialization sequence for GC9D01 display
*/
void GC9D01_LTSM::cmdInitSequence(void)
{
	writeCommand(GC9D01_INREGEN1);
	writeCommand(GC9D01_INREGEN2); 

	// Undocumented in datasheet registers
	constexpr uint8_t startCmd = 0x80;
	constexpr uint8_t endCmd   = 0x8F;
	for (uint8_t cmd = startCmd; cmd <= endCmd; ++cmd)
	{
		writeCommand(cmd);
		writeData(0xFF);
	}

	writeCommand(GC9D01_COLMOD);
	writeData(0x05);
	writeCommand(GC9D01_INVERSION);
	writeData(0x01);

	// Undocumented in datasheet registers
	writeCommand(0x74); 
	uint8_t seqReg1[] = {0x02, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00};
	spiWriteDataBuffer(seqReg1, sizeof(seqReg1));
	writeCommand(0x98); 
	writeData(0x3E);
	writeCommand(0x99);
	writeData(0x3E);

	writeCommand(GC9D01_BLANK_PORCH_CTRL);
	writeData(0x0D);
	writeData(0x0D);
	writeData(0x00); //Third parameter must write but is not valid
	
	 // Undocumented in datasheet registers
	writeCommand(0x60);
	uint8_t seqReg2[] = {0x38, 0x0F, 0x79, 0x67};
	spiWriteDataBuffer(seqReg2, sizeof(seqReg2));
	writeCommand(0x61);
	uint8_t seqReg3[] = {0x38, 0x11, 0x79, 0x67};
	spiWriteDataBuffer(seqReg3, sizeof(seqReg3));
	writeCommand(0x64);
	uint8_t seqReg4[] = {0x38, 0x17, 0x71, 0x5F, 0x79, 0x67};
	spiWriteDataBuffer(seqReg4, sizeof(seqReg4));
	writeCommand(0x65);
	uint8_t seqReg5[] = {0x38, 0x13, 0x71, 0x5B, 0x79, 0x67};
	spiWriteDataBuffer(seqReg5, sizeof(seqReg5));
	writeCommand(0x6A);
	writeData(0x00);
	writeData(0x00);
	writeCommand(0x6C);
	uint8_t segReg6[] = {0x22, 0x02, 0x22, 0x02, 0x22, 0x02, 0x22, 0x22, 0x50};
	spiWriteDataBuffer(segReg6, sizeof(segReg6));
	writeCommand(0x6E);
	uint8_t segReg7[] = {0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x0F, 0x0F, 0x0D, 0x0D,
						 0x0B, 0x0B, 0x09, 0x09, 0x00, 0x00, 0x00,
						 0x00, 0x0A, 0x0A, 0x0C, 0x0C, 0x0E, 0x0E,
						 0x10, 0x10, 0x00, 0x00, 0x02, 0x02, 0x04, 0x04};
	spiWriteDataBuffer(segReg7, sizeof(segReg7));

	writeCommand(GC9D01_DUAL_SINGLE);
	switch (_currentResolution)
	{
		case Resolution_e::RGB160x160_DualGate:
		case Resolution_e::RGB120x160_DualGate:
			writeData(0x01);// Dual-Single Gate Select (BFh) 0=>Single gate
		break;
		case Resolution_e::RGB80x160_SingleGate:
		case Resolution_e::RGB40x160_SingleGate:
			writeData(0x00); // Single gate
		break;
	}
	 // Undocumented in datasheet registers
	writeCommand(0xF9);
	writeData(0x40);
	writeCommand(0x9b);
	writeData(0x3b);
	writeCommand(0x93);
	uint8_t segReg8[] = {0x33, 0x7F, 0x00};
	spiWriteDataBuffer(segReg8, sizeof(segReg8));
	writeCommand(0x7E);
	writeData(0x30);
	writeCommand(0x70);
	uint8_t segReg9[] = {0x0D, 0x02, 0x08, 0x0D, 0x02, 0x08};
	spiWriteDataBuffer(segReg9, sizeof(segReg9));
	writeCommand(0x71);
	uint8_t segReg10[] = {0x0D, 0x02, 0x08};
	spiWriteDataBuffer(segReg10, sizeof(segReg10));
	writeCommand(0x91);
	writeData(0x0E);
	writeData(0x09);

	writeCommand(GC9D01_VREG1A_CTRL);
	writeData(0x18);
	writeCommand(GC9D01_VREG1B_CTRL);
	writeData(0x18);
	writeCommand(GC9D01_VREG2A_CTRL);
	writeData(0x3c);

	writeCommand(GC9D01_GAMMA1);
	uint8_t seqGamma1[] = {0x13, 0x15, 0x04, 0x05, 0x01, 0x38};
	spiWriteDataBuffer(seqGamma1, sizeof(seqGamma1));
	writeCommand(GC9D01_GAMMA3);
	uint8_t seqGamma3[] = {0x13, 0x15, 0x04, 0x05, 0x01, 0x34};
	spiWriteDataBuffer(seqGamma3, sizeof(seqGamma3));
	writeCommand(GC9D01_GAMMA2);
	uint8_t seqGamma2[] = {0x4B, 0xB8, 0x7B, 0x34, 0x35, 0xEF};
	spiWriteDataBuffer(seqGamma2, sizeof(seqGamma2));
	writeCommand(GC9D01_GAMMA4);
	uint8_t seqGamma4[] = {0x47, 0xB4, 0x72, 0x34, 0x35, 0xDA};
	spiWriteDataBuffer(seqGamma4, sizeof(seqGamma4));

	writeCommand(GC9D01_MADCTL);
	writeData(0x08); // BGR
	writeCommand(GC9D01_SLPOUT);
	MILLISEC_DELAY(200); // wait at least 120ms after sending Sleep Out cmd(4.2.4.)
	writeCommand(GC9D01_DISPON);
}


/*!
  @brief SPI displays set an address window rectangle for blitting pixels
  @param  x1 Top left corner x coordinate
  @param  y1  Top left corner y coordinate
  @param  w  Width of window
  @param  h  Height of window
  @note https://en.wikipedia.org/wiki/Bit_blit
 */
void GC9D01_LTSM::setAddrWindow(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h)
{	
	//if drawing a single pixel we need do this to avoid a blank pixel for this device
	if (w - x1 == 1)  {w = x1;}
	if( h - y1 == 1)  {h = y1;}
	uint8_t x1Higher = (x1 >> 8) ;
	uint8_t x1Lower  = (x1 &  0xFF);
	uint8_t x2Higher = (w >> 8);
	uint8_t x2Lower  = (w &  0xFF);
	uint8_t seqCASET[]    {x1Higher ,x1Lower,x2Higher,x2Lower};
	uint8_t y1Higher = (y1 >> 8); 
	uint8_t y1Lower  = (y1 &  0xFF);
	uint8_t y2Higher = (h >> 8);
	uint8_t y2Lower  = (h &  0xFF);
	uint8_t seqRASET[]    {y1Higher,y1Lower,y2Higher,y2Lower};
	writeCommand(GC9D01_CASET); //Column address set
	spiWriteDataBuffer(seqCASET, sizeof(seqCASET));
	writeCommand(GC9D01_RASET); //Row address set
	spiWriteDataBuffer(seqRASET, sizeof(seqRASET));
	writeCommand(GC9D01_RAMWR); // Write to RAM
}

/*!
	@brief This method defines the Vertical Scrolling Area of the display where:
	@param topFixed describes the Top Fixed Area.
	@param scrollArea describes the Scrolling Area.
	@param bottomFixed describes the Bottom Fixed Area.
*/
void GC9D01_LTSM::TFTsetScrollArea(uint16_t topFixed, uint16_t scrollArea, uint16_t bottomFixed) {

	writeCommand(GC9D01_VSCRDEF);
	writeData(topFixed >> 8);
	writeData(topFixed & 0xFF);
	writeData(scrollArea >> 8);
	writeData(scrollArea  & 0xFF);
	writeData(bottomFixed >> 8);
	writeData(bottomFixed & 0xFF);
}

/*!
	@brief This method is used together with the setScrollDefinition.
	@param vsp scrolling mode
*/
void GC9D01_LTSM::TFTsetScrollStart(uint16_t vsp) {
	writeCommand(GC9D01_VSCRSADD);
	writeData(vsp >> 8);
	writeData(vsp & 0xFF);
}

/*! @brief Scroll Mode can be left ,by the Normal Display Mode ON cmd*/
void GC9D01_LTSM::TFTScrollModeLeave(void) {writeCommand(GC9D01_NORON);}

/*!
	@brief Software reset command
*/
void GC9D01_LTSM::TFTresetSWDisplay(void) 
{
  writeCommand(GC9D01_SWRESET);
  MILLISEC_DELAY(150);
}

/*!
	@brief enable /disable display mode
	@param enableDisplay true enable on false disable
	@note Temporarily blank the screen.
	Use Case: Screen blanking, brief off periods without resetting or reinitializing the display.
*/
void GC9D01_LTSM::TFTenableDisplay(bool enableDisplay){
	if(enableDisplay) {
		writeCommand(GC9D01_DISPON);
		_displayOn = true;
	} else {
		writeCommand(GC9D01_DISPOFF);
		_displayOn = false;
	}
}

/*!
	@brief Set the power mode of the display
	@param mode The power state to set
	@details 
		Power states are based on the power control flow chart in the datasheet.
		FIG 89 5.10.2. Power Flow Chart	
*/
void GC9D01_LTSM::TFTsetPowerMode(PowerState_e mode) {
	// If already in the desired state or off , skip
	if (_currentPowerState == mode || _displayOn != true) 
	{
		#ifdef dislib16_DEBUG_MODE_ENABLE
			Serial.println("Warning: TFTsetPowerMode: Display already in this state or off");
		#endif
		return;
	}
	
	// Always return to a known base state
	writeCommand(GC9D01_SLPOUT);
	MILLISEC_DELAY(_sleepDelay);

	switch (mode) {
		case PowerState_e::NormalIdleOff:
			writeCommand(GC9D01_NORON);
			writeCommand(GC9D01_IDLEOFF);
			break;
		case PowerState_e::NormalIdleOn:
			writeCommand(GC9D01_NORON);
			writeCommand(GC9D01_IDLEON);
			break;
		case PowerState_e::PartialIdleOff:
			writeCommand(GC9D01_PTLON);
			writeCommand(GC9D01_IDLEOFF);
			break;
		case PowerState_e::PartialIdleOn:
			writeCommand(GC9D01_PTLON);
			writeCommand(GC9D01_IDLEON);
			break;
		case PowerState_e::SleepNormalIdleOff:
			writeCommand(GC9D01_NORON);
			writeCommand(GC9D01_IDLEOFF);
			writeCommand(GC9D01_SLPIN);
			MILLISEC_DELAY(_sleepDelay);
			break;
		case PowerState_e::SleepNormalIdleOn:
			writeCommand(GC9D01_NORON);
			writeCommand(GC9D01_IDLEON);
			writeCommand(GC9D01_SLPIN);
			MILLISEC_DELAY(_sleepDelay);
			break;
		case PowerState_e::SleepPartialIdleOff:
			writeCommand(GC9D01_PTLON);
			writeCommand(GC9D01_IDLEOFF);
			writeCommand(GC9D01_SLPIN);
			MILLISEC_DELAY(_sleepDelay);
			break;
		case PowerState_e::SleepPartialIdleOn:
			writeCommand(GC9D01_PTLON);
			writeCommand(GC9D01_IDLEON);
			writeCommand(GC9D01_SLPIN);
			MILLISEC_DELAY(_sleepDelay);
			break;
	}
	_currentPowerState = mode;
}

/*!
  @brief Set display brightness (0–255).
  @param level Brightness level, 0 = darkest, 255 = brightest
  @note This is a software brightness control, not hardware PWM, may not work on all displays.
*/
void GC9D01_LTSM::TFTsetBrightness(uint8_t level)
{
	writeCommand(GC9D01_SETCTRL);
	writeData(0x24); // Brightness registers are active, Display Dimming is on, Backlight Off
	writeCommand(GC9D01_SETBRIGHT);
	writeData(level);
}
//**************** EOF *****************
