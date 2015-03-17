#include "baza.h"
#include <stdio.h>
#include <stdlib.h>


void stworz_baze ( baza_t *nowa, int N)
{	
	int i;
	
	nowa->komorka = malloc (256 * sizeof(*(nowa->komorka)));
	if (nowa->komorka == NULL)
	{
		fprintf (stderr, "Nie można zarezerwować pamięci\n");
		nowa->N = 0;
		return;
	}

	for (i=0; i<256; i++)
	{
		nowa->komorka[i].slowo = malloc (64 * sizeof(char*));
		nowa->komorka[i].nast = malloc (128 * sizeof(char*));

		if (nowa->komorka[i].slowo == NULL || nowa->komorka[i].nast == NULL)
		{
			fprintf (stderr, "Nie można zarezerwować pamięci\n");
			nowa->N = 0;
			return;
		}
	}	

	nowa->aktualnie = 0;
	nowa->rozmiar = 256;
	nowa->N = N;
	
	return;
}


void dodaj_do_bazy ( baza_t *baza, char *plik)
{
/*	char *sufiks, *prefiks;
	FILE *odczyt = fopen (plik, "r");

	if (odczyt == NULL)
	{
		fprintf (stderr, "Nie udało się otworzyć pliku \"%s\"\n", plik);
		return;
	}

	while ( prefiks != EOF
	
	fclose (odczyt);
*/	return;
}

void zwolnij_baze ( baza_t *baza)
{
	int i;
	for (i = 0; i < baza->rozmiar; i++)
	{
		
		free(baza->komorka[i].nast);
		free(baza->komorka[i].slowo);
	}
	
	free(baza->komorka);
	return;
}

void powieksz_baze ( baza_t *baza )
{
	baza->rozmiar *= 2;
/*	baza->komorka = realloc (baza->komorka, baza->rozmiar * sizeof(nowa->komorka));
	baza->komorka->slowo = malloc (baza->komorka->slowo, 64 * sizeof(baza->komorka->slowo));
	baza->komorka->nast = malloc (baza->komorka->nast, 128 * sizeof(baza->komorka->nast));
*/	return;
}
