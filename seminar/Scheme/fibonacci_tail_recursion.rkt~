

(define (fib n)
  (case n
    ((0) 0)
    ((1) 1)
    (else (+ (fib (- n 1)) (fib (- n 2))))))

(display "Výsledek: ")
(display (fib 5))
(newline)



(define (fib-helper a b count)
  (if (= count 0)
      a
      (fib-helper b (+ a b) (- count 1))))

(define (tr-fib n)
  (fib-helper 0 1 n))

(display "Výsledek: ")
(display (tr-fib 60))
(newline)

