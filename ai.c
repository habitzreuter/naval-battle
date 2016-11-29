/**
 * Batalha Naval
 *
 * Autor: Marco Antônio Habitzreuter	(marco.habitzreuter@ufrgs.br)
 *					(mahabitzreuter@inf.ufrgs.br)
 *
 * Funções relacionadas ao computador como oponente
 *
 */

#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "game.h"

/**
 * Gera linha e coluna do tabuleiro
 */
void ai_generate_coords(uint8_t max, uint8_t *r, uint8_t *c)
{
	*r = rand() % (max - 1); // Gera valor entre 0 e (MAX - 1)
	*c = rand() % (max - 1); // Gera valor entre 0 e (MAX - 1)
}

/**
 * Gera coordenadas para posicionamento de navio (linha, coluna e direção)
 */
void ai_generate_ship_coords(uint8_t max, ship_st *ship)
{
	ai_generate_coords(max, &(ship->initial_row), &(ship->initial_column));
	ship->direction = rand() % 2; // Gera valor TRUE ou FALSE
}

