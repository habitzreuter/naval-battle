/**
 * Batalha Naval
 *
 * Autor: Marco Antônio Habitzreuter (marco.habitzreuter@ufrgs.br)
 *				     (mahabitzreuter@inf.ufrgs.br)
 *
 * Funções relacionadas à entrada de dados de um jogador humano
 *
 */

#include <stdio.h>
#include <stdio_ext.h>
#include <stdint.h>
#include <stdbool.h>
#include "game.h"

/**
 * Lê coordenadas
 */
void scan_ship_position(uint16_t *row, uint16_t *col, bool *direction)
{
	uint16_t tmp_row, tmp_direction;
	char tmp_col;

	scanf("%hu %c %hu", &tmp_row, &tmp_col, &tmp_direction);
	__fpurge(stdin);

	*direction = tmp_direction;
	convert_coords_to_index(tmp_col, tmp_row, col, row);
}

