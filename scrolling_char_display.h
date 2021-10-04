/*
 * scrolling_char_display.h
 *
 * Author: Peter Sutton
 */

#ifndef SCROLLING_CHAR_DISPLAY_H_
#define SCROLLING_CHAR_DISPLAY_H_

#include <stdint.h>
#include "pixel_colour.h"

/* Sets the text to be displayed and the colour it will be
 * scrolled with. The message will start displaying immediately
 * so will overwrite/interfere with any currently scrolling
 * message. To avoid this, wait until the scroll_display()
 * function below has returned 0 to indicate the message scrolling
 * is complete. Note that this string is not 
 * copied, so it is important that this string not change
 * after this function is called while the string is still
 * being displayed.
 */
void set_scrolling_display_text(char* string, PixelColour colour);

/* Scroll the display. Should be called whenever the display
 * is to be scrolled one pixel to the left. It is recommended that
 * this function NOT be called from an interrupt service routine as
 * it will wait for SPI communication to be finished before returning. 
 * This could take over 1ms.
 * Returns 1 while a message is still scrolling, 0 when done.
 */
uint8_t scroll_display(void);
	
#endif /* SCROLLING_CHAR_DISPLAY_H_ */