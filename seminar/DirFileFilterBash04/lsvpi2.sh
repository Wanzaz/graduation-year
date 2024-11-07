#!/bin/bash

HIDDEN=false
INTERACTIVE=false
MODE=""

list_files() {
    echo "Seznam souborů:"
    for item in *; do
        [ -f "$item" ] && echo "$item"
    done
    $HIDDEN && for item in .?*; do
        [ -f "$item" ] && echo "$item"
    done
}

list_dirs() {
    echo "Seznam adresářů:"
    for item in *; do
        [ -d "$item" ] && echo "$item"
    done
    $HIDDEN && for item in .?*; do
        [ -d "$item" ] && echo "$item"
    done
}

print_help() {
    echo "Nápověda:"
    echo "-f   - Vypíše seznam souborů aktuálního adresáře."
    echo "-d   - Vypíše seznam podadresářů aktuálního adresáře."
    echo "-h, --help - Zobrazí tuto nápovědu."
    echo "--hidden - Vypíše i skryté soubory nebo adresáře."
    echo "-i, --interactive - Spustí interaktivní mód."
}

interactive_mode() {
    PS3="Zadejte číslo volby: "
    select choice in "Zobrazit soubory" "Zobrazit adresáře" "Zobrazit nápovědu" "Konec"; do
        case $choice in
            "Zobrazit soubory") list_files ;;
            "Zobrazit adresáře") list_dirs ;;
            "Zobrazit nápovědu") print_help ;;
            "Konec") echo "Konec"; exit 0 ;;
            *) echo "Neplatná volba" ;;
        esac
    done
}

process_params() {
    while [[ "$#" -gt 0 ]]; do
        case $1 in
            -f) MODE="files" ;;
            -d) MODE="dirs" ;;
            -h|--help) print_help; exit 0 ;;
            --hidden) HIDDEN=true ;;
            -i|--interactive) INTERACTIVE=true ;;
            *) echo "Neznámý parametr: $1"; exit 1 ;;
        esac
        shift
    done
}

process_params "$@"

$INTERACTIVE && interactive_mode || {
    [ "$MODE" == "files" ] && list_files
    [ "$MODE" == "dirs" ] && list_dirs
    { [ -z "$MODE" ] && echo "Vyberte režim s parametry -f nebo -d." && exit 1; }
}

