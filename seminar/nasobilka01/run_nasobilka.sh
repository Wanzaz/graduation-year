#!/bin/bash

# Zkompiluje program
gcc -o nasobilka main.c

# Ověří, zda byla kompilace úspěšná
if [ $? -eq 0 ]; then
    echo "Kompilace proběhla úspěšně."

    # Spustí program s argumenty
    if [ $# -lt 2 ]; then
        echo "Chyba: Nezadali jste dostatek argumentů."
        echo "Použití: ./run_nasobilka.sh <pocet> <zaklad>"
        exit 1
    fi

    ./nasobilka "$1" "$2"
else
    echo "Kompilace selhala."
    exit 1
fi
