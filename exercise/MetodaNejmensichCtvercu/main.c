#include "matice.h"
#include "pole.h"
#include <stdio.h>
#include <stdlib.h>


void vyresSoustavu(Tmatice *m) {
    for (int r = 0; r < 2; r++) {
        if (m->prvek[r][r] == 0.0f) {
            return; // Chyba: diagonální prvek je nula
        }
        for (int radek = r + 1; radek < 2; radek++) {
            float koef = m->prvek[radek][r] / m->prvek[r][r];
            for (int sloupec = r; sloupec < 3; sloupec++) {
                m->prvek[radek][sloupec] -= koef * m->prvek[r][sloupec];
            }
        }
    }
    for (int radek = 1; radek >= 0; radek--) {
        float suma = 0.0f;
        for (int sloupec = radek + 1; sloupec < 2; sloupec++) {
            suma += m->prvek[radek][sloupec] * m->prvek[sloupec][2];
        }
        m->prvek[radek][2] = (m->prvek[radek][2] - suma) / m->prvek[radek][radek];
    }
}

float konstantniRegrese(const float y[], int n) {
    float sumy = 0.0;

    for (unsigned int i = 0; i < n; ++i) {
        sumy += y[i];
    }

    return sumy / n;
}

void linearniRegrese(const float x[], const float y[], int n, float* a, float* b) {
    float sumx = 0.0;
    float sumx2 = 0.0;
    float sumy = 0.0;
    float sumxy = 0.0;

    for (int i = 0; i < n; ++i) {
        sumx += x[i];
        sumx2 += x[i] * x[i];
        sumy += y[i];
        sumxy += x[i] * y[i];
    }

    Tmatice m;

    m.prvek[0][0] = sumx2;
    m.prvek[0][1] = sumx;
    m.prvek[0][2] = sumxy;
    m.prvek[1][0] = sumx;
    m.prvek[1][1] = n;
    m.prvek[1][2] = sumy;

    vyresSoustavu(&m);

    *a = m.prvek[0][2];
    *b = m.prvek[1][2];
}

int main(void) {
    float a, b;
    linearniRegrese(TAB_X, TAB_Y, TAB_LEN, &a, &b);
    printf("a = %f, b = %f\n", a, b);
    return 0;
}

