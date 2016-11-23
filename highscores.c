/**
 * Batalha Naval
 *
 * Autor: Marco Antônio habitzreuter	(marco.habitzreuter@ufrgs.br)
 *					(mahabitzreuter@inf.ufrgs.br)
 *
 * Funções relacionadas ao ranking de pontuações
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "highscores.h"

void read_highscores(FILE *source, score_record_st *highscores)
{
	uint8_t r_count;
	score_record_st record, empty_record = {"", 0};

	for(uint8_t i = 0; i < HIGHSCORE_COUNT; i++) {
		// Busca um registro do arquivo
		r_count = fread(&record, sizeof(score_record_st), 1, source);

		// Se conseguiu ler, armazena registro no vetor
		if(r_count == 1) *(highscores + i) = record;

		// Se não leu nada, já chegou no final do arquivo, então
		// armazena registro vazio para o resto dos índices do vetor
		else *(highscores + i) = empty_record;
	}
}

