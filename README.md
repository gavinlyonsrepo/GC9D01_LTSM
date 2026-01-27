# GC9D01 Readme 

## Note 

Library is in Beta. 
Not all functions in default mode currently work fully.
All functions in advanced buffer mode work.
See 'Notes and Issues' section for list of problems. 

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)

## Table of contents

* [Overview](#overview)
* [Installation](#installation)
* [Dependency](#dependency)
* [Documentation](#documentation)
* [Software](#software)
	* [Examples](#examples)
	* [SPI](#spi)
* [Hardware](#hardware)
* [Tested](#tested)
* [Output](#output)
* [Notes and Issues](#notes-and-issues)
	* [Function Status](#function-status)


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

There are 2 example files included currently. This library is ported from [URL github link](https://github.com/gavinlyonsrepo/GC9A01_LTSM)
There are more examples there that can be ported easily. I will include more examples in future. 

| Filename .ino | Function  | Note |
| --- | --- | --- |
| HELLO WORLD | Hello world  | basic use case |
| TEXT | testing fonts and text | --- |
| FUNCTIONS | Function testing Scroll, invert, Power modes etc.| --- |
| GRAPHICS | testing graphics: shapes + lines | dislib16 ADVANCED GRAPHICS ENABLE must be enabled for all tests to work |
| BITMAP| 1,8 & 16 bit bitmaps tests + bitmap FPS tests| Bitmap test data is stored in arrays |
| DEMOS| A demo showing a gauge |dislib16 ADVANCED SCREEN BUFFER ENABLE + dislib16 ADVANCED GRAPHICS ENABLE must be enabled |
| FRAME BUFFER | Testing frame Buffer mode | dislib16 ADVANCED SCREEN BUFFER ENABLE must be enabled user option 2 |

### SPI

In the example ino files. There are sections in "setup()" function
where user can make adjustments.

1. USER OPTION 1 GPIO, SPI_SPEED + TYPE
2. USER OPTION 2 SCREEN SECTION


*USER OPTION 1 GPIO SPI SPEED*

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

*USER OPTION 2 Screen size*

User can adjust screen pixel height, screen pixel width and resolution(4 enum values available).


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

## Output

Output of DEMOS.ino(Advance buffer mode):

[![pic ](https://github.com/gavinlyonsrepo/GC9D01_LTSM/blob/main/extras/images/output.jpg)](https://github.com/gavinlyonsrepo/GC9D01_LTSM/blob/main/extras/images/output.jpg)

## Notes and Issues

### Function Status

In Advanced buffer mode all functions are working with Display16_graphics_LTSM.
See 'dependency' section readme for link to details of this mode. 

In default mode functions not currently working are listed below:

| Function            | Status     | Notes |
|---------------------|------------|-------|
| Draw text: row buffer mode |  Working  | the default and best text mode |
| Draw text: pixel by pixel mode |Not working | Distorted text, missing pixels |
| Bitmaps      |  Working  |  |
| Sprites      | Not working |  |
| Shapes & lines | Not fully working | Varying degrees of distortion, missing pixels | 
| Rotation | Not fully working in all orientations | Distortions appearing |
