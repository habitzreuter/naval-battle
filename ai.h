/**
 * Batalha Naval
 *
 * Copyright (C) 2017 Marco Antônio Habitzreuter (marco.habitzreuter@ufrgs.br)
 *
 * Para detalhes sobre a licença, ver "main.c" e "LICENSE"
 *
 */

#ifndef __AI_H__
#define __AI_H__

void ai_generate_coords(uint8_t, uint8_t*, uint8_t*);
void ai_generate_ship_coords(uint8_t, ship_st *ship);

#endif
