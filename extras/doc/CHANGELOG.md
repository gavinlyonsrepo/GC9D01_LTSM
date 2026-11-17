# CHANGELOG GC9D01

* Version 1.0.1 02-2026
  * first version
* Version 1.0.2 02-2026
  * fix for 90 and 270 degree rotation for 40x160 single gate displays see github issue 1
* Version 1.1.0 05-2026
  * Added Support for 50x160 single gate display
  * Added Support for 60x160 single gate display
  * Added two new parameters to TFTInitScreenSize() :portrait Column start offset
  & portrait Row start offset required for new displays added.
* Version 1.1.1 11-2026
	* Minor update
	* Updated FRAME_BUFFER.ino example for display16_LTSM graphics library version 1.1.0.
	* NOTE: When IN frame buffer mode, on a display whose width and height differ,
	  the buffer is laid out for the current rotation:
		1. Set the rotation before calling setBuffer() in setup(), IF non-default.
		2. If the program switches between landscape and portrait during
		   operation (a 90 or 270 degree change), call setBuffer() again after
		   the rotation change, then redraw.
		3. See the FRAME_BUFFER.ino example, and the buffer mode README in the
		   display16_LTSM dependency library, for details.
		4. This change only effects users using frame buffer mode AND non-default rotation
		AND on displays where width != height.
* Version 1.1.2 11-2026
	* minor update, correction to V1.1.1 in drawPixel() function for frame buffer mode. 
