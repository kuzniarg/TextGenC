all: gen clean

gen: main.o pliki.o
	$(CC) -o gen -Wall main.o pliki.o
clean:
	-rm *.o
