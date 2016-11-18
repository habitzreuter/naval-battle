#ifndef __SHIPS_H__
#define __SHIPS_H__

#include "game.h"

char get_ship_type(enum ships);
void stringify_ship_type(enum ships, char*);
void set_ships(player_st*, size_t, bool);

#endif
