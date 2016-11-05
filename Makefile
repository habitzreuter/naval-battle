#
# Naval Battle game
#
# Author: Marco Antônio Habitzreuter (mahabitzreuter@inf.ufrgs.br)
# 				     (marco.habitzreuter@ufrgs.br)
#

WARNING_FLAGS = -Wall -Wextra
SRC_FILES=$(wildcard *.c)

BIN_NAME=naval_battle.out

all:
	@gcc -std=c99 ${WARNING_FLAGS} ${SRC_FILES} -o ${BIN_NAME}

run:
	./${BIN_NAME}

clean:
	rm ${BIN_NAME} *.o
