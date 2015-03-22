all: gen clean

gen: main.o pliki.o flagi.o baza.o gen.o
	$(CC) -o gen main.o pliki.o flagi.o baza.o gen.o
clean:
	-rm *.o
