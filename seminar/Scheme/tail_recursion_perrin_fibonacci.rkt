
; FIBONACCI RECURSION
(define (fib n)
  (case n
    ((0) 0)
    ((1) 1)
    (else (+ (fib (- n 1)) (fib (- n 2))))))

; FIBONACCI TAIL RECURSION
; vzdy po dvojcich pr -> 0 1 -> 1 1 -> 1 2 -> 2 3 -> 3 5 .....
(define (fib-helper a b count)
  (cond 
    ((= count 0) a)
    ((= count 1) b)
    (else (fib-helper b (+ a b) (- count 1)))))

(define (tr-fib n)
  (fib-helper 0 1 n))



(display "Fibonacci recursion: ")
(display (fib 5))
(newline)
(display "Fibonacci tail recursion: ")
(display (tr-fib 60))
(newline)



; PERRIN RECURSION
(define (perrin n)
  (case n
    ((0) 3)
    ((1) 0)
    ((2) 2)
    (else (+ (perrin (- n 2)) (perrin (- n 3))))))


; PERRIN TAIL RECURSION
(define (perrin-helper a b c count)
  (cond 
    ((= count 0) a)
    ((= count 1) b)
    ((= count 2) c)
    (else (perrin-helper b c (+ a b) (- count 1)))))

(define (tr-perrin n)
  (perrin-helper 3 0 2 n))


(display "Perrin recursion: ")
(display (perrin 30))
(newline)
(display "Perrin tail recursion: ")
(display (tr-perrin 30))
(newline)
