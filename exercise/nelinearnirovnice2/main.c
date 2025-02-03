#include <math.h>
#include<stdio.h>

typedef struct {
    double* exponents;
    int degree;
} TPolynomial;

double horner(TPolynomial* polynomial, double value)
{
    double result = 0;
    for (int i = 0; i <= polynomial->degree; i++) {
        result = result * value + polynomial->exponents[i];
    }

    return result;
}

double solve(TPolynomial* polynomial, double a, double b, double e)
{
    double c = (b + a) / 2;
    double fa = horner(polynomial, a);
    double fc;
    while (fabs(fc = horner(polynomial, c)) >= e) {
        if (fa * fc < 0) {
            b = c;
        } else {
            fa = fc;
            a = c; 
        }

        c = (b + a) / 2;
    }

    return c;
}

double solveRegula(TPolynomial* polynomial, double a, double b, double e)
{
    double fa = horner(polynomial, a);
    double fb = horner(polynomial, b);
    double c = (a * fb - b * fa) / (fb - fa);
    double fc;
    while (fabs(fc = horner(polynomial, c)) >= e) {
        if (fa * fc < 0) {
            b = c;
            fb = fc;
        } else {
            fa = fc;
            a = c; 
        }

        c = (a * fb - b * fa) / (fb - fa);
    }

    return c;
}

double solveIntersection(TPolynomial* polynomial, double a, double b, double e)
{
    double fa = horner(polynomial, a);
    double fb = horner(polynomial, b);
    double c = (a * fb - b * fa) / (fb - fa);
    double fc;
    while (fabs(fc = horner(polynomial, c)) >= e) {
        fa = fb;
        a = b;
        fb = fc;
        b = c;
        c = (a * fb - b * fa) / (fb - fa);
    }

    return c;
}

double solveNewton(TPolynomial* polynomial, TPolynomial* derivative, double x, double e)
{
    double fx = horner(polynomial, x);
    double dfx = horner(derivative, x);
    if (dfx == 0) {
        return NAN;
    }
    double x1 = x - (fx / dfx);
    double fx1;
    while (fabs(fx1 = horner(polynomial, x1)) >= e) {
        x = x1;
        fx = fx1;
        dfx = horner(derivative, x);
        if (dfx == 0) {
            return NAN;
        }
        x1 = x - (fx / dfx);
    }

    return x1;
}

int main(void)
{
    double exponents[7] = {12, 0.7, 3, 0, 6, 22, 4.62};
    TPolynomial p = {
        .exponents = exponents,
        .degree = 6,
    };

    double de[6] = {72, 3.5, 12, 0, 12, 22};
    TPolynomial dp = {
        .exponents = de, 
        .degree = 5,
    };

    printf("%f\n", horner(&p, 7.1));
    printf("%f\n", solve(&p, -1, -0.9, 0.0000001));
    printf("%f\n", solveRegula(&p, -1, -0.9, 0.0000001));
    printf("%f\n", solveIntersection(&p, -1, -0.9, 0.0000001));
    printf("%f\n", solveNewton(&p, &dp, -1, 0.0000001));
}
