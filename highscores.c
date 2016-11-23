/**
 * Batalha Naval
 *
 * Autor: Marco Antônio habitzreuter	(marco.habitzreuter@ufrgs.br)
 *					(mahabitzreuter@inf.ufrgs.br)
 *
 * Funções relacionadas ao ranking de pontuações
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "highscores.h"

/**
 * Le arquivo do ranking e preenche as posições não registradas com uma struct
 * vazia
 */
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

/**
 * Verifica se pontuação do jogador é maior do que alguma do ranking e salva
 * novo ranking
 */
void update_ranking(FILE *hs, player_st player, score_record_st highscores[HIGHSCORE_COUNT])
{
	bool file_changed = false;
	uint32_t write_count;

	// Verifica se pontuação do jogador é maior do que alguma do ranking
	for(uint8_t i = 0; (i < HIGHSCORE_COUNT) && !(file_changed); i++) {
		if(player.score > highscores[i].score) {
			// Move o ranking um elemento para trás antes de inserir
			// o novo jogador
			for(uint8_t j = (HIGHSCORE_COUNT - 1); j > i; j--) {
				highscores[j] = highscores[j-1];
			}

			highscores[i].score = player.score;
			strcpy(highscores[i].name, player.name);
			file_changed = true;

		}
	}

	// Se for maior, atualiza arquivo
	if(file_changed) {
		rewind(hs);
		write_count = fwrite(
				highscores,
				sizeof(score_record_st),
				HIGHSCORE_COUNT,
				hs
				);
		if(write_count != HIGHSCORE_COUNT)
			printf("Erro ao escrever pontuação no arquivo!\n");
	}
}

