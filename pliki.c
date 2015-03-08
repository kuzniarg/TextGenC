#include "pliki.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void dodaj_plik (char *plik, char *element)
{
	char c = plik[0];
	int i = 0, j = 0;

	while ( c != '!' )
	{
		c = plik[++i];
	}
	
	if ( i != 0)
		plik[i++] = ';';
	
	c = element[j];

	while ( c != '\0' )
	{
		plik[i++] = c;
		c = element[++j];
	}
	plik[i] = '!';

	return;
}


char *pobierz_nazwe_pliku (char *skad)
{
	int i = 0, j = 0;
	char *nazwa = malloc ( 128 * sizeof(char));

	while ( skad [i]  == '!')
		i++;

	while ( skad [i] != ';' && skad [i] != '!')
	{
		nazwa[j++] = skad[i];
		skad[i++] = '!';
	}
	skad [i] = '!';

	return nazwa;
}
