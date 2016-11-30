/**
 * Batalha Naval
 *
 * Autor: Marco Antônio Habitzreuter	(marco.habitzreuter@ufrgs.br)
 *					(mahabitzreuter@inf.ufrgs.br)
 *
 * Funções relacionadas à entrada de dados de um jogador humano
 *
 */

#include <stdio_ext.h>
#include <stdint.h>
#include "game.h"
#include "ships.h"

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
void scan_ship_position(WINDOW *board, WINDOW *info, player_st *player, size_t board_size, uint8_t ship_index, char ship_name[MAX_NAME_SIZE])
{
	player_st tmp_player; // Tabuleiro temporário, apenas para exibição
	bool end, valid_pos, valid_bounds, valid_coord;
	ship_st *ship = &(player->ships[ship_index]);
	ship_st tmp_ship; // Para validação da entrada

	do {
		end = false, tmp_player = *player;
		update_board(&(tmp_player.board[0][0]), *ship, ship_index);
		print_player_board(board, board_size, tmp_player);
		valid_pos = valid_position(board_size, &(player->board[0][0]), *ship);
		tmp_ship = *ship;

		end = get_position(&(tmp_ship.pos));
		werase(info);
		box(info, 0, 0);
		wrefresh(info);
		mvwprintw(info, 1, 2, "Posicione seu %s", ship_name);
		wrefresh(info);

		valid_bounds = valid_ship_bounds(board_size, tmp_ship);
		valid_coord = valid_coordinates(board_size, tmp_ship.pos.row, tmp_ship.pos.col);
		if(valid_bounds && valid_coord) *ship = tmp_ship;
		if(end && !valid_pos) {
			wattron(info, A_BOLD | A_ERROR);
			mvwprintw(info, 1, 2, "%s em posição inválida! Tente novamente", ship_name);
			wattroff(info, A_BOLD | A_ERROR);
			wrefresh(info);
			refresh();
		}
	} while(!end || !valid_pos);
}

/**
 * Chama função que faz leitura da coordenada de tiro e valida essa entrada
 */
void scan_shot_position(WINDOW *board, WINDOW *info, player_st *player, player_st *enemy, uint8_t board_size, position_st *pos)
{
	bool end, valid_coords;
	position_st tmp_pos = {0, 0, 0};
	player_st tmp_player; // Tabuleiro temporário, apenas para exibição

	do {
		end = false;
		tmp_player = *player;
		// Navio falso, apenas para exibir o 'X' no tabuleiro
		tmp_player.enemy_board[tmp_pos.row][tmp_pos.col] = MAX_SHIPS;
		print_enemy_board(board, board_size, tmp_player, *enemy);
		valid_coords = valid_coordinates(board_size, tmp_pos.row, tmp_pos.col);
		end = get_position(&(tmp_pos));
	} while(!end || !valid_coords);

	*pos = tmp_pos;

}

