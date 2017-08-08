objects = main.o

entry: $(objects)
	cc -o entry $(objects)

main.o: ./src/main.c
	cc -c ./src/main.c