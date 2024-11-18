#include "packed_list.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define NthElement(list, n) ((char*)(list)->base + (list)->elem_offsets[n])

void PackedListInit(PackedList* list, Free free_fn) {
  list->base = NULL;
  list->alloc_bytes = 0;
  list->bytes_used = 0;
  list->num_elems = 0;
  list->elem_offsets = NULL;
  list->free_fn = free_fn;
}

void PackedListDispose(PackedList* list) {
  if (list->free_fn != NULL) {
    for (int i = 0; i < list->num_elems; ++i) {
      list->free_fn(NthElement(list, i));
    }
  }
  free(list->base);
  free(list->elem_offsets);
}

const void* PackedListGet(const PackedList* list, int index) {
  return NthElement(list, index);
}

const void* PackedListAppend(PackedList* list, void* elem, size_t elem_size) {
  list->base = realloc(list->base, list->alloc_bytes + elem_size);
  assert(list->base != NULL);
  memcpy(list->base + list->alloc_bytes, elem, elem_size);
  list->elem_offsets = realloc(list->elem_offsets,
			       (list->num_elems + 1) * sizeof(size_t));
  assert(list->elem_offsets != NULL);
  list->elem_offsets[list->num_elems] = list->alloc_bytes;
  list->alloc_bytes += elem_size;
  ++list->num_elems;
  return NthElement(list, list->num_elems - 1);
}

void PackedListSort(PackedList* list, CompareFn cmp) {
  for (int i = 0; i < list->num_elems; ++i) {
    int smallest = i;
    for (int j = i + 1; j < list->num_elems; ++j) {
      if (cmp(NthElement(list, j), NthElement(list, smallest)) < 0) {
	smallest = j;
      }
    }
    if (i == smallest) {
      continue;
    }
    size_t smallest_elem_size = -list->elem_offsets[smallest];
    if (smallest == list->num_elems - 1) {
      smallest_elem_size += list->alloc_bytes;
    } else {
      smallest_elem_size += list->elem_offsets[smallest + 1];
    }
    void* tmp = malloc(smallest_elem_size);
    assert(tmp != NULL);
    memcpy(tmp, NthElement(list, smallest), smallest_elem_size);
    memmove(NthElement(list, i) + smallest_elem_size, NthElement(list, i),
	    list->elem_offsets[smallest] - list->elem_offsets[i]);
    memcpy(NthElement(list, i), tmp, smallest_elem_size);
    for (int j = smallest; j > i; --j) {
      list->elem_offsets[j] = list->elem_offsets[j - 1] + smallest_elem_size;
    }
  }
}
