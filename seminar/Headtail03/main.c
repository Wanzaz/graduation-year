/*
 * Projekt:
 * Autor:
 * Datum:
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // pro praci s textovymi retezci
// #include <stdbool.h> // pro praci s typem bool a konstantami true a false
// #include <ctype.h>   // isalpha, isspace, islower, isupper, ...
// #include <math.h>    // funkce z matematicke knihovny
// #include <float.h>   // konstanty pro racionalni typy DBL_MAX, DBL_DIG, ...
// #include <limits.h>  // konstanty pro celociselne typy INT_MAX, INT_MIN, ...
// #include <time.h>    // funkce time a dalsi pro praci s casem
#include <stdbool.h>

// Vytvoř program headtail, který podle zadaných parametrů vypíše zadaný počet prvních nebo posledních řádků standardního vstupu.
                             //
                             // headtail.exe [-h] (head | tail) pocet
                             // -h -- nepovinný parametr - vytiskne nápovědu
                             // head nebo tail -- volitelný (jeden nebo druhý) parametr
                             // pocet -- počet řádků, které se budou tisknout

        // Příklady:
          // Vypíše maximálně 5 prvních řádků vstupu: headtail.exe head 5 < vstupnidata.txt
          // Vypíše maximálně posledních 10 řádků vstupu: headtail.exe tail 10 < vstupnidata.txt


void head(FILE * f, int pocet)
{
    int c;
    while ((c = fgetc(f)) != EOF && pocet > 0) {
        putchar(c);
        if (c == '\n') {
            pocet--;
        }
    }
}


int main(int argc, char *argv[])  // pro parametry prikazoveho radku
{
    if (argc < 2) {
        fprintf(stderr, "Moc malo parametru, pokud nevis, jak ovladat program zadej parametr -h pro pomoc\n");
    }

    if (strcmp(argv[1], "-h") == 0) {
        fprintf(stderr, "Pouzit argument -h\n\n"
                        "Napoveda jak spusti program:\n\n"
                        "   PREDPIS: headtail.exe [-h] (head | tail) pocet < soubor\n"
                        "   -h -- nepovinný parametr - vytiskne nápovědu\n"
                        "   head nebo tail -- volitelný (jeden nebo druhý) parametr\n"
                        "   pocet -- počet řádků, které se budou tisknout\n"
                        "   napr. headtail.txt head 10 < vstupnidata.txt\n\n");
        return 0;
    }

    if (argc  != 5) {
        fprintf(stderr, "Spatny pocet parametru, pokud nevis, jak ovladat program zadej parametr -h pro pomoc\n");
        return 1;
    }

    // otevirani testovaciho souboru
    // FILE * f = fopen("vstupnisoubor.txt", "r");
    FILE * f = fopen(argv[4], "r");
    if (f == NULL) {
        printf("Chyba při otevírání souboru.\n");
        return 1;
    }
    head(f, 10);

    // pomoci -h pamateru - zapamatovat, kde lezi cislo....

    int c;
    while ((c = getchar()) != EOF) {
        putchar(c);
    }

    fclose(f);

    return 0;
}
