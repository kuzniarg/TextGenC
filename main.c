#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pliki.h"

int jaki_tryb_flag (char *tekst)
{
	if (strcmp (tekst, "-r") == 0)
		return 1;
	else if (strcmp (tekst, "-w") == 0)
		return 2;
	else if (strcmp (tekst, "-p") == 0)
		return 3;
	else if (strcmp (tekst, "-n") == 0)
		return 4;
	else if (strcmp (tekst, "-s") == 0)
		return 5;
	else return 0;
}

void blad_warunkow_flag (int flaga)
{
	char nazwa;
	switch (flaga)
	{
		case 1:
			nazwa = 'r';
		case 2:
			nazwa = 'w';
		case 3:
			nazwa = 'p';
		case 4:
			nazwa = 'n';
		case 5:
			nazwa = 's';
	}
	fprintf (stderr, "Flaga -%c występuje wielokrotnie (program pobrał dane z najwcześniej podanej)\n", nazwa);
	return;
}


int main (int argc, char *argv[])
{
	int n_flag=1, tryb_flag=0;
	char *wczytaj=NULL, *zapisz=NULL;
	int limit_slow = 0, limit_akapitow = 0, N_gram = 2;
	int warunki[5] = { 0, 0, 0, 0, 0 };

	if (argc == 1)
		{
		fprintf (stderr, "Brak argumentów\n");
		return 1;
		}
	
	if (argc == 2)
		{
		fprintf (stderr, "Za mało argumentów\n");
		return 1;
		}
	
//////////////      FLAGI      /////////////// 
	while (n_flag < argc)
	{
		if (tryb_flag == 0)
		{	tryb_flag = jaki_tryb_flag (argv[n_flag]);
		
			if (tryb_flag == 0)
				fprintf (stderr, "Podano argument bez flagi, pominięto %s \n", argv[n_flag] );
				
			else if (warunki [tryb_flag-1] > 0)
				{
				
				if (warunki [tryb_flag-1] == 1)
					blad_warunkow_flag (tryb_flag);
				warunki [tryb_flag-1]++;
				tryb_flag = 0;

				n_flag++;
				if ( n_flag != argc )
					while (jaki_tryb_flag (argv[n_flag]) == 0)
						{
						n_flag++;
						if ( n_flag == argc ) break;
						}
				}
		}
		else if (jaki_tryb_flag (argv[n_flag]) == 0)
		{
			if (tryb_flag == 1)
			{			
				dodaj_plik (wczytaj, argv[n_flag]);
				warunki[0]++;
				
				if ( n_flag + 1 < argc )
				if ( jaki_tryb_flag ( argv[n_flag+1] ) == 0);
				tryb_flag = 1;
			}
			
			else if (tryb_flag == 2)
			{
					limit_slow = atoi (argv[n_flag]);
					if (limit_slow > 0) 
					{
						tryb_flag = 0;
						warunki[1] = 1;
					}
					else 
					{
						fprintf (stderr, "Flaga -w otrzymała złą wartość. Argument %s zostanie zignorowany\n", argv[n_flag]);
						if ( jaki_tryb_flag ( argv[n_flag+1] ) == 0) 
							tryb_flag = 2;
					}
			}
			else if (tryb_flag == 3)
				{
					limit_akapitow = atoi (argv[n_flag]);
					if (limit_akapitow > 0) 
					{
						warunki[2] = 1;
						tryb_flag = 0;
					}
					else
					{
						fprintf (stderr, "Flaga -p otrzymała złą wartość. Zostanie ona zignorowana\n");
						if ( jaki_tryb_flag ( argv[n_flag] ) == 0)
							tryb_flag = 3;
					}
				}
			else if (tryb_flag == 4)
				{
					N_gram = atoi (argv[n_flag]);
					if (N_gram > 0)
					{
						tryb_flag = 0;	
						warunki[3] = 1;
					}
					else
					{
						fprintf (stderr, "Flaga -n otrzymała złą wartość. Zostanie ona zignorowana\n");
						if ( jaki_tryb_flag ( argv[n_flag] ) == 0)
							tryb_flag = 0;
					}
				}
			else if (tryb_flag == 5)
				{
					dodaj_plik (zapisz, argv[n_flag]);
					warunki[4] = 1;
					tryb_flag = 0;
				}
		}	
		
		else
		{
			tryb_flag = 0;
			n_flag--;
		}

		n_flag++;
	}


//////////////      GENEROWANIE BAZY      ////////////////
	return 0;
}
