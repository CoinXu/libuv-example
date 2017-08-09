vpath %.c ./src

entry := ./test/entry.out#
main: ./src/main.c
	gcc ./src/main.c -o $(entry) -luv
