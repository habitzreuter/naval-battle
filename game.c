#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_BOARD_SIZE 100
#define MAX_SHIPS 12

typedef enum {
	true = 1,
	false = 0
} bool;

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
	uint16_t score;
	ship_st ships[MAX_SHIPS];
	uint8_t board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
} player_st;

typedef struct {
	uint32_t time;
	uint8_t board_size;
	player_st player1, player2;
} game_st;

void print_board(uint8_t size, uint8_t board[size][size])
{
	for(uint8_t i = 0; i < size + 1; i++) {

		// Print row number
		if(i != 0) printf("%2d --> ", i);
		else printf("       ");

		for(uint8_t j = 0; j < size; j++) {
			if(i == 0) {
				// Print column in the first line
				printf("%2c ", (65 + j));
			} else {
				char c;
				switch (board[i - 1][j]) {
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
				}
				printf("%2c ", c);
			}
		}
		printf("\n");
	}
}

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
 * Verify if there are no other ships in the coordinates passed by the user
 * Returns 'true' if there is a superposition
 */
bool ship_superposition(uint8_t board_size, uint8_t ship_size,
		uint8_t board[board_size][board_size], uint8_t row,
		uint8_t col, bool direction)
{
	bool status = true;
	uint8_t empty_cell = ship_size;

	switch(direction) {
	case 1:
		for(uint8_t i = 0; i < ship_size; i++)
			if(board[row + i][col] != 0) empty_cell--;
		break;
	case 0:
		for(uint8_t i = 0; i < ship_size; i++)
			if(board[row][col + i] != 0) empty_cell--;
		break;
	}

	if(empty_cell == ship_size) status = false;

	return status;
}

/**
 * Verify if coordinates passed by the user won't set the ship in a position
 * inside the board
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

bool valid_coordinates(uint8_t board_size, uint8_t row, uint8_t col)
{
	return !(row > board_size || col > board_size);
}

bool valid_position(uint8_t board_size, uint8_t board[board_size][board_size],
		ship_st ship)
{
	uint8_t size = ship.size, row = ship.initial_row,
		col = ship.initial_column, direction = ship.direction;

	return !ship_superposition(board_size, size, board, row, col, direction)
		&& valid_ship_bounds(board_size, size, row, col, direction)
		&& valid_coordinates(board_size, row, col);
}

void convert_coords_to_index(char tmp_col, uint16_t tmp_row, uint16_t *col, uint16_t *row)
{
	*col = (uint16_t) toupper(tmp_col) - 65;
	*row = tmp_row - 1;
}

void scan_ship_position(uint16_t *row, uint16_t *col, bool *direction)
{
	uint16_t tmp_row, tmp_direction;
	char tmp_col;

	scanf("%hu %c %hu", &tmp_row, &tmp_col, &tmp_direction);

	*direction = tmp_direction;
	convert_coords_to_index(tmp_col, tmp_row, col, row);
}

void update_board(uint8_t board_size, uint8_t board[board_size][board_size], ship_st ship)
{
	uint8_t in_row = ship.initial_row, in_col = ship.initial_column;

	switch(ship.direction) {
	case 0:
		for(uint8_t i = 0; i < ship.size; i++)
			board[in_row][in_col + i] = ship.type;
		break;
	case 1:
		for(uint8_t i = 0; i < ship.size; i++)
			board[in_row + i][in_col] = ship.type;
		break;
	}
}

/*
 * Prompts player to select where to place the ships
 */
void set_ships(player_st *player, uint8_t board_size)
{
	uint8_t ship_count = sizeof(player->ships) / sizeof(ship_st);
	char ship_name[20];

	for(uint8_t i = 0; i < ship_count; i++) {
		ship_st *ship = &(player->ships[i]);

		stringify_ship_type(ship->type, ship_name);
		print_board(board_size, player->board);

		do {
			printf("Coordenada inicial para o %s (linha, coluna e direção): ", ship_name);
			scan_ship_position(
					&(ship->initial_row),
					&(ship->initial_column),
					&(ship->direction));
		} while(!valid_position(board_size, player->board, *ship));

		update_board(board_size, player->board, *ship);

	}
}

/**
 * Create variables with defaut values
 * TODO: use this to configure game difficulty
 */
game_st set_default_values()
{
	ship_st aircraft_carrier = {5, -1, -1, 0, AIRCRAFT_CARRIER};
	ship_st destroyer = {4, -1, -1, 0, DESTROYER};
	ship_st cruzer= {2, -1, -1, 0, CRUZER};
	ship_st submarine = {1, -1, -1, 0, SUBMARINE};
	
	player_st player = {0, {
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
	}, {{0}}};

	game_st game = {0, 15, player, player};
	return game;
}

void game_new()
{
	game_st game = set_default_values();
	set_ships(&game.player1, game.board_size);

}

