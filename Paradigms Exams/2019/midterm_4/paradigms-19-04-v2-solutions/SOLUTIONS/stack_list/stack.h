#ifndef PARADIGMS_PROBLEMS_STACK_LIST_STACK_H_
#define PARADIGMS_PROBLEMS_STACK_LIST_STACK_H_

#include "map_list.h"

#include <stdbool.h>

typedef bool(*CmpFn)(void* a, void* b);

typedef MapList Stack;

void StackInit(Stack* stack, size_t elem_size);
void StackDestroy(Stack* stack);
void StackPush(Stack* stack, void* elem);
void* StackPop(Stack* stack);
bool StackContains(Stack* stack, void* elem, CmpFn cmp_fn);

#endif // PARADIGMS_PROBLEMS_STACK_LIST_STACK_H_
