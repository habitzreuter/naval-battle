#ifndef __HIGHSCORES_H__
#define __HIGHSCORES_H__

#define HIGHSCORE_COUNT 10
#define MAX_NAME_SIZE 31

typedef struct {
	char name[MAX_NAME_SIZE];
	uint16_t score;
} score_record_st;

void read_highscores(FILE*, score_record_st*);

#endif

