/**
 * Batalha Naval
 *
 * Autor: Marco Antônio Habitzreuter	(marco.habitzreuter@ufrgs.br)
 *					(mahabitzreuter@inf.ufrgs.br)
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
	menu();

	// Le ranking dos 10 melhores jogadores
	hs = fopen("highscores.bin", "rb+");
	if(hs == NULL) printf("Erro ao abrir highscores.bin\n");
	else read_highscores(hs, highscores);

	//game_st game = game_new();
	//game_st game;
	//game.player1.score = 80;
	//strcpy(&(game.player1.name[0]), "Jogador3");

	//update_ranking(hs, game.player1, highscores);

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
	//start_color(); // Habilita modo de cores da ncurses
	curs_set(0); // Esconde cursor
}

