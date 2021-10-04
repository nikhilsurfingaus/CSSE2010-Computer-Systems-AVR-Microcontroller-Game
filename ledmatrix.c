/*
 * ledmatrix.c
 *
 * Author: Peter Sutton
 * 
 * See the LED matrix Reference for details of the SPI commands used.
 */ 

#include <avr/io.h>
#include "ledmatrix.h"
#include "spi.h"

#define CMD_UPDATE_ALL 0x00
#define CMD_UPDATE_PIXEL 0x01
#define CMD_UPDATE_ROW 0x02
#define CMD_UPDATE_COL 0x03
#define CMD_SHIFT_DISPLAY 0x04
#define CMD_CLEAR_SCREEN 0x0F

void ledmatrix_setup(void) {
	// Setup SPI - we divide the clock by 128.
	// (This speed guarantees the SPI buffer will never overflow on
	// the LED matrix.)
	spi_setup_master(128);
}

void ledmatrix_update_all(MatrixData data) {
	(void)spi_send_byte(CMD_UPDATE_ALL);
	for(uint8_t y=0; y<MATRIX_NUM_ROWS; y++) {
		for(uint8_t x=0; x<MATRIX_NUM_COLUMNS; x++) {
			(void)spi_send_byte(data[x][y]);
		}
	}
}

void ledmatrix_update_pixel(uint8_t x, uint8_t y, PixelColour pixel) {
	if(x >= MATRIX_NUM_COLUMNS || y >= MATRIX_NUM_ROWS) {
		// Position isn't valid - we ignore the request.
		return;
	}
	(void)spi_send_byte(CMD_UPDATE_PIXEL);
	(void)spi_send_byte( ((y & 0x07)<<4) | (x & 0x0F));
	(void)spi_send_byte(pixel);
}

void ledmatrix_update_row(uint8_t y, MatrixRow row) {
	if(y >= MATRIX_NUM_ROWS) {
		// y value is too large - we ignore the request
		return;
	}
	(void)spi_send_byte(CMD_UPDATE_ROW);
	(void)spi_send_byte(y & 0x07);	// row number
	for(uint8_t x = 0; x<MATRIX_NUM_COLUMNS; x++) {
		(void)spi_send_byte(row[x]);
	}
}

void ledmatrix_update_column(uint8_t x, MatrixColumn col) {
	if(x >= MATRIX_NUM_COLUMNS) {
		// x value is too large - we ignore the request
		return;
	}
	(void)spi_send_byte(CMD_UPDATE_COL);
	(void)spi_send_byte(x & 0x0F); // column number
	for(uint8_t y = 0; y<MATRIX_NUM_ROWS; y++) {
		(void)spi_send_byte(col[y]);
	}
}

void ledmatrix_shift_display_left(void) {
	(void)spi_send_byte(CMD_SHIFT_DISPLAY);
	(void)spi_send_byte(0x02);
}

void ledmatrix_shift_display_right(void) {
	(void)spi_send_byte(CMD_SHIFT_DISPLAY);
	(void)spi_send_byte(0x01);
}

void ledmatrix_shift_display_up(void) {
	(void)spi_send_byte(CMD_SHIFT_DISPLAY);
	(void)spi_send_byte(0x08);
}

void ledmatrix_shift_display_down(void) {
	(void)spi_send_byte(CMD_SHIFT_DISPLAY);
	(void)spi_send_byte(0x04);
}

void ledmatrix_clear(void) {
	(void)spi_send_byte(CMD_CLEAR_SCREEN);
}

void copy_matrix_column(MatrixColumn from, MatrixColumn to) {
	for(uint8_t row = 0; row <MATRIX_NUM_ROWS; row++) {
		to[row] = from[row];
	}
}

void copy_matrix_row(MatrixRow from, MatrixRow to) {
	for(uint8_t col = 0; col < MATRIX_NUM_COLUMNS; col++) {
		to[col] = from[col];
	}
}

void set_matrix_column_to_colour(MatrixColumn matrix_column, PixelColour colour) {
	for(uint8_t row = 0; row < MATRIX_NUM_ROWS; row++) {
		matrix_column[row] = colour;
	}
}

void set_matrix_row_to_colour(MatrixRow matrix_row, PixelColour colour) {
	for(uint8_t column = 0; column < MATRIX_NUM_COLUMNS; column++) {
		matrix_row[column] = colour;
	}
}
