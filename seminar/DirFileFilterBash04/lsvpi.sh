#!/bin/bash

# Pro testovani
# set -x

print_help() {
    echo "Použití: lsvpi.sh -f | -d [-h | --help] [--hidden] [-i | --interactive]"
    echo ""
    echo "Parametry:"
    echo "  -f            Vypíše seznam souborů aktuálního adresáře."
    echo "  -d            Vypíše seznam podadresářů aktuálního adresáře."
    echo "  -h, --help    Zobrazí nápovědu."
    echo "  --hidden      Vypíše i skryté soubory nebo adresáře."
    echo "  -i, --interactive  Zobrazí interaktivní menu."
}

list_files() {
    for item in $(ls -1a); do # 1 - je pro to aby se kazdy soubor zobrazil na svem radku
        if [[ "$HIDDEN" == true || "${item:0:1}" != "." ]]; then # itema:0:1 - je slicing stringu itemu, pricemz 0 je kde to zacina a 1 je jak velke to je ten string
            [ -f "$item" ] && echo "$item" # pouziti logickeho operatoru AND (&&) - misto if
        fi
    done
}

list_dirs() {
    for item in $(ls -1a); do
        if [[ "$HIDDEN" == true || "${item:0:1}" != "." ]]; then
            [ -d "$item" ] && echo "$item"
        fi
    done
}

interactive_mode() {
    while true; do
        echo "--------------------------------"
        echo "Vyberte možnost:"
        echo "1) Zobrazit soubory"
        echo "2) Zobrazit adresáře"
        echo "3) Zobrazit nápovědu"
        echo "4) Konec"
        read -p "Zadejte číslo volby: " choice
        echo "--------------------------------"
        case $choice in
            1) list_files ;;
            2) list_dirs ;;
            3) print_help ;;
            4) echo "Konec"; exit 0 ;;
            *) echo "Neplatná volba" ;;
        esac
    done
}

# interactive_mode() {
#     PS3="Zadejte číslo volby: "  # nastaveni promptu pro `select`
    
#     select choice in "Zobrazit soubory" "Zobrazit adresáře" "Zobrazit nápovědu" "Konec"; do
#         case $choice in
#             "Zobrazit soubory") list_files ;;
#             "Zobrazit adresáře") list_dirs ;;
#             "Zobrazit nápovědu") print_help ;;
#             "Konec") echo "Konec"; exit 0 ;;
#             *) echo "Neplatná volba" ;;
#         esac
#     done
# }


HIDDEN=false
INTERACTIVE=false
MODE=""

while [[ "$#" -gt 0 ]]; do # $# pocet argumentu, ktere byly skriptu predany
    case $1 in
        -f) MODE="files" ;;
        -d) MODE="dirs" ;;
        -h|--help) print_help; exit 0 ;;
        --hidden) HIDDEN=true ;;
        -i|--interactive) INTERACTIVE=true ;;
        *) echo "Neznámý parametr: $1"; print_help; exit 1 ;;
    esac
    shift # posouva vsechny argumenty doleva (takze $2 se stane 1$ atd...)
done

if [ "$INTERACTIVE" = true ]; then
    interactive_mode
else
    case $MODE in
        files) list_files ;;
        dirs) list_dirs ;;
        *) echo "Chyba: Musíte zadat -f nebo -d"; print_help; exit 1 ;;
    esac
fi

