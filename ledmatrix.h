/*
 * ledmatrix.h
 *
 * Author: Peter Sutton
 */ 


#ifndef LEDMATRIX_H_
#define LEDMATRIX_H_

#include <stdint.h>
#include "pixel_colour.h"

// The matrix has 16 columns (x ranges from 0 to 15, left to right) and 
// 8 rows (y ranges from 0 to 7, bottom to top) - as per the X,Y
// coordinates marked on the board.
#define MATRIX_NUM_COLUMNS 16
#define MATRIX_NUM_ROWS 8

// Data types which can be used to store display information
typedef PixelColour MatrixData[MATRIX_NUM_COLUMNS][MATRIX_NUM_ROWS];
typedef PixelColour MatrixRow[MATRIX_NUM_COLUMNS];
typedef PixelColour MatrixColumn[MATRIX_NUM_ROWS];

// Setup SPI communication with the LED matrix.
// This function must be called before the LED matrix functions
// below are used.
void ledmatrix_setup(void);

// Functions to update the display
// For those functions which take an x or a y value, the value must be valid
// or the request will be ignored. (i.e. x must be < MATRIX_NUM_COLUMNS
// and y must be < MATRIX_NUM_ROWS)
void ledmatrix_update_all(MatrixData data);
void ledmatrix_update_pixel(uint8_t x, uint8_t y, PixelColour pixel);
void ledmatrix_update_row(uint8_t y, MatrixRow row);
void ledmatrix_update_column(uint8_t x, MatrixColumn col);
void ledmatrix_shift_display_left(void);
void ledmatrix_shift_display_right(void);
void ledmatrix_shift_display_up(void);
void ledmatrix_shift_display_down(void);
void ledmatrix_clear(void);

// Functions to operate on MatrixRow and MatrixColumn data structures
void copy_matrix_column(MatrixColumn from, MatrixColumn to);
void copy_matrix_row(MatrixRow from, MatrixRow to);
void set_matrix_column_to_colour(MatrixColumn matrix_column, PixelColour colour);
void set_matrix_row_to_colour(MatrixRow matrix_row, PixelColour colour);

#endif /* LEDMATRIX_H_ */
