CC     := gcc
CFLAGS := -O0
LFLAGS := -omain -lncurses

main: main.o
	$(CC) main.o $(LFLAGS)

main.o: main.c
	$(CC) main.c $(CFLAGS) -c

clean:
	rm *.o -f
	rm *.out -f
