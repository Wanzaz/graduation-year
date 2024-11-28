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

echo "HTML soubor byl úspěšně vygenerován jako $output_file."

