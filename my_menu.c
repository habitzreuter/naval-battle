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

/**
 * Cria menu e faz a leitura da entrada do usuário
 */
void menu(FILE *highscore, score_record_st highscore_data[HIGHSCORE_COUNT])
{
	ITEM **items, *cur;
	int menu_op_count, end_game = 0, c, index_exit, force_reload = 0;
	MENU *my_menu;
	char *menu_options[] = {
		"Novo jogo",
		"Exibir ranking de jogadores",
		"Sair",
	};

	menu_op_count = sizeof(menu_options) / sizeof(menu_options[0]);
	items = (ITEM **)calloc(menu_op_count + 1, sizeof(ITEM *));

	index_exit = menu_op_count - 1; // Sair é a última opção
	// Último item do menu indicado com NULL para marcar o final
	items[menu_op_count] = (ITEM *) NULL;
	// Seta os itens do menu
	for(int i = 0; i < menu_op_count; ++i)
		items[i] = new_item(menu_options[i], "");
	
	my_menu = new_menu((ITEM **) items);
	post_menu(my_menu);
	box(stdscr, 0, 0);
	refresh();

	while(!end_game){
		if(force_reload) {
			// Ao voltar de uma opção, exibição do menu deve ser
			// forçada
			clear();
			force_reload = 0;
			menu_driver(my_menu, REQ_DOWN_ITEM);
			menu_driver(my_menu, REQ_UP_ITEM);
		}
		c = getch();
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
	free_menu(my_menu);
}

