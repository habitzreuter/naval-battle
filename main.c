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
#include <time.h>
#include "game.h"
#include "highscores.h"

int main()
{
	FILE *hs;
	score_record_st highscores[HIGHSCORE_COUNT];

	// Le ranking dos 10 melhores jogadores
	hs = fopen("highscores.bin", "ab+");
	if(hs == NULL) printf("Erro ao abrir highscores.bin\n");
	else read_highscores(hs, highscores), fclose(hs);

	srand(time(NULL)); // Semente de números pseudo-aleatórios
	setlocale(LC_ALL,"portuguese"); // Configura caracteres em português

	game_new();

	return 0;
}

