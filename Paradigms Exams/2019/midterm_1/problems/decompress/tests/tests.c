#include "../decompress.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gl/lunit/lunit.h"

typedef struct {
  void* data;
  size_t len;
  int rep;
} Seg;

typedef struct {
  char* data;
  char* dec;
  size_t dec_size;
} TestData;

void TestDataInit(TestData* d, void* data, void* dec, size_t dec_size) {
  d->data = data;
  d->dec = dec;
  d->dec_size = dec_size;
}

TestData TestDataNew(void* data, void* dec, size_t dec_size) {
  TestData test;
  TestDataInit(&test, data, dec, dec_size);
  return test;
}

void TestDataDispose(TestData* d) {
  free(d->data);
  free(d->dec);
}

void* E(int n, Seg items[]) {
  size_t len = 1;
  for (int i = 0; i < n; ++i) {
    len += items[i].len + 1;
  }
  void* ret = malloc((int)len);
  void* pt = ret;
  for (int i = 0; i < n; ++i) {
    sprintf((char*)pt, "%c", (int)(items[i].len << 4) + items[i].rep);
    pt++;
    memcpy(pt, items[i].data, items[i].len);
    pt += items[i].len;
  }
  *(char*)pt = '\0';
  return ret;
}

char* D(int n, Seg items[]) {
  size_t len = 1;
  for (int i = 0; i < n; ++i) {
    len += items[i].len * items[i].rep;
  }
  void* ret = malloc(len);
  void* pt = ret;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < items[i].rep; ++j) {
      memcpy(pt, items[i].data, items[i].len);
      pt += items[i].len;
    }
  }
  *(char*)pt = '\0';
  return ret;
}

size_t L(int n, Seg items[]) {
  size_t len = 0;
  for (int i = 0; i < n; ++i) {
    len += items[i].len * items[i].rep;
  }
  return len;
}

TestData SampleData() {
  Seg items[] = {{"a", 1, 2}, {"bc", 2, 3}, {"x", 1, 1}};
  return TestDataNew(E(3, items), D(3, items), L(3, items));
}

TestData NoRepData() {
  Seg items[] = {{"a", 1, 1}, {"b", 1, 1}, {"c", 1, 1}};
  return TestDataNew(E(3, items), D(3, items), L(3, items));
}

TestData SingleLetterHighRepData() {
  Seg items[] = {{"a", 1, 15}, {"b", 1, 14}, {"c", 1, 13}, {"d", 1, 12}};
  return TestDataNew(E(4, items), D(4, items), L(4, items));
}

TestData LongDataNoRepData() {
  Seg items[] = {{"abcdefghijklmno", 15, 1}, {"abcdefghijklmn", 14, 1},
		 {"abcdefghijklm", 13, 1}};
  return TestDataNew(E(3, items), D(3, items), L(3, items));
}

TestData ZerosData() {
  Seg items[] = {{"\0", 1, 1}, {"\0", 1, 15}};
  return TestDataNew(E(2, items), D(2, items), L(2, items));
}

TestData FullAsciiNoRepData() {
  int num_items = 255;
  Seg items[255];
  for (int i = 0; i < num_items; ++i) {
    items[i] = (Seg){malloc(2), 1, 1};
    sprintf(items[i].data, "%c", i + 1);
  }
  TestData ret = TestDataNew(E(num_items, items), D(num_items, items),
			     L(num_items, items));
  for (int i = 0; i < num_items; ++i) {
    free(items[i].data);
  }
  return ret;
}

// Prefix

TEST(Sample) {
  TestData d = SampleData();
  Decompress(&d.data);
  ASSERT_MEM_EQ(d.data, d.dec, d.dec_size);
  TestDataDispose(&d);
}

TEST(NoRep) {
  TestData d = NoRepData();
  Decompress(&d.data);
  ASSERT_MEM_EQ(d.data, d.dec, d.dec_size);
  TestDataDispose(&d);
}

TEST(SingleLetterHighRep) {
  TestData d = SingleLetterHighRepData();
  Decompress(&d.data);
  ASSERT_MEM_EQ(d.data, d.dec, d.dec_size);
  TestDataDispose(&d);
}

TEST(LongDataNoRep) {
  TestData d = LongDataNoRepData();
  Decompress(&d.data);
  ASSERT_MEM_EQ(d.data, d.dec, d.dec_size);
  TestDataDispose(&d);
}

TEST(Zeros) {
  TestData d = ZerosData();
  Decompress(&d.data);
  ASSERT_MEM_EQ(d.data, d.dec, d.dec_size);
  TestDataDispose(&d);
}

TEST(FullAsciiNoRep) {
  TestData d = FullAsciiNoRepData();
  Decompress(&d.data);
  ASSERT_MEM_EQ(d.data, d.dec, d.dec_size);
  TestDataDispose(&d);
}

// Full

TEST(SampleFull) {
  TestData d = SampleData();
  Decompress(&d.data);
  ASSERT_MEM_EQ(d.data, d.dec, d.dec_size + 1);
  TestDataDispose(&d);
}

TEST(NoRepFull) {
  TestData d = NoRepData();
  Decompress(&d.data);
  ASSERT_MEM_EQ(d.data, d.dec, d.dec_size + 1);
  TestDataDispose(&d);
}

TEST(SingleLetterHighRepFull) {
  TestData d = SingleLetterHighRepData();
  Decompress(&d.data);
  ASSERT_MEM_EQ(d.data, d.dec, d.dec_size + 1);
  TestDataDispose(&d);
}

TEST(LongDataNoRepFull) {
  TestData d = LongDataNoRepData();
  Decompress(&d.data);
  ASSERT_MEM_EQ(d.data, d.dec, d.dec_size + 1);
  TestDataDispose(&d);
}

TEST(ZerosFull) {
  TestData d = ZerosData();
  Decompress(&d.data);
  ASSERT_MEM_EQ(d.data, d.dec, d.dec_size + 1);
  TestDataDispose(&d);
}

TEST(FullAsciiNoRepFull) {
  TestData d = FullAsciiNoRepData();
  Decompress(&d.data);
  ASSERT_MEM_EQ(d.data, d.dec, d.dec_size + 1);
  TestDataDispose(&d);
}

int main(int argc, char* argv[]) {
  LUnitOpts opts;
  LUnitOptsInit(&opts, argc, argv);
  
  TEST_SUITE_WITH_WEIGHT(prefix, 0.9);
  TEST_SUITE_WITH_WEIGHT(full, 0.05);
  TEST_SUITE_WITH_WEIGHT(zeros, 0.05);  
  
  ADD_TEST(prefix, Sample);
  ADD_TEST(prefix, NoRep);
  ADD_TEST(prefix, SingleLetterHighRep);
  ADD_TEST(prefix, LongDataNoRep);
  ADD_TEST(prefix, FullAsciiNoRep);

  ADD_TEST(full, SampleFull);
  ADD_TEST(full, NoRepFull);
  ADD_TEST(full, SingleLetterHighRepFull);
  ADD_TEST(full, LongDataNoRepFull);
  ADD_TEST(full, FullAsciiNoRepFull);

  ADD_TEST(zeros, Zeros);
  ADD_TEST(zeros, ZerosFull);
  
  TestSuite* suites[] = {&prefix, &full, &zeros};
  ProcessTestSuites(3, suites, &opts);
  TestSuiteDispose(&prefix);
  TestSuiteDispose(&full);
  TestSuiteDispose(&zeros);
  return 0;
}
