/**
 * Batalha Naval
 *
 * Autor: Marco Antônio Habitzreuter (marco.habitzreuter@ufrgs.br)
 *				     (mahabitzreuter@inf.ufrgs.br)
 *
 * Funções relacionadas ao computador como oponente
 *
 */

#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * Gera coordenadas para posicionamento de navio (linha, coluna e direção)
 */
void ai_generate_ship_coords(uint8_t max, uint16_t *r, uint16_t *c, bool *d)
{
	*r = rand() % (max - 1); // Gera valor entre 0 e (MAX - 1)
	*c = rand() % (max - 1); // Gera valor entre 0 e (MAX - 1)
	*d = rand() % 1; // Gera valor TRUE ou FALSE
}

