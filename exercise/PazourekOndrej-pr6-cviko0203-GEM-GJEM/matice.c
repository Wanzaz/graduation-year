/* Modul pro práci s maticemi.
 *
 * Copyright (c) David Martinek, 2018--2024
 */

#include "matice.h"
#include <stdio.h>
#include <stdlib.h>


/** \brief Vytvoří matici zadaných rozměrů.*/
Tmatice * maticeAlokuj(int radku, int sloupcu)
{
  if (radku >= 0 && radku <= MATICE_MAX_VELIKOST &&
      sloupcu >= 0 && sloupcu <= MATICE_MAX_VELIKOST)
  {
    // Tady se dynamicky alokuje jen struktura typu Tmatice.
    // Samotné pole uvnitř má v této implementaci pevné rozměry, přičemž
    // my z něj využijeme jen část odpovídající rozměrům radku a sloupcu.
    // Skutečně dynamická 2D matice se dělá jinak.
    Tmatice * m = malloc(sizeof(Tmatice));
    if (m == NULL)
      return NULL;

    m->radku = radku;
    m->sloupcu = sloupcu;

    return m;
  }
  else
  {
    return NULL;
  }
}


/** \brief Zruší zadanou matici.*/
void maticeUvolni(Tmatice *matice)
{
  free(matice);
  // Takto jednoduše to jde jen v naší primitivní implementaci. Skutečné
  // 2D dynamické pole by se muselo rušit postupně po řádcích.
}


/** \brief Inicializuje všechny prvky matice počáteční hodnotou.*/
void maticeNastavPrvky(Tmatice *matice, float initValue)
{
  for (int r = 0; r < matice->radku ; ++r)
  {
    for (int s = 0; s < matice->sloupcu ; ++s)
    {
      matice->prvek[r][s] = initValue;
    }
  }
}


/** \brief Vytvoří duplikát zadané matice. */
Tmatice * maticeDuplikat(const Tmatice *zdroj)
{
  Tmatice *m = maticeAlokuj(zdroj->radku, zdroj->sloupcu);
  if (m != NULL)
  {
    for (int r = 0; r < zdroj->radku; ++r)
    {
      for (int s = 0; s < zdroj->sloupcu; ++s)
      {
        m->prvek[r][s] = zdroj->prvek[r][s];
      }
    }
  }

  return m;
}


/** \brief Inicializuje všechny prvky matice náhodnými hodnotami. */
void maticeRandom(Tmatice *matice)
{
  for (int r = 0; r < matice->radku ; ++r)
  {
    for (int s = 0; s < matice->sloupcu ; ++s)
    {
      {
        matice->prvek[r][s] = (float)((rand()%2000)-1000) / 100;
      }
    }
  }
}


/** \brief Vytiskne matici na obrazovku.*/
void maticeTiskni(const Tmatice *matice)
{
  maticeTiskniSoubor(stdout, matice);
}


/** \brief Vytiskne matici do souboru.*/
void maticeTiskniSoubor(FILE *file, const Tmatice *matice)
{
  fprintf(file, "%u %u\n", matice->radku, matice->sloupcu);
  for (int r = 0; r < matice->radku ; ++r)
  {
    for (int s = 0; s < matice->sloupcu ; ++s)
    {
      fprintf(file, "%7.2f ", matice->prvek[r][s]);
    }
    fprintf(file, "\n");
  }
}


/** \brief Vytvoří a přečte matici ze standardního vstupu (klávesnice).*/
int maticeCtiZeVstupu(Tmatice *matice)
{
  return maticeCtiZeSouboru(stdin, matice);
}


/** \brief Vytvoří a přečte matici ze souboru.*/
int maticeCtiZeSouboru(FILE *file, Tmatice *matice)
{
  int radku, sloupcu;
  int ecode =  fscanf(file, "%d %d", &radku, &sloupcu);
  if (ecode != 2)
    return EMREAD;

  //ecode = maticeAlokuj(matice, radku, sloupcu);
  matice->radku = radku;
  matice->sloupcu = sloupcu;

  for (int r = 0; r < matice->radku ; ++r)
  {
    for (int s = 0; s < matice->sloupcu ; ++s)
    {
      ecode = fscanf(file, "%f ", &matice->prvek[r][s]);
      if (ecode != 1)
        return EMREAD;
    }
  }
  return EMOK;
}


/** \brief Vymění v matici řádky zadané pomocí indexů.*/
void maticeVymenRadky(Tmatice *m, int r1, int r2)
{
  if (r1 == r2 || r1*r2 < 0 || r1 >= m->radku || r2 >= m->radku)
    return;

  for (int s = 0; s < m->sloupcu; ++s)
  {
    float pom = m->prvek[r1][s];
    m->prvek[r1][s] = m->prvek[r2][s];
    m->prvek[r2][s] = pom;
  }
}


