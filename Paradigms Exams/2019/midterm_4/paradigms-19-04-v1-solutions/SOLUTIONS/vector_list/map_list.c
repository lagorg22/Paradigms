#include "map_list.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define NTH_ELEM(list, index) ((void*)((char*)(list->base_) + (index) * (list->elem_size_)))

void MapListInit(MapList* list, size_t elem_size) {
  list->alloc_len_ = 4;
  list->base_ = malloc(list->alloc_len_ * elem_size);
  assert(list->base_ != NULL);
  list->elem_size_ = elem_size;
  list->log_len_ = 0;
}

void MapListDestroy(MapList* list) {
  if (list->base_ != NULL) {
    free(list->base_);
  }
}

void MapListAppend(MapList* list, void* elem) {
  if (list->log_len_ == list->alloc_len_) {
    list->alloc_len_ *= 2;
    list->base_ = realloc(list->base_, list->alloc_len_ * list->elem_size_);
    assert(list->base_ != NULL);
  }
  memcpy(NTH_ELEM(list, list->log_len_), elem, list->elem_size_);
  ++list->log_len_;
}

void MapListRemoveLast(MapList* list) {
  assert(list->log_len_ > 0);
  --list->log_len_;
}

void MapListMap(MapList* list, MapFn map_fn, void* aux_data) {
  assert(map_fn != NULL);
  for (int i = 0; i < list->log_len_; ++i) {
    map_fn(NTH_ELEM(list, i), aux_data);
  }
}
