/**
 * Batalha Naval
 *
 * Autor: Marco Antônio Habitzreuter	(marco.habitzreuter@ufrgs.br)
 *					(mahabitzreuter@inf.ufrgs.br)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "highscores.h"

int main()
{
	FILE *hs;
	score_record_st highscores[HIGHSCORE_COUNT];

	// Le ranking dos 10 melhores jogadores
	hs = fopen("highscores.bin", "rb+");
	if(hs == NULL) printf("Erro ao abrir highscores.bin\n");
	else read_highscores(hs, highscores);

	for(uint8_t i = 0; i < HIGHSCORE_COUNT; i++) {
		printf("%d %d %s\n", i, highscores[i].score, highscores[i].name);
	}

	srand(time(NULL)); // Semente de números pseudo-aleatórios
	setlocale(LC_ALL,"portuguese"); // Configura caracteres em português

	//game_st game = game_new();
	game_st game;
	game.player1.score = 80;
	strcpy(&(game.player1.name[0]), "Jogador3");

	update_ranking(hs, game.player1, highscores);

	fclose(hs);

	return 0;
}

