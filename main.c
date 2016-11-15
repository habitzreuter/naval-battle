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

int main()
{
	srand(time(NULL)); // Semente de números pseudo-aleatórios
	setlocale(LC_ALL,"portuguese"); // Configura caracteres em português

	game_new();

	return 0;
}

