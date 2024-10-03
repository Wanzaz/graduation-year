#include <stdlib.h>
#include "mojematice.h"
#include "gvid.h"


/** \brief Vyrobi novou matici
 *
 * \param radku uint Pocet radku matici
 * \param sloupcu uint Pocet sloupcu v matici
 * \return Tmatice* Vraci ukazatel na matici nebo NULL
 *
 */
Tmatice * novaMatice(uint radku, uint sloupcu)
{
    Tmatice * matice = malloc(sizeof(Tmatice));

    // Pokud se nepovedlo alokovat strukturu, vrátím NULL.
    if (matice == NULL) {
        return NULL;
    }

    matice->radku = radku;
    matice->sloupcu = sloupcu;

    matice->prvek = malloc(radku * sizeof(float*));

    for (uint r = 0; r < sloupcu; ++r) {
        matice->prvek[r] = malloc(sloupcu * sizeof(float));

        if (matice->prvek[r] == NULL) {
            uvolniMatici(matice);
            return NULL;
        }
    }

    return matice;
}


/** \brief Uvolni alokovanou pamet
 *
 * \param m Tmatice* Ukazatel na alokovanou strukturu s matici
 * \return void
 *
 */
void uvolniMatici(Tmatice *m)
{
    // Free each row
    for (uint i = 0; i < m->radku; i++) {
        free(m->prvek[i]);
    }

    // Free the array of pointers
    free(m->prvek);

    // Free the structure itself
    free(m);
}


/** \brief Tiskne matici
 *
 * \param out FILE* Vystupni proud
 * \param m Tmatice* Ukazatel na strukturu matice
 * \return void
 *
 */
void tiskMatice(FILE *out, const Tmatice *m)
{
    fprintf(out, "%d %d\n", m->radku, m->sloupcu);

    for (uint r = 0; r < m->radku; ++r)
    {
        for (uint s = 0; s < m->sloupcu; ++s)
        {
            fprintf(out, "%.f ", m->prvek[r][s]);
        }
        fprintf(out, "\n");
    }
}

/** \brief Nacteni matice ze souboru, s podporou dynamicky alokovanych poli
 *
 * \param in FILE* Vstupni proud
 * \return Tmatice* Vraci ukazatel na matici nebo NULL
 *
 */
Tmatice * nactiMatice(FILE *in)
{
    int radku = 0, sloupcu = 0;

    // Načtení počtu řádků a sloupců matice
    if (fscanf(in, "%d %d", &radku, &sloupcu) != 2 || radku <= 0 || sloupcu <= 0) {
        return NULL;
    }

    // Dynamická alokace nové matice
    Tmatice *m = novaMatice((uint)radku, (uint)sloupcu);
    if (m == NULL) {
        return NULL;
    }

    // Načtení hodnot prvků matice
    for (uint r = 0; r < m->radku; ++r) {
        for (uint s = 0; s < m->sloupcu; ++s) {
            if (fscanf(in, "%f", &m->prvek[r][s]) != 1) {
                // Pokud dojde k chybě při načítání, uvolníme paměť a vrátíme NULL
                uvolniMatici(m);
                return NULL;
            }
        }
    }

    return m;
}


/** \brief Secteni vsech prvku dvou matic
 *
 * \param input1 FILE* Vstupni soubor pro prvni matici
 * \param input2 FILE* Vstupni soubor pro druhou matici
 * \return float Vraci vysledek souctu matic nebo NULL
 *
 */
float soucetMatic(FILE *input1, FILE *input2) {
    // Načtení první matice ze souboru
    Tmatice *m1 = nactiMatice(input1);
    if (m1 == NULL) {
        printf("Chyba při načítání první matice.\n");
        return -1;
    }

    // Načtení druhé matice ze souboru
    Tmatice *m2 = nactiMatice(input2);
    if (m2 == NULL) {
        printf("Chyba při načítání druhé matice.\n");
        uvolniMatici(m1);
        return -1;
    }

    // Kontrola, zda mají matice stejné rozměry
    if (m1->radku != m2->radku || m1->sloupcu != m2->sloupcu) {
        printf("Matice nemají stejné rozměry.\n");
        uvolniMatici(m1);
        uvolniMatici(m2);
        return -1;
    }

    // Výpočet celkového součtu odpovídajících prvků matic
    float soucet = 0.0;
    for (uint r = 0; r < m1->radku; ++r) {
        for (uint s = 0; s < m1->sloupcu; ++s) {
            soucet += m1->prvek[r][s] + m2->prvek[r][s];
        }
    }

    // Uvolnění paměti pro načtené matice
    uvolniMatici(m1);
    uvolniMatici(m2);

    // Vrácení celkového součtu
    return soucet;
}

/**
 * \brief Vypočítá maticový součin dvou matic
 *
 * Funkce načte dvě matice a provede maticový součin těchto matic, pokud mají správné
 * rozměry pro maticový součin. Výslednou matici vrátí jako funkční hodnotu.
 *
 * \param m1 Tmatice* Ukazatel na první matici
 * \param m2 Tmatice* Ukazatel na druhou matici
 * \return Tmatice* Ukazatel na výslednou matici, nebo NULL pokud rozměry nejsou vhodné
 *
 * Matice m1 má rozměry [a x b] a matice m2 musí mít rozměry [b x c].
 * Výsledná matice bude mít rozměry [a x c].
 */
Tmatice * soucinMatic(Tmatice *m1, Tmatice *m2) {
    // Kontrola, zda počet sloupců první matice odpovídá počtu řádků druhé matice
    if (m1->sloupcu != m2->radku) {
        printf("Chyba: Počet sloupců první matice se nerovná počtu řádků druhé matice.\n");
        return NULL;
    }

    // Vytvoření výsledné matice s rozměry [počet řádků první matice] x [počet sloupců druhé matice]
    Tmatice *vyslednaMatice = novaMatice(m1->radku, m2->sloupcu);
    if (vyslednaMatice == NULL) {
        printf("Chyba při alokaci paměti pro výslednou matici.\n");
        return NULL;
    }

    // Výpočet součinu matic
    for (uint i = 0; i < m1->radku; ++i) {
        for (uint j = 0; j < m2->sloupcu; ++j) {
            vyslednaMatice->prvek[i][j] = 0;  // Inicializace prvku
            for (uint k = 0; k < m1->sloupcu; ++k) {
                vyslednaMatice->prvek[i][j] += m1->prvek[i][k] * m2->prvek[k][j];
            }
        }
    }

    // Vrácení výsledné matice
    return vyslednaMatice;
}
