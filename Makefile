#
# Batalha Naval
#
# Autor: Marco Antônio Habitzreuter 	(mahabitzreuter@inf.ufrgs.br)
# 					(marco.habitzreuter@ufrgs.br)
#

COMPILER_FLAGS = -lpanel -lmenu -lncurses
WARNING_FLAGS = -Wall -Wextra
SRC_FILES=$(wildcard *.c)

BIN_NAME=naval_battle.out

all:
	@cppcheck -q ${SRC_FILES}
	@gcc ${COMPILER_FLAGS} -std=c99 ${WARNING_FLAGS} ${SRC_FILES} -o ${BIN_NAME}

run: all
	./${BIN_NAME}

clean:
	rm ${BIN_NAME} *.o

