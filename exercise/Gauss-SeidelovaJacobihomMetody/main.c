/* Téma matice a moduly - varianta s polem pevných rozměrů
 *
 * Autor: Podepiš se
 * Datum: today (+ Ctrl+J)
 *
 * Verze: 2.1
 * Errata:
 * - maticeVymenRadky: viz záhlaví souboru matice.h
 * - maticeCtiZeSouboru: viz záhlaví souboru matice.h
 * - maticeCtiZeVstupu: viz záhlaví souboru matice.h
 * - testFileRW: Opraveno nesmyslné alokování prázdné matice před čtením
 *   ze souboru.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <time.h>

#include "gvid.h"
#include "matice.h"

enum gemErrors {
    OK,          /**< Kód pro stav bez chyby. */
    CHYBA_RESENI,      /**< Kód chyby kdyz na diagonale je 0.  */
};


bool jeDDM(Tmatice *m)
{
    float sum = 0.0;
    for (int r = 0; r < m->radku; ++r) {
        for (int s = 0; s < m->sloupcu - 1; ++s) {
            if (s != r) {
                sum += m->prvek[r][s];
            }
        }

        if (sum > m->prvek[r][r]) {
            return false;
        }
        sum = 0.0;
    }

    return true;
}

void upravaMatice(Tmatice *m)
{
    for (int r = 0; r < m->radku; ++r) {
        for (int s = 0; s < m->sloupcu; ++s) {
            if (r != s) {
                m->prvek[r][s] /= m->prvek[r][r]; // vydelime kazdy prvek diagonalnim prvkem
            }
        }
        m->prvek[r][r] = 0.0;
    }
}

void nulovaniDiagonaly(Tmatice *m)
{
    for(int r = 0; r < m->radku; r++)
    {
        m->prvek[r][r] = 0.0;
    }
}


void gaussSeidlova(Tmatice *m, float eps, Tmatice *x)
{
    bool jePresny = false; // na zacatku zaciname pesimisticky
    float xpred = 0.0;
    float sum = 0.0;

    nulovaniDiagonaly(m);

    while(!jePresny) {
        jePresny = true; // na zacatku kazde iterace - nastavit optimisticky
        for (int r = 0; r < m->radku; ++r) {
            xpred = x->prvek[r][0];

            for (int s = 0; s < m->sloupcu; ++s) {
                sum += m->prvek[r][s] * x->prvek[s][0];
            }

            x->prvek[r][0] = m->prvek[r][m->sloupcu - 1] - sum; // nova hodnota podle vzorce

            sum = 0.0;
            jePresny = jePresny && fabs(xpred - x->prvek[r][0]) < eps;
        }
    }
}

void jacobiho(Tmatice *m, float eps, float *x)
{

}

void testJ(void)
{

}

void tiskReseni(Tmatice *m)
{
    for(int r = 0; r < m->radku; r++){
        printf("x%d = %7.7f\n", r, m->prvek[r][0]);
    }
}

void clearbuffer()
{
    while(getchar()!='\n');
}

void testGS(char * adresaSouboru, float eps)
{
    printf("------ Gauss-Seidelova metoda ------\n");
    FILE* f = fopen(adresaSouboru,"r");
    if(f == NULL){
        printf("\nChyba pri otevirani souboru.\n");
        return;
    }

    Tmatice *m = maticeCtiZeSouboru(f);
    fclose(f);

    if(m == NULL){
        printf("\nChyba pri alokaci matice.\n");
        return;
    }

    Tmatice * x = maticeAlokuj(m->radku,1);
    if(x == NULL){
        printf("\nChyba pri alokaci vysledkove matice.\n");
        return;
    }

    maticeTiskni(m);

    if(!jeDDM(m)){
        printf("\nMatice neni DDM.\n");
        return;
    }

    upravaMatice(m);
    gaussSeidlova(m, eps, x);

    tiskReseni(x);
    maticeUvolni(x);
    maticeUvolni(m);
}

void testDDM(char *adresaSouboru)
{
    FILE* f = fopen(adresaSouboru,"r");
    if(f == NULL){
        printf("\nChyba pri otevirani souboru.\n");
        return;
    }

    Tmatice * m = maticeCtiZeSouboru(f);
    fclose(f);

    if(m == NULL){
        printf("Chyba pri zpracovani matice.\n");
        return;
    }

    maticeTiskni(m);

    printf("\nMatice %s\n", jeDDM(m) ? "je DDM." : "neni diagonalne dominantni.");

    upravaMatice(m);
    maticeTiskni(m);

    maticeUvolni(m);
}


// TODO: Matice, ktera neni DDM - C.txt maticeC.txt
int main(void)
{
    float eps = 0.001;
    // testDDM("B.txt");
    testGS("A.txt", eps);

    return EXIT_SUCCESS;
}
