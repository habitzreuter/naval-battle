/**
 * Batalha Naval
 *
 * Autor: Marco Antônio Habitzreuter	(marco.habitzreuter@ufrgs.br)
 *					(mahabitzreuter@inf.ufrgs.br)
 *
 * Funções relacionadas à entrada e saída do jogador humano
 *
 */

#include <stdio_ext.h>
#include <stdint.h>
#include "game.h"
#include "ships.h"

/*
 * Imprime tabuleiro na tela
 */
void print_2d_char_array(WINDOW *window, size_t size, char *array)
{
	size_t board_width = 2 * size + 7, board_height = size + 1;
	size_t board_max_x, board_max_y;
	size_t begin_x, begin_y;
	char c;

	getmaxyx(window, board_max_y, board_max_x);
	begin_x = (board_max_x - board_width) / 2;
	begin_y = (board_max_y - board_height) / 2;

	// Imprime identificadores das colunas
	for(uint8_t i = 0; i < size; i++)
		mvwprintw(window, begin_y, begin_x + 2 * i + 7, "%c", (65 + i));

	// 1 a mais porque identificação das colunas ocupa uma linha
	for(uint8_t i = 1; i < size + 1; i++) {
		mvwprintw(window, begin_y + i, begin_x, "%2d --> ", i);
		for(uint8_t j = 0; j < size; j++) {
			// Le valor na célula do tabuleiro
			c = *(array + MAX_BOARD_SIZE * (i - 1) + j);

			if(c == 'X') wattron(window, COLOR_PAIR(1));
			if(c == '~') wattron(window, COLOR_PAIR(2));
			wattron(window, A_BOLD);

			mvwprintw(window, begin_y + i, begin_x + 2 * j + 7, "%c ", c);

			wattroff(window, A_BOLD | COLOR_PAIR(2));
		}
	}

	wrefresh(window);
}

/**
 * Gera matriz de caracteres a partir do tabuleiro do jogador
 */
void gen_player_board(size_t board_size, player_st player, char *destination)
{
	ship_st *ships;
	char c;
	uint8_t content, *board;
	uint32_t offset;

	ships = &(player.ships[0]);
	board = &(player.board[0][0]);

	// Processa tabuleiro, convertendo-o para matriz de caracteres
	for(uint8_t i = 0; i < board_size; i++) {
		for(uint8_t j = 0; j < board_size; j++) {
			// Le valor na célula do tabuleiro
			offset = MAX_BOARD_SIZE * i + j;
			content = *(board + offset);

			// Verifica os códigos de status
			if(content == NO_SHIP) c = '~';
			// Não é código de status, então é navio
			else c = get_ship_type(ships[content].size);

			*(destination +  offset) = c;
		}
	}
}

/**
 * Gera matriz de caracteres a partir do tabuleiro do adversário
 */
void gen_enemy_board(size_t board_size, player_st player, player_st enemy, char *destination)
{
	ship_st *ships;
	bool destroyed;
	char c;
	uint8_t content, hits, *board, ship_size;

	ships = &(enemy.ships[0]);
	board = &(player.enemy_board[0][0]);

	// Processa tabuleiro, convertendo-o para matriz de caracteres
	for(uint8_t i = 0; i < board_size; i++) {
		for(uint8_t j = 0; j < board_size; j++) {
			// Le valor na célula do tabuleiro
			content = *(board + MAX_BOARD_SIZE * i + j);

			// Verifica os códigos de status
			if(content == NOT_SHOT) c = '~';
			else if(content == MISSED_SHOT) c = '*';
			else {
				// Não é código de status, então é navio
				hits = ships[content].hits;
				ship_size = ships[content].size;
				destroyed = (hits == ship_size);

				if(destroyed)
					c = get_ship_type(ships[content].size);
				else
					c = 'X';
			}

			*(destination + MAX_BOARD_SIZE * i + j) = c;
		}
	}
}

/**
 * Le entrada do usuário e executa ação correspondente no tabuleiro
 * Returna true se tecla ENTER foi pressionada
 */
bool get_position(position_st *pos)
{
	int c;
	c = getch();
	switch(c) {
	case KEY_DOWN:
		pos->row++;
		break;
	case KEY_UP:
		pos->row--;
		break;
	case KEY_LEFT:
		pos->col--;
		break;
	case KEY_RIGHT:
		pos->col++;
		break;
	case 'd':
		pos->direction = !pos->direction;
		break;
	case 10: // ENTER
		return true;
		break;
	}

	return false;
}

/**
 * Chama função de leitura de coordenada e faz a consistência da leitura
 */
void scan_ship_position(WINDOW *board, WINDOW *info, player_st *player, size_t board_size, uint8_t ship_index)
{
	player_st tmp_player; // Tabuleiro temporário, apenas para exibição
	bool end, valid_pos, valid_bounds, valid_coord;
	ship_st *ship = &(player->ships[ship_index]);
	ship_st tmp_ship; // Para validação da entrada
	char ship_name[MAX_NAME_SIZE], tmp_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];

	stringify_ship_type(ship->size, ship_name);

	do {
		end = false, tmp_player = *player;

		update_board(&(tmp_player.board[0][0]), *ship, ship_index);
		gen_player_board(board_size, tmp_player, &(tmp_board[0][0]));
		print_2d_char_array(board, board_size, &(tmp_board[0][0]));
		valid_pos = valid_position(board_size, &(player->board[0][0]), *ship);

		tmp_ship = *ship;
		end = get_position(&(tmp_ship.pos));
		werase(info), box(info, 0, 0), wrefresh(info);
		mvwprintw(info, 1, 2, "Posicione seu %s", ship_name);
		wrefresh(info);

		// Atualiza posição se novas coordenadas são válidas
		valid_bounds = valid_ship_bounds(board_size, tmp_ship);
		valid_coord = valid_coordinates(board_size, tmp_ship.pos.row, tmp_ship.pos.col);
		if(valid_bounds && valid_coord) *ship = tmp_ship;

		// Tiro na nova posição é inválido
		if(end && !valid_pos) {
			wattron(info, A_BOLD | A_ERROR);
			mvwprintw(info, 1, 2, "%s em posição inválida! Tente novamente", ship_name);
			wattroff(info, A_BOLD | A_ERROR);
			wrefresh(info);
		}
	} while(!end || !valid_pos);
}

/**
 * Lê posição de tiro do usuário
 */
void scan_shot_position(WINDOW *board, WINDOW *info, player_st *player, player_st *enemy, uint8_t board_size, position_st *pos)
{
	bool end, valid_coords, position_shot;
	char tmp_board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
	// Inicializa conteúdo do ponteiro para evitar erro
	position_st tmp_pos = {0, 0, 0};
	*pos = tmp_pos;

	do {
		end = false, tmp_pos = *pos;
		// Gera um tabuleiro temporário, marcando posição do tiro
		gen_enemy_board(board_size, *player, *enemy, &(tmp_board[0][0]));
		tmp_board[tmp_pos.row][tmp_pos.col] = 'X';
		print_2d_char_array(board, board_size, &(tmp_board[0][0]));

		end = get_position(&(tmp_pos));
		werase(info), box(info, 0, 0), wrefresh(info);

		// Atualiza posição se novas coordenadas são válidas
		valid_coords = valid_coordinates(board_size, tmp_pos.row, tmp_pos.col);
		if(valid_coords) *pos = tmp_pos;

		// Tiro na nova posição é inválido
		position_shot = player->enemy_board[tmp_pos.row][tmp_pos.col] != NOT_SHOT;
		if(end && position_shot) {
			werase(info), box(info, 0, 0), wrefresh(info);
			wattron(info, A_BOLD | A_ERROR);
			mvwprintw(info, 1, 2, "Posição inválida! Tente novamente");
			wattroff(info, A_BOLD | A_ERROR);
			wrefresh(info);
		}
	} while(!end || position_shot);
	*pos = tmp_pos;
}

