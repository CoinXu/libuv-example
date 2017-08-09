vpath %.c ./src

entry := entry.out#

main: ./src/main.c
	gcc ./src/main.c -o $(entry) -luv
