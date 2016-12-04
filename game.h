#ifndef __GAME_H__
#define __GAME_H__

#include <stdbool.h>
#include <ncurses.h>

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

#define A_ERROR COLOR_PAIR(1)

enum ships {
	AIRCRAFT_CARRIER = 5,
	DESTROYER = 4,
	CRUZER = 2,
	SUBMARINE = 1
};

typedef struct {
	uint8_t row, col;
	bool direction;
} position_st;

typedef struct {
	size_t size, // Tamanho e tipo do navio
		hits; // Quantos tiros atingiram o navio
	position_st pos;
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
	bool vs_computer;
	player_st player1, player2;
} game_st;

game_st game_new();
bool valid_coordinates(size_t, uint8_t, uint8_t);

#endif

