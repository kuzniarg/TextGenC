#ifndef BAZA_H
#define BAZA_H

typedef struct komorka_pom
{
	char *prefiks;
	char *sufiks;
	int ile_razy;

} komorka_t;

typedef struct bazunia
{
	komorka_t *komorka;
	int N;
	int aktualnie;
	int rozmiar;
	int pom_los;
} baza_t;

void stworz_baze (baza_t *baza, int N);
void dodaj_do_bazy (baza_t *baza, char *plik);
void dolacz_do_bazy (baza_t *baza, char *plik);
void zwolnij_baze (baza_t *baza);
void powieksz_baze (baza_t *baza);
void porzadkuj_baze (baza_t *baza);
#endif
