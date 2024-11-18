(define (merge l r)
  (cond ((null? l) r)
	((null? r) l)
	((< (car l) (car r)) (cons (car l) (merge (cdr l) r)))
	(else (cons (car r) (merge l (cdr r))))))

(define (unique-helper last l)
  (cond ((null? l) '())
	((= last (car l)) (unique-helper last (cdr l)))
	(else (cons (car l) (unique-helper (car l) (cdr l))))))

(define (unique l)
  (if (null? l) '()
      (cons (car l) (unique-helper (car l) (cdr l)))))

(define (helper sum seq)
  (if (null? seq) (list sum)
      (unique (merge (helper sum (cdr seq)) (helper (+ sum (car seq)) (cdr seq))))))
  
(define (unique-sums seq)
  (length (helper 0 seq)))
