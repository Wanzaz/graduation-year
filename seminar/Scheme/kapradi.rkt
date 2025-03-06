; Kreslení fraktálů

; Teachpack turtle - modul pro želví grafiku.
; V adresáři teachpack musí být soubor turtle.scm.
(require "teachpack/turtle.scm")

; Seznam operací, které modul turtle poskytuje
; --------------------------------------------
;(init-playground size); Inicializuje kreslící plochu o rozměru N.
;(erase-playground)    ; Vymaže kreslící plochu.
;(close-playground)    ; Zavře kreslící plochu.
;
;(set-turtle x y uhel) ; Postaví želvu na souřadnice x, y a otočí ji pod úhlem
;(turtle-x)            ; Vrací x souřadnici želvy.
;(turtle-y)            ; Vrací y souřadnici želvy.
;(turtle-angle)        ; Vrací úhel natočení želvy.
;(playground-size)     ; Vrací velikost kreslící plochy.
;
;(forward delka barva) ; Želva ujde vzdálenost delka a za sebou kreslí čáru
;                      ; danou barvou. Barva není povinný parametr. Implicitně
;                      ; kreslí černou čáru.
;(backward delka)      ; Želva kreslí pozpátku. Barva není povinná.
;(jump-forward delka)  ; Želva skočí dopředu, ale nic nenakreslí.
;(jump-backward delka) ; Želvy ninja umí skákat i dozadu.
;(left uhel)           ; Otočí želvu na místě o daný úhel doleva.
;(right uhel)          ; Otočí želvu na místě o daný úhel doprava.
;(remember)            ; Uloží aktuální pozici a úhel želvy do zásobníku.
;(return)              ; Vybere ze zásobníku poslední pozici a úhel želvy a položí
;                      ; ji na ni.
;(make-rgb r g b)      ; Vyrobí barvu po RGB složkách (desetinná čísla <0, 1>).



; Funkce pro náhodné číslo mezi dvěma hodnotami
(define (rand-between a b)
  (+ a (random (- b a))))

; Rekurzivní kreslení kapradí
(define (kapradi n size)
  (if (> n 0)
      (begin
        (remember)                 ; Zapamatuje aktuální pozici a úhel
        (forward size)             ; Pohyb vpřed
        (left (rand-between 15 45))  ; Otočení doleva o náhodný úhel
        (kapradi (- n 1) (* size 0.7))  ; Rekurzivní volání pro menší velikost
        (right (rand-between 30 60)) ; Otočení doprava o náhodný úhel
        (kapradi (- n 1) (* size 0.7))  ; Další rekurze pro druhou větev
        (return))                   ; Návrat na původní pozici
      0))

; Inicializace plochy a želvy
(init-playground 500)
(set-turtle 0 -450 0)             ; Nastavení počáteční pozice
(kapradi 10 250)                   ; Vykreslení kapradí s hloubkou 10 a počáteční velikostí 50
