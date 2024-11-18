#include "stack.h"

#include "map_list.h"

void StackInit(Stack* stack, size_t elem_size) {
  MapListInit(stack, elem_size);
}

void StackDestroy(Stack* stack) {
  MapListDestroy(stack);
}

void StackPush(Stack* stack, void* elem) {
  MapListAppend(stack, elem);
}

void PopFn(void* list_elem, void* aux_data) {
  *(void**)aux_data = list_elem;
}

void* StackPop(Stack* stack) {
  void* last = NULL;
  MapListMap(stack, PopFn, &last);
  MapListRemoveLast(stack);
  return last;
}

typedef struct {
  void* elem;
  CmpFn cmp_fn;
  bool found;
} Contains;

void ContainsFn(void* list_elem, void* aux_data) {
  Contains* contains = aux_data;
  if (!contains->found && contains->cmp_fn(list_elem, contains->elem)) {
    contains->found = true;
  }
}

bool StackContains(Stack* stack, void* elem, CmpFn cmp_fn) {
  Contains contains;
  contains.elem = elem;
  contains.cmp_fn = cmp_fn;
  contains.found = false;
  MapListMap(stack, ContainsFn, &contains);
  return contains.found;
}
