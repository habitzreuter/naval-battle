/**
 * Batalha Naval
 *
 * Copyright (C) 2017 Marco Antônio Habitzreuter (marco.habitzreuter@ufrgs.br)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Para detalhes sobre a licença, ver "main.c" e "LICENSE"
 *
 */

#include <stdlib.h>
#include <stdint.h>
#include <locale.h>
#include <time.h>
#include <ncurses.h>
#include "highscores.h"
#include "my_menu.h"

void init();
void end();

int main()
{
	FILE *hs;
	score_record_st highscores[HIGHSCORE_COUNT];

	init();

	// Le ranking dos 10 melhores jogadores
	hs = fopen("highscores.bin", "rb+");
	if(hs == NULL) printf("Erro ao abrir highscores.bin\n");
	else read_highscores(hs, highscores);

	menu(hs, highscores);

	fclose(hs);
	endwin();

	return 0;
}

/**
 * Função que agrupa as chamadas para as funções de inicialização
 */
void init()
{
	srand(time(NULL)); // Semente de números pseudo-aleatórios
	setlocale(LC_ALL,"portuguese"); // Configura caracteres em português

	initscr(); // Inicializa ncurses
	keypad(stdscr, TRUE); // Habilita outras teclas, inclusive as setas
	noecho(); // Desativa impressão dos caracteres inseridos pelo usuário
	raw(); // Desativa buffer do teclado
	curs_set(0); // Esconde cursor

	start_color(); // Habilita modo de cores da ncurses
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
}

