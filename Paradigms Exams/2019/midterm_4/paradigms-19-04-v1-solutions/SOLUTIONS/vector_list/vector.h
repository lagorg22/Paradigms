#ifndef PARADIGMS_PROBLEMS_VECTOR_LIST_VECTOR_H_
#define PARADIGMS_PROBLEMS_VECTOR_LIST_VECTOR_H_

#include "map_list.h"

#include <stdbool.h>

typedef bool(*CmpFn)(void* a, void* b);

typedef MapList Vector;

void VectorInit(Vector* vector, size_t elem_size);
void VectorDestroy(Vector* vector);
void VectorAppend(Vector* vector, void* elem);
void* VectorGet(Vector* vector, int index);
int VectorFind(Vector* vector, void* elem, CmpFn cmp_fn);

#endif // PARADIGMS_PROBLEMS_VECTOR_LIST_VECTOR_H_
