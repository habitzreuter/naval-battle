/**
 * Batalha Naval
 *
 * Autor: Marco Antônio Habitzreuter	(marco.habitzreuter@ufrgs.br)
 *					(mahabitzreuter@inf.ufrgs.br)
 *
 * Funções relacionadas ao controle do menu
 *
 */

#include <menu.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "highscores.h"

/**
 * Recebe um item do menu e executa a função correspondente a ele
 */
void execute_item(ITEM *item, FILE *highscore, score_record_st highscore_data[HIGHSCORE_COUNT])
{
	game_st game;

	switch(item_index(item)) {
	case 0: // Novo jogo
		game = game_new();
		update_ranking(highscore, game.player1, highscore_data);
		break;
	case 1: // Exibe ranking de pontuações
		show_ranking(highscore_data);
		break;
	}
}

void print_header(int starty)
{
	int lenght, startx;
	char title[5][100] = {{" ____        _        _ _             _   _                  _ "},
		{"| __ )  __ _| |_ __ _| | |__   __ _  | \\ | | __ ___   ____ _| |"},
		{"|  _ \\ / _` | __/ _` | | '_ \\ / _` | |  \\| |/ _` \\ \\ / / _` | |"},
		{"| |_) | (_| | || (_| | | | | | (_| | | |\\  | (_| |\\ V / (_| | |"},
		{"|____/ \\__,_|\\__\\__,_|_|_| |_|\\__,_| |_| \\_|\\__,_| \\_/ \\__,_|_|"}};

	lenght = strlen(title[0]);
	startx = (COLS - lenght) / 2;
	mvprintw(starty, startx, title[0]);
	mvprintw(starty + 1, startx, title[1]);
	mvprintw(starty + 2, startx, title[2]);
	mvprintw(starty + 3, startx, title[3]);
	mvprintw(starty + 4, startx, title[4]);
	refresh();
}

/**
 * Cria menu e faz a leitura da entrada do usuário
 */
void menu(FILE *highscore, score_record_st highscore_data[HIGHSCORE_COUNT])
{
	WINDOW *menu = newwin(9, 40, (LINES - 9) / 2, (COLS - 35) / 2);
	wrefresh(menu);
	ITEM **items, *cur;
	int menu_op_count, end_game = 0, c, index_exit, force_reload = 0;
	MENU *my_menu;
	char *menu_options[] = {
		"Novo jogo",
		"Exibir ranking de jogadores",
		"Sair",
	};

	print_header(((LINES - 9) / 2) - 6);

	menu_op_count = sizeof(menu_options) / sizeof(menu_options[0]);
	items = (ITEM **)calloc(menu_op_count + 1, sizeof(ITEM *));

	index_exit = menu_op_count - 1; // Sair é a última opção
	// Último item do menu indicado com NULL para marcar o final
	items[menu_op_count] = (ITEM *) NULL;
	// Seta os itens do menu
	for(int i = 0; i < menu_op_count; ++i)
		items[i] = new_item(menu_options[i], "");

	my_menu = new_menu((ITEM **) items);
	set_menu_win(my_menu, menu);
	set_menu_sub(my_menu, derwin(menu, 6, 38, 3, 1));
	set_menu_mark(my_menu, " * ");
	keypad(menu, TRUE);

	post_menu(my_menu);
	wrefresh(menu);

	while(!end_game){
		if(force_reload) {
			// Ao voltar de uma opção, exibição do menu deve ser
			// forçada
			clear();
			force_reload = 0;
			menu_driver(my_menu, REQ_DOWN_ITEM);
			menu_driver(my_menu, REQ_UP_ITEM);
			print_header(((LINES - 9) / 2) - 6);
		}
		c = wgetch(menu);
		switch(c){
		case KEY_DOWN:
			menu_driver(my_menu, REQ_DOWN_ITEM);
			break;
		case KEY_UP:
			menu_driver(my_menu, REQ_UP_ITEM);
			break;
		case 10: // Usuário selecionou opção pressionando enter
			force_reload = 1;
			cur = current_item(my_menu);
			end_game = (index_exit == item_index(cur));
			if(!end_game) execute_item(cur, highscore, highscore_data);
			break;
		}
	}

	for(int i = 0; i < menu_op_count; ++i) free_item(items[i]);
	unpost_menu(my_menu);
	free_menu(my_menu);
	delwin(menu);
}

