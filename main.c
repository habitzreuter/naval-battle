#include <stdint.h>
#include <stdio.h>
#include <locale.h>
#include "game.h"

int main()
{
	setlocale(LC_ALL,"portuguese"); // Configura caracteres em português

	game_new();

	return 0;
}

