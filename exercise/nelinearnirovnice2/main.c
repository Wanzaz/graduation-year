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

double solve(TPolynomial* polynomial, double left, double right, double e)
{
    double middle = (right + left) / 2;
    double fleft = horner(polynomial, left);
    double fmiddle;
    while (fabs(fmiddle = horner(polynomial, middle)) >= e) {
        if (fleft * fmiddle < 0) {
            right = middle;
        } else {
            fleft = fmiddle;
            left = middle; 
        }

        middle = (right + left) / 2;
    }

    return middle;
}

double solveRegula(TPolynomial* polynomial, double left, double right, double e)
{
    double fleft = horner(polynomial, left);
    double fright = horner(polynomial, right);
    double c = (left * fright - right * fleft) / (fright - fleft);
    double fmiddle;
    while (fabs(fmiddle = horner(polynomial, c)) >= e) {
        if (fleft * fmiddle < 0) {
            right = c;
            fright = fmiddle;
        } else {
            fleft = fmiddle;
            left = c; 
        }

        c = (left * fright - right * fleft) / (fright - fleft);
    }

    return c;
}

double solveIntersection(TPolynomial* polynomial, double left, double right, double e)
{
    double fleft = horner(polynomial, left);
    double fright = horner(polynomial, right);
    double c = (left * fright - right * fleft) / (fright - fleft);
    double fmiddle;
    while (fabs(fmiddle = horner(polynomial, c)) >= e) {
        fleft = fright;
        left = right;
        fright = fmiddle;
        right = c;
        c = (left * fright - right * fleft) / (fright - fleft);
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
