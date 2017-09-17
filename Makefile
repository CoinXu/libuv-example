entry := ./test/entry.o#

main: ./src/file-system/main.c
	gcc ./src/file-system/main.c -o $(entry) -luv
