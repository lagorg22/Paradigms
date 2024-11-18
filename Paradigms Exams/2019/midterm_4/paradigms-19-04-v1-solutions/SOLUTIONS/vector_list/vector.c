#include "vector.h"

#include "map_list.h"

void VectorInit(Vector* vector, size_t elem_size) {
  MapListInit(vector, elem_size);
}

void VectorDestroy(Vector* vector) {
  MapListDestroy(vector);
}

void VectorAppend(Vector* vector, void* elem) {
  MapListAppend(vector, elem);
}

typedef struct {
  int index;
  int cur_index;
  void* elem;
} Get;

void GetFn(void* list_elem, void* aux_data) {
  Get* get = aux_data;
  ++get->cur_index;
  if (get->index == get->cur_index) {
    get->elem = list_elem;
  }
}

void* VectorGet(Vector* vector, int index) {
  Get get;
  get.index = index;
  get.cur_index = -1;
  get.elem = NULL;
  MapListMap(vector, GetFn, &get);
  return get.elem;
}

typedef struct {
  void* elem;
  CmpFn cmp_fn;
  int cur_index;
  int index;
} Find;

void FindFn(void* list_elem, void* aux_data) {
  Find* find = aux_data;
  ++find->cur_index;
  if (find->index == -1 && find->cmp_fn(list_elem, find->elem)) {
    find->index = find->cur_index;
  }
}

int VectorFind(Vector* vector, void* elem, CmpFn cmp_fn) {
  Find find;
  find.elem = elem;
  find.cmp_fn = cmp_fn;
  find.cur_index = -1;
  find.index = -1;
  MapListMap(vector, FindFn, &find);
  return find.index;
}
