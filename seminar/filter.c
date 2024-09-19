/*
 * Projekt:
 * Autor:
 * Datum:
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


//int main(int argc, char *argv[])  // pro parametry prikazoveho radku
int main(void)
{
  int c;
  while ((c = getchar()) != EOF) {
     putchar(toupper(c));
  }

  fprintf(stderr, "Thole patri na chybovy vystup.\n");

  return 0;
}
