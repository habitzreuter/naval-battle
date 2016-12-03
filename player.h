#ifndef __PLAYER_H__
#define __PLAYER_H__

void print_2d_char_array(WINDOW*, size_t, char*);
void print_player_board(WINDOW*, size_t, player_st, player_st);
void scan_ship_position(WINDOW*, WINDOW*, player_st*, player_st, size_t, uint8_t);
void scan_shot_position(WINDOW*, WINDOW*, player_st*, player_st*, uint8_t, position_st*);

#endif
