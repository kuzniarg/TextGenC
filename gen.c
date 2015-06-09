#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "baza.h"

int jest_slowo (baza_t b, char *slowo)
{	
	int i=0;

	while ( strcmp(slowo, b.komorka[i].prefiks) != 0)
	{
		if ( i == b.aktualnie - 1 )
		{
			i++;
			break;
		}
		else i++;
	}
	
	if ( i == b.aktualnie )
		return 0;
	else return 1;
}


char *podaj_slowo (baza_t b, char *slowo)
{
	int i=0, j=0, k=0, ile_sufiksow = 1, losuj = 0, l = 0;
	char *wydruk = malloc ( 32 * sizeof (char));
	int los1, los2, los3;
	
	while ( strcmp(slowo, b.komorka[i].prefiks) != 0)
		i++;
		
	while ( b.komorka[i].prefiks[j] != ' ' )
		j++;
	j++;	
	
	while ( b.komorka[i].prefiks[j] != '$' )
		slowo[k++] = b.komorka[i].prefiks[j++];
	slowo[k++] = ' ';
	j=0;
	
	while ( b.komorka[i].sufiks[j] != '$' )
	{
		if ( b.komorka[i].sufiks[j] == ' ' )
			ile_sufiksow++;	
		j++;
	}
	
	losuj = rand()% ile_sufiksow;

	j = 0;
	while ( losuj < ile_sufiksow - 1 )
	{
		while ( b.komorka[i].sufiks[j] != ' ' )
			j++;
		j++;
		losuj++;
	}

	while ( b.komorka[i].sufiks[j] != '$' && b.komorka[i].sufiks[j] != ' ' )
	{
		slowo[k++] = b.komorka[i].sufiks[j];
		wydruk[l++] = b.komorka[i].sufiks[j];
		j++;
	}
	slowo[k++] = '$';
	slowo[k] = '\0';
	wydruk[l] = '\0';
	
	return wydruk;
}


char *nowy_poczatek (baza_t b, char *slowo)
{
	int i = rand()%b.aktualnie;
	char *wydruk = malloc ( 32 * sizeof (char));
	
	while (!isupper(b.komorka[i].prefiks[0]))
	{
		if (i > 1)
			i--;
		else i = b.aktualnie-1;
	}
	
	strcpy(slowo, b.komorka[i].prefiks);
	
	i=0;
	while (slowo[i] != '$')
	{
		wydruk[i] = slowo[i];
		i++;
	}
	wydruk[++i] = '\0';
	return wydruk;
}
