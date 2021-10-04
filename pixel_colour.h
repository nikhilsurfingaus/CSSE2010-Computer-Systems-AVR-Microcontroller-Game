/*
 * pixel_colour.h
 *
 * Author: Peter Sutton
 */ 


#ifndef PIXEL_COLOUR_H_
#define PIXEL_COLOUR_H_

// Each PixelType is an 8 bit number - 4 bits of green in the high bits,
// 4 bits of red in the low bits
typedef uint8_t PixelColour;

// Some useful colour definitions
#define COLOUR_BLACK		0x00
#define COLOUR_RED			0x0F
#define COLOUR_GREEN		0xF0
#define COLOUR_YELLOW		0xDF
#define COLOUR_ORANGE		0x3C
#define COLOUR_LIGHT_ORANGE 0x13
#define COLOUR_LIGHT_YELLOW 0x35
#define COLOUR_LIGHT_GREEN	0x11


#endif /* PIXEL_COLOUR_H_ */