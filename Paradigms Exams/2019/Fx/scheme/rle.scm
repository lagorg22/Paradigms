
(define (rle seq)

)
  
      
(display (equal? (rle '(a b b c d d d)) '(a (b 2) c (d 3))))
(newline)
(display (equal? (rle '(1 2 3 4)) '(1 2 3 4)))
(newline)
(display (equal? (rle '(0 0 0 0 0)) '((0 5))))
(newline)
(display (equal? (rle '()) '()))
(newline)
(display (equal? (rle '(a a a b 1 2 2)) '((a 3) b 1 (2 2))))
(newline)
