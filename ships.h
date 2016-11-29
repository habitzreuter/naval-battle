#ifndef __SHIPS_H__
#define __SHIPS_H__

#include <ncurses.h>
#include "game.h"

char get_ship_type(enum ships);
void stringify_ship_type(enum ships, char*);
void set_ships(WINDOW*, WINDOW*, player_st*, size_t);
void update_board(uint8_t*, ship_st, uint8_t);
bool valid_ship_bounds(size_t, ship_st);
bool valid_position(size_t, uint8_t*, ship_st);

#endif
