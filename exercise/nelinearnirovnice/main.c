#include <math.h>
#include <stdio.h>

typedef float (*Tfun)(float);
typedef float (*Talg)(float, float, float, Tfun);

// Struktura pro testovací případ
typedef struct {
    const char *name;    // Název funkce
    Tfun func;           // Testovaná funkce
    float a;             // Dolní mez intervalu
    float b;             // Horní mez intervalu
    float expected;      // Očekávaný výsledek (NAN pro žádný kořen)
    float epsilon;       // Přesnost
} TestCase;

// 1. Kvadratická funkce: x^2 - 4 (má kořeny ±2)
float quadratic(float x) {
    return x * x - 4;
}

// 2. Lineární funkce: 2x - 6 (má kořen 3)
float linear(float x) {
    return 2 * x - 6;
}

// 3. Trigonometrická funkce: sin(x) (má nekonečně mnoho kořenů, použijte omezený interval)
float sine(float x) {
    return sin(x);
}

// 4. Funkce bez kořene v daném intervalu: x^2 + 1 (nemá žádný reálný kořen)
float no_root(float x) {
    return x * x + 1;
}



float hornerScheme(float coef[], int n, float x)
{
    float sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum  = sum*x + coef[i];
    }

    return sum;
}

float bisekce(float a, float b, float eps, Tfun func)
{
    float fa = func(a); // vycisleni f
    float fb = func(b); // vycisleni f

    // Kontrola, zda jsou hodnoty na koncích intervalu opačných znamének
    if (fa * fb > 0) {
        return 0.0 / 0.0; // Vrací NAN, pokud není kořen v intervalu
    }

    float middle = (a + b) / 2;
    float fmiddle;

    while (fabs(fmiddle = func(middle)) >= eps) {
        if (fa * fmiddle < 0) {
            b = middle;
        } else {
            a = middle;
            fa = fmiddle;
        }
        middle = (a + b) / 2;
     }

    return middle; // vraci middle, ne fmiddle!
}


// metoda tetiv
float regulaFalsi(float a, float b, float eps, Tfun func)
{
    float fa = func(a);
    float fb = func(b);
    float fc;

    float c = a + fa*(b - a)/(fa - fb);

    while (fabs(fc = func(c)) >= eps) {
        if (fa * fc < 0) {
            b = c;
            fb = fc;
        } else {
            a = c;
            fa = fc;
        }
        c = a + fa*(b - a)/(fa - fb);
    }

    return c;
}

// float metodaSecen(float a, float b, float eps, Tfun func)
// {
    // float fa = func(a);
    // float fb = func(b);
    // float fc;

    // while (fabs(fc = func(b)) >= eps) {
        // // Vypočítáme nový bod pomocí vzorce metody sečen
        // float c = b - fb * (b - a) / (fb - fa);

        // // Posuneme hodnoty pro další iteraci
        // a = b;
        // fa = fb;
        // b = c;
        // fb = fc;
    // }

    // return b;  // Návrat posledního odhadu kořene
// }

float metodaSecen(float a, float b, float eps, Tfun func)
{
    float fa = func(a);
    float fb = func(b);
    float c, fc;

    while (fabs(fb) >= eps) {
        // Vypočítáme nový bod pomocí vzorce metody sečen
        c = b - fb * (b - a) / (fb - fa);
        fc = func(c);

        // Posuneme hodnoty pro další iteraci
        a = b;     // Posuneme bod 'a' na původní 'b'
        fa = fb;   // Aktualizujeme f(a)
        b = c;     // Posuneme bod 'b' na nový bod 'c'
        fb = fc;   // Aktualizujeme f(b)
    }

    return b;  // Návrat posledního odhadu kořene
}



float newtonovaMetoda(float a, float b, float eps, Tfun func)
{

}

void test_horner_scheme() {
    printf("Spouštím testy pro Hornerovo schéma...\n\n");

    // Testovací případy
    float coef1[] = {1.0, -4.0, 4.0};      // x^2 - 4x + 4
    float coef2[] = {1.0, 2.0, 1.0, 1.0};  // x^3 + 2x^2 + x + 1
    float coef3[] = {3.0, 1.0, -2.0, 7.0, -4.0}; // 3x^4 + x^3 - 2x^2 + 7x - 4
    float coef4[] = {5.0};                 // Konstanta 5

    // Testy
    printf("Test 1: x^2 - 4x + 4 při x = 2\n");
    printf("  Výsledek: %f (očekávaný: 0.000000)\n", hornerScheme(coef1, 3, 2.0));

    printf("Test 2: x^3 + 2x^2 + x + 1 při x = 1\n");
    printf("  Výsledek: %f (očekávaný: 5.000000)\n", hornerScheme(coef2, 4, 1.0));

    printf("Test 3: 3x^4 + x^3 - 2x^2 + 7x - 4 při x = 2\n");
    printf("  Výsledek: %f (očekávaný: 58.000000)\n", hornerScheme(coef3, 5, 2.0));

    printf("Test 4: Konstanta 5 při x = 10\n");
    printf("  Výsledek: %f (očekávaný: 5.000000)\n", hornerScheme(coef4, 1, 10.0));

    printf("\nTesty Hornerova schématu dokončeny.\n");
}

void test(Talg func) {
    TestCase tests[] = {
        {"Kvadratická (x^2 - 4)", quadratic, -5, 0, -2.0, 0.001},
        {"Lineární (2x - 6)", linear, 0, 5, 3.0, 0.001},
        {"Trigonometrická (sin(x))", sine, 3, 4, 3.14159, 0.001},
        {"Bez kořene (x^2 + 1)", no_root, -2, 2, NAN, 0.001}
    };

    size_t num_tests = sizeof(tests) / sizeof(TestCase);

    for (size_t i = 0; i < num_tests; i++) {
        TestCase test = tests[i];
        printf("Test %zu: %s v intervalu <%f, %f>\n", i + 1, test.name, test.a, test.b);

        float result = func(test.a, test.b, test.epsilon, test.func);

        if (isnan(test.expected)) {
            if (isnan(result)) {
                printf("  Test úspěšný: Nenalezen žádný kořen, jak očekáváno.\n");
            } else {
                printf("  Selhání: Neočekávaný kořen nalezen: %f\n", result);
            }
        } else {
            if (fabs(result - test.expected) <= test.epsilon) {
                printf("  Test úspěšný: Nalezený kořen: %f (očekávaný: %f)\n", result, test.expected);
            } else {
                printf("  Selhání: Nalezený kořen: %f (očekávaný: %f)\n", result, test.expected);
            }
        }
    }

    printf("\nTesty dokončeny.\n\n\n\n");
}





int main()
{
    test_horner_scheme();

    printf("Spouštím testy pro funkci Bisekce...\n\n");
    test(bisekce);
    printf("Spouštím testy pro funkci Regula Falsi...\n\n");
    test(regulaFalsi);
    printf("Spouštím testy pro funkci Metoda secen...\n\n");
    test(metodaSecen);
    return 0;
}
