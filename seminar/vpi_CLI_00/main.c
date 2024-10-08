/*
 * Projekt:
 * Autor:
 * Datum:
 */

#include "gvid.h"       // par drobnosti pro zjednoduseni prace
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>  // pro praci s textovymi retezci
// #include <stdbool.h> // pro praci s typem bool a konstantami true a false
// #include <ctype.h>   // isalpha, isspace, islower, isupper, ...
// #include <math.h>    // funkce z matematicke knihovny
// #include <float.h>   // konstanty pro racionalni typy DBL_MAX, DBL_DIG, ...
// #include <limits.h>  // konstanty pro celociselne typy INT_MAX, INT_MIN, ...
// #include <time.h>    // funkce time a dalsi pro praci s casem

void printArguments(int count, char *arguments[])
{
  for (int i = 0; i < count; i++) {
    printf("%i -- %s\n", i, arguments[i]);
  }
}


//int main(int argc, char *argv[])  // pro parametry prikazoveho radku
int main(int argc, char *argv[])
{
  printArguments(argc, argv);
  return 0;
}
