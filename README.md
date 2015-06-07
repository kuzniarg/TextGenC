# TextGenC
Instrukcja używania programu TextGenC:
 + Flagi:
	-r "nazwa_pliku" odczyt tekstu z pliku, możliwość podania wielu
	-w "liczba_naturalna" maksymalna długość tekstu (domyślnie 50)
	-p "liczba_naturalna" maksymalna ilość akapitów (domyślnie 1)
	-n "liczba_naturalna" długość N-gramu (domyślnie 2)
	-s "nazwa_pliku" zapis wygenerowanego tekstu do pliku (domyślnie wydruk na ekran)
	-g "nazwa_pliku" zapis bazy generowanej przez program do pliku
	-b "nazwa_pliku" odczyt pliku z bazą wygenerowaną wcześniej przez program
 + Każdą z flag powinno się użyć maksymalnie raz. Wszystkie kolejne użycia są ignorowane przez program.
 + Program wymaga podania przynajmniej jednego pliku z danymi do odczytu (tekstu lub wcześniej wygenerowanej bazy).
 + Wartości niezdefiniowane przez użytkownika przyjmują wartości domyślne (podane wraz z flagami)
 + Długość N-gramu nie może przekraczać liczby wyrazów w pliku do odczytu
