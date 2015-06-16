#include "baza.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dodaj_slowo (char *gdzie, char jakie[]);
void dodaj_sufiks ( char *gdzie, char *jakie );
void wyczysc_slowo (char *slowo, int dl);
void polacz_komorki(komorka_t a, komorka_t b);
void zastap_komorke(komorka_t a, komorka_t b);
int oczysc_komorke(komorka_t a, int N);
int czy_zawiera (char *co, char *wyraz);

void stworz_baze ( baza_t *nowa, int N)
{	
	int i;
	
	nowa->aktualnie = 0;
	nowa->rozmiar = 16;
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
		nowa->komorka[i].sufiks = (char*) malloc (128 * sizeof(char));

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
	wyczysc_slowo (slowo, 32);
	if (odczyt == NULL)
	{
		fprintf (stderr, "Nie udało się otworzyć pliku \"%s\"\n", plik);
		return;
	}
	

	znak = fgetc (odczyt);
	while (znak != EOF || koniec == 1)
	{
		if ( znak == '\t' )
			znak = ' ';
		if ( znak != ' ' && znak != EOF )
		{
			if (isalnum(znak) || znak == '.' || znak == ',' || znak == '!' || znak == '?' 
							  || znak == ':' || znak == '-' || znak == '(' || znak == ')')
				slowo[i++] = znak;
		}
		else
		{
			baza->aktualnie++;
			if (baza->aktualnie > baza->rozmiar)
				powieksz_baze(baza);
			
			i = 0;
			baza->komorka[nr_slowa].ile_razy = 1;
			baza->komorka[nr_slowa].stat = 0;
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
			wyczysc_slowo (slowo, 32);
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

void wyczysc_slowo (char *slowo, int dl)
{
	int i;
	for (i=0; i<dl; i++)
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
		baza->komorka[i].ile_razy = oczysc_komorke (baza->komorka[i], baza->N);	

	for (i = 0; i < baza->aktualnie; i++)
		if ( baza->komorka[i].prefiks[0] == '$')
		{
			baza->aktualnie--;
			j = baza->aktualnie;
			strcpy ( baza->komorka[i].prefiks, baza->komorka[j].prefiks );
			strcpy ( baza->komorka[i].sufiks, baza->komorka[j].sufiks );
			baza->komorka[i].ile_razy = baza->komorka[j].ile_razy;
		}
		
	return;
}


void polacz_komorki(komorka_t a, komorka_t b)
{
	if ( czy_zawiera (a.sufiks, b.sufiks) != 0 )
	{
		dodaj_sufiks (a.sufiks, b.sufiks);
		b.sufiks[0] = '$';
		b.sufiks[1] = '\0';
		b.prefiks[0] = '$';
		b.prefiks[1] = '\0';
	}
	return;
}


int czy_zawiera (char *co_org, char *wyraz_org)
{
	if ( co_org[0] == '$' || wyraz_org[0] == '$' )
		return 0;
		
	char *spacja, co[128], wyraz[128], *pom;
	int licznik = -1, i;
	strcpy(co, co_org);
	strcpy(wyraz, wyraz_org);
	
	if ( strcmp(co, wyraz) == 0 )
		return 0;
	
	spacja = strchr(co, ' ');
	while ( spacja != NULL )
	{
		licznik = spacja - co;
		
		pom = malloc ( 128 * sizeof(char));
		
		for (i = 0; i < licznik; i ++)
		{	
			pom[i] = co[i];
			co[i] = '$';
		}
		pom[i++] = '$';
		pom[i] = '\0';
		
		if ( strcmp(pom, wyraz) == 0 )
			return 0;
		
		licznik = 0;
		i = 0;
		while ( co[licznik] == '$' )
			licznik++;
		
		while ( co[i] != '\0' )
			co[i++] = co[licznik++];
		co[i] = '\0';
		
		free(pom);
		spacja = strchr(spacja+1, ' ');
	}
	return 1;
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
	gdzie[szukaj+1] = '\0';
	return;
}


int oczysc_komorke (komorka_t a, int N)
{
	int wyrazy = 1, i;
		for (i = 0; a.prefiks[i] != '$'; i++)
			if (a.prefiks[i] == ' ')
				wyrazy++;
	if (wyrazy != N-1)
	{
		a.sufiks[0] = '$';
		a.sufiks[1] = '\0';
		a.prefiks[0] = '$';
		a.prefiks[1] = '\0';
		a.ile_razy = 0;
	}
	else if ( !isalnum(a.sufiks[0]) )
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
		a.prefiks[i+1] = '\0';
	}

	wyrazy = 1;
	for (i = 0; a.sufiks[i] != '$'; i++)
		if (a.sufiks[i] == ' ')
			wyrazy++;
	if (i != 0)
		a.ile_razy = wyrazy;
	else wyrazy = 0;
	
	return wyrazy;
}


void dolacz_do_bazy ( baza_t *baza, char *plik)
{
	char znak, poprz, prefiks[128], sufiks[128];
	FILE *odczyt = fopen (plik, "r");
	int N = baza->N, i = 0, k, nr_slowa = 0, pom1 = 1, przelacznik = 0, koniec = 0;
	wyczysc_slowo (prefiks, 128);
	wyczysc_slowo (sufiks, 128);

	if (odczyt == NULL)
	{
		fprintf (stderr, "Nie udało się otworzyć pliku \"%s\"\n", plik);
		return;
	}

	znak = fgetc (odczyt);
	if ( znak == EOF || !isdigit(znak) || baza->N != znak-48 )
	{
		fprintf (stderr, "Baza w pliku \"%s\" ma zły format\n", plik);
		return;
	}
	
	znak = fgetc (odczyt);
	if (znak != ';')
	{
		fprintf (stderr, "Baza w pliku \"%s\" ma zły format\n", plik);
		return;
	}


	znak = fgetc (odczyt);
	while (znak != EOF || koniec == 1)
	{	
		if ( znak != ';' && znak != EOF )
		{
			if ( znak == '#' )
			{
				przelacznik = 1;
				i=0;
			}
			else if (przelacznik == 0)
				prefiks[i++] = znak;
			else sufiks[i++] = znak;
		}
		else
		{
			if ( isalnum(prefiks[0]) && isalnum(sufiks[0]) )
			{
				baza->aktualnie++;
				if (baza->aktualnie >= baza->rozmiar)
					powieksz_baze(baza);
				
				strcpy( baza->komorka[baza->aktualnie].prefiks, prefiks);
				strcpy( baza->komorka[baza->aktualnie].sufiks, sufiks);
			}
			wyczysc_slowo (prefiks, 128);
			wyczysc_slowo (sufiks, 128);
			przelacznik = 0;
			i = 0;
		}
		
		poprz = znak;

		if (koniec == 0)
			znak = fgetc (odczyt);
		if (znak == EOF) 
			koniec++;
		if (koniec > 1)
			break;
		if ( poprz == ';' && !isalnum(znak) )
			while (znak != ';' && znak != EOF)
				znak = fgetc (odczyt);
		if (znak == EOF) 
			koniec++;
		if (koniec > 1)
			break;
		}
	
	fclose (odczyt);powieksz_baze(baza);
	return;
}
