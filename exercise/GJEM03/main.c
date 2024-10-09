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



/** \brief Otestuje operace maticeAlokuj, maticeRandom a maticeTiskni. */
void testInit(void)
{
  printf("==========================================\n");
  printf("Test vytvoreni a inicializace matice\n");
  Tmatice * matice = maticeAlokuj(50000000, 30000000);
//  Tmatice * matice = maticeAlokuj(5, 3);
//  maticeNastavPrvky(matice, 1.2);
  if (matice == NULL)
  { // Matici se nepovedlo vytvořit, tak vytisknu proč a skončím.
    maticeAktualniChyboveHlaseni(stderr);
    return;
  }
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
void testFileRW(char *input, char *output)
{
  printf("==========================================\n");
  printf("Test cteni a zapisu ze a do souboru\n");
  FILE *fin = stdin;
  FILE *fout = stdout;

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

  // Tady teď není potřeba volat funkci maticeAlokuj, protože to se už dělá
  // uvnitř funkce maticeCtiZeSouboru.
  Tmatice * m = maticeCtiZeSouboru(fin);
  if (m != NULL)
  { // Není chyba => zpracuji načtenou matici.
    maticeTiskniSoubor(fout, m);
    maticeUvolni(m);
  }
  else
  { // Při chybě tisknu chybové hlášení, zavřu soubory a končím.
    maticeAktualniChyboveHlaseni(stderr);
  }

  if (input != NULL) fclose(fin);
  if (output != NULL) fclose(fout);
  printf("==========================================\n");
  return;
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

/** \brief Test operací maticeRandom a maticeMult. */
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
 * \param r int Index referenčního řádku (a sloupce)
 * \return int Index řádku s pivotem.
 *
 */
int maxAbsPivot(Tmatice *m, int r)
{
    int maxi = r;
    for (int radek = r + 1; radek < m->radku - 1; ++radek) {
        if (fabs(m->prvek[radek][r]) > fabs(m->prvek[maxi][r])) {
            maxi = radek;
        }
    }

    return maxi;
}


/** \brief Testuje, zda je zadaná soustava ve tvaru horní trojúhelníkové matice.
 *
 * \param m Tmatice* Ukazatel na rozšířenou matici soustavy.
 * \return bool Vrací logickou hodnotu.
 *
 */
bool jeHorni(Tmatice *m)
{
    // Procházíme všechny řádky
    for (int radek = 0; radek < m->radku; ++radek) {
        // Kontrola diagonálního prvku
        if (m->prvek[radek][radek] == 0.0 ) {
            return false;  // Pokud je jakýkoli diagonální prvek 0, vrátíme false
        }
        // Procházíme prvky pod diagonálou
        for (int sloupec = 0; sloupec < radek; ++sloupec) {
            // Pokud najdeme prvek pod diagonálou, který není 0, vrátíme false
            if (m->prvek[radek][sloupec] != 0.0) {
                return false;
            }
        }
    }
    // Pokud jsou všechny podmínky splněny, matice je horní trojúhelníková a diagonální prvky jsou nenulové
    return true;
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
        float koeficient = m->prvek[radek][r] / m->prvek[r][r]; // koeficient pro úpravu řádku
        m->prvek[radek][r] = 0.0;  // Nulování prvků pod diagonálním prvkem

        // Úprava zbývajících sloupců
        for (int sloupec = r + 1; sloupec < m->sloupcu; ++sloupec) {
            m->prvek[radek][sloupec] -= m->prvek[r][sloupec] * koeficient;
        }
    }
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
int pivotovani(Tmatice *m, int r)
{
    int p = maxAbsPivot(m, r); // index pivota
    if (m->prvek[p][r] == 0.0) { // 0 nesmí být na diagonále
        return -1;
    }

    if (r != p) { // aby se neměnily stejné řádky..
        maticeVymenRadky(m, r, p);
    }

    return p;
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

        int p = pivotovani(m, radek);

        if (m->prvek[p][radek] == 0.0) { // 0 nesmí být na diagonále
            return CHYBA_RESENI;
        }

        // Provádění řádkových úprav pomocí funkce radkoveUpravy
        radkoveUpravy(m, radek);
    }

    return OK;
}

/**
 * \brief Provádí úpravy v matici nad zadaným řádkem (pivotem)
 *
 * \param m Tmatice* Ukazatel na alokovanou matici.
 * \param r int Index řádku, na kterém proběhne úprava (pivot).
 * \return void
 */
void radkoveUpravyNad(Tmatice *m, int r)
{
    for (int radek = 0; radek < r; ++radek) { // Pro každý řádek nad aktuálním (pivot)
        float koef = m->prvek[r][r] / m->prvek[radek][r]; // Vypočteme koeficient
        for (int s = 0; s < m->sloupcu; ++s) {
            m->prvek[radek][s] -= koef * m->prvek[r][s]; // Zrušíme prvek nad
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
int gjemPrimy(Tmatice *m)
{
    for (int radek = 0; radek < m->radku; ++radek) {
        // Pivotování pro aktuální řádek
        if (pivotovani(m, radek) == -1) { // Pokud je pivot 0, vrátí chybu
            return CHYBA_RESENI;
        }

        // Nulování prvků pod aktuálním řádkem
        radkoveUpravy(m, radek); // Provádí úpravy pro řádky pod aktuálním řádkem

        // Nulování prvků nad aktuálním řádkem
        radkoveUpravyNad(m, radek);

        // Vypisování matice po každé úpravě
        maticeTiskni(m); // SMAZAT PO TESTOVANI
    }

    return OK;
}


/** \brief Test přímého chodu, tj. operací gemPrimy a gjemPrimy.
 *
 * Zadání:
 * Pro řešení využij funkce z modulu matice.h a funkce gemPrimy a gjemPrimy.
 * 1. Načti soubor s maticí (nebo si ji inicializuj přímo tady ve funkci).
 *    Pomocí funkce maticeDuplikat si vyrob její duplikát.
 * 2. Proveď na ní přímý chod GEM a vytiskni výsledek.
 * 2. Na duplikátu vstupní matice proveď přímý chod GJEM a vytiskni výsledek.
 * 3. Ukliď po sobě.
 */

void testPrimehoChodu(char *jmenoSouboru)
{
    printf("==========================================\n");
    printf("Test přímého chodu GEM\n");

    // Načtení matice ze souboru
    FILE *fin = fopen(jmenoSouboru, "r");
    if (fin == NULL) {
        fprintf(stderr, "Chyba: Nelze otevřít soubor %s.\n", jmenoSouboru);
        return;
    }

    // Načtení matice ze souboru
    Tmatice *m = maticeCtiZeSouboru(fin); // Funkce vrací ukazatel na matici
    fclose(fin);

    // Zkontrolujeme, zda byla matice správně načtena
    if (m == NULL) {
        fprintf(stderr, "Chyba při čtení matice ze souboru.\n");
        return;
    }

    // Vytiskneme původní matici
    printf("Původní matice:\n");
    maticeTiskni(m);

    // Provedeme přímý chod GEM na původní matici
    printf("Matice po přímém chodu GEM:\n");
    if (gemPrimy(m) == CHYBA_RESENI) {
        printf("Chyba: Matice má 0 na diagonále, nelze provést GEM.\n");
    } else {
        maticeTiskni(m); // Vytiskneme matici po přímém chodu
    }

    printf("Test přímého chodu GJEM\n");
    printf("Matice po přímém chodu GJEM:\n");
    if (gjemPrimy(m) == CHYBA_RESENI) {
        printf("Chyba: Matice má 0 na diagonále, nelze provést GJEM.\n");
    } else {
        maticeTiskni(m); // Vytiskneme matici po přímém chodu
    }

    // Uvolníme alokovanou paměť
    maticeUvolni(m);

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
    int n = m->radku;        // Počet řádků matice
    int msloupce = m->sloupcu; // Počet sloupců matice

    // Kontrola rozměrů matice, matice musí mít o jeden sloupec více než řádků
    if (msloupce != n + 1) {
        return 0; // Chybné rozměry matice
    }

    // Použití funkce jeHorni k ověření, že matice je horní trojúhelníková
    if (!jeHorni(m)) {
        return 0; // Matice není ve tvaru horní trojúhelníkové matice
    }

    // Projdeme všechny řádky matice
    for (int radek = 0; radek < n; ++radek) {
        bool vsechnyNuly = true; // Testujeme, zda je celý řádek nulový

        // Projdeme všechny sloupce kromě posledního (poslední je pravá strana rovnice)
        for (int sloupec = 0; sloupec < msloupce - 1; ++sloupec) {
            if (m->prvek[radek][sloupec] != 0.0) {
                vsechnyNuly = false; // Našli jsme nenulový prvek, řádek není nulový
                break;
            }
        }

        // Pokud jsou všechny prvky rovny nule, zkontrolujeme pravou stranu (sloupec n)
        if (vsechnyNuly) {
            if (m->prvek[radek][msloupce - 1] != 0.0) {
                // Pokud je pravá strana nenulová a ostatní prvky jsou nulové, soustava nemá žádné řešení
                return 0;
            }
            // Pokud jsou všechny prvky i pravá strana nulové, soustava má nekonečně mnoho řešení
            return -1;
        }
    }

    // Pokud jsme nenašli žádné problémy, soustava má jedno řešení
    return 1;
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
    int pocetNulovychR = 0; // Počet nulových řádků
    int pocetPromenych = m->sloupcu - 1; // Počet proměnných (sloupců - 1 pro pravou stranu)

    // Kontrola každého řádku matice
    for (int r = 0; r < m->radku; ++r) {
        bool jeNulovyRadek = true; // Předpokládáme, že řádek je nulový

        for (int s = 0; s < pocetPromenych; ++s) {
            if (m->prvek[r][s] != 0.0) { // Pokud najdeme nenulový prvek
                jeNulovyRadek = false; // Řádek není nulový
                break;
            }
        }

        if (jeNulovyRadek) {
            pocetNulovychR++; // Počítáme nulový řádek
            if (m->prvek[r][pocetPromenych] != 0.0) {
                // Pokud je nulový řádek a poslední prvek není 0, nemáme řešení
                return 0; // Žádné řešení
            }
        }
    }

    // Kontrola počtu řešení
    if (pocetNulovychR > 0) {
        // Pokud je počet nulových řádků větší než 0
        if (pocetNulovychR + m->radku < pocetPromenych) {
            return -1; // Nekonečně mnoho řešení
        }
    }

    return 1; // Jedno řešení
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
    printf("Testování matice po přímém chodu...\n");

    // Krok 1: Otevření souboru pro čtení
    FILE *file = fopen(jmenoSouboru, "r");
    if (file == NULL) {
        printf("Chyba při otevírání souboru.\n");
        return;
    }

    // Krok 2: Načtení matice ze souboru
    Tmatice *matice = maticeCtiZeSouboru(file);
    fclose(file); // Zavření souboru po načtení
    if (matice == NULL) {
        printf("Chyba při načítání matice ze souboru.\n");
        return;
    }

    // Krok 3: Otestuj, zda je matice ve tvaru horní trojúhelníkové matice
    if (jeHorni(matice)) {
        printf("Matice je ve tvaru horní trojúhelníkové matice.\n");
    } else {
        printf("Matice není ve tvaru horní trojúhelníkové matice. Provedu přímý chod GEM...\n");

        // Krok 4: Provedení přímého chodu GEM
        if (gemPrimy(matice) == CHYBA_RESENI) {
            printf("Chyba při provádění přímého chodu GEM.\n");
            maticeUvolni(matice); // Uvolnění paměti v případě chyby
            return;
        }

        // Otestuj znovu, zda je matice nyní ve tvaru horní trojúhelníkové matice
        if (jeHorni(matice)) {
            printf("Matice je nyní ve tvaru horní trojúhelníkové matice.\n");
        } else {
            printf("Matice stále není ve tvaru horní trojúhelníkové matice.\n");
            maticeUvolni(matice); // Uvolnění paměti v případě neúspěchu
            return;
        }
    }

    // Krok 5: Zhodnocení počtu řešení soustavy po přímém chodu
    int pocetReseni = gemPoPrimem(matice); // Funkce pro určení počtu řešení
    if (pocetReseni == 1) {
        printf("Soustava má jedno řešení.\n");
    } else if (pocetReseni == 0) {
        printf("Soustava nemá žádné řešení.\n");
    } else if (pocetReseni == -1) {
        printf("Soustava má nekonečně mnoho řešení.\n");
    }

    // Krok 6: Provedení Gauss-Jordanovy eliminační metody (GJEM)
    printf("Provádím Gauss-Jordanovu eliminační metodu...\n");
    if (gjemPrimy(matice) == CHYBA_RESENI) {
        printf("Chyba při provádění Gauss-Jordanovy eliminační metody.\n");
        maticeUvolni(matice); // Uvolnění paměti v případě chyby
        return;
    }

    // Krok 7: Zhodnocení počtu řešení po GJEM
    pocetReseni = gjemPoPrimem(matice);
    if (pocetReseni == 1) {
        printf("Soustava má jedno řešení po GJEM.\n");
    } else if (pocetReseni == 0) {
        printf("Soustava nemá žádné řešení po GJEM.\n");
    } else if (pocetReseni == -1) {
        printf("Soustava má nekonečně mnoho řešení po GJEM.\n");
    }

    // Krok 8: Uvolnění paměti pro matici
    maticeUvolni(matice); // Správná funkce pro uvolnění matice
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
    // Procházej od posledního řádku k prvnímu
    for (int radek = m->radku - 1; radek >= 0; --radek) {
        // Inicializuj proměnnou pro uložení hodnoty
        float suma = 0.0;

        // Procházej sloupce v aktuálním řádku, kromě posledního
        for (int sloupec = radek + 1; sloupec < m->sloupcu - 1; ++sloupec) {
            // Odečti hodnoty neznámých násobené koeficienty z aktuálního řádku
            suma += m->prvek[radek][sloupec] * m->prvek[sloupec][m->radku];
        }

        // Vypočítaná hodnota se uloží do posledního sloupce
        m->prvek[radek][m->sloupcu - 1] = (m->prvek[radek][m->radku] - suma) / m->prvek[radek][radek];
    }
}

/** \brief Provede zpětný chod GJEM.
 *
 * Hodnoty neznámých nastaví do posledního sloupce rozšířené matice soustavy.
 *
 * \param m Tmatice* Ukazatel na rozšířenou matici soustavy.
 */
int gjemZpetny(Tmatice *m) {
    if (m == NULL) {
        return -1; // Chyba: ukazatel na matici je NULL
    }

    for (int r = 0; r < m->radku; ++r) {
        // Ověření, že řádek má platný pivot
        if (m->prvek[r][r] == 0) {
            continue; // Pokud je pivot nula, nemůžeme provést žádnou úpravu
        }

        // Nastavení hodnoty neznámé do posledního sloupce
        float hodnota = m->prvek[r][m->sloupcu - 1]; // Poslední prvek řádku
        for (int s = 0; s < r; ++s) {
            hodnota -= m->prvek[s][r] * m->prvek[s][m->sloupcu - 1]; // Odečteme ostatní známé
        }
        m->prvek[r][m->sloupcu - 1] = hodnota; // Uložení hodnoty do posledního sloupce
    }

    return 0; // Úspěch
}

/** \brief Tiskne řešení soustavy rovnic, které je uloženo v posledním sloupci rozšířené matice soustavy.
 *
 * \param m Tmatice* Ukazatel na rozšířenou matici soustavy.
 */
void tiskReseni(Tmatice *m)
{
  printf("Reseni soustavy rovnic:\n");
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

    // Krok 1: Otevření souboru pro čtení
    FILE *file = fopen(jmenoSouboru, "r");
    if (file == NULL) {
        printf("Chyba při otevírání souboru.\n");
        return;
    }

    // Krok 2: Načtení matice ze souboru
    Tmatice *matice = maticeCtiZeSouboru(file);
    fclose(file); // Zavření souboru po načtení
    if (matice == NULL) { // Zkontrolujeme, zda načtení proběhlo úspěšně
        printf("Chyba při načítání matice ze souboru.\n");
        return;
    }

    // Krok 3: Uprav matici do požadovaného tvaru pro zpětný chod
    printf("Provádím přímý chod GEM...\n");
    gemPrimy(matice); // Provede GEM přímý chod (Gaussova eliminace)

    // Krok 4: Otestuj, zda je matice v horním trojúhelníkovém tvaru
    if (!jeHorni(matice)) {
        printf("Matice není ve tvaru horní trojúhelníkové matice. Nemá jednoznačné řešení.\n");
        maticeUvolni(matice); // Uvolníme matici v případě chyby
        return;
    }

    // Krok 5: Provedení zpětného chodu GEM pro řešení soustavy
    printf("Provádím zpětný chod GEM...\n");
    gemZpetny(matice); // Řeší soustavu po přímém chodu

    // Krok 6: Vypiš řešení soustavy
    printf("Řešení soustavy:\n");
    tiskReseni(matice); // Výpis výsledného řešení

    printf("Provádím zpětný chod GJEM...\n");
    gjemZpetny(matice); // Řeší soustavu po přímém chodu

    printf("Řešení soustavy:\n");
    tiskReseni(matice); // Výpis výsledného řešení

    maticeUvolni(matice);
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

  testPrimehoChodu("A.txt");          // otestuj i jiné soubory
  testMaticePoPrimemChodu("A.txt");   // otestuj i jiné soubory
  testZpetnyChod("A.txt");            // otestuj i jiné soubory
  return EXIT_SUCCESS;
}
