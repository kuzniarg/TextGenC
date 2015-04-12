#include "baza.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dodaj_slowo (char *gdzie, char jakie[]);
void dodaj_sufiks ( char *gdzie, char *jakie );
void wyczysc_slowo (char* slowo);
void polacz_komorki(komorka_t a, komorka_t b);
void zastap_komorke(komorka_t a, komorka_t b);
void oczysc_komorke(komorka_t a, int N);

void stworz_baze ( baza_t *nowa, int N)
{	
	int i;
	
	nowa->aktualnie = 0;
	nowa->rozmiar = 8;
	nowa->N = N;
	
	nowa->komorka = malloc (nowa->rozmiar * sizeof( komorka_t ));
	if (nowa->komorka == NULL)
	{
		fprintf (stderr, "Nie można zarezerwować pamięci\n");
		nowa->N = 0;
		return;
	}

	for (i=0; i < nowa->rozmiar; i++)
	{
		nowa->komorka[i].prefiks = (char*) malloc (128 * sizeof(char));
		nowa->komorka[i].sufiks = (char*) malloc (64 * sizeof(char));

		if (nowa->komorka[i].prefiks == NULL || nowa->komorka[i].sufiks == NULL)
		{
			fprintf (stderr, "Nie można zarezerwować pamięci\n");
			nowa->N = 0;
			return;
		}	
	nowa->komorka[i].prefiks[0] = '$';
	nowa->komorka[i].sufiks[0] = '$';	
	}	
	
	return;
}


void dodaj_do_bazy ( baza_t *baza, char *plik)
{
	char znak, slowo[32];
	FILE *odczyt = fopen (plik, "r");
	int N = baza->N, i = 0, j = baza->aktualnie, k, nr_slowa = 0, pom1 = 1, czy_pierwsze = 0, koniec = 0;
	wyczysc_slowo (slowo);
	if (odczyt == NULL)
	{
		fprintf (stderr, "Nie udało się otworzyć pliku \"%s\"\n", plik);
		return;
	}
	

	znak = fgetc (odczyt);
	while (znak != EOF || koniec == 1)
	{
		if ( znak != ' ' && znak != EOF )
		{
			slowo[i++] = znak;
		}
		else
		{
			baza->aktualnie++;
			if (baza->aktualnie > baza->rozmiar)
				powieksz_baze(baza);
			
			i = 0;
			baza->komorka[nr_slowa].ile_razy = 1;
			nr_slowa++;
			
			if ( nr_slowa % N != 0 && czy_pierwsze == 0 )
			{
				while (pom1 <= nr_slowa)
				{	
					dodaj_slowo ( baza->komorka[j + pom1 - 1].prefiks, slowo );
					pom1++;
				}
				pom1=1;
			}
			else 
			{
				czy_pierwsze = 1;

				for ( k = 1; k < N; k++)
					dodaj_slowo ( baza->komorka[j+k].prefiks, slowo);
				
				dodaj_slowo (baza->komorka[j].sufiks, slowo);
				j++;
			}
			wyczysc_slowo (slowo);
		}
		if (koniec == 0)
			znak = fgetc (odczyt);
		if (znak == EOF) 
			koniec++;
		if (koniec > 1)
			break;
	}
	
	fclose (odczyt);
	return;
}

void zwolnij_baze ( baza_t *baza)
{
	int i;
	for (i = 0; i < baza->rozmiar; i++)
	{
		free(baza->komorka[i].sufiks);
		free(baza->komorka[i].prefiks);
	}
	free(baza->komorka);
	return;
}

void powieksz_baze ( baza_t *baza )
{
	int i;
	baza->rozmiar *= 2;
	baza->komorka = realloc (baza->komorka, baza->rozmiar * sizeof( komorka_t ));
	for ( i = baza->rozmiar/2; i < baza->rozmiar; i++)
	{
		baza->komorka[i].prefiks = (char*) malloc (128 * sizeof(char));
		baza->komorka[i].sufiks = (char*) malloc (64 * sizeof(char));

		if (baza->komorka[i].prefiks == NULL || baza->komorka[i].sufiks == NULL)
		{
			fprintf (stderr, "Nie można zarezerwować pamięci\n");
			baza->N = 0;
			return;
		}	
		baza->komorka[i].prefiks[0] = '$';
		baza->komorka[i].sufiks[0] = '$';	
	}	
}

void dodaj_slowo ( char *gdzie, char jakie[] )
{
	int szukaj = 0, pom=0;
	while (gdzie[szukaj] != '$')
		szukaj++;
	
	if (szukaj != 0)	
	{	
		gdzie[szukaj] = ' ';
		szukaj++;
	}
	
	while (jakie[pom] != '\0')
	{
		gdzie[szukaj] = jakie[pom];
		pom++;
		szukaj++;
	}
	gdzie[szukaj] = '$';
	gdzie[szukaj+1] = '\0';
	
	return;
}

void wyczysc_slowo (char* slowo)
{
	int i;
	for (i=0; i<32; i++)
		slowo[i] = '\0';
	return;
}


void porzadkuj_baze (baza_t *baza)
{
	int i, j;
	
	for (i = 0; i < baza->aktualnie; i++)
		for (j = i+1; j < baza->aktualnie; j++)
			if ( strcmp(baza->komorka[i].prefiks, baza->komorka[j].prefiks) == 0)
				polacz_komorki(baza->komorka[i], baza->komorka[j]);
	
	for (i = 0; i < baza->aktualnie; i++)
		oczysc_komorke (baza->komorka[i], baza->N);

	for (i = 0; i < baza->aktualnie; i++)
		if ( baza->komorka[i].prefiks[0] == '$')
			zastap_komorke(baza->komorka[i], baza->komorka[--baza->aktualnie]);

	return;
}


void polacz_komorki(komorka_t a, komorka_t b)
{
	if ( strcmp (a.sufiks, b.sufiks) != 0)
	{
		dodaj_sufiks (a.sufiks, b.sufiks);
		b.sufiks[0] = '$';
		b.sufiks[1] = '\0';
		b.prefiks[0] = '$';
		b.prefiks[1] = '\0';
		a.ile_razy+= b.ile_razy;
		b.ile_razy = 0;
	}
	return;
}


void zastap_komorke(komorka_t a, komorka_t b)
{
	strcpy ( a.prefiks, b.prefiks );
	strcpy ( a.sufiks, b.sufiks );
	a.ile_razy = b.ile_razy;
	return;
}


void dodaj_sufiks ( char *gdzie, char *jakie )
{
	int szukaj = 0, pom=0;
	while (gdzie[szukaj] != '$')
		szukaj++;

	if (szukaj != 0)	
	{	
		gdzie[szukaj] = ' ';
		szukaj++;
	}

	while (jakie[pom] != '$')
	{
		gdzie[szukaj] = jakie[pom];
		pom++;
		szukaj++;
	}
	gdzie[szukaj] = '$';
	return;
}


void oczysc_komorke (komorka_t a, int N)
{
	int wyrazy = 1, i;
		for (i = 0; a.prefiks[i] != '$'; i++)
			if (a.prefiks[i] == ' ')
				wyrazy++;
	if (wyrazy < N-1)
	{
		a.sufiks[0] = '$';
		a.sufiks[1] = '\0';
		a.prefiks[0] = '$';
		a.prefiks[1] = '\0';
		a.ile_razy = 0;
	}
	else
	{
		for (i = 0; a.prefiks[i] != '$'; i++);
		a.prefiks[i] = '\0';
	}
	return;
}


void dolacz_do_bazy ( baza_t *baza, char *plik)
{
	return;
}
