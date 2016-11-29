/**
 * Batalha Naval
 *
 * Autor: Marco Antônio Habitzreuter	(marco.habitzreuter@ufrgs.br)
 *					(mahabitzreuter@inf.ufrgs.br)
 *
 * Funções relacionadas à entrada de dados de um jogador humano
 *
 */

#include <stdio_ext.h>
#include <stdint.h>
#include "game.h"
#include "ships.h"

/**
 * Lê coordenadas e direção para posicionar navio
 */
void scan_ship_position(WINDOW *board, WINDOW *info, player_st *player, size_t board_size, uint8_t ship_index)
{
	player_st tmp_player; // Tabuleiro temporário, apenas para exibição
	int c;
	bool end, valid_coords;
	ship_st *ship = &(player->ships[ship_index]);

	do {
		end = false, tmp_player = *player;
		update_board(&(tmp_player.board[0][0]), *ship, ship_index);
		print_player_board(board, board_size, tmp_player);
		valid_coords = valid_position(board_size, &(player->board[0][0]), *ship);
		c = getch();
		switch(c) {
		case KEY_DOWN:
			ship->initial_row++;
			if(!valid_ship_bounds(board_size, *ship) || !valid_coordinates(board_size, ship->initial_row, ship->initial_column))
				ship->initial_row--;
			break;
		case KEY_UP:
			ship->initial_row--;
			if(!valid_ship_bounds(board_size, *ship) || !valid_coordinates(board_size, ship->initial_row, ship->initial_column))
				ship->initial_row++;
			break;
		case KEY_LEFT:
			ship->initial_column--;
			if(!valid_ship_bounds(board_size, *ship) || !valid_coordinates(board_size, ship->initial_row, ship->initial_column))
				ship->initial_column++;
			break;
		case KEY_RIGHT:
			ship->initial_column++;
			if(!valid_ship_bounds(board_size, *ship) || !valid_coordinates(board_size, ship->initial_row, ship->initial_column))
				ship->initial_column--;
			break;
		case 'd':
			ship->direction = !ship->direction;
			if(!valid_ship_bounds(board_size, *ship) || !valid_coordinates(board_size, ship->initial_row, ship->initial_column))
				ship->direction = !ship->direction;
			break;
		case 10: // ENTER
			end = true;
			break;
		}
	} while(!end || !valid_coords);
}

/**
 * Lê coordenadas de tiro
 */
void scan_shot_position(uint8_t *row, uint8_t *col)
{
	uint16_t tmp_row;
	char tmp_col;

	scanf("%hu %c", &tmp_row, &tmp_col);
	__fpurge(stdin);

	convert_coords_to_index(tmp_col, tmp_row, col, row);
}

