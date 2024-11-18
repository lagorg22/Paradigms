(define (last-element seq)
  (if (= (length seq) 1)
      (car seq)
      (last-element (cdr seq))))

(define (is-greater-list? a b)
  (eval (cons and (map >= a b))))

(define (max-max-2 a b)
  (cond ((> (length a) (length b)) a)
	((< (length a) (length b)) b)
	((is-greater-list? a b) a)
	(else b)))

(define (max-max-3 a b c) (max-max-2 a (max-max-2 b c)))

(define (helper left right)
  (cond ((null? right) left)
	((and
	  (not (null? left))
	  (> (last-element left) (car right)))
	 (helper (append left (list (car right)))
		 (cdr right)))
	(else
	 (max-max-3
	  left
	  (helper (list (car right)) (cdr right))
	  (helper '() (cdr right))))))

(define (max-decreasing-subsegment sequence)
  (helper '() sequence))

(display (equal? (max-decreasing-subsegment '(1 2 3 4 3 2 1)) '(4 3 2 1)))(newline)
(display (equal? (max-decreasing-subsegment '(8 7 9 4 6 5)) '(8 7)))(newline)
(display (equal? (max-decreasing-subsegment '(2 2 2 2 1 1 1 1 1 )) '(2 1)))(newline)
(display (equal? (max-decreasing-subsegment '(5 4 3 2 4 3 2 1)) '(5 4 3 2)))(newline)
(display (equal? (max-decreasing-subsegment '(7 7 6 6 5 5 4 4 3 3)) '(7 6)))(newline)
(display (equal? (max-decreasing-subsegment '(1 2 3 4 5 6 7 8 9)) '(9)))(newline)
(display (equal? (max-decreasing-subsegment '(9 7 8 6 4 5 2 3 1)) '(8 6 4)))(newline)
(display (equal? (max-decreasing-subsegment '(2 2 2 2)) '(2)))(newline)
(display (equal? (max-decreasing-subsegment '(9 8 7 6 5 4 3 2 1 0)) '(9 8 7 6 5 4 3 2 1 0)))(newline)
(display (equal? (max-decreasing-subsegment '(8 6 4 2 0 9 7 5 3 1)) '(9 7 5 3 1)))(newline)
