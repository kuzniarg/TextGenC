#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "pliki.h"
#include "flagi.h"
#include "baza.h"
#include "gen.h"

int main (int argc, char *argv[])
{
	int n_flag = 1, tryb_flag=0;
	char *zapisz, *generuj, *wczytaj, *baza_pliki;
	int limit_slow = 50, limit_akapitow = 1, N_gram = 2;
	int warunki[7] = { 0, 0, 0, 0, 0, 0, 0 };

	char *obecny_plik = malloc (128 * sizeof(char));

	baza_t baza;
	
	srand (time(NULL));

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

//////////////      SPRAWDZENIE OBECNOŚCI NIEZBĘDNYCH DANYCH 1     ////////////////
	
	if ( (warunki[0] == 0) && (warunki[6] == 0))
	{
		fprintf( stderr, "Program otrzymał za mało danych - nie ma z czego wygenerować tekstu\n");
		return 1;
	}

//////////////      INICJOWANIE BAZY      ////////////////
	
	stworz_baze (&baza, N_gram);
	if (baza.N == 0)
	{
		fprintf (stderr,"Baza nie została zainicjowana\n");
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
		pobierz_nazwe_pliku (obecny_plik, baza_pliki);

		if (obecny_plik != NULL)
		{
			dolacz_do_bazy (&baza, obecny_plik);
			warunki[6]--;
		}
		porzadkuj_baze (&baza);
	}

//////////////      SPRAWDZENIE OBECNOŚCI NIEZBĘDNYCH DANYCH 2    ////////////////

	if ( baza.aktualnie < 10 )
	{
		fprintf (stderr,"Baza ma mniej niż 10 pól - nie ma sensu generować tekstu\n");
		return 1;
	}
	
//////////////      ZAPIS BAZY DO PLIKU      //////////////

	FILE *zapis = NULL;
	if ( warunki[5] == 1 )
		zapis = fopen (generuj, "w");
	else
		zapis = stdout;
	
	if ( zapis != NULL )
	{
		fprintf( zapis, "%d;", baza.N );
		
		int gen_pom = 1;
		int gen_pom_suf = 0;
		while ( gen_pom < baza.aktualnie )
		{
			fprintf( zapis, "%s#%s;", baza.komorka[gen_pom].prefiks, baza.komorka[gen_pom].sufiks);
			gen_pom++;
		}
	}

//////////////      GENEROWANIE I WYDRUK TEKSTU      //////////////

	int l_slow = N_gram - 1;
	int i = 0;
	char c = ' ';
	char *slowo = malloc (128 * sizeof(char));
	
	FILE *druk = NULL;
	if ( warunki[4] == 1 )
		druk = fopen (zapisz, "w");
	else
		druk = stdout;

	strcpy(slowo, baza.komorka[1].prefiks);
	while ( slowo[i] != '$' )
		i++;
	slowo[i] = '\0';
	
	fprintf (druk, "\t%s", slowo);
	
	slowo[i] = '$';

	while (l_slow < limit_slow)
	{
		if ( jest_slowo(baza, slowo) == 1 )
			fprintf (druk, " %s", podaj_slowo(baza, slowo));
		else
		{
			i = 0;
			while (slowo[i] != '$' )
			{
				if (slowo[i] == '.')
					c = '.';
				if (slowo[i] == ' ')
					c = ' ';
				i++;
			}
			
			if ( c == ' ' )
				fprintf (druk, ".");
				
			if (limit_akapitow > 1)
			{
				fprintf (druk, "\n\t");
				limit_akapitow--;
			}
			else fprintf (druk, " ");
			fprintf (druk, "%s", nowy_poczatek(baza, slowo));		
		}	
		l_slow++;
	}

	i = 0;
	while (slowo[i] != '$' )
	{
		if (slowo[i] == '.')
			c = '.';
		if (slowo[i] == ' ')
			c = ' ';
		i++;
	}
	
	if ( c == ' ' )
		fprintf (druk, ".\n");

//////////////      ZWALNIANIE DANYCH      //////////////

	zwolnij_baze(&baza);
	free(wczytaj);
	free(obecny_plik);
	free(baza_pliki);
	return 0;
}
