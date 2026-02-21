CC     := gcc
CFLAGS := -O0
LFLAGS := -omain -lncurses -lopenblas -lm 

test: main.o utils.o test.o
	$(CC) main.o test.o utils.o $(LFLAGS)

main.o test.o: main.c test.c
	$(CC) main.c test.c $(CFLAGS) -c

utils.o: utils.c
	$(CC) $^ $(CFLAGS) -c

clean:
	rm *.o -f
	rm *.out -f
