/*!
	@file    GC9D01_LTSM.hpp
	@author  Gavin Lyons, LionTron Systems
	@brief   Arduino Library header file, Contains driver methods for GC9D01_LTSM display
	@details Depends on arduino library display16_graphics_LTSM  by LionTron systems
	@todo    investigate brightness mode, not working?
	@todo    investigate idle mode on , some lines on screen missing?
*/

#pragma once

// Section Libraries
#include "display16_graphics_LTSM.hpp"

/*! @brief Class to control GC9D01 TFT basic functionality. */
class GC9D01_LTSM : public display16_graphics_LTSM
{

public:
	GC9D01_LTSM();
	~GC9D01_LTSM() {};

	/*!
	 * @brief GC9D01 power states based on the power control flow chart.
	 * These states combine the modes: Normal/Partial, Idle ON/OFF, and Sleep IN/OUT.
	 * They are listed from highest to lowest power consumption.
	 * 	Power states are based on the power control flow chart in the datasheet.
		figure89 5.10.2. Power Flow Chart	
	 */
	enum class PowerState_e : uint8_t{
		NormalIdleOff,  	/**< SLPOUT + NORON + IDMOFF */
		NormalIdleOn,		/**< SLPOUT + NORON + IDMON */
		PartialIdleOff,		/**< SLPOUT + PLTON + IDMOFF */
		PartialIdleOn,		/**< SLPOUT + PLTON + IDMON */
		SleepNormalIdleOff,	/**< SLPIN + NORON + IDMOFF */
		SleepNormalIdleOn,	/**< SLPIN + NORON + IDMON */
		SleepPartialIdleOff,/**< SLPIN + PLTON + IDMOFF */
		SleepPartialIdleOn	/**< SLPIN + PLTON + IDMON */
	};
	
	/*!
	 * @brief GC9D01 panel resolution and gate configuration
	 */
	enum class Resolution_e : uint8_t
	{
		RGB160x160_DualGate,	 /**< 160RGB × 160, S1–S240, Dual gate (default) */
		RGB120x160_DualGate,	 /**< 120RGB × 160, S31–S210, Dual gate */
		RGB80x160_SingleGate,	 /**< 80RGB × 160, S1–S240, Single gate */
		RGB40x160_SingleGate	 /**< 40RGB × 160, S61–S180, Single gate */
	};
	
	/*!
		* @brief User-selectable workaround mode for narrow-window quirks on some GC9D01 displays
		*	i.e. missing pixels when drawing vertical lines or single pixel draws.
		*   behaviour may depend on display module variant and manufacturer
	*/
	enum class PixelFixMode_e : uint8_t {
		Off          = 0,   /**< Normal behavior – may show gaps, missing pixels*/
		DoublePixel  = 1,   /**< Apply double-pixel padding only in drawPixel */
		VFastOff     = 2,   /**< Apply pixel by pixel only in drawFastVLine, no fast draw */
		Both         = 3    /**< Apply both fixes*/
	};

	virtual void setAddrWindow(uint16_t, uint16_t, uint16_t, uint16_t) override;
	virtual void drawPixel(uint16_t x, uint16_t y, uint16_t color) override;
	virtual void drawFastVLine(uint16_t x, uint16_t y, uint16_t h, uint16_t color) override;

	void TFTsetupGPIO_SPI(uint16_t CommDelay, int8_t rst, int8_t dc, int8_t cs, int8_t sclk, int8_t din);
	void TFTsetupGPIO_SPI(uint32_t baudrate, int8_t rst, int8_t dc, int8_t cs);
	void TFTInitScreenSize(uint16_t w = 160, uint16_t h = 160, 
		Resolution_e r = Resolution_e::RGB160x160_DualGate, PixelFixMode_e p = PixelFixMode_e::Both);
	void TFTGC9D01Initialize(void);
	void TFTPowerDown(void);
	uint16_t TFTSwSpiGpioDelayGet(void);
	void TFTSwSpiGpioDelaySet(uint16_t);
	void TFTresetSWDisplay(void);

	void TFTsetRotation(display_rotate_e r);
	void TFTchangeInvertMode(bool m);
	void TFTsetPowerMode(PowerState_e m);
	void TFTenableDisplay(bool m);
	void TFTsetBrightness(uint8_t level);
	void TFTsetScrollArea(uint16_t topFixed, uint16_t scrollArea, uint16_t bottomFixed);
	void TFTsetScrollStart(uint16_t vsp);
	void TFTScrollModeLeave(void);
	void TFTsetPixelFixMode(PixelFixMode_e mode);

private:
	void TFTHWSPIInitialize(void);
	void TFTResetPIN(void);
	void cmdInitSequence(void);
	
	// Display 
	PowerState_e _currentPowerState = PowerState_e::NormalIdleOff; /**< Enum to hold display mode */
	Resolution_e  _currentResolution = Resolution_e::RGB160x160_DualGate ; /**< enum to hold display resolution */
	PixelFixMode_e _currentPixelFixMode = PixelFixMode_e::Both; /**< enum to hold pixel fix mode */
	bool _displayOn = false; /**< Enum to hold display on/off status */
	const uint16_t _sleepDelay = 120; /**< Sleep delay in ms, datasheet 4.2.4. */
	// SPI
	bool _resetPinOn = true; /**< reset pin? true:hw rst pin, false:sw rt*/
	// Screen
	uint8_t _colstart = 0;			/**< Used to offset column in the event of defect at edge of screen */
	uint8_t _rowstart = 0;			/**< Used to offset row in the event of defect at edge of screen */
	uint16_t _widthStartTFT = 160;	/**<  never change after first init */
	uint16_t _heightStartTFT = 160; /**< never change after first init */

	/*!
	 * @brief MADCTL bit flags for register GC9D01_MADCTL (0x36).
	 */
	struct MADCTL_FLAGS_t
	{
		static constexpr uint8_t MY = 0x80;	 /**< Row Address Order: bottom to top */
		static constexpr uint8_t MX = 0x40;	 /**< Column Address Order: right to left */
		static constexpr uint8_t MV = 0x20;	 /**< Row/Column Exchange (Transpose) */
		static constexpr uint8_t ML = 0x10;	 /**< Vertical Refresh Order: bottom to top */
		static constexpr uint8_t RGB = 0x00; /**< RGB color order */
		static constexpr uint8_t BGR = 0x08; /**< BGR color order */
		static constexpr uint8_t MH = 0x04;	 /**< Horizontal Refresh Order: right to left */
	};

	// GC9D01 registers + Commands
	
	static constexpr uint8_t GC9D01_SWRESET = 0x01; /**< Software Reset undocumented ???*/
	static constexpr uint8_t GC9D01_RDDID = 0x04;	/**< Read display identification information */
	static constexpr uint8_t GC9D01_RDDST = 0x09;	/**< Read Display Status */

	static constexpr uint8_t GC9D01_SLPIN = 0x10;	/**< Enter Sleep Mode */
	static constexpr uint8_t GC9D01_SLPOUT = 0x11;	/**< Sleep Out */
	static constexpr uint8_t GC9D01_PTLON = 0x12;	/**< Partial Mode ON */
	static constexpr uint8_t GC9D01_NORON = 0x13;	/**< Normal Display Mode ON */
	static constexpr uint8_t GC9D01_DISPOFF = 0x28; /**< Display OFF */
	static constexpr uint8_t GC9D01_DISPON = 0x29;	/**< Display ON */
	static constexpr uint8_t GC9D01_IDLEOFF = 0x38; /**< Idle mode OFF */
	static constexpr uint8_t GC9D01_IDLEON = 0x39;	/**< Idle mode ON */

	static constexpr uint8_t GC9D01_INVOFF = 0x20;	  /**< Display Inversion OFF */
	static constexpr uint8_t GC9D01_INVON = 0x21;	  /**< Display Inversion ON */
	static constexpr uint8_t GC9D01_CASET = 0x2A;	  /**< Column Address Set */
	static constexpr uint8_t GC9D01_RASET = 0x2B;	  /**< Row/page Address Set */
	static constexpr uint8_t GC9D01_RAMWR = 0x2C;	  /**< Memory Write */
	static constexpr uint8_t GC9D01_PTLAR = 0x30;	  /**< Partial Area */
	static constexpr uint8_t GC9D01_VSCRDEF = 0x33;   /**< Vertical Scrolling Definition */
	static constexpr uint8_t GC9D01_TEOFF = 0x34;	  /**< Tearing Effect Line OFF */
	static constexpr uint8_t GC9D01_TEON = 0x35;	  /**< Tearing Effect Line ON */
	static constexpr uint8_t GC9D01_MADCTL = 0x36;	  /**< Memory Access Control */
	static constexpr uint8_t GC9D01_VSCRSADD = 0x37;  /**< Vertical Scrolling Start Address */
	static constexpr uint8_t GC9D01_COLMOD = 0x3A;	  /**< Pixel Format Set */
	static constexpr uint8_t GC9D01_CONTINUE = 0x3C;  /**< Write Memory Continue */
	static constexpr uint8_t GC9D01_TEARSET = 0x44;	  /**< Set Tear Scanline */
	static constexpr uint8_t GC9D01_GETLINE = 0x45;	  /**< Get Scanline */
	static constexpr uint8_t GC9D01_SETBRIGHT = 0x51; /**< Write Display Brightness */
	static constexpr uint8_t GC9D01_SETCTRL = 0x53;	  /**< Write CTRL Display */

	static constexpr uint8_t GC9D01_RGB_CTRL = 0xB0;	  /**< RGB Interface Signal Control*/
	static constexpr uint8_t GC9D01_TEWC = 0xB4;			 /**< Tearing effect width control */
	static constexpr uint8_t GC9D01_BLANK_PORCH_CTRL = 0xB5; /**< Blanking Porch Control */
	static constexpr uint8_t GC9D01_FUNCTION_CTRL = 0xB6;	 /**< Display Function Control */
	static constexpr uint8_t GC9D01_DUAL_SINGLE = 0xBF;	 /**< Dual-Single gate select */

	static constexpr uint8_t GC9D01_POWER_CTRL  = 0xC1; /**< Power Criterion Control */
	static constexpr uint8_t GC9D01_VREG1A_CTRL = 0xC3; /**< Vreg1a voltage Control  */
	static constexpr uint8_t GC9D01_VREG1B_CTRL = 0xC4; /**< Vreg1b voltage Control  */
	static constexpr uint8_t GC9D01_VREG2A_CTRL = 0xC9; /**< Vreg2a voltage Control  */

	static constexpr uint8_t GC9D01_RDID1 = 0xDA; /**< Read ID 1 */
	static constexpr uint8_t GC9D01_RDID2 = 0xDB; /**< Read ID 2 */
	static constexpr uint8_t GC9D01_RDID3 = 0xDC; /**< Read ID 3 */
	
	static constexpr uint8_t GC9D01_GAMMA1 = 0xF0;	 /**< Set gamma 1 */
	static constexpr uint8_t GC9D01_GAMMA2 = 0xF1;	 /**< Set gamma 2 */
	static constexpr uint8_t GC9D01_GAMMA3 = 0xF2;	 /**< Set gamma 3 */
	static constexpr uint8_t GC9D01_GAMMA4 = 0xF3;	 /**< Set gamma 4 */

	static constexpr uint8_t GC9D01_SPI2DATA    = 0xB1;  /**< SPI 2DATA control */
	static constexpr uint8_t GC9D01_INVERSION   = 0xEC; /**< Inversion */
	static constexpr uint8_t GC9D01_IFACE  = 0xF6;	 /**< Interface control */
	static constexpr uint8_t GC9D01_INREGEN1 = 0xFE; /**< Inter register enable 1 */
	static constexpr uint8_t GC9D01_INREGEN2    = 0xEF; /**< Inter register enable 2 */
}; // end of class

// ********************** EOF *********************
