/*
 * scrolling_char_display.c
 *
 * Author: Peter Sutton
 *
 * This is an example of how the LED display board can be used. 
 * This program scrolls a message from right to left on the
 * board. The font used is defined below and is 7 dots high and
 * varies between 3 and 5 dots wide, depending on the character.
 * Letters and numbers can be handled (though lower case
 * letters are displayed as upper case). All other characters
 * display as a blank column.
 * 
 * The program also demonstrates how data can be stored in the
 * program (flash) memory, without also taking up space in RAM.
 * If the arrays below were defined in the normal C way, they
 * would take up space in both the program memory (where the
 * constants would be stored) and the RAM (where the values 
 * would be copied on start-up). The use of the PROGMEM attribute
 * and functions/macros like pgm_read_byte() means that the
 * constants can live just in the program memory and not be 
 * copied to RAM. (This saves several hundred bytes of RAM.)
 *
 */

#include "scrolling_char_display.h"
#include "ledmatrix.h"
#include <avr/pgmspace.h>

/* FONT DEFINITION
 *
 * The following define the columns of data to be displayed
 * for each character (a-z and 0-9). The most significant
 * 7 bits (bit 7 to bit 1) represent the data for rows 7 to 1 
 * (top to bottom). The least significant bit is 1 only for
 * the last column of letter data. (This is how the software
 * will know when it has reached the last column for this 
 * character. We do not display data for this bit, i.e. 
 * row y=0 on the display will always be blank.
 * As an example, the data for the 4 columns of letter A is as 
 * follows:
 * bit 7  ** 
 * bit 6 *  *
 * bit 5 *  *
 * bit 4 ****
 * bit 3 *  *
 * bit 2 *  *
 * bit 1 *  *
 * bit 0    *
 */

/* Data for letters A-Z */
static const uint8_t cols_A[] PROGMEM = {126, 144, 144, 127};
static const uint8_t cols_B[] PROGMEM = {254, 146, 146, 109};
static const uint8_t cols_C[] PROGMEM = {124, 130, 130, 69};
static const uint8_t cols_D[] PROGMEM = {254, 130, 130, 125};
static const uint8_t cols_E[] PROGMEM = {254, 146, 146, 131};
static const uint8_t cols_F[] PROGMEM = {254, 144, 144, 129};
static const uint8_t cols_G[] PROGMEM = {124, 130, 146, 93};
static const uint8_t cols_H[] PROGMEM = {254, 16, 16, 255};
static const uint8_t cols_I[] PROGMEM = {130, 254, 131};
static const uint8_t cols_J[] PROGMEM = {4, 2, 2, 253};
static const uint8_t cols_K[] PROGMEM = {254, 16, 40, 199};
static const uint8_t cols_L[] PROGMEM = {254, 2, 2, 3};
static const uint8_t cols_M[] PROGMEM = {254, 64, 48, 64, 255};
static const uint8_t cols_N[] PROGMEM = {254, 32, 16, 255};
static const uint8_t cols_O[] PROGMEM = {124, 130, 130, 125};
static const uint8_t cols_P[] PROGMEM = {254, 144, 144, 97};
static const uint8_t cols_Q[] PROGMEM = {124, 130, 138, 124, 3};
static const uint8_t cols_R[] PROGMEM = {254, 144, 152, 103};
static const uint8_t cols_S[] PROGMEM = {100, 146, 146, 77};
static const uint8_t cols_T[] PROGMEM = {128, 128, 254, 128, 129};
static const uint8_t cols_U[] PROGMEM = {252, 2, 2, 253};
static const uint8_t cols_V[] PROGMEM = {248, 4, 2, 4, 249};
static const uint8_t cols_W[] PROGMEM = {252, 2, 28, 2, 253};
static const uint8_t cols_X[] PROGMEM = {198, 40, 16, 40, 199};
static const uint8_t cols_Y[] PROGMEM = {224, 16, 14, 16, 225};
static const uint8_t cols_Z[] PROGMEM = {134, 138, 146, 162, 195};

/* Data for numbers 0 to 9 */
static const uint8_t cols_0[] PROGMEM = {124, 146, 162, 125};
static const uint8_t cols_1[] PROGMEM = {66, 254, 3};
static const uint8_t cols_2[] PROGMEM = {70, 138, 146, 99};
static const uint8_t cols_3[] PROGMEM = {68, 146, 146, 109};
static const uint8_t cols_4[] PROGMEM = {24, 40, 72, 255};
static const uint8_t cols_5[] PROGMEM = {228, 162, 162, 157};
static const uint8_t cols_6[] PROGMEM = {124, 146, 146, 77};
static const uint8_t cols_7[] PROGMEM = {128, 158, 160, 193};
static const uint8_t cols_8[] PROGMEM = {108, 146, 146, 109};
static const uint8_t cols_9[] PROGMEM = {100, 146, 146, 125};

/* The following two arrays point to the font data above. 
 * We store pointers to the beginning of the column data
 * for each letter 
*/
static const uint8_t* const letters[26] PROGMEM = {
		cols_A, cols_B, cols_C, cols_D, cols_E, cols_F,
		cols_G, cols_H, cols_I, cols_J, cols_K, cols_L,
		cols_M, cols_N, cols_O, cols_P, cols_Q, cols_R, 
		cols_S, cols_T, cols_U, cols_V, cols_W, cols_X,
		cols_Y, cols_Z };
		
static const uint8_t* const numbers[10] PROGMEM = {
		cols_0, cols_1, cols_2, cols_3, cols_4, 
		cols_5, cols_6, cols_7, cols_8, cols_9 };

/* Keep track of the pixel colour to be used */
static PixelColour colour = COLOUR_RED;

/* Keep track of which column of data is next to be displayed. 
 * next_col_ptr points to that column, or is 0 if there is
 * no next column.
 */
static volatile const uint8_t* next_col_ptr = 0;

/* String to be displayed. 
 * next_char_to_display will be used to point to the next
 * character from this string to be displayed.
 */
static char* display_string;

static volatile char* next_char_to_display = 0;

/*
 * Set the message to be displayed - we just copy the 
 * pointer not the string it points to, so it is important
 * that the original string not change after this function
 * is called while the string is still being displayed.
 * We reset the pointers to ensure the next column to be displayed
 * comes from the first character of this string.
 */
void set_scrolling_display_text(char* string_to_display, PixelColour c) {
	colour = c;
	display_string = string_to_display;
	next_col_ptr = 0;
	next_char_to_display = 0;
}

/*
 * Scroll the display. Should be called whenever the display
 * is to be scrolled. 
 * Returns 1 if still scrolling display.
 */
uint8_t scroll_display(void) {
	static uint8_t shift_countdown = 0;
	uint8_t i;
	uint8_t col_data;
	char next_char;
	uint8_t finished = 0;

	/* Data to be displayed in the next column - by 
	 * default we show a blank column. Bit 7 of this
	 * column data corresponds to row 7 of the display
	 * etc.
	 */
	col_data = 0;

	if(next_col_ptr) {
		/* We're currently outputting a character and next_col_ptr
		 * points to the display data for the next column. We
		 * extract that data from program memory.
		 */
		col_data = pgm_read_byte(next_col_ptr);

		if(col_data & 1) {
			/* Least significant bit is set - this is the last
			 * column of this character
			 */
			next_col_ptr = 0;
		} else {
			/* This is not the last column of this character - make
			 * the pointer point to the data for the next column.
			 */
			next_col_ptr++;
		}
	} else if(next_char_to_display) {
		/* We're not currently outputting a character, but we
		 * do have more characters to display. We will output
		 * a blank column this time (col_data value remains 0)
		 * but we will set up our pointer (next_col_ptr) so that
		 * it points to the data for the first column of dots for
		 * the next character. We first get the next character to be 
		 * displayed and advance our next character pointer 
		 * (next_char_to_display) so that it points to the character 
		 * after.
		 */
		next_char = *(next_char_to_display++);
		if(next_char == 0) {
			/* We reached the null character at the end of the string.
			 * There is no next character, reset our pointer to 
			 * the next character and set our countdown until the 
			 * message disappears from the display.
			 */
			next_char_to_display = 0;
			shift_countdown = 16;
		} else if (next_char >= 'a' && next_char <= 'z') {
			/* Character is a lower case letter - the next column to 
			 * be displayed will be the first column of the letter
			 * data for that letter
			 */
			next_col_ptr = (const uint8_t*)pgm_read_word(&letters[next_char - 'a']);
		} else if (next_char >= 'A' && next_char <= 'Z') {
			/* Upper case character */
			next_col_ptr = (const uint8_t*)pgm_read_word(&letters[next_char - 'A']);
		} else if (next_char >= '0' && next_char <= '9') {
			/* Digit */
			next_col_ptr = (const uint8_t*)pgm_read_word(&numbers[next_char - '0']);
		}
	} else {
		/* We're not outputting a column of dots and there is 
		 * no next character. Move on to the string that we
		 * have stored (if any).
		 */
		if(!display_string) {
			/* May be finished - flag this and adjust below if we're still
			 * showing pixels
			 */
			finished = 1;
		}
		next_char_to_display = display_string;
		display_string = 0;
	}
	
	/* Shift the current display one pixel to the left and insert the 
	 * new column data at column 15.
	 * Adjust our "finished" variable if we've finished scrolling the
	 * message off the display
	 */
	ledmatrix_shift_display_left();
	MatrixColumn column_colour_data;
	for(i=7; i>=1; i--) {
		// If the relevant font bit is set, we make this a red pixel, otherwise blank
		if(col_data & 0x80) {
			column_colour_data[i] = colour;
		} else {
			column_colour_data[i] = 0;
		}
		col_data <<= 1;
	}
	column_colour_data[0] = 0;
	ledmatrix_update_column(15, column_colour_data);
	if(shift_countdown > 0) {
		shift_countdown--;
	}
	finished = finished && (shift_countdown == 0);
	return !finished;
}
