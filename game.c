/**
 * Batalha Naval
 *
 * Autor: Marco Antônio habitzreuter	(marco.habitzreuter@ufrgs.br)
 *					(mahabitzreuter@inf.ufrgs.br)
 */

#include <stdio_ext.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "player.h"
#include "ai.h"

#define MAX_BOARD_SIZE 100
#define MAX_SHIPS 12
#define MAX_NAME_SIZE 31

#define MISSED_SHOT 255

enum ships {
	AIRCRAFT_CARRIER = 1,
	DESTROYER = 2,
	CRUZER = 3,
	SUBMARINE = 4
};

typedef struct {
	uint8_t size;
	uint16_t initial_row, initial_column;
	bool direction; // 1: vertical, 0: horizontal
	enum ships type;
} ship_st;

typedef struct {
	char name[MAX_NAME_SIZE];
	uint16_t score;
	uint16_t left_shots;
	ship_st ships[MAX_SHIPS];
	uint8_t board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
	uint8_t enemy_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
} player_st;

typedef struct {
	uint32_t time;
	uint8_t board_size;
	player_st player1, player2;
} game_st;

/*
 * Imprime tabuleiro na tela
 */
void print_board(uint8_t size, uint8_t *board)
{
	char c;
	uint8_t content;
	for(uint8_t i = 0; i < size + 1; i++) {

		// Imprime número da linha
		if(i != 0) printf("%2d --> ", i);
		else printf("       ");

		for(uint8_t j = 0; j < size; j++) {
			if(i == 0) {
				// Imprime letra da coluna na primeira linha
				printf("%2c ", (65 + j));
			} else {
				content = *(board + MAX_BOARD_SIZE * (i - 1) + j);
				switch (content) {
				case 0:
					c = '~';
					break;
				case 1:
					c = 'P';
					break;
				case 2:
					c = 'D';
					break;
				case 3:
					c = 'C';
					break;
				case 4:
					c = 'S';
					break;
				case MISSED_SHOT:
					c = 'X';
				}
				printf("%2c ", c);
			}
		}
		printf("\n");
	}
}

/**
 * Retorna string correspondente ao número do navio
 */
void stringify_ship_type(enum ships type, char *destination)
{
	switch(type) {
	case 1:
		strcpy(destination, "Porta Aviões");
		break;
	case 2:
		strcpy(destination, "Destroyer");
		break;
	case 3:
		strcpy(destination, "Cruzador");
		break;
	case 4:
		strcpy(destination, "Submarino");
		break;
	}
}

/**
 * Verifica se as coordenadas informadas pelo usuário não farão o navio se
 * sobrepor a outro navio já colocado
 */
bool ship_superposition(uint8_t ship_size, uint8_t *board, uint8_t row,
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
		if(cell_content != 0) empty_cell--;
	}

	if(empty_cell == ship_size) status = false;

	return status;
}

/**
 * Verifica se coordenadas informadas pelo usuário não vão colocar o navio
 * fora dos limites do tabuleiro
 */
bool valid_ship_bounds(uint8_t board_size, uint8_t ship_size, uint8_t row,
		uint8_t col, bool direction)
{
	char status = false;
	uint8_t final_row, final_col;

	switch(direction) {
	case 1:
		final_col = col;
		final_row = row + ship_size;
		if(final_row <= board_size) status = true;
		break;
	case 0:
		final_row = row;
		final_col = col + ship_size;
		if(final_col <= board_size) status = true;
		break;
	}

	return status;
}

/**
 * Testa se coordenadas estão dentro dos limites do tabuleiro
 */
bool valid_coordinates(uint8_t board_size, uint8_t row, uint8_t col)
{
	return !(row > (board_size - 1) || col > (board_size - 1));
}

/**
 * Verifica se coordenadas especificadas para um determinado navio são
 * válidas
 */
bool valid_position(uint8_t board_size, uint8_t *board,
		ship_st ship)
{
	uint8_t size = ship.size, row = ship.initial_row,
		col = ship.initial_column, direction = ship.direction;

	return !ship_superposition(size, board, row, col, direction)
		&& valid_ship_bounds(board_size, size, row, col, direction)
		&& valid_coordinates(board_size, row, col);
}

/**
 * Converte coordenadas para índices da matriz do tabuleiro
 */
void convert_coords_to_index(char tmp_col, uint16_t tmp_row, uint16_t *col, uint16_t *row)
{
	*col = (uint16_t) toupper(tmp_col) - 65;
	*row = tmp_row - 1;
}

/**
 * Insere navio no tabuleiro
 */
void update_board(uint8_t *board, ship_st ship)
{
	uint8_t in_row = ship.initial_row, in_col = ship.initial_column;

	switch(ship.direction) {
	case 0:
		for(uint8_t i = 0; i < ship.size; i++)
			*(board + MAX_BOARD_SIZE * in_row + in_col + i) = ship.type;
		break;
	case 1:
		for(uint8_t i = 0; i < ship.size; i++)
			*(board + MAX_BOARD_SIZE * (in_row + i) + in_col) = ship.type;
		break;
	}
}

/*
 * Seleção da posição dos navios
 */
void set_ships(player_st *player, uint8_t board_size, bool human)
{
	uint8_t ship_count = sizeof(player->ships) / sizeof(ship_st);
	char ship_name[20];

	for(uint8_t i = 0; i < ship_count; i++) {
		ship_st *ship = &(player->ships[i]);

		do {
			if(human) {
				stringify_ship_type(ship->type, ship_name);
				print_board(board_size, &(player->board[0][0]));

				printf("Coordenada inicial para o %s (linha, coluna e direção): ", ship_name);
				scan_ship_position(
					&(ship->initial_row),
					&(ship->initial_column),
					&(ship->direction));
			} else ai_generate_ship_coords(
					board_size,
					&(ship->initial_row),
					&(ship->initial_column),
					&(ship->direction));

		} while(!valid_position(board_size, &(player->board[0][0]), *ship));

		update_board(&(player->board[0][0]), *ship);
	}
}

/**
 * Inicializa estruturas
 * TODO: Usar isso para configurar dificuldade do jogo
 */
game_st set_default_values()
{
	ship_st aircraft_carrier = {5, -1, -1, 0, AIRCRAFT_CARRIER};
	ship_st destroyer = {4, -1, -1, 0, DESTROYER};
	ship_st cruzer= {2, -1, -1, 0, CRUZER};
	ship_st submarine = {1, -1, -1, 0, SUBMARINE};
	
	player_st player = {"Jogador", 0, 15,{
		aircraft_carrier,
		destroyer,
		destroyer,
		cruzer,
		cruzer,
		cruzer,
		cruzer,
		submarine,
		submarine,
		submarine,
		submarine,
		submarine
	}, {{0}}, {{0}}};

	game_st game = {0, 15, player, player};
	return game;
}

/**
 * Realiza uma tentativa de tiro
 */
void shot_try(uint8_t board_size, player_st *player, player_st *enemy)
{
	uint16_t tmp_row, row, col;
	char tmp_col;

	// Le e valida coordenadas
	do {
		printf("%s, insira as coordenadas do tiro: ", player->name);
		scanf("%c %hu", &tmp_col, &tmp_row);
		__fpurge(stdin);
		convert_coords_to_index(tmp_col, tmp_row, &col, &row);
	} while(!valid_coordinates(board_size, row, col));

	// Caso hava um navio inimigo nas coordenadas indicadas
	if(enemy->board[row][col] != 0)
		player->enemy_board[row][col] = enemy->board[row][col];
	else player->enemy_board[row][col] = MISSED_SHOT;
}

void game_new()
{
	game_st game = set_default_values();
	//set_ships(&game.player1, game.board_size, true);
	set_ships(&game.player2, game.board_size, false);

	//shot_try(game.board_size, &game.player1, &game.player2);
	//print_board(game.board_size, &(game.player1.enemy_board[0][0]));
	print_board(game.board_size, &(game.player2.board[0][0]));

}

