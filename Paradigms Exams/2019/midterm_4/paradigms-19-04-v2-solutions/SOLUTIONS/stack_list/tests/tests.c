#include "../stack.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gl/lunit/lunit.h"

typedef long long LL;

typedef struct {
  int a;
  LL b;
} P;

P NewP(int a, LL b) {
  P p;
  p.a = a;
  p.b = b;
  return p;
}

int PCmp(void* a, void* b) {
  P* x = a;
  P* y = b;
  if (x->a != y->a) {
    return x->a - y->a;
  }
  return x->b - y->b;
}

bool PairEq(char* msg, P* actual, P expected) {
  if (PCmp(actual, &expected) == 0) {
    return true;
  }
  sprintf(msg, "Expected (%d, %lld) got (%d, %lld) instead.", expected.a,
	  expected.b, actual->a, actual->b);
  return false;
}

#define ASSERT_PAIR_EQ(actual, expected) \
  __ASSERT_EQ(PairEq, PairEqMsg, actual, expected)

// Ints

TEST(Ints_Push_Pop) {
  Stack s;
  StackInit(&s, sizeof(int));
  int a = 1, b = 2, c = 3;
  
  StackPush(&s, &a);
  ASSERT_INT_EQ(*(int*)StackPop(&s), a);

  StackPush(&s, &a);
  StackPush(&s, &b);
  ASSERT_INT_EQ(*(int*)StackPop(&s), b);  
  ASSERT_INT_EQ(*(int*)StackPop(&s), a);

  StackPush(&s, &a);
  StackPush(&s, &b);
  StackPush(&s, &c);
  ASSERT_INT_EQ(*(int*)StackPop(&s), c);
  ASSERT_INT_EQ(*(int*)StackPop(&s), b);
  ASSERT_INT_EQ(*(int*)StackPop(&s), a);
  
  StackDestroy(&s);
}

bool IntCmp(void* a, void* b) {
  return *(int*)a == *(int*)b;
}

bool LLCmp(void* a, void* b) {
  return *(LL*)a == *(LL*)b;
}

TEST(Ints_Contains) {
  Stack s;
  StackInit(&s, sizeof(int));
  int a = 1, b = 2, c = 3;
  int x = 1, y = 2, z = 3;
  
  StackPush(&s, &a);
  ASSERT_TRUE(StackContains(&s, &x, IntCmp));
  
  StackPush(&s, &b);
  ASSERT_TRUE(StackContains(&s, &x, IntCmp));
  ASSERT_TRUE(StackContains(&s, &y, IntCmp));

  StackPush(&s, &c);
  ASSERT_TRUE(StackContains(&s, &x, IntCmp));
  ASSERT_TRUE(StackContains(&s, &y, IntCmp));
  ASSERT_TRUE(StackContains(&s, &z, IntCmp));

  StackDestroy(&s);
}

TEST(Ints_Contains_Unknown) {
  Stack s;
  StackInit(&s, sizeof(int));

  int x = 5;
  ASSERT_FALSE(StackContains(&s, &x, IntCmp));
  
  int a = 1, b = 2, c = 3;
  StackPush(&s, &a);
  StackPush(&s, &b);
  StackPush(&s, &c);

  ASSERT_FALSE(StackContains(&s, &x, IntCmp));
  
  StackDestroy(&s);
}

TEST(LL_Big) {
  const int n = 20000;
  Stack s;
  StackInit(&s, sizeof(LL));

  for (LL i = 0; i < n; ++i)  {
    StackPush(&s, &i);
  }
  for (LL i = 0; i < n; ++i)  {
    ASSERT_INT_EQ(*(LL*)StackPop(&s), n - i - 1);
  }
    
  StackDestroy(&s);
}

TEST(LL_Big_Contains) {
  const int n = 20000;
  Stack s;
  StackInit(&s, sizeof(LL));

  for (LL i = n - 1; i >= 0; --i)  {
    LL x = -i;
    StackPush(&s, &x);
  }
  for (LL i = 0; i < n; ++i) {
    LL x = -i;
    ASSERT_TRUE(StackContains(&s, &x, LLCmp));
  }
    
  StackDestroy(&s);
}

TEST(LL_Big_Contains_Unknown) {
  const int n = 20000;
  Stack s;
  StackInit(&s, sizeof(LL));

  for (LL i = n - 1; i >= 0; --i)  {
    LL x = -i;
    StackPush(&s, &x);
  }
  LL x = 1;
  ASSERT_FALSE(StackContains(&s, &x, LLCmp));
    
  StackDestroy(&s);
}

// Strings

TEST(Strings_Push_Pop) {
  Stack s;
  StackInit(&s, sizeof(char*));
  char *a = "ab", *b = "cd", *c = "xy";
  
  StackPush(&s, &a);
  ASSERT_STR_EQ(*(char**)StackPop(&s), "ab");

  StackPush(&s, &b);
  StackPush(&s, &a);  
  ASSERT_STR_EQ(*(char**)StackPop(&s), "ab");
  ASSERT_STR_EQ(*(char**)StackPop(&s), "cd");  

  StackPush(&s, &c);
  StackPush(&s, &b);
  StackPush(&s, &a);  
  ASSERT_STR_EQ(*(char**)StackPop(&s), "ab");
  ASSERT_STR_EQ(*(char**)StackPop(&s), "cd");  
  ASSERT_STR_EQ(*(char**)StackPop(&s), "xy");    
  
  StackDestroy(&s);
}

bool StrCmp(void* a, void* b) {
  return strcmp(*(char**)a, *(char**)b) == 0;
}

TEST(Strings_Contains) {
  Stack s;
  StackInit(&s, sizeof(char*));
  char *a = "ab", *b = "cd", *c = "xy";
  char *x = "ab", *y = "cd", *z = "xy"; 
  
  StackPush(&s, &a);
  ASSERT_TRUE(StackContains(&s, &x, StrCmp));
  
  StackPush(&s, &b);
  ASSERT_TRUE(StackContains(&s, &x, StrCmp));
  ASSERT_TRUE(StackContains(&s, &y, StrCmp));

  StackPush(&s, &c);
  ASSERT_TRUE(StackContains(&s, &x, StrCmp));
  ASSERT_TRUE(StackContains(&s, &y, StrCmp));
  ASSERT_TRUE(StackContains(&s, &z, StrCmp));

  // Duplicate b
  char *e = "cd";
  StackPush(&s, &e);
  ASSERT_TRUE(StackContains(&s, &y, StrCmp));
  
  StackDestroy(&s);
}

TEST(Strings_Contains_Unknown) {
  Stack s;
  StackInit(&s, sizeof(char*));

  char* x = "aasdqwe";
  ASSERT_FALSE(StackContains(&s, &x, StrCmp));
  
  char *a = "ab", *b = "bc", *c = "ef";
  StackPush(&s, &a);
  StackPush(&s, &b);
  StackPush(&s, &c);

  ASSERT_FALSE(StackContains(&s, &x, StrCmp));
  
  StackDestroy(&s);
}

// Structs
			  
TEST(Structs_Push_Pop) {
  Stack s;
  StackInit(&s, sizeof(P));
  P a = NewP(1, 2);
  P b = NewP(3, 4);
  P c = NewP(3, 5);
  
  StackPush(&s, &a);
  ASSERT_PAIR_EQ(StackPop(&s), a);

  StackPush(&s, &a);  
  StackPush(&s, &b);
  ASSERT_PAIR_EQ(StackPop(&s), b);    
  ASSERT_PAIR_EQ(StackPop(&s), a);

  StackPush(&s, &a);  
  StackPush(&s, &b);
  StackPush(&s, &c);
  ASSERT_PAIR_EQ(StackPop(&s), c);
  ASSERT_PAIR_EQ(StackPop(&s), b);
  ASSERT_PAIR_EQ(StackPop(&s), a);
  
  StackDestroy(&s);
}

bool PairPtrCmp(void* a, void* b) {
  return PCmp(a, b) == 0;
}

TEST(Structs_Contains) {
  Stack s;
  StackInit(&s, sizeof(P));
  P a = NewP(1, 2);
  P b = NewP(3, 4);
  P c = NewP(3, 5);
  P x = NewP(1, 2);
  P y = NewP(3, 4);
  P z = NewP(3, 5);
  
  
  StackPush(&s, &a);
  ASSERT_TRUE(StackContains(&s, &x, PairPtrCmp));
  
  StackPush(&s, &b);
  ASSERT_TRUE(StackContains(&s, &x, PairPtrCmp));
  ASSERT_TRUE(StackContains(&s, &y, PairPtrCmp));

  StackPush(&s, &c);
  ASSERT_TRUE(StackContains(&s, &x, PairPtrCmp));
  ASSERT_TRUE(StackContains(&s, &y, PairPtrCmp));
  ASSERT_TRUE(StackContains(&s, &z, PairPtrCmp));

  // Duplicate b
  P e = NewP(3, 4);
  StackPush(&s, &e);
  ASSERT_TRUE(StackContains(&s, &y, PairPtrCmp));  
  
  StackDestroy(&s);
}

TEST(Structs_Contains_Unknown) {
  Stack s;
  StackInit(&s, sizeof(P));

  P x = NewP(5, 10);
  ASSERT_FALSE(StackContains(&s, &x, PairPtrCmp));

  P a = NewP(1, 2);
  P b = NewP(3, 4);
  P c = NewP(3, 5);
  StackPush(&s, &a);
  StackPush(&s, &b);
  StackPush(&s, &c);

  ASSERT_FALSE(StackContains(&s, &x, PairPtrCmp));
  
  StackDestroy(&s);
}

TEST(Structs_Big) {
  int n = 20000;
  P a[20000];
  
  Stack s;
  StackInit(&s, sizeof(P));

  for (int i = 0; i < n; ++i) {
    a[i] = NewP(n - i - 1, 2 * i);
    StackPush(&s, &a[i]);
  }
  for (int i = n - 1; i >= 0; --i) {
    ASSERT_PAIR_EQ(StackPop(&s), NewP(n - i - 1, 2 * i));
  }
  
  StackDestroy(&s);
}

TEST(Structs_Big_Contains) {
  int n = 20000;
  P a[20000];
  
  Stack s;
  StackInit(&s, sizeof(P));

  for (int i = 0; i < n; ++i) {
    a[i] = NewP(n - i - 1, 2 * i);
    StackPush(&s, &a[i]);
  }
  for (int i = n - 1; i >= 0; --i) {
    ASSERT_TRUE(StackContains(&s, &a[i], PairPtrCmp));
  }
  
  StackDestroy(&s);
}

TEST(Structs_Big_Contains_Unknown) {
  int n = 20000;
  P a[20000];
  
  Stack s;
  StackInit(&s, sizeof(P));

  for (int i = 0; i < n; ++i) {
    a[i] = NewP(n - i - 1, 2 * i);
    StackPush(&s, &a[i]);
  }
  P x = NewP(0, 1);
  ASSERT_FALSE(StackContains(&s, &x, PairPtrCmp));
  
  StackDestroy(&s);
}

int main(int argc, char* argv[]) {
  LUnitOpts opts;
  LUnitOptsInit(&opts, argc, argv);

  TEST_SUITE_WITH_WEIGHT(all, 1.0);
  
  ADD_TEST(all, Ints_Push_Pop);
  ADD_TEST(all, Ints_Contains);
  ADD_TEST(all, Ints_Contains_Unknown);
  ADD_TEST(all, LL_Big);
  ADD_TEST(all, LL_Big_Contains);
  ADD_TEST(all, LL_Big_Contains_Unknown);
  ADD_TEST(all, Strings_Push_Pop);
  ADD_TEST(all, Strings_Contains);
  ADD_TEST(all, Strings_Contains_Unknown);
  ADD_TEST(all, Structs_Push_Pop);
  ADD_TEST(all, Structs_Contains);
  ADD_TEST(all, Structs_Contains_Unknown);
  ADD_TEST(all, Structs_Big);
  ADD_TEST(all, Structs_Big_Contains);
  ADD_TEST(all, Structs_Big_Contains_Unknown);

  TestSuite* suites[] = {&all};
  ProcessTestSuites(1, suites, &opts);
  TestSuiteDispose(&all);  
  return 0;
}
