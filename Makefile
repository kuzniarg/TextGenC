all: gen clean

gen: main.o pliki.o flagi.o baza.o
	$(CC) -o gen main.o pliki.o flagi.o baza.o
clean:
	-rm *.o
