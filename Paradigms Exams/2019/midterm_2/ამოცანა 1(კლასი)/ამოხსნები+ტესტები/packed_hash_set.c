#include "packed_hash_set.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "gl/logging/logging.h"

#define NthElement(set, n) ((char*)(set)->base + (set)->elem_offsets[n])

void PackedHashSetInit(PackedHashSet* set, UniqueHash hash_fn, Free free_fn) {
  assert(hash_fn != NULL);
  set->base = NULL;
  set->alloc_bytes = 0;
  set->bytes_used = 0;
  set->num_elems = 0;
  set->elem_offsets = NULL;
  set->elem_hashes = NULL;
  set->free_fn = free_fn;
  set->hash_fn = hash_fn;
}

void PackedHashSetDispose(PackedHashSet* set) {
  if (set->free_fn != NULL) {
    for (int i = 0; i < set->num_elems; ++i) {
      set->free_fn(NthElement(set, i));
    }
  }
  free(set->base);
  free(set->elem_offsets);
  free(set->elem_hashes);
}

const void* PackedHashSetFind(PackedHashSet* set, const void* elem) {
  const int hash = set->hash_fn(elem);
  for (int i = 0; i < set->num_elems; ++i) {
    if (set->elem_hashes[i] == hash) {
      return NthElement(set, i);
    }
  }
  return NULL;
}

const void* PackedHashSetGet(PackedHashSet* set, int index) {
  return NthElement(set, index);
}

bool PackedHashSetInsert(PackedHashSet* set, void* elem, size_t elem_size) {
  const int hash = set->hash_fn(elem);
  for (int i = 0; i < set->num_elems; ++i) {
    if (set->elem_hashes[i] == hash) {
      return false;
    }
  }
  set->base = realloc(set->base, set->alloc_bytes + elem_size);
  assert(set->base != NULL);
  memcpy(set->base + set->alloc_bytes, elem, elem_size);
  set->elem_offsets = realloc(set->elem_offsets,
			      (set->num_elems + 1) * sizeof(size_t));
  assert(set->elem_offsets != NULL);
  set->elem_offsets[set->num_elems] = set->alloc_bytes;
  set->alloc_bytes += elem_size;
  set->elem_hashes = realloc(set->elem_hashes,
			     (set->num_elems + 1) * sizeof(int));
  assert(set->elem_hashes != NULL);
  set->elem_hashes[set->num_elems] = hash;
  ++set->num_elems;
  return true;
}

void PackedHashSetSortByHash(PackedHashSet* set) {
  for (int i = 0; i < set->num_elems; ++i) {
    int smallest = i;
    for (int j = i + 1; j < set->num_elems; ++j) {
      if (set->elem_hashes[j] < set->elem_hashes[i]) {
	smallest = j;
      }
    }
    if (i == smallest) {
      continue;
    }
    size_t smallest_elem_size = -set->elem_offsets[smallest];
    if (smallest == set->num_elems - 1) {
      smallest_elem_size += set->alloc_bytes;
    } else {
      smallest_elem_size += set->elem_offsets[smallest + 1];
    }
    void* tmp = malloc(smallest_elem_size);
    assert(tmp != NULL);
    memcpy(tmp, NthElement(set, smallest), smallest_elem_size);
    memmove(NthElement(set, i) + smallest_elem_size, NthElement(set, i),
	    set->elem_offsets[smallest] - set->elem_offsets[i]);
    memcpy(NthElement(set, i), tmp, smallest_elem_size);
    int smallest_hash = set->elem_hashes[smallest];
    for (int j = smallest; j > i; --j) {
      set->elem_offsets[j] = set->elem_offsets[j - 1] + smallest_elem_size;
      set->elem_hashes[j] = set->elem_hashes[j - 1];
    }
    set->elem_hashes[i] = smallest_hash;
  }
}

