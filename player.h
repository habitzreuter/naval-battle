/**
 * Batalha Naval
 *
 * Copyright (C) 2017 Marco Antônio Habitzreuter (marco.habitzreuter@ufrgs.br)
 *
 * Para detalhes sobre a licença, ver "main.c" e "LICENSE"
 *
 */

#ifndef __PLAYER_H__
#define __PLAYER_H__

void print_player_board(WINDOW*, size_t, player_st, player_st);
void scan_ship_position(WINDOW*, WINDOW*, player_st*, player_st, size_t, uint8_t);
void scan_shot_position(WINDOW*, WINDOW*, player_st*, player_st*, uint8_t, position_st*);

#endif
