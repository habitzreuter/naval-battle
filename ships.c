/**
 * Batalha Naval
 *
 * Autor: Marco Antônio habitzreuter	(marco.habitzreuter@ufrgs.br)
 *					(mahabitzreuter@inf.ufrgs.br)
 *
 * Funções relacionadas à manipulação dos navios
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "ships.h"
#include "player.h"
#include "ai.h"
#include "game.h"

/**
 * Retorna string correspondente ao navio
 */
void stringify_ship_type(enum ships type, char *destination)
{
	switch(type) {
	case 5:
		strcpy(destination, "Porta Aviões");
		break;
	case 4:
		strcpy(destination, "Destroyer");
		break;
	case 2:
		strcpy(destination, "Cruzador");
		break;
	case 1:
		strcpy(destination, "Submarino");
		break;
	}
}

/**
 * Retorna caracere correspondente ao navio
 */
char get_ship_type(enum ships type)
{
	char tmp[] = "String that will contain the ship name";
	char *c = tmp;
	stringify_ship_type(type, c);
	return c[0];
}

/**
 * Verifica se as coordenadas informadas pelo usuário não farão o navio se
 * sobrepor a outro navio já colocado
 */
bool ship_superposition(size_t ship_size, uint8_t *board, uint8_t row,
		uint8_t col, bool direction)
{
	bool status = true;
	uint8_t empty_cell = ship_size;
	uint8_t cell_content;

	for(uint8_t i = 0; i < ship_size; i++) {
		switch(direction) {
		case 0:
			cell_content = *(board + MAX_BOARD_SIZE * row + col + i);
			break;
		case 1:
			cell_content = *(board + MAX_BOARD_SIZE * (row + i) + col);
			break;
		}
		if(cell_content != NO_SHIP) empty_cell--;
	}

	if(empty_cell == ship_size) status = false;

	return status;
}

/**
 * Verifica se coordenadas informadas pelo usuário não vão colocar o navio
 * fora dos limites do tabuleiro
 */
bool valid_ship_bounds(size_t board_size, ship_st ship)
{
	char status = false;
	uint8_t final_row, final_col;

	switch(ship.direction) {
	case 1:
		final_col = ship.initial_column;
		final_row = ship.initial_row + ship.size;
		if(final_row <= board_size) status = true;
		break;
	case 0:
		final_row = ship.initial_row;
		final_col = ship.initial_column + ship.size;
		if(final_col <= board_size) status = true;
		break;
	}

	return status;
}

/**
 * Verifica se coordenadas especificadas para um determinado navio são
 * válidas
 */
bool valid_position(size_t board_size, uint8_t *board, ship_st ship)
{
	size_t size = ship.size;
	uint8_t row = ship.initial_row, col = ship.initial_column,
		direction = ship.direction;

	return !ship_superposition(size, board, row, col, direction)
		&& valid_ship_bounds(board_size, ship)
		&& valid_coordinates(board_size, row, col);
}

/**
 * Insere navio no tabuleiro
 */
void update_board(uint8_t *board, ship_st ship, uint8_t ship_index)
{
	uint8_t in_row = ship.initial_row, in_col = ship.initial_column;

	switch(ship.direction) {
	case 0:
		for(uint8_t i = 0; i < ship.size; i++)
			*(board + MAX_BOARD_SIZE * in_row + in_col + i) = ship_index;
		break;
	case 1:
		for(uint8_t i = 0; i < ship.size; i++)
			*(board + MAX_BOARD_SIZE * (in_row + i) + in_col) = ship_index;
		break;
	}
}

/*
 * Seleção da posição dos navios
 */
void set_ships(WINDOW *board, WINDOW *info, player_st *player, size_t board_size)
{
	uint8_t ship_count = MAX_SHIPS;
	char ship_name[20];
	bool human = (board != NULL && info != NULL), valid_coords;
	ship_st *ship;

	for(uint8_t i = 0; i < ship_count; i++) {
		ship = &(player->ships[i]);
		if(human) {
			scan_ship_position(board, info, player, board_size, i);
		} else {
			do {
				ai_generate_ship_coords(board_size, ship);
				valid_coords = valid_position(board_size, &(player->board[0][0]), *ship);
			} while (!valid_coords);
		}
		update_board(&(player->board[0][0]), *ship, i);
	}
}

