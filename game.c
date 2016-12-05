/**
 * Batalha Naval
 *
 * Autor: Marco Antônio habitzreuter	(marco.habitzreuter@ufrgs.br)
 *					(mahabitzreuter@inf.ufrgs.br)
 */

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <ncurses.h>
#include "game.h"
#include "player.h"
#include "ships.h"
#include "ai.h"

/**
 * Testa se coordenadas estão dentro dos limites do tabuleiro
 */
bool valid_coordinates(size_t board_size, uint8_t row, uint8_t col)
{
	return !(row > (board_size - 1) || col > (board_size - 1));
}

/**
 * Realiza uma tentativa de tiro
 */
void shot_try(WINDOW *board, WINDOW *messages, size_t board_size, player_st *player, player_st *enemy)
{
	uint8_t index, ship_hits, ship_size;
	bool human = (board != NULL && messages != NULL), destroyed;
	position_st pos;

	// Le/gera coordenadas
	if(human) {
		mvwprintw(messages, 1, 2, "%s, use as setas para selecionar o local de tiro no tabuleiro.", player->name);
		wrefresh(messages);
		scan_shot_position(board, messages, player, enemy, board_size, &pos);
	}else do {
		ai_generate_coords(board_size, &(pos.row), &(pos.col));
	} while(!valid_coordinates(board_size, pos.row, pos.col));

	if(enemy->board[pos.row][pos.col] != NO_SHIP) { // Caso atinja um navio inimigo
		// Armazena índice do navio acertado
		player->enemy_board[pos.row][pos.col] = enemy->board[pos.row][pos.col];
		index = enemy->board[pos.row][pos.col];
		enemy->ships[index].hits++;

		// Tiro certo dá um ponto e uma munição
		player->score++, player->ammo++;

		// Navio completamente destruído dá munição e score extra
		ship_hits = enemy->ships[index].hits;
		ship_size = enemy->ships[index].size;
		destroyed = (ship_hits == ship_size);
		if(destroyed) player->ammo += 3, player->score += 2;
	} else {
		// Se errou o tiro, armazena que não há navio nessa posição
		player->enemy_board[pos.row][pos.col] = MISSED_SHOT;
		player->ammo--;
	}
}

bool all_ships_destroyed(player_st *player)
{
	uint8_t ship_count = MAX_SHIPS, destroyed_ship_count = 0, ship_size,
		ship_hits;
	bool destroyed, all_destroyed;

	for(uint8_t i = 0; i < ship_count; i++) {
		ship_size = player->ships[i].size;
		ship_hits = player->ships[i].hits;
		destroyed = (ship_hits == ship_size);
		if(destroyed) destroyed_ship_count++;
	}

	if(destroyed_ship_count == ship_count) all_destroyed = true;
	else all_destroyed = false;

	return all_destroyed;
}

/**
 * Verifica se as codições de fim de jogo foram atingidas
 * Retorna 0 se ainda não terminou, 1 se player1 ganhou e 2 se player2 ganhou
 */
uint8_t game_end(player_st *player1, player_st *player2)
{
	uint8_t winner;
	bool player1_without_ships, player1_without_ammo;
	bool player2_without_ships, player2_without_ammo;

	player1_without_ships = all_ships_destroyed(player1);
	player1_without_ammo = (player1->ammo == 0);

	player2_without_ships = all_ships_destroyed(player2);
	player2_without_ammo = (player2->ammo == 0);

	if(player2_without_ships) {
		winner = 1;
		player1->score += 10;
	} else if(player1_without_ships) {
		winner = 2;
		player2->score += 10;
	} else if(player1_without_ammo) {
		winner = 2;
		player2->score += 3;
	} else if(player2_without_ammo) {
		winner = 1;
		player1->score += 3;
	} else winner = 0;

	return winner;

}

/**
 * Inicializa estruturas
 */
game_st set_default_values(uint8_t board_size)
{
	uint8_t ship_index, ship_count = 0;
	ship_st aircraft_carrier = {5, 0, {0, 0, 0}};
	ship_st destroyer = {4, 0, {0, 0, 0}};
	ship_st cruzer= {2, 0, {0, 0, 0}};
	ship_st submarine = {1, 0, {0, 0, 0}};

	// Regra de três usando o tabuleiro de tamanho 15 como base para
	// definir quantidade de navios em outros níveis
	uint8_t aircraft_count = board_size / 15;
	uint8_t destroyer_count = 2 * board_size / 15;
	uint8_t cruzer_count = 4 * board_size / 15;
	uint8_t submarine_count = 5 * board_size / 15;

	player_st player;
	player.score = 0;
	player.ammo = board_size;

	// Inicializa tabuleiros
	for(uint8_t i = 0; i < MAX_BOARD_SIZE; i++) {
		for(uint8_t j = 0; j < MAX_BOARD_SIZE; j++) {
			player.board[i][j] = NO_SHIP;
			player.enemy_board[i][j] = NOT_SHOT;
		}
	}

	ship_index = 0;
	ship_count += aircraft_count;
	for(; ship_index < ship_count; ship_index++)
		player.ships[ship_index] = aircraft_carrier;

	ship_count += destroyer_count;
	for(; ship_index < ship_count; ship_index++)
		player.ships[ship_index] = destroyer;

	ship_count += cruzer_count;
	for(; ship_index < ship_count; ship_index++)
		player.ships[ship_index] = cruzer;

	ship_count += submarine_count;
	for(; ship_index < ship_count; ship_index++)
		player.ships[ship_index] = submarine;

	game_st game = {board_size, false, ship_count, player, player};
	strcpy(game.player1.name, "Jogador humano");
	strcpy(game.player2.name, "");

	return game;
}

game_st game_new()
{
	WINDOW *begin= newwin(5, 100, (LINES - 5) / 2, (COLS - 100) / 2);
	WINDOW *board = newwin(LINES - 5, COLS / 2, 0, 0);
	WINDOW *enemy_board = newwin(LINES - 5, COLS / 2, 0, COLS / 2);
	WINDOW *messages = newwin(5, COLS / 2 , LINES - 5, 0);
	WINDOW *info = newwin(5, COLS / 2 , LINES - 5, COLS / 2);
	WINDOW *end = newwin(10, 60, (LINES - 10) / 2, (COLS - 60) / 2);
	game_st game;
	uint8_t winner;
	int board_size;

	clear(), refresh();

	// Le nomes dos jogadores e nível de dificuldade desejado
	box(begin, 0, 0);
	echo();
	mvwprintw(begin, 1, 1, "Insira o tamanho desejado do tabuleiro: ");
	wrefresh(begin);
	wscanw(begin, "%d", &board_size);
	game = set_default_values(board_size);
	mvwprintw(begin, 2, 1, "Insira o nome do jogador 1: ");
	wrefresh(begin);
	wscanw(begin, "%s", game.player1.name);
	mvwprintw(begin, 3, 1, "Insira o nome do jogador 2 (deixe em branco para jogar contra o computador): ");
	wrefresh(begin);
	wscanw(begin, "%s", game.player2.name);
	noecho();
	wclear(begin), wrefresh(begin), delwin(begin);

	if(game.player2.name[0] == '\0') { // String está vazia: é contra o PC
		strcpy(game.player2.name, "Computador");
		game.vs_computer = true;
	}

	box(info, 0, 0), box(board, 0, 0), box(enemy_board, 0, 0), box(messages, 0, 0);
	wrefresh(info), wrefresh(board), wrefresh(enemy_board), wrefresh(messages);

	set_ships(board, messages, &game.player1, game.player2, game.ship_count, game.board_size);
	if(game.vs_computer)
		set_ships(NULL, NULL, &game.player2, game.player1, game.ship_count, game.board_size);
	else
		set_ships(board, messages, &game.player2, game.player1, game.ship_count, game.board_size);

	do {
		print_player_board(board, game.board_size, game.player1, game.player2);
		shot_try(enemy_board, messages, game.board_size, &(game.player1), &(game.player2));

		if(game.vs_computer)
			shot_try(NULL, NULL, game.board_size, &(game.player2), &(game.player1));
		else {
			print_player_board(board, game.board_size, game.player2, game.player1);
			shot_try(enemy_board, messages, game.board_size, &(game.player2), &(game.player1));
		}
		winner = game_end(&game.player1, &game.player2);
		wclear(info), box(info, 0, 0), wrefresh(info);
		mvwprintw(info, 1, 2, "Pontuação do %s: %d", game.player1.name, game.player1.score);
		mvwprintw(info, 2, 2, "Munições restantes: %d", game.player1.ammo);
		mvwprintw(info, 1, 40, "Pontuação do %s: %d", game.player2.name, game.player2.score);
		mvwprintw(info, 2, 40, "Munições restantes: %d", game.player2.ammo);
		wrefresh(info);
	} while(winner == 0);

	// Após o fim do jogo, soma munições restantes ao score do jogador
	game.player1.score += game.player1.ammo;
	game.player2.score += game.player2.ammo;
	
	// Remove janelas
	wclear(info), wclear(board), wclear(enemy_board), wclear(messages);
	wborder(info, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wborder(board, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wborder(enemy_board, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wborder(messages, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(info), wrefresh(board), wrefresh(enemy_board), wrefresh(messages);
	delwin(info), delwin(board), delwin(enemy_board), delwin(messages);

	// Pontuação do computador não deve ir para o ranking, então
	// reseta para o valor mínimo
	if(game.vs_computer) game.player2.score = 0;

	if(winner == 1)
		wprintw(end, "%s venceu!\nPressione qualquer tecla para retornar ao menu", game.player1.name);
	else {
		wprintw(end, "%s venceu!\nPressione qualquer tecla para retornar ao menu", game.player2.name);
		// Jogador 1 é o que é armazenado no ranking
		game.player1 = game.player2;
	}


	wrefresh(end);
	delwin(end);
	getch();

	return game;
}

