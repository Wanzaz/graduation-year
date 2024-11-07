#!/bin/bash


#Vytvoř skript v Bashi, který opíše zadaný (víceřádkový) vstup opačně na standardní výstup. To znamená, že poslední znak vstupu se vypíše jako první, předposlední jako druhý, až po první znak vstupu, který se vypíše jako poslední.

# Tento příkaz
# naruby.sh << END
# Skakal pes
# pres oves.
# END

# vypíše toto:
# .sevo serp

# sep lakakS

tac | rev
