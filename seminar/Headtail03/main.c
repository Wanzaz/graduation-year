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

// readline - alokovane pole
// buffer - ukazatel radky, cyklicke pole


#define CHUNK_SIZE 64

char *readLine(FILE *f) {
    size_t bufferSize = CHUNK_SIZE;
    size_t length = 0;
    char *buffer = malloc(bufferSize);

    if (buffer == NULL) {
        fprintf(stderr, "Alokacni error\n");
        return NULL;
    }

    int c;
    while ((c = fgetc(f)) != EOF) {
        buffer[length++] = c;

        if (c == '\n') {
            break;
        }

        if (length == bufferSize) {
            bufferSize += CHUNK_SIZE;
            char *newBuffer = realloc(buffer, bufferSize);
            if (newBuffer == NULL) {
                free(buffer);
                fprintf(stderr, "Realokacni error\n");
                return NULL;
            }
            buffer = newBuffer;
        }
    }

    if (length == 0 && c == EOF) {
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0';
    return buffer;
}

void tail(FILE *f, int pocet) {
    char **lines = malloc(pocet * sizeof(char *));
    if (!lines) {
        fprintf(stderr, "Alokacni error\n");
        return;
    }

    for (int i = 0; i < pocet; i++) {
        lines[i] = NULL;
    }

    int lineIndex = 0;
    char *line;
    while ((line = readLine(f)) != NULL) {
        if (lines[lineIndex]) {
            free(lines[lineIndex]);
        }
        lines[lineIndex] = line;
        lineIndex = (lineIndex + 1) % pocet;
    }

    for (int i = 0; i < pocet; i++) {
        int idx = (lineIndex + i) % pocet;
        if (lines[idx]) {
            printf("%s", lines[idx]);
            free(lines[idx]);
        }
    }

    free(lines);
}


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

void headOrTail(FILE * f, char *akce, char* radku)
{
    char *ptr;
    int pocet = strtol(radku, &ptr, 10);

    if (*ptr != '\0') {
        fprintf(stderr, "Invalid input.\n");
    }


    if (strcmp(akce, "head") == 0) {
        head(f, pocet);
    } else if (strcmp(akce, "tail") == 0) {
        tail(f, pocet);
    } else {
        fprintf(stderr, "Spatny parametr na pozici head/tail, pokud nevis, jak ovladat program zadej parametr -h pro pomoc\n");
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

    headOrTail(f, argv[1], argv[2]);


    fclose(f);

    return 0;
}
