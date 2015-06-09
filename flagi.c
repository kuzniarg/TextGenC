#include "flagi.h"
#include <string.h>
#include <stdio.h>

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
	else if (strcmp (tekst, "-g") == 0)
		return 6;
	else if (strcmp (tekst, "-b") == 0)
		return 7;
	else return 0;
}


void blad_warunkow_flag (int flaga)
{
	char nazwa;
	switch (flaga)
	{
		case 1:
			nazwa = 'r';
			break;
		case 2:
			nazwa = 'w';
			break;
		case 3:
			nazwa = 'p';
			break;
		case 4:
			nazwa = 'n';
			break;
		case 5:
			nazwa = 's';
			break;
		case 6:
			nazwa = 'g';
			break;
		case 7:
			nazwa = 'b';
			break;
		default:
			nazwa = '?';
			break;
	}   
	fprintf (stderr, "Flaga -%c występuje wielokrotnie (program pobrał dane z najwcześniej podanej)\n", nazwa);
	return;
}


void blad_brak_argumentow ()
{
	fprintf (stderr,
	"Instrukcja używania programu TextGenC:\n"
	"   + Flagi:\n"
	"      -r \"nazwa_pliku\" odczyt tekstu z pliku, możliwość podania wielu\n"
	"      -w \"liczba_naturalna\" maksymalna długość tekstu (domyślnie 50)\n"
	"      -p \"liczba_naturalna\" maksymalna ilość akapitów (domyślnie 1)\n"
	"      -n \"liczba_naturalna\" długość N-gramu (domyślnie 3)\n"
	"      -s \"nazwa_pliku\" zapis wygenerowanego tekstu do pliku (domyślnie wydruk na ekran)\n"
	"      -g \"nazwa_pliku\" zapis bazy generowanej przez program do pliku\n"
	"      -b \"nazwa_pliku\" odczyt pliku z bazą wygenerowaną wcześniej przez program\n"
	"   + Każdą z flag powinno się użyć maksymalnie raz. Wszystkie kolejne użycia są ignorowane przez program.\n"
	"   + Program wymaga podania przynajmniej jednego pliku z danymi do odczytu (tekstu lub wcześniej wygenerowanej bazy).\n"
	"   + Wartości niezdefiniowane przez użytkownika przyjmują wartości domyślne (podane wraz z flagami)\n"
	"   + Długość N-gramu nie może przekraczać liczby wyrazów w pliku do odczytu\n");
	return;
}
