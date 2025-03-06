; Práce se seznamy
(define la '(1 . (2 . (3 . (4 . (5 . ()))))))    ; jak je vidět, je to lineární spojový seznam
(define lb '(6 . (7 . (8 . (9 . ())))))



; Vrací i-tý prvek seznamu nebo null
; (list-get s 3) - vrátí prvek na indexu 3, tedy 4. prvek
(define (list-get s i)
   (cond ((< i 0) null)
        ((= i 0) (car s)) 
        (else (list-get (cdr s) (- i 1)))))

; Vrací agregovaný výsledek nad všemi prvky.
; (agreguj s +) - vrátí sumu všech prvků
; (agreguj s *) - vrátí součit všech prvků
(define (jednoprvkovy? s)
  (and (not (null? s))
       (null? (cdr s))))  ; zjistí, jestli je seznam jednoprvkový

(define (agreguj s op)
  (cond ((null? s) (if (eq? op +) 0 1))   ; pokud je seznam prázdný, vrátí 0 pro součet a 1 pro součin
        ((jednoprvkovy? s) (car s))       ; pokud má seznam jen jeden prvek, vrátí tento prvek
        (else (op (car s) (agreguj (cdr s) op)))))  ; rekurzivně použije operaci na první prvek a zbytek seznamu


; Vrací nový seznam, který je reverzí zadaného seznamu.
; (otoc (list 1 2 3)) -- vrátí (list 3 2 1)

; Funkce pro reverzování seznamu s akumulátorem
(define (otoc-acc s acc)
  (cond ((null? s) acc)  ; pokud je seznam prázdný, vrátí akumulátor jako reverzní seznam
        (else (otoc-acc (cdr s) (cons (car s) acc)))))  ; přidá prvek na začátek akumulátoru

; Hlavní funkce pro reverzování seznamu
(define (otoc s)
  (otoc-acc s '()))  ; zavolá pomocnou funkci s počátečním prázdným seznamem



; Vrátí nový seznam vzniklý připojením senzamu b na konec seznamu a.
; (pripoj (list 1 2 3) (list 4 5 6)) -- vrátí (list 1 2 3 4 5 6)
(define (pripoj a b)
  (cond ((null? a) b)            ; pokud je seznam a prázdný, vrátí seznam b
        (else (cons (car a) (pripoj (cdr a) b)))))  ; přidá první prvek seznamu a na začátek rekurzivně zpracovaného zbytku




(list-get la 3)
(agreguj la +)
(otoc la)
(pripoj la lb)