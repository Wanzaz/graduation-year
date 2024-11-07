#!/bin/bash

HIDDEN=false
INTERACTIVE=false
MODE=""

list_items() {
    local type=$1

    for item in * .?*; do # zahrnuje vsechny soubory a adresare i skryte (? je wild card - 1 jakkykoliv znak)
        if [ "${item:0:1}" != "." ] || $HIDDEN; then
            if [ "$type" == "file" ] && [ -f "$item" ]; then
                echo "$item"
            elif [ "$type" == "dir" ] && [ -d "$item" ]; then
                echo "$item"
            fi
        fi
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
            "Zobrazit soubory") list_items "file" ;;
            "Zobrazit adresáře") list_items "dir" ;;
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
    if [ "$MODE" == "files" ]; then
        list_items "file"
    elif [ "$MODE" == "dirs" ]; then
        list_items "dir"
    else
        echo "Vyberte režim s parametry -f nebo -d."
        exit 1
    fi
}

