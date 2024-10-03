/*
 * Projekt:
 * Autor:
 * Datum:
 */

// #include "gvid.h"       // par drobnosti pro zjednoduseni prace
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>  // pro praci s textovymi retezci
// #include <stdbool.h> // pro praci s typem bool a konstantami true a false
// #include <ctype.h>   // isalpha, isspace, islower, isupper, ...
// #include <math.h>    // funkce z matematicke knihovny
// #include <float.h>   // konstanty pro racionalni typy DBL_MAX, DBL_DIG, ...
// #include <limits.h>  // konstanty pro celociselne typy INT_MAX, INT_MIN, ...
// #include <time.h>    // funkce time a dalsi pro praci s casem

void nasobilka(int pocet, int zaklad)
{
  for (int i = 1; i <= pocet; ++i) {
    printf("%dx%d = %d\n", zaklad, i, zaklad * i);
  }
}


int main(int argc, char *argv[])  // pro parametry prikazoveho radku
{

  if (argc < 3) {
    fprintf(stderr, "Nespravny pocet argumentu\n\n"
                    "Napoveda jak spusti program s argumenty:\n"
                    "PREDPIS: nasobilka pocet zaklad\n"
                    "napr. nasobilka 3 2\n\n");

    return -1;
  }

  char *endptr1, *endptr2;

  int pocet = strtol(argv[1], &endptr1, 10);
  int zaklad = strtol(argv[2], &endptr2, 10);

  if (endptr1[0] != '\0' || endptr2[0] != '\0') {
    fprintf(stderr, "Na vstupu byla neocekavana hodnota");
    return -1;
  }

  if (pocet <= 0 || zaklad <= 0) {
    fprintf(stderr, "Na vstupu bylo nalezeno zaporne cislo nebo nula");
    return -1;
  }

  nasobilka(pocet, zaklad);

  return 0;
}
