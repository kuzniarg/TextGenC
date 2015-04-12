#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pliki.h"
#include "flagi.h"
#include "baza.h"
#include "gen.h"

int main (int argc, char *argv[])
{
	int n_flag = 1, tryb_flag=0;
	char *zapisz, *generuj;
	char *wczytaj, *baza_pliki;
	int limit_slow = 50, limit_akapitow = 1, N_gram = 2;
	int warunki[7] = { 0, 0, 0, 0, 0, 0, 0 };

	char *obecny_plik = malloc (128 * sizeof(char));

	baza_t baza;

	wczytaj = malloc (1024 * sizeof(char));
	wczytaj[0] = '!';
	baza_pliki = malloc (1024 * sizeof(char)); 
	baza_pliki[0] = '!';

	if (argc < 3)
		{
		blad_brak_argumentow();
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

				blad_warunkow_flag (tryb_flag);
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
					if ( jaki_tryb_flag ( argv[n_flag+1] ) == 0)
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
						if ( n_flag + 1 < argc )
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
					fprintf (stderr, "Flaga -p otrzymała złą wartość. Argument %s zostanie zignorowany\n", argv[n_flag]);
					if ( n_flag + 1 < argc )
						if ( jaki_tryb_flag ( argv[n_flag+1] ) == 0)
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
					fprintf (stderr, "Flaga -n otrzymała złą wartość. Argument %s zostanie zignorowany\n", argv[n_flag]);
					if ( n_flag + 1 < argc )
						if ( jaki_tryb_flag ( argv[n_flag+1] ) == 0)
							tryb_flag = 4;
				}
			}
			else if (tryb_flag == 5)
			{
				zapisz = argv[n_flag];
				warunki[4] = 1;
				tryb_flag = 0;
			}
			else if (tryb_flag == 6)
			{
				generuj = argv[n_flag];
				warunki[5] = 1;
				tryb_flag = 0;
			}
			else if (tryb_flag == 7)
			{
				dodaj_plik (baza_pliki, argv[n_flag]);
				warunki[6]++;

				if ( n_flag + 1 < argc )
						if ( jaki_tryb_flag ( argv[n_flag+1] ) == 0)
						tryb_flag = 7;
			}
		}	
		
		else
		{
			tryb_flag = 0;
			n_flag--;
		}

		n_flag++;
	}

//////////////      SPRAWDZENIE OBECNOŚCI NIEZBĘDNYCH DANYCH      ////////////////
	
	if ( (warunki[0] == 0) && (warunki[6] == 0))
	{
		fprintf( stderr, "Program otrzymał za mało danych - nie ma z czego wygenerować tekstu\n");
		return 1;
	}

//////////////      INICJOWANIE BAZY      ////////////////
	
	stworz_baze (&baza, N_gram);
	if (baza.N == 0)
	{
		fprintf (stderr,"Baza nie została zainicjowana");
		return 1;
	}

//////////////      GENEROWANIE BAZY      ////////////////

	while ( warunki[0] != 0 )
	{
		pobierz_nazwe_pliku (obecny_plik, wczytaj);

		if (obecny_plik != NULL)
		{
			dodaj_do_bazy (&baza, obecny_plik);
			warunki[0]--;
		}
		porzadkuj_baze (&baza);
	}

	while ( warunki[6] != 0 )
	{
		pobierz_nazwe_pliku (obecny_plik, wczytaj);

		if (obecny_plik != NULL)
		{
			dolacz_do_bazy (&baza, obecny_plik);
			warunki[6]--;
		}
		porzadkuj_baze (&baza);
	}
	
//////////////      ZAPIS BAZY DO PLIKU      //////////////

//////////////      GENEROWANIE I WYDRUK TEKSTU      //////////////

	int l_slow = N_gram - 1;
	int l_akapitow = 1;
	
	FILE *druk = NULL;
	if ( warunki[4] == 1 )
		druk = fopen (zapisz, "w");
	else
		druk = stdout;


	fprintf (druk, "%s ", baza.komorka[0].prefiks);

	while (l_slow < limit_slow)
	{
		//Znajdź kolejne słowo
		//Wydrukuj je
		fprintf (druk, "%s ", baza.komorka[l_slow].sufiks);
		l_slow++;
	}

	fprintf (druk, ".");


	/*
	Wybierz słowo początkowe
	Dokładaj losowo słowa
	Jeżeli natrafisz na słowo końcowe to zacznij od początkowego
	Jeżeli musisz kończyć to urwij
	*/

//////////////      ZWALNIANIE DANYCH      //////////////

	//zwolnij_baze(&baza);
	//free(wczytaj);
	//free(obecny_plik);
	//free(baza_pliki);
	return 0;
}
