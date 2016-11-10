#include <stdio.h>
#include <stdint.h>

#define MAX_BOARD_SIZE 100
#define MAX_SHIPS 12

typedef enum {
	true = 1,
	false = 0
} bool;

enum ships {
	AIRCRAFT_CARRIER = 0,
	DESTROYER = 1,
	CRUZER = 2,
	SUBMARINE = 3
};

typedef struct {
	uint8_t size;
	char initial_column;
	int8_t initial_row;
	bool direction; // 0: vertical, 1: horizontal
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
				switch (board[i][j]) {
					case 0:
						c = '~';
				}
				printf("%2c ", c);
			}
		}
		printf("\n");
	}
}

void set_ships(player_st *player)
{
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
	set_ships(&game.player1);

}

