[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)

# GC9D01 Readme 

## Note 

## Table of contents

* [Overview](#overview)
* [Installation](#installation)
* [Dependency](#dependency)
* [Documentation](#documentation)
* [Software](#software)
	* [Examples](#examples)
	* [Setup](#setup)
* [Hardware](#hardware)
* [Tested](#tested)
* [Output](#output)
* [Notes and Issues](#notes-and-issues)


## Overview

* Name: GC9D01_LTSM
* Author: Gavin Lyons
* Description:

C++ Arduino library for a TFT SPI LCD, GC9D01N Driver.

GC9D01N is a 262,144-color single-chip SOC driver for a-TFT liquid crystal display with
resolution of 160RGBx160 dots, comprising a 240-channel source driver, a 32-channel gate
driver, 57,600 bytes GRAM for graphic display data of 160RGBx160 dots, and power supply
circuit.

1. Power modes, Invert, Scroll, Rotate supported.
2. Hardware & software SPI
3. Tested on 160x160 Round Display
4. Arduino eco-system library.
5. 16 ASCII fonts included, fonts can easily be removed or added.
6. Advanced graphics class included.
7. Advanced frame buffer mode included.
8. Bitmaps supported: 1, 8 and 16 bit.
9. Sprites supported.
10. [Project url link](https://github.com/gavinlyonsrepo/GC9D01_LTSM)

* [DataSheet](https://buydisplay.com/download/ic/GC9D01N.pdf)

## Installation

The library will be included in the official Arduino library manger in future.
For now user will have to install the library into their libraries folder manually. 

## Dependency

This library requires the Arduino library 'display16_LTSM' as a dependency. display16_LTSM library contains
the graphics, bitmaps, and font methods as well as font data and bitmap test data. Its also 
where the user sets options(debug, advanced graphics and advanced buffer mode).
The 'display16_LTSM' project and readme is at [URL github link.](https://github.com/gavinlyonsrepo/display16_LTSM)
'display16_LTSM' is also written by author of this library. 

## Documentation

Code is commented for the 'doxygen' API generation tool.
Documents on fonts, bitmaps and graphics can be found at 
the dependency 'display16_LTSM' repository, [URL github link](https://github.com/gavinlyonsrepo/display16_LTSM)

## Software

### Examples

There are 2 example files included currently. 

| Filename .ino | Function | Advanced Graphics mode| Advanced buffer mode |
| --- | --- | --- | --- |
| HELLO WORLD | Hello world  | NO | NO |
| TEXT | Fonts and text | NO | NO|
| FUNCTIONS | Function testing Scroll, invert, Power modes etc.| NO | NO |
| DEMO_ONE| A demo showing a gauge | NO | NO |
| BITMAP| 1,8 & 16 bit bitmaps tests + bitmap FPS tests| NO | NO |
| GRAPHICS | Graphics: shapes + lines | YES | NO |
| DEMO_TWO| A demo showing a gauge | YES | YES |
| FRAME BUFFER | Advanced Buffer mode | YES | YES |

### Setup

In the example ino files. There are sections in "setup()" function
where user can make adjustments.

1. USER OPTION 1 GPIO, SPI Speed + type
2. USER OPTION 2 Screen Size, Resolution, Gate type and Missing Pixel Fix. 


*GPIO SPI SPEED*

Two different constructors which one is called depends on 'bhardwareSPI', 
true for hardware spi, false for software SPI.

Hardware SPI:

Here the user can pass the SPI Bus freq in Hertz, Currently set to 8 Mhz,
and the Reset, chip select and data or command line. Any GPIO can be used for these.
The MOSI and CLk are tied to default MCU SPI interface GPIO.

Software SPI:

The optional GPIO software uS delay,which by default is zero.
Setting this higher can be used to slow down Software SPI
which may be beneficial on Fast MCU's.
The 5 GPIO pins used. Any GPIO can be used for these.

*USER OPTIONS 2*

There are 4 options here user can adjust:
 
1. 2A screen pixel height 
2. 2B screen pixel width 
3. 2C Resolution and gate type. 
4. 2D Pixel fix mode


*USER OPTION 2C- Resolution & Gate Configuration (Resolution_e)*

The GC9D01 controller supports multiple display sizes and gate driving modes.  
Select the correct enum value for your physical module.
Only tested on 160x160 round. Value of Inversion register(0xEC) in 
init command sequence may have to be changed.

| Enum Value             | Resolution | Gate Mode | Typical Module Description | Notes     |
|------------------------|------------|-----------|---------------------------|-------------------|
| `RGB160x160_DualGate`  | 160×160  | Dual   |  Most round GC9D01 displays            | **Default** – no need to set |
| `RGB120x160_DualGate`  | 120×160  | Dual   | Some rectangular / bar-type modules         | Dual-gate scanning |
| `RGB80x160_SingleGate` | 80×160   | Single |  Smaller rectangular / bar displays          | Single-gate scanning |
| `RGB40x160_SingleGate` | 40×160   | Single | Very narrow bar-type or specialty modules   | Single-gate scanning  |


*USER OPTION 2D – Pixel drawing workaround (PixelFixMode_e)*

Some GC9D01 modules may have a hardware quirk:  
very narrow address windows (1-pixel wide or 1-pixel high) 
can cause missing pixels, gaps in vertical lines.
When Setting up Library, It was discovered that when drawing pixel by pixel in vertical
direction, some pixels went missing or changed color. The solution was to send the 
pixel twice for drawPixel() and for drawFastVLine() to disable "fast burst mode".

Advanced buffer mode: No problems, this setting is ignored.

Default mode: 
No problem with bitmaps or drawing text in local buffer mode(default).
Problems occur with certain functions: like drawing text when in pixel mode,
certain shapes + lines and sprites. This fix slows these functions down.

Behavior may depend on display module variant and manufacturer so 
I made it optional to switch 'fix' off. 
In the examples files it is set to default 'both'.
See table for options

| Mode    | Description   |Notes    |
|--------------|---------|---------------|
| Off    | Normal drawing (no workaround)  | Fastest, but may show gaps   |
| DoublePixel | Double-pixel padding in `drawPixel()` | Fixes single pixels    |
| VFastOff|Pixel-by-pixel only in `drawFastVLine()`| Forces slow vertical lines (no fast burst)|
| Both    | Applies both fixes  | Default     |


## Hardware

[![pic ](https://github.com/gavinlyonsrepo/GC9D01_LTSM/blob/main/extras/images/gc2.jpg)](https://github.com/gavinlyonsrepo/GC9D01_LTSM/blob/main/extras/images/gc2.jpg)

Connections as setup in HELLO_WORLD.ino  test file.

| TFT PinNum | Pindesc | Hardware SPI | Software SPI |
| --- | --- | --- | --- |
| 1 | GND | GND | GND |
| 2 | VCC | VCC 3.3 | VCC 3.3|
| 3 | SCLK | MCU SPI CLK | GPIO12 |
| 4 | SDA | MCU MOSI | GPIO13 |
| 5 | RESET | GPIO4 | GPIO4 |
| 6 | DC | GPIO5 | GPIO5 |
| 7 | CS | GPIO15 | GPIO15 |
| 8 | LED | VCC 3.3 | VCC 3.3|


1. This is a 3.3V logic device do NOT connect the I/O logic lines to 5V logic device.
2. SW SPI pick any GPIO you like , HW SPI SCLK and SDA will be tied to MCU SPI interface.
3. Backlight on/off control is left to user.
4. NOTE: Connect LED backlight pin 1 thru a resistor to VCC.

## Tested

* Tested on ESP32 

> Some examples on low-RAM MCUs will fail( insufficient memory ), if numerous fonts and excessive bitmap data are included.  
> Advanced screen buffer mode requires sufficient dynamic memory for the buffer — see the README in display16_LTSM for details.
> The Size of Frame buffer needed is: height by width by 2. So for 160x160 screen = 51,200 bytes or 40x160 = 12,800 bytes.

## Output

Output of DEMO_X.ino :

[![pic ](https://github.com/gavinlyonsrepo/GC9D01_LTSM/blob/main/extras/images/output.jpg)](https://github.com/gavinlyonsrepo/GC9D01_LTSM/blob/main/extras/images/output.jpg)

## Notes and Issues

