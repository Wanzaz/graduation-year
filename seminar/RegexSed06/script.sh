#!/bin/bash

input_file="osoby.csv"
output_file="osoby.html"

sed -e '1i\
        <html lang="cs">\
        <head>\
        <title>Tabulka generovaná sedem</title>\
        </head>\
        <body>\
        <table border="1">' \
    -e '$a\
        </table>\
        </body>\
        </html>' \
    -e 's/^#/<\!-- & -->/' \
    -e '/^$/d' \
    -e '/!table/s/.*/<table border="1">/' \
    -e '/!!table/s/.*/<\/table>/' \
    -e 's/^\([^,]*\),\([^,]*\),\([^,]*\),\([^,]*\),\([^,]*\)$/<tr><td>\2<\/td><td>\1<\/td><td>\3<\/td><td>\4<\/td><td>\5<\/td><\/tr>/' \
    -e 's/,/<\/td><td>/g' \
    "$input_file" > "$output_file"

# & - vklada puvodni text, ktery odpovidal vzoru
# d - odstraneni
# 1i - pridani textu pred prvni radek
# s - nahrazeni (pri pouziti g na konci, tak je to global vsude)
# '2,4d' - smazani rozmezi 2 az 4 radku
# -e - expression - pro vice regularnich vyrazu
# .* - libovolny znak na od zacatku radku za do konce
# -e '/!table/s/.*/<table border="1">/' - omezeni dane, ze radek musi zacinat !table, jinak by se to aplikovalo uplne vsude...
#
# \([^,]*\) - zachyceni sloupce s oddelovacem carky --> rovna se pak 1/ (pripadne 2/, 3/, ...)
# [^,]* - vsechno krome carek


echo "HTML done $output_file."

