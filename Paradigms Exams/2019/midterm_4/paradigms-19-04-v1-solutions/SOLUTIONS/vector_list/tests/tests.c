#include "../vector.h"

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

TEST(Ints_Append_Get) {
  Vector v;
  VectorInit(&v, sizeof(int));
  int a = 1, b = 2, c = 3;
  
  VectorAppend(&v, &a);
  ASSERT_INT_EQ(*(int*)VectorGet(&v, 0), a);
  
  VectorAppend(&v, &b);
  ASSERT_INT_EQ(*(int*)VectorGet(&v, 0), a);
  ASSERT_INT_EQ(*(int*)VectorGet(&v, 1), b);
  
  VectorAppend(&v, &c);
  ASSERT_INT_EQ(*(int*)VectorGet(&v, 0), a);
  ASSERT_INT_EQ(*(int*)VectorGet(&v, 1), b);
  ASSERT_INT_EQ(*(int*)VectorGet(&v, 2), c);
  
  VectorDestroy(&v);
}

bool IntCmp(void* a, void* b) {
  return *(int*)a == *(int*)b;
}

bool LLCmp(void* a, void* b) {
  return *(LL*)a == *(LL*)b;
}

TEST(Ints_Find) {
  Vector v;
  VectorInit(&v, sizeof(int));
  int a = 1, b = 2, c = 3;
  int x = 1, y = 2, z = 3;
  
  VectorAppend(&v, &a);
  ASSERT_INT_EQ(VectorFind(&v, &x, IntCmp), 0);
  
  VectorAppend(&v, &b);
  ASSERT_INT_EQ(VectorFind(&v, &x, IntCmp), 0);
  ASSERT_INT_EQ(VectorFind(&v, &y, IntCmp), 1);

  VectorAppend(&v, &c);
  ASSERT_INT_EQ(VectorFind(&v, &x, IntCmp), 0);
  ASSERT_INT_EQ(VectorFind(&v, &y, IntCmp), 1);
  ASSERT_INT_EQ(VectorFind(&v, &z, IntCmp), 2);

  // Duplicate b
  VectorAppend(&v, &b);
  ASSERT_INT_EQ(*(int*)VectorGet(&v, 1), b);
  ASSERT_INT_EQ(VectorFind(&v, &y, IntCmp), 1);
  
  VectorDestroy(&v);
}

TEST(Ints_Find_Unknown) {
  Vector v;
  VectorInit(&v, sizeof(int));

  int x = 5;
  ASSERT_INT_EQ(VectorFind(&v, &x, IntCmp), -1);
  
  int a = 1, b = 2, c = 3;
  VectorAppend(&v, &a);
  VectorAppend(&v, &b);
  VectorAppend(&v, &c);

  ASSERT_INT_EQ(VectorFind(&v, &x, IntCmp), -1);
  
  VectorDestroy(&v);
}

TEST(LL_Big) {
  const int n = 20000;
  Vector v;
  VectorInit(&v, sizeof(LL));

  for (LL i = 0; i < n; ++i)  {
    VectorAppend(&v, &i);
    ASSERT_INT_EQ(*(LL*)VectorGet(&v, i), i);
  }
    
  VectorDestroy(&v);
}

TEST(LL_Big_Find) {
  const int n = 20000;
  Vector v;
  VectorInit(&v, sizeof(LL));

  for (LL i = n - 1; i >= 0; --i)  {
    LL x = -i;
    VectorAppend(&v, &x);
  }
  for (LL i = 0; i < n; ++i) {
    LL x = -i;
    ASSERT_INT_EQ(VectorFind(&v, &x, LLCmp), n - i - 1);
  }
    
  VectorDestroy(&v);
}

TEST(LL_Big_Find_Unknown) {
  const int n = 20000;
  Vector v;
  VectorInit(&v, sizeof(LL));

  for (LL i = n - 1; i >= 0; --i)  {
    LL x = -i;
    VectorAppend(&v, &x);
  }
  LL x = 1;
  ASSERT_INT_EQ(VectorFind(&v, &x, LLCmp), -1);
    
  VectorDestroy(&v);
}

// Strings

TEST(Strings_Append_Get) {
  Vector v;
  VectorInit(&v, sizeof(char*));
  char *a = "ab", *b = "cd", *c = "xy";
  
  VectorAppend(&v, &a);
  ASSERT_STR_EQ(*(char**)VectorGet(&v, 0), "ab");
  
  VectorAppend(&v, &b);
  ASSERT_STR_EQ(*(char**)VectorGet(&v, 0), "ab");
  ASSERT_STR_EQ(*(char**)VectorGet(&v, 1), "cd");  
  
  VectorAppend(&v, &c);
  ASSERT_STR_EQ(*(char**)VectorGet(&v, 0), "ab");
  ASSERT_STR_EQ(*(char**)VectorGet(&v, 1), "cd");  
  ASSERT_STR_EQ(*(char**)VectorGet(&v, 2), "xy");    
  
  VectorDestroy(&v);
}

bool StrCmp(void* a, void* b) {
  return strcmp(*(char**)a, *(char**)b) == 0;
}

TEST(Strings_Find) {
  Vector v;
  VectorInit(&v, sizeof(char*));
  char *a = "ab", *b = "cd", *c = "xy";
  char *x = "ab", *y = "cd", *z = "xy"; 
  
  VectorAppend(&v, &a);
  ASSERT_INT_EQ(VectorFind(&v, &x, StrCmp), 0);
  
  VectorAppend(&v, &b);
  ASSERT_INT_EQ(VectorFind(&v, &x, StrCmp), 0);
  ASSERT_INT_EQ(VectorFind(&v, &y, StrCmp), 1);

  VectorAppend(&v, &c);
  ASSERT_INT_EQ(VectorFind(&v, &x, StrCmp), 0);
  ASSERT_INT_EQ(VectorFind(&v, &y, StrCmp), 1);
  ASSERT_INT_EQ(VectorFind(&v, &z, StrCmp), 2);

  // Duplicate b
  char *e = "cd";
  VectorAppend(&v, &e);
  ASSERT_STR_EQ(*(char**)VectorGet(&v, 1), "cd");
  ASSERT_INT_EQ(VectorFind(&v, &y, StrCmp), 1);
  
  VectorDestroy(&v);
}

TEST(Strings_Find_Unknown) {
  Vector v;
  VectorInit(&v, sizeof(char*));

  char* x = "aasdqwe";
  ASSERT_INT_EQ(VectorFind(&v, &x, StrCmp), -1);
  
  char *a = "ab", *b = "bc", *c = "ef";
  VectorAppend(&v, &a);
  VectorAppend(&v, &b);
  VectorAppend(&v, &c);

  ASSERT_INT_EQ(VectorFind(&v, &x, StrCmp), -1);
  
  VectorDestroy(&v);
}

// Structs
			  
TEST(Structs_Append_Get) {
  Vector v;
  VectorInit(&v, sizeof(P));
  P a = NewP(1, 2);
  P b = NewP(3, 4);
  P c = NewP(3, 5);
  
  VectorAppend(&v, &a);
  ASSERT_PAIR_EQ(VectorGet(&v, 0), a);
  
  VectorAppend(&v, &b);
  ASSERT_PAIR_EQ(VectorGet(&v, 0), a);
  ASSERT_PAIR_EQ(VectorGet(&v, 1), b);  
  
  VectorAppend(&v, &c);
  ASSERT_PAIR_EQ(VectorGet(&v, 0), a);
  ASSERT_PAIR_EQ(VectorGet(&v, 1), b);  
  ASSERT_PAIR_EQ(VectorGet(&v, 2), c);    
  
  VectorDestroy(&v);
}

bool PairPtrCmp(void* a, void* b) {
  return PCmp(a, b) == 0;
}

TEST(Structs_Find) {
  Vector v;
  VectorInit(&v, sizeof(P));
  P a = NewP(1, 2);
  P b = NewP(3, 4);
  P c = NewP(3, 5);
  P x = NewP(1, 2);
  P y = NewP(3, 4);
  P z = NewP(3, 5);
  
  
  VectorAppend(&v, &a);
  ASSERT_INT_EQ(VectorFind(&v, &x, PairPtrCmp), 0);
  
  VectorAppend(&v, &b);
  ASSERT_INT_EQ(VectorFind(&v, &x, PairPtrCmp), 0);
  ASSERT_INT_EQ(VectorFind(&v, &y, PairPtrCmp), 1);

  VectorAppend(&v, &c);
  ASSERT_INT_EQ(VectorFind(&v, &x, PairPtrCmp), 0);
  ASSERT_INT_EQ(VectorFind(&v, &y, PairPtrCmp), 1);
  ASSERT_INT_EQ(VectorFind(&v, &z, PairPtrCmp), 2);

  // Duplicate b
  P e = NewP(3, 4);
  VectorAppend(&v, &e);
  ASSERT_PAIR_EQ(VectorGet(&v, 1), NewP(3, 4));
  ASSERT_INT_EQ(VectorFind(&v, &y, PairPtrCmp), 1);  
  
  VectorDestroy(&v);
}

TEST(Structs_Find_Unknown) {
  Vector v;
  VectorInit(&v, sizeof(P));

  P x = NewP(5, 10);
  ASSERT_INT_EQ(VectorFind(&v, &x, PairPtrCmp), -1);

  P a = NewP(1, 2);
  P b = NewP(3, 4);
  P c = NewP(3, 5);
  VectorAppend(&v, &a);
  VectorAppend(&v, &b);
  VectorAppend(&v, &c);

  ASSERT_INT_EQ(VectorFind(&v, &x, PairPtrCmp), -1);
  
  VectorDestroy(&v);
}

TEST(Structs_Big) {
  int n = 20000;
  P a[20000];
  
  Vector v;
  VectorInit(&v, sizeof(P));

  for (int i = 0; i < n; ++i) {
    a[i] = NewP(n - i - 1, 2 * i);
    VectorAppend(&v, &a[i]);
  }
  for (int i = n - 1; i >= 0; --i) {
    ASSERT_PAIR_EQ(VectorGet(&v, i), NewP(n - i - 1, 2 * i));
  }
  
  VectorDestroy(&v);
}

TEST(Structs_Big_Find) {
  int n = 20000;
  P a[20000];
  
  Vector v;
  VectorInit(&v, sizeof(P));

  for (int i = 0; i < n; ++i) {
    a[i] = NewP(n - i - 1, 2 * i);
    VectorAppend(&v, &a[i]);
  }
  for (int i = n - 1; i >= 0; --i) {
    ASSERT_INT_EQ(VectorFind(&v, &a[i], PairPtrCmp), i);
  }
  
  VectorDestroy(&v);
}

TEST(Structs_Big_Find_Unknown) {
  int n = 20000;
  P a[20000];
  
  Vector v;
  VectorInit(&v, sizeof(P));

  for (int i = 0; i < n; ++i) {
    a[i] = NewP(n - i - 1, 2 * i);
    VectorAppend(&v, &a[i]);
  }
  P x = NewP(0, 1);
  ASSERT_INT_EQ(VectorFind(&v, &x, PairPtrCmp), -1);
  
  VectorDestroy(&v);
}

int main(int argc, char* argv[]) {
  LUnitOpts opts;
  LUnitOptsInit(&opts, argc, argv);

  TEST_SUITE_WITH_WEIGHT(all, 1.0);
  
  ADD_TEST(all, Ints_Append_Get);
  ADD_TEST(all, Ints_Find);
  ADD_TEST(all, Ints_Find_Unknown);
  ADD_TEST(all, LL_Big);
  ADD_TEST(all, LL_Big_Find);
  ADD_TEST(all, LL_Big_Find_Unknown);
  ADD_TEST(all, Strings_Append_Get);
  ADD_TEST(all, Strings_Find);
  ADD_TEST(all, Strings_Find_Unknown);
  ADD_TEST(all, Structs_Append_Get);
  ADD_TEST(all, Structs_Find);
  ADD_TEST(all, Structs_Find_Unknown);
  ADD_TEST(all, Structs_Big);
  ADD_TEST(all, Structs_Big_Find);
  ADD_TEST(all, Structs_Big_Find_Unknown);

  TestSuite* suites[] = {&all};
  ProcessTestSuites(1, suites, &opts);
  TestSuiteDispose(&all);  
  return 0;
}
