/* Téma matice a moduly - varianta s polem pevných rozměrů
 *
 * Autor: Podepiš se
 * Datum: today (+ Ctrl+J)
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

/** \brief Otestuje operace #maticeAlokuj, #maticeRandom a #maticeTiskni. */
void testInit(void)
{
  printf("==========================================\n");
  printf("Test vytvoreni a inicializace matice\n");
  Tmatice * matice = maticeAlokuj(5, 3);
//  maticeNastavPrvky(matice, 1.2);
  maticeRandom(matice);
  maticeTiskni(matice);

  maticeUvolni(matice);

  printf("==========================================\n");
}






/** \brief Otestuje funkce pro čtení ze a zápis do souboru.
 *
 * \param input char*   Jméno vstupního souboru nebo NULL
 * \param output char*  Jméno výstupního souboru nebo NULL
 * \return int Vrací EXIT_FAILURE v případě chyby při práci se soubory.
 *
 * <p>Pokud je místo jména vstupního nebo výstupního souboru prázdný ukazatel
 * NULL, použije se soubor stdin nebo stdout.</p>
 *
 */
int testFileRW(char *input, char *output)
{
  printf("==========================================\n");
  printf("Test cteni a zapisu ze a do souboru\n");
  FILE *fin = stdin;
  FILE *fout = stdout;

  int exitCode = EXIT_SUCCESS;

  if (input != NULL)
    fin = fopen(input, "r");

  if (output != NULL)
    fout = fopen(output, "w");

  if (fin == NULL)
  {
    fprintf(stderr, "Chybny nazev vstupniho souboru.");
    fin = stdin;
    input = NULL;
  }

  if (fout == NULL)
  {
    fprintf(stderr, "Chybny nazev vystupniho souboru.");
    fout = stdout;
    output = NULL;
  }

  Tmatice * m = maticeAlokuj(0, 0); // připravím si prázdnou matici
  if (maticeCtiZeSouboru(fin, m) == EMOK)
  {
    maticeTiskniSoubor(fout, m);
    maticeUvolni(m);
  }
  else
  {
    fprintf(stderr, "Chyba pri cteni vstupniho souboru.");
    exitCode = EXIT_FAILURE;
  }


  if (input != NULL) fclose(fin);
  if (output != NULL) fclose(fout);
  printf("==========================================\n");
  return exitCode;
}


/** \brief Vynásobí dvě matice a výsledek uloží do třetí nové matice.
 *
 * \param a const Tmatice* První vstupní matice.
 * \param b const Tmatice* Druhá vstupní matice.
 * \param c Tmatice* Výsledná, zatím nevytvořená matice.
 * \return bool Vrací true, pokud se operace povedla, jinak false.
 *
 * <p>Parametr c musí ukazovat na prozatím nevytvořenou matici, jinak dojde ke
 * ztrátě dat.</p>
 *
 * <p>Pokud jsou vstupní matice kompatibilní, tj. a->sloupcu == b->radku, dojde
 * k vytvoření výsledné matice o rozměrech c->radku = a-radku a c->sloupcu = b->sloupcu.
 * Do této matice pak bude uložen výsledek násobení a*b v tomto pořadí.</p>
 *
 * <p>Pozor! Operace maticového násobení není komutativní, tudíž záleží na
 * pořadí operandů.</p>
 */
Tmatice * maticeMult(const Tmatice *a, const Tmatice *b)
{
  if (a->sloupcu != b->radku)
    return NULL;

  Tmatice * c = maticeAlokuj(a->radku, b->sloupcu);
  if (c == NULL)
    return NULL;

  for (int row = 0; row < c->radku ; ++row)
  {
    for (int col = 0; col < c->sloupcu ; ++col)
    {
      double ssum;
      ssum = 0.0;
      for (int k = 0; k < a->sloupcu ; ++k)
      {
        ssum += a->prvek[row][k] * b->prvek[k][col];
      }
      c->prvek[row][col] = ssum;
    }
  }
  return c;

//  Poznámka: Všimni si, že operace se sama stará o alokaci výsledku a ošetření
//  chyb, které to může generovat. Jako autor takové operace nemůžu chtít po
//  uživateli (této funkce), aby před samotným spuštěním funkce prováděl nějaké
//  kroky nutné pro správné chování této funkce. Programy, které toto vyžadují,
//  jsou takzvaně "křehké", což je protiklad k programům "robustním".
}

/** \brief Test operací #maticeRandom a #maticeMult. */
void testMult(void)
{
  printf("==========================================\n");
  printf("Test nasobeni nahodnych matic\n");
  Tmatice * a = maticeAlokuj(4, 2);
  Tmatice * b = maticeAlokuj(2, 3);

  maticeRandom(a);
  maticeRandom(b);

  maticeTiskni(a);
  printf("*\n");
  maticeTiskni(b);
  printf("=\n");
  Tmatice * c = maticeMult(a, b);
  if (c != NULL)
  {
    maticeTiskni(c);
    maticeUvolni(c);
  }
  else
    printf("Tohle nejde!\n");

  maticeUvolni(a);
  maticeUvolni(b);
  printf("==========================================\n");
}




/** \brief Vrátí index řádku s pivotem.
 *
 * Poznámka: Použij funkci fabs pro práci s absolutní hodnotou.
 *
 * \param m Tmatice* Ukazatel na rozšířenou matici soustavy.
 * \param r int Index referenčního řádku (a sloupce) !!!!!!!!!!!
 * \return int Index řádku s pivotem.
 *
 */
int maxAbsPivot(Tmatice *m, int r)
{
    int maxi = r;
    for (int radek = r; radek < m->radku; ++radek) {
        if (fabs(m->prvek[radek][r]) > fabs(m->prvek[maxi][r])) {
            maxi = radek;
        }
    }

    return maxi;
}

/** \brief Testuje, zda funkce maxAbsPivot funguje spravne tisknutim matice a indexu jejich nejvetsich cisel v absolutni hodnote ve sloupcich
 *
 * \param m Tmatice* Ukazatel na rozšířenou matici soustavy.
 * \return void
 *
 */
void testMaxAbsPivot(void)
{
    printf("==========================================\n");
    printf("Test pivotovani - indexy maximalnich hodnot v matici\n");
    Tmatice * matice = maticeAlokuj(5, 3);
    //  maticeNastavPrvky(matice, 1.2);
    maticeRandom(matice);
    maticeTiskni(matice);

    printf("Maximalni prvky sloupcu: ");
    for (int sloupec = 0; sloupec < matice->sloupcu; ++sloupec) {
        printf("%d ", maxAbsPivot(matice, sloupec));
    }
    printf("\n");

    maticeUvolni(matice);

    printf("==========================================\n");
}


/** \brief Testuje, zda je zadaná soustava ve tvaru horní trojúhelníkové matice.
 *
 * \param m Tmatice* Ukazatel na rozšířenou matici soustavy.
 * \return bool Vrací logickou hodnotu.
 *
 */
bool jeHorni(Tmatice *m)
{
  // TODO: naprogramuj ji
}

/** \brief Provede přímý chod GEM.
 *
 * Změní vstupní matici na ekvivalentní horní trojúhelníkovou matici. Využije
 * přitom pivotování a ekvivalentních úprav.
 *
 * Poznámka: Vytvoř si samostatnou funkci pro pivotování a samostatnou funkci
 * pro aplikaci ekvivalentních úprav (nulování prvků pod diagonálním prvkem),
 * které zde zavoláš.
 *
 * \param m Tmatice* Ukazatel na rozšířenou matici soustavy.
 * \return int Vrací kód chyby.
 */
int gemPrimy(Tmatice *m)
{
    for (int radek = 0; radek < m->radku - 1; ++radek) { // Jedu do radku n - 2
        int p = maxAbsPivot(m, radek); // index pivota
        if (m->prvek[radek][p] == 0) { // 0 nesmi byt na diagonale
            return CHYBA_RESENI;
        }

        if (radek != p) { // aby se nemenili stejne radky..
            maticeVymenRadky(m, radek, p);
        }
        maticeTiskni(m);


        // Radkove upravy


    }
  // printf("Funkce gemPrimy neni hotova.");
}

/** \brief Provadi radkove upravy v matici
 *
 * \param m Tmatice* Ukazatel na alokovanou matici.
 * \param radku int  Radek na kterem probehne uprava
 * \return void
 */
void radkoveUpravy(Tmatice *m, int r)
{
    for (int radek = r + 1; radek < m->radku; ++radek) {
        float koeficient = m->prvek[radek][r]/m->prvek[r][r];
        m->prvek[radek][r] = 0.0;

        for (int sloupec = 0; sloupec <= m->sloupcu; ++sloupec) {
            m->prvek[radek][sloupec] = m->prvek[r][sloupec] * koeficient - m->prvek[radek][sloupec];
        }
    }
}

/** \brief Provede přímý chod GJEM.
 *
 * Změní vstupní matici na ekvivalentní diagonální matici.
 *
 * Poznámka: Výsledná matice může, ale nemusí být jednotková.
 *
 * Poznámka: Funkci vytvoříš úpravou funkce gemPrimy.
 *
 * \param m Tmatice* Ukazatel na rozšířenou matici soustavy.
 */
void gjemPrimy(Tmatice *m)
{
  // TODO: naprogramuj ji
  printf("Funkce gjemPrimy neni hotova.");
}

/** \brief Test přímého chodu, tj. operací #gemPrimy a #gjemPrimy.
 *
 * Zadání:
 * Pro řešení využij funkce z modulu matice.h a funkce #gemPrimy a #gjemPrimy.
 * 1. Načti soubor s maticí (nebo si ji inicializuj přímo tady ve funkci).
 *    Pomocí funkce #maticeDuplikat si vyrob její duplikát.
 * 2. Proveď na ní přímý chod GEM a vytiskni výsledek.
 * 2. Na duplikátu vstupní matice proveď přímý chod GJEM a vytiskni výsledek.
 * 3. Ukliď po sobě.
 */
void testPrimehoChodu(char *jmenoSouboru)
{
  printf("==========================================\n");
  // TODO: naprogramuj ji
  printf("Funkce testPrimehoChodu neni hotova.\n");
  printf("==========================================\n");
}






/** \brief Vrací počet řešení upravené soustavy po provedení přímého chodu GEM.
 *
 * Součástí testu jsou i správné rozměry.
 *
 * \param m Tmatice* Ukazatel na rozšířenou matici soustavy.
 * \return bool Vrací hodnotu 1, když matice je rozšířenou maticí soustavy
 *              ve tvaru po provedení přímého chodu Gaussovy eliminační metody
 *              a soustava je dále řešitelná. Hodnotu 0 vrací, když soustava
 *              není ve tvaru horní trojúhelníkové matice nebo nemá žádné
 *              řešení. Hodnotu -1 vrací, když má soustava nekonečně mnoho
 *              řešení.
 */
int gemPoPrimem(Tmatice *m)
{
  // TODO: naprogramuj ji
  return false;
}

/** \brief Vrací počet řešení upravené soustavy po provedení přímého chodu GJEM.
 *
 * Součástí testu jsou i správné rozměry.
 *
 * \param m Tmatice* Ukazatel na rozšířenou matici soustavy.
 * \return bool Vrací hodnotu 1, když matice je rozšířenou maticí soustavy
 *              ve tvaru po provedení přímého chodu Gauss-Jordanovy eliminační
 *              metody a soustava je dále řešitelná. Hodnotu 0 vrací, když
 *              soustava není ve tvaru diagonální matice nebo nemá žádné
 *              řešení. Hodnotu -1 vrací, když má soustava nekonečně mnoho
 *              řešení.
 */
int gjemPoPrimem(Tmatice *m)
{
  // TODO: naprogramuj ji
  return false;
}

/** \brief Test matic po přímém chodu
 *
 * Zadání:
 * Pro řešení využij předchozí hotové funkce.
 * 1. Načti soubor s maticí (nebo si ji inicializuj přímo tady ve funkci).
 * 2. Otestuj ji a vytiskni, zda jde o matici po provedení přímého chodu GEM nebo GJEM.
 * 3. Pokud nejde o upravenou matici, proveď přímý chod GEM, GJEM (funkce pro
 *    to už máš hotové).
 * 4. Výsledek znovu otestuj.
 * 5. Vypiš, kolik řešení zadaná soustava má (0, 1, nekonečno).
 */
void testMaticePoPrimemChodu(char *jmenoSouboru)
{
  printf("==========================================\n");
  // TODO: naprogramuj ji
  printf("Funkce testMaticePoPrimemChodu neni hotova.\n");
  printf("==========================================\n");
}



/** \brief Provede zpětný chod GEM.
 *
 * Hodnoty neznámých nastaví do posledního sloupce rozšířené matice soustavy.
 *
 * \param m Tmatice* Ukazatel na rozšířenou matici soustavy.
 */
void gemZpetny(Tmatice *m)
{
  // TODO: naprogramuj ji
  printf("Funkce gemZpetny neni hotova.");
}

/** \brief Provede zpětný chod GJEM.
 *
 * Hodnoty neznámých nastaví do posledního sloupce rozšířené matice soustavy.
 *
 * \param m Tmatice* Ukazatel na rozšířenou matici soustavy.
 */
void gjemZpetny(Tmatice *m)
{
  // TODO: naprogramuj ji
  printf("Funkce gjemZpetny neni hotova.");
}

/** \brief Tiskne řešení soustavy rovnic, které je uloženo v posledním sloupci rozšířené matice soustavy.
 *
 * \param m Tmatice* Ukazatel na rozšířenou matici soustavy.
 */
void tiskReseni(Tmatice *m)
{
  printf("Reseni soustavy rovnic:");
  for (int r = 0; r < m->radku; ++r)
  {
    printf("x%d = %f\n", r, m->prvek[r][m->sloupcu-1]);
  }
}

/** \brief Vyřeší upravenou soustavu rovnic.
 *
 * Zadání:
 * Pro řešení využij funkce z modulu matice.h a funkce gemPoPrimem, gjemPoPrimem, gemZpetny a gjemZpetny.
 * 1. Načti soubor s maticí.
 * 1a. Uprav ji do požadovaného tvaru pro zpětný chod.
 * 2. Otestuj zda jde o matici s jedním řešením po provedení přímého chodu GEM nebo GJEM.
 * 3. Vyřeš ji.
 * 4. Vypiš řešení soustavy pomocí funkce tiskReseni.
 */
void testZpetnyChod(char *jmenoSouboru)
{
  printf("==========================================\n");
  // TODO: naprogramuj ji
  printf("Funkce testZpetnyChod neni hotova.\n");
  printf("==========================================\n");
}





/** Startovní bod programu. */
int main(void)
{
  // Co nepotřebuješ, si můžeš zakomentovat.

  srand(time(NULL));

  testInit();
  testFileRW("A.txt", NULL); // NULL -> bude zapisovat na stdout
  testMult();

  testMaxAbsPivot();

  testPrimehoChodu("B.txt");          // otestuj i jiné soubory
  testMaticePoPrimemChodu("D.txt");   // otestuj i jiné soubory
  testZpetnyChod("E.txt");            // otestuj i jiné soubory
  return EXIT_SUCCESS;
}
