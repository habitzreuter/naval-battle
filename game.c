/**
 * Batalha Naval
 *
 * Autor: Marco Antônio habitzreuter	(marco.habitzreuter@ufrgs.br)
 *					(mahabitzreuter@inf.ufrgs.br)
 */

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include "game.h"
#include "player.h"
#include "ships.h"
#include "ai.h"

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
 * Testa se coordenadas estão dentro dos limites do tabuleiro
 */
bool valid_coordinates(size_t board_size, uint8_t row, uint8_t col)
{
	return !(row > (board_size - 1) || col > (board_size - 1));
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

