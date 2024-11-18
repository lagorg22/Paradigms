#include "sorted_multi_set.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>


void SortedMultiSetInit(SortedMultiSet* set, size_t elem_size, CmpFn cmp_fn, FreeFn free_fn){
    set->alloc_len = 20;
    set->cmp_fn = cmp_fn;
    set->free_fn = free_fn;
    set->log_len = 0;
    set->elems = malloc((elem_size + sizeof(int)) * set->alloc_len);
    assert(set->elems);
    for(int i = 0; i < set->alloc_len; ++i){
        int* curr_freq = (int*)((char*)set->elems + (elem_size + sizeof(int)));
        (*curr_freq) = 0;
    }
}

void SortedMultiSetDestroy(SortedMultiSet* set){
    if(set->free_fn){
        for(int i = 0; i < set->log_len; ++i){
            void* curr_elem = (char*)set->elems + i * (set->elem_size + sizeof(int)) + sizeof(int);
            set->free_fn(curr_elem);
        }
    }
    free(set->elems);
}

int find(SortedMultiSet* set, void* elem){
    for(int i = 0; i < set->log_len; ++i){
        void* curr_elem = (char*)set->elems + i * (set->elem_size + sizeof(int)) + sizeof(int);
        if(!set->cmp_fn(elem, curr_elem)) return i;
    }
    return -1;
}

void SortedMultiSetInsert(SortedMultiSet* set, void* elem){
    int target_index = find(set, elem);
    if(target_index != -1){
        int* curr_freq = (int*)((char*)set->elems + target_index * (set->elem_size + sizeof(int)));
        (*curr_freq)++;
        return;
    }

    if(set->log_len == set->alloc_len){
        set->alloc_len *= 2;
        set->elems = realloc(set->elems, set->alloc_len * (set->elem_size + sizeof(int)));
        assert(set->elems);
    }

    for(int i = 0; i < set->log_len; ++i){
        void* curr_elem = (char*)set->elems + i * (set->elem_size + sizeof(int));
        if(set->cmp_fn(elem, curr_elem) < 0){
            void* src = (char*)curr_elem - sizeof(int);
            void* dst = (char*)src + set->elem_size + sizeof(int);
            int size_to_move = (set->log_len - i) * (set->elem_size + sizeof(int));
            memmove(dst, src, size_to_move);
            memcpy(curr_elem, elem, set->elem_size);
            int* curr_freq = (int*)((char*)src - sizeof(int));
            *curr_freq = 1;
            set->log_len++;
            return;
        }
    }
    void* dst = (char*)set->elems + set->log_len * (set->elem_size + sizeof(int));
    int* dst_freq = (int*)((char*)dst - sizeof(int));
    *dst_freq = 1;
    memcpy(dst, elem, set->elem_size);
    set->log_len++;
}

void SortedMultiSetRemove(SortedMultiSet* set, void* elem){

}

void* SortedMultiSetGet(SortedMultiSet* set, int index){

    return NULL;
}

int SortedMultiSetGetCount(SortedMultiSet* set, int index){
    return 0;
}

int SortedMultiSetSize(SortedMultiSet* set){
    return 0;
}