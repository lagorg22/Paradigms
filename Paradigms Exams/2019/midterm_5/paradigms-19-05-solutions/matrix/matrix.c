#include "matrix.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define ELEM(m, i, j) (void*)((char*)(m->row[(i)]) + ((j) * m->elem_size))

void MatrixInit(Matrix* m, int n, size_t elem_size, Free free_fn) {
  m->row = malloc(n * sizeof(void*));
  assert(m->row != NULL);
  for (int i = 0; i < n; ++i) {
    m->row[i] = malloc(n * elem_size);
    assert(m->row[i] != NULL);
  }
  m->n = n;
  m->elem_size = elem_size;
  m->free_fn = free_fn;
}

void MatrixDestroy(Matrix* m) {
  if (m->free_fn != NULL) {
    for (int i = 0; i < m->n; ++i) {
      for (int j = 0; j < m->n; ++j) {
	m->free_fn(ELEM(m, i, j));
      }
    }
  }
  for (int i = 0; i < m->n; ++i) {
    free(m->row[i]);
  }
  free(m->row);
}

void MatrixSet(Matrix* m, int row, int col, void* elem) {
  memcpy(ELEM(m, row, col), elem, m->elem_size);
}

void* MatrixGet(Matrix* m, int row, int col) {
  return ELEM(m, row, col);
}

void MatrixRotate(Matrix* m) {
  int n = m->n;
  void* tmp = malloc(m->elem_size);
  assert(tmp != NULL);
  for (int i = 0; i < m->n / 2; ++i) {
    for (int j = i; j < m->n - i - 1; ++j) {
      memcpy(tmp, ELEM(m, n - j - 1, i), m->elem_size);
      memcpy(ELEM(m, n - j - 1, i), ELEM(m, n - i - 1, n - j - 1), m->elem_size);
      memcpy(ELEM(m, n - i - 1, n - j - 1), ELEM(m, j, n - i - 1), m->elem_size);
      memcpy(ELEM(m, j, n - i - 1), ELEM(m, i, j), m->elem_size);
      memcpy(ELEM(m, i, j), tmp, m->elem_size);
    }
  }
  free(tmp);
}

void MatrixAccumulate(Matrix* m, int row, int col, Accumulate accumulate_fn,
		      void* aux_data) {
  accumulate_fn(ELEM(m, row, col), aux_data);
  for (int dr = -1; dr < 2; ++dr) {
    for (int dc = -1; dc < 2; ++dc) {
      if (dr * dc == 0) {
	continue;
      }
      int i = row + dr;
      int j = col + dc;
      while (0 <= i && i < m->n && 0 <= j && j < m->n) {
	accumulate_fn(ELEM(m, i, j), aux_data);
	i += dr;
	j += dc;
      }
    }
  }
}

void IntMatrixInit(IntMatrix* m, int n) {
  MatrixInit(m, n, sizeof(int), /*free_fn=*/NULL);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      IntMatrixSet(m, i, j, 0);
    }
  }
}

void IntMatrixDestroy(IntMatrix* m) {
  MatrixDestroy(m);
}

void IntMatrixSet(IntMatrix* m, int row, int col, int elem) {
  MatrixSet(m, row, col, &elem);
}
int IntMatrixGet(IntMatrix* m, int row, int col) {
  return *(int*)MatrixGet(m, row, col);
}

void Add(void* elem, void* aux_data) {
  *(int*)aux_data += *(int*)elem;
}

int IntMatrixSum(Matrix* m, int row, int col) {
  int sum = 0;
  MatrixAccumulate(m, row, col, Add, &sum);
  return sum;
}
