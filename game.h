/*
** game.h
**
** Written by Peter Sutton
**
** Function prototypes for those functions available externally.
*/

#ifndef GAME_H_
#define GAME_H_

#include <inttypes.h>

// The game field is 16 rows in size by 8 columns, i.e. x (column number)
// ranges from 0 to 7 (left to right) and y (row number) ranges from
// 0 to 15 (bottom to top).
#define FIELD_HEIGHT 16
#define FIELD_WIDTH 8

// Limits on the number of asteroids and projectiles we can have on the 
// game field at any one time. (These numbers should fit within the 
// range of an int8_t type - i.e. max 127, though in reality
// there are tighter constraints than this - e.g. there are only 128
// positions on the game field.)
#define MAX_PROJECTILES 4
#define MAX_ASTEROIDS 20

// Arguments that can be passed to move_base() below
#define MOVE_LEFT 0
#define MOVE_RIGHT 1

// Initialise the game and output the initial display
void initialise_game(void); 

// Attempt to move the base station to the left or the right. Returns
// 1 if successful, 0 otherwise (e.g. already at edge). The "direction"
// argument takes on the value MOVE_LEFT or MOVE_RIGHT (see above).
int8_t move_base(int8_t direction);

// Fire a projectile - release a projectile from the base station.
// Returns 1 if successful, 0 otherwise (e.g. already a projectile
// which is in the position immediately above the base station, or
// the maximum number of projectiles in flight has been reached.
int8_t fire_projectile(void);

// Advance the projectiles that have been fired. Any projectiles that
// go off the top or that hit an asteroid are removed.
void advance_falling_astroid(void);
void advance_projectiles(void);
void baseDisplayRight(void);
void baseDisplayLeft(void);
// Returns 1 if the game is over, 0 otherwise
int8_t is_game_over(void);
void animation(void);

#endif
