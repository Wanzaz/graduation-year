/*
 * Projekt: Cviko č. 6 - Numerické integrování
 * Autor: Ondrej Pazourek
 * Datum: 4.2.2025
 */

#include "gvid.h"       // par drobnosti pro zjednoduseni prace
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>  // pro praci s textovymi retezci
// #include <stdbool.h> // pro praci s typem bool a konstantami true a false
// #include <ctype.h>   // isalpha, isspace, islower, isupper, ...
#include <math.h>    // funkce z matematicke knihovny
#include <float.h>   // konstanty pro racionalni typy DBL_MAX, DBL_DIG, ...
                     // #include <limits.h>  // konstanty pro celociselne typy INT_MAX, INT_MIN, ...
                     // #include <time.h>    // funkce time a dalsi pro praci s casem

#define N 20

typedef struct _polynom Tpolynom;

struct _polynom
{
    int rad;
    double k[N];
};

void tiskniPolynom(char * zprava, const Tpolynom * fce)
{
    printf("%s", zprava);
    if (fce->rad > 1)
    {
        for (int i = 0; i < fce->rad-1; ++i)
        {
            printf("%+gx^%d ", fce->k[i], fce->rad - i);
        }
        printf("%+gx ", fce->k[fce->rad - 1]);
    }
    printf("%+g\n", fce->k[fce->rad]);
}

double horner(const Tpolynom * fce, double x)
{
    double suma = fce->k[0];
    for (int i = 1; i <= fce->rad; ++i)
    {
        suma = suma*x + fce->k[i];
    }
    return suma;
}

double integralObdelniky(const Tpolynom *fce, double a, double b, double eps) {

    if (a == b) return 0.0;
    if (a > b) {
        double c = a;
        a = b;
        b = c;
    }

    double S0 = NAN, S1 = NAN;
    double K = 2;

    do {
        double suma = 0.0;
        double h = (b - a) / K;
        for (int i = 0; i < K; ++i) { // SUMA ZACINA OD 0
            double x = a + i * h + h / 2.0;
            suma += horner(fce, x);
        }
        S0 = S1;
        S1 = suma * h;
        K *= 2;
    } while (K <= 4 || fabs(S0 - S1) >= eps);

    return S1;
}

double integralLichobezniky(const Tpolynom *fce, double a, double b, double eps) {

    if (a == b) return 0.0;
    if (a > b) {
        double c = a;
        a = b;
        b = c;
    }

    double S0 = NAN, S1 = NAN;
    double K = 2;

    do {
        double suma = 0.0;
        double h = (b - a) / K;
        suma += (horner(fce, a) + horner(fce, b)) / 2.0;
        for (int i = 1; i < K; ++i) { // SUMA ZACINA OD 1 - uz predpocitane f(a)!!!!
            double x = a + i * h;
            suma += horner(fce, x);
        }
        S0 = S1;
        S1 = suma * h;
        K *= 2;
    } while (K <= 4 || fabs(S0 - S1) >= eps);

    return S1;
}


int main(void)
{
    Tpolynom sfce = {
        .rad = 3,
        .k = {-15.0, -2.0, +7.0, +1.0}
    };
    // Při načítání ze souboru bych to vyráběl dynamicky, ale tohle je ukázka,
    // takže takto líně...
    Tpolynom * fce = &sfce;

    double a = -0.7;
    double b = +0.7;
    double epsilon = 0.000001;

    double So = integralObdelniky(fce, a, b, epsilon);
    double Sl = integralLichobezniky(fce, a, b, epsilon);

    tiskniPolynom("Zadana funkce: ", fce);
    printf("Integral od a=%+g do b=%+g\n", a, b);
    printf("   obdelnikovou metodou: %.6f\n", So);
    printf("lichobeznikovou metodou: %.6f\n", Sl);
    // Má vyjít 707/750 = 0.9426666666666667.


    return 0;
}
