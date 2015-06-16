#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "baza.h"

void najNGram (baza_t b)
{
	int i = 0, najV = 0, najI = 0, najVV = 0, najII = 00;
	
	while (i < b.aktualnie)
	{
		if (b.komorka[i].ile_razy > najV)
		{
			najV = b.komorka[i].ile_razy;
			najI = i;
		}
		
		if (b.komorka[i].stat > najVV)
		{
			najVV = b.komorka[i].stat;
			najII = i;
		}
		i++;
	}
	printf("Najwięcej sufiksów ma prefiks:\n");
	printf("%s - %d\n", b.komorka[najI].prefiks, najV);
	printf("Najczęściej występujący prefiks:\n");
	printf("%s - %d\n", b.komorka[najII].prefiks, najVV);
	printf("-----------------------------------------------\n");

	return;
}
