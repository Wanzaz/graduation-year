/*
 * Projekt: PazourekOndrej-pr6-cviko01
 * Autor: Ondrej Pazourek
 * Datum: 17.9.2024
 */

#include <stdio.h>
#include <stdlib.h>
// #include <string.h>  // pro praci s textovymi retezci
// #include <stdbool.h> // pro praci s typem bool a konstantami true a false
// #include <ctype.h>   // isalpha, isspace, islower, isupper, ...
// #include <math.h>    // funkce z matematicke knihovny
// #include <float.h>   // konstanty pro racionalni typy DBL_MAX, DBL_DIG, ...
// #include <limits.h>  // konstanty pro celociselne typy INT_MAX, INT_MIN, ...
// #include <time.h>    // funkce time a dalsi pro praci s casem

#include "gvid.h"       // par drobnosti pro zjednoduseni prace
#include "mojematice.h"


void testCteni(char *cesta)
{
  printf("-----------------------------\n");
  printf("Test cteni ze souboru\n");
  printf("Ctu ze souboru %s\n", cesta);

  FILE *f = fopen(cesta, "r");
  if (f == NULL) {
    printf("Soubor nejde otevrit.\n");
    return;
  }

  Tmatice * m = nactiMatice(f);
  if (m == NULL) {
    printf("Matici nejde nacist\n");
    fclose(f);
    return;
  }

  printf("Nactena matice:\n");
  tiskMatice(stdout, m);
}

void testSoucet(char *cesta1, char *cesta2)
{
    printf("-----------------------------\n");
    printf("Test souctu matic ze souboru\n");
    printf("Ctu matice ze souboru %s a %s\n", cesta1, cesta2);

    FILE *f1 = fopen(cesta1, "r");
    if (f1 == NULL) {
        printf("Soubor %s nejde otevrit.\n", cesta1);
        return;
    }

    FILE *f2 = fopen(cesta2, "r");
    if (f2 == NULL) {
        printf("Soubor %s nejde otevrit.\n", cesta2);
        fclose(f1);
        return;
    }

    // Spočítání součtu matic pomocí funkce soucetMatic
    float soucet = soucetMatic(f1, f2);

    if (soucet == -1) {
        printf("Chyba při výpočtu součtu matic.\n");
    } else {
        printf("Součet všech prvků matic: %.2f\n", soucet);
    }

    fclose(f1);
    fclose(f2);
}

void testSoucin(char *cesta1, char *cesta2) {
    printf("-----------------------------\n");
    printf("Test soucinu matic ze souboru\n");
    printf("Ctu matice ze souboru %s a %s\n", cesta1, cesta2);

    FILE *f1 = fopen(cesta1, "r");
    if (f1 == NULL) {
        printf("Soubor %s nejde otevrit.\n", cesta1);
        return;
    }

    FILE *f2 = fopen(cesta2, "r");
    if (f2 == NULL) {
        printf("Soubor %s nejde otevrit.\n", cesta2);
        fclose(f1);
        return;
    }

    // Načtení matic ze souborů
    Tmatice *m1 = nactiMatice(f1);
    Tmatice *m2 = nactiMatice(f2);
    fclose(f1);
    fclose(f2);

    if (m1 == NULL || m2 == NULL) {
        printf("Chyba při načítání matic.\n");
        if (m1) uvolniMatici(m1);
        if (m2) uvolniMatici(m2);
        return;
    }

    // Spočítání součinu matic
    Tmatice *vyslednaMatice = soucinMatic(m1, m2);

    if (vyslednaMatice == NULL) {
        printf("Chyba při výpočtu součinu matic.\n");
    } else {
        printf("Výsledná matice:\n");
        tiskMatice(stdout, vyslednaMatice);
        uvolniMatici(vyslednaMatice);
    }

    uvolniMatici(m1);
    uvolniMatici(m2);
}


//int main(int argc, char *argv[])  // pro parametry prikazoveho radku
int main(void)
{
    char *cesta1 = "data1.txt";
    char *cesta2 = "data2.txt";
    testCteni(cesta1);
    testSoucet(cesta1, cesta2);
    testSoucin(cesta1, cesta2);
    return 0;
}
