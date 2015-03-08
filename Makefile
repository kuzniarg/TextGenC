all: gen clean

gen: main.o pliki.o flagi.o
	$(CC) -o gen main.o pliki.o flagi.o
clean:
	-rm *.o
