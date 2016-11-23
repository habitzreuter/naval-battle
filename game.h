#ifndef __GAME_H__
#define __GAME_H__

#include <stdbool.h>
#include "highscores.h"

// Constantes de tamanhos máximos de vetores
#define MAX_BOARD_SIZE	100
#define MAX_SHIPS	12
#define MAX_NAME_SIZE	31

// Constantes do tabuleiro do jogador
#define NO_SHIP		250

// Constantes do tabuleiro do adversário
#define MISSED_SHOT	255
#define NOT_SHOT	NO_SHIP

#define HUMAN	true
#define AI	false

#define PLAYER	false
#define ENEMY	true

enum ships {
	AIRCRAFT_CARRIER = 5,
	DESTROYER = 4,
	CRUZER = 2,
	SUBMARINE = 1
};

typedef struct {
	size_t size; // Tamanho e tipo do navio
	uint8_t initial_row, initial_column,
		hits; // Quantos tiros atingiram o navio
	bool direction; // 1: vertical, 0: horizontal
} ship_st;

typedef struct {
	char name[MAX_NAME_SIZE];
	uint16_t score;
	uint16_t ammo;
	ship_st ships[MAX_SHIPS];
	uint8_t board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
	uint8_t enemy_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
} player_st;

typedef struct {
	uint32_t time;
	size_t board_size;
	player_st player1, player2;
} game_st;

void game_new();
void convert_coords_to_index(char, uint16_t, uint8_t*, uint8_t*);
bool valid_coordinates(size_t, uint8_t, uint8_t);
void print_board(size_t, player_st, bool);

#endif

