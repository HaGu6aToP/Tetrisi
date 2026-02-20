CC     := gcc
CFLAGS := -O0
LFLAGS := -omain -lncurses -lopenblas -lm 

main: main.o utils.o
	$(CC) main.o utils.o $(LFLAGS)

main.o: main.c
	$(CC) main.c $(CFLAGS) -c

utils.o: utils.c
	$(CC) $^ $(CFLAGS) -c

clean:
	rm *.o -f
	rm *.out -f
