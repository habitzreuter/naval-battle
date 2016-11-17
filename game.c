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

/*
 * Imprime tabuleiro na tela
 */
void print_board(size_t size, uint8_t *board, ship_st ships[MAX_SHIPS])
{
	char c;
	uint8_t index;
	for(uint8_t i = 0; i < size + 1; i++) {

		// Imprime número da linha
		if(i != 0) printf("%2d --> ", i);
		else printf("       ");

		for(uint8_t j = 0; j < size; j++) {
			// Imprime letra da coluna na primeira linha
			if(i == 0) printf("%2c ", (65 + j));
			else {
				// Le valor na célula do tabuleiro
				index = *(board + MAX_BOARD_SIZE * (i - 1) + j);

				// Verifica os códigos de status
				switch(index) {
				case NO_SHIP:
					c = '~';
					break;
				case MISSED_SHOT:
					c = 'X';
					break;
				default:
					// Não é código de status, então é navio
					c = get_ship_type(ships[index].size);
				}

				printf("%2c ", c);
			}
		}
		printf("\n");
	}
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
bool valid_ship_bounds(size_t board_size, size_t ship_size, uint8_t row,
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
bool valid_coordinates(size_t board_size, uint8_t row, uint8_t col)
{
	return !(row > (board_size - 1) || col > (board_size - 1));
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
		&& valid_ship_bounds(board_size, size, row, col, direction)
		&& valid_coordinates(board_size, row, col);
}

/**
 * Converte coordenadas para índices da matriz do tabuleiro
 */
void convert_coords_to_index(char tmp_col, uint16_t tmp_row, uint8_t *col, uint8_t *row)
{
	*col = (uint8_t) (toupper(tmp_col) - 65);
	*row = (uint8_t) (tmp_row - 1);
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
void set_ships(player_st *player, size_t board_size, bool human)
{
	uint8_t ship_count = MAX_SHIPS;
	char ship_name[20];

	for(uint8_t i = 0; i < ship_count; i++) {
		ship_st *ship = &(player->ships[i]);

		do {
			if(human) {
				stringify_ship_type(ship->size, ship_name);
				print_board(board_size, &(player->board[0][0]), player->ships);

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

		update_board(&(player->board[0][0]), *ship, i);
	}
}

/**
 * Inicializa estruturas
 * TODO: Usar isso para configurar dificuldade do jogo
 */
game_st set_default_values()
{
	ship_st aircraft_carrier = {5, 0, 0, 0, 0};
	ship_st destroyer = {4, 0, 0, 0, 0};
	ship_st cruzer= {2, 0, 0, 0, 0};
	ship_st submarine = {1, 0, 0, 0, 0};
	
	player_st player;
	player.score = 0;
	player.ammo = 15;

	// Inicializa tabuleiros
	for(uint8_t i = 0; i < MAX_BOARD_SIZE; i++) {
		for(uint8_t j = 0; j < MAX_BOARD_SIZE; j++) {
			player.board[i][j] = NO_SHIP;
			player.enemy_board[i][j] = NOT_SHOT;
		}
	}

	// Inicializa navios
	player.ships[0] = aircraft_carrier;
	player.ships[1] = destroyer;
	player.ships[2] = destroyer;
	player.ships[3] = cruzer;
	player.ships[4] = cruzer;
	player.ships[5] = cruzer;
	player.ships[6] = cruzer;
	player.ships[7] = submarine;
	player.ships[8] = submarine;
	player.ships[9] = submarine;
	player.ships[10] = submarine;
	player.ships[11] = submarine;

	game_st game = {0, 15, player, player};
	return game;
}

/**
 * Realiza uma tentativa de tiro
 */
void shot_try(size_t board_size, player_st *player, player_st *enemy, bool human)
{
	uint8_t row, col, index;

	// Le e valida coordenadas
	do {

		if(human) {
			printf("%s, insira as coordenadas do tiro (linha e coluna): ", player->name);
			scan_shot_position(&row, &col);
		} else ai_generate_coords(board_size, &row, &col);

	} while(!valid_coordinates(board_size, row, col));

	if(enemy->board[row][col] != NO_SHIP) { // Caso atinja um navio inimigo
		player->enemy_board[row][col] = enemy->board[row][col];
		index = enemy->board[row][col];
		enemy->ships[index].hits++;

		// Se cada tiro gasta uma munição e cada acerto dá duas, então
		// cada acerto soma uma munição
		player->ammo++;

		// Recebe 3 munições extra se navio foi completamente destruído
		if(enemy->ships[index].hits == enemy->ships[index].size)
			player->ammo += 3;

	} else {
		// Se errou o tiro, armazena que não há navio nessa posição
		player->enemy_board[row][col] = MISSED_SHOT;
		player->ammo--;
	}
}

/**
 * Verifica se as codições de fim de jogo foram atingidas
 * Retorna 0 se ainda não terminou, 1 se player1 ganhou e 2 se player2 ganhou
 */
uint8_t game_end(player_st *player1, player_st *player2)
{
	uint8_t winner;

	if(player1->ammo == 0) winner = 2;
	else if(player2->ammo == 0) winner = 1;
	else winner = 0;

	return winner;

}

void game_new()
{
	game_st game = set_default_values();
	uint8_t winner;

	set_ships(&game.player1, game.board_size, AI);
	set_ships(&game.player2, game.board_size, AI);

	do {
		shot_try(game.board_size, &game.player1, &game.player2, HUMAN);
		shot_try(game.board_size, &game.player2, &game.player1, AI);
		print_board(game.board_size, &(game.player2.board[0][0]), game.player2.ships);
		print_board(game.board_size, &(game.player1.enemy_board[0][0]), game.player1.ships);
		winner = game_end(&game.player1, &game.player2);
	} while(winner == 0);

	printf("\nGanhador: %d\n", winner);

}

