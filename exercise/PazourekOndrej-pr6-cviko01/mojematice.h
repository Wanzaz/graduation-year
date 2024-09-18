#ifndef MOJEMATICE_H_INCLUDED
#define MOJEMATICE_H_INCLUDED

#include <stdio.h>
#include "gvid.h"

#define MAXN 100

typedef struct _matice
{
    float prvek[MAXN][MAXN];
    uint radku, sloupcu;
} Tmatice;


/** \brief Vyrobi novou matici
 *
 * \param radku uint Pocet radku matici
 * \param sloupcu uint Pocet sloupcu v matici
 * \return Tmatice* Vraci ukazatel na matici nebo NULL
 *
 */
Tmatice * novaMatice(uint radku, uint sloupcu);


/** \brief Uvolni alokovanou pamet
 *
 * \param m Tmatice* Ukazatel na alokovanou strukturu s matici
 * \return void
 *
 */
void uvolniMatici(Tmatice *m);


/** \brief Tiskne matici
 *
 * \param out FILE* Vystupni proud
 * \param m Tmatice* Ukazatel na strukturu matice
 * \return void
 *
 */
void tiskMatice(FILE *out, const Tmatice *m);


/** \brief Nacteni matice ze souboru
 *
 * \param out FILE* Vstupni proud
 * \return Tmatice* Vraci ukazatel na matici nebo NULL
 *
 */
Tmatice * nactiMatice(FILE *in);


/** \brief Secteni vsech prvku dvou matic
 *
 * \param input1 FILE* Vstupni soubor pro prvni matici
 * \param input2 FILE* Vstupni soubor pro druhou matici
 * \return float Vraci vysledek souctu matic nebo NULL
 *
 */
float soucetMatic(FILE *input1, FILE *input2);


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
Tmatice * soucinMatic(Tmatice *m1, Tmatice *m2);


#endif // MOJEMATICE_H_INCLUDED
