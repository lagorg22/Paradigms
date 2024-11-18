// მატრიცა
//
// თქვენი ამოცანაა იმპლემენტაცია გაუკეთოთ ორ განზომილებიან მატრიცას ზომებით N x N. მატრიცას უნდა ქონდეს შემდეგი ინტერფეისი:
//
// void MatrixInit(Matrix m, int n, size_t elem_size, Free free_fn); - ქმნის მატრიცას n ხაზით და n სვეტით,
// სადაც თითოეული ელემენტის ზომაა elem_size ბაიტებში და free_fn ფუნქცია (თუ გადმოგეცათ) უნდა გამოიძახოთ
// მატრიცის გასუფთავებისას.
// void MatrixDestroy(Matrix m); - ათავისუფლებს მატრიცის მიერ გამოყენებულ მეხსიერებას.
// void MatrixSet(Matrix m, int row, int col, void elem); - აყენებს row რიგისა და col სვეტის ელემენტს
// გადმოცემულ ელემენტზე.
// *void MatrixGet(Matrix m, int row, int col, void elem); - იღებს row რიგისა და col სვეტის ელემენტს.
// void MatrixRotate(Matrix m); - უნდა მოაბრუნოს მატრიცა 90 გრადუსით საათის ისრის მოძრაობის მიმართულებით.
// დეტალები იხილეთ ქვემოთ.
// *void MatrixAccumulate(Matrix m, int row, int col, Accumulate accumulate_fn, void aux_data); -
// უნდა გაიაროს დიაგონალების, რომლებიც გადიან (row, col) ელემენტზე, ყოველი ელემენტისთვის უნდა გამოიძახოს
// გადმოცემული accumulate ფუნქცია და ელემენტის მნიშვნელობასთან ერთად გადასცეს aux_data.
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef void (*FreeFn)(void *);
typedef void (*AccumulateFn)(void *elem, void *aux_data);

typedef struct {
    void* data;
    size_t elem_size;
    int n;
    FreeFn free_fn;
} Matrix;

void MatrixInit(Matrix* m, int n, size_t elem_size, FreeFn free_fn) {
    m->elem_size = elem_size;
    m->n = n;
    m->free_fn = free_fn;
    m->data = malloc(elem_size * n * n);
    assert(m->data);
}

void MatrixSet(Matrix* m, int row, int col, void* elem) {
    assert(row < m->n && col < m->n) ;
    void* dest = (char*)m->data + row * m->n * m->elem_size + col * m->elem_size;
    memcpy(dest, elem, m->elem_size);
}

void* MatrixGet(Matrix* m, int row, int col) {
    assert(row < m->n && col < m->n) ;
    void* res = (char*)m->data + row * m->n * m->elem_size + col * m->elem_size;
    return res;
}
void MatrixRotate(Matrix* m) {

}








