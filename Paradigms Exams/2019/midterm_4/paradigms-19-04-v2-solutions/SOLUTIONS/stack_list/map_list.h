#ifndef PARADIGMS_PROBLEMS_VECTOR_LIST_MAP_LIST_H_
#define PARADIGMS_PROBLEMS_VECTOR_LIST_MAP_LIST_H_

#include <stddef.h>

typedef void(*MapFn)(void* list_elem, void* aux_data);

typedef struct {
  void* base_;
  size_t elem_size_;
  int log_len_;
  int alloc_len_;
} MapList;

void MapListInit(MapList* list, size_t elem_size);
void MapListDestroy(MapList* list);
void MapListAppend(MapList* list, void* elem);
void MapListRemoveLast(MapList* list);
void MapListMap(MapList* list, MapFn map_fn, void* aux_data);

#endif // PARADIGMS_PROBLEMS_VECTOR_LIST_MAP_LIST_H_
