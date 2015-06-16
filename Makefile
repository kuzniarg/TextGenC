all: gen clean

gen: main.o pliki.o flagi.o baza.o gen.o stat.o
	$(CC) -o gen main.o pliki.o flagi.o baza.o gen.o stat.o
clean:
	-rm *.o
