#include "tuple_hash_set.h"

void CharPairInit(CharPair* pair, char first, char second) {
  pair->tuple_type = CHAR_PAIR;
  pair->first = first;
  pair->second = second;
}

void CharTripleInit(CharTriple* triple, char first, char second, char third) {
  triple->tuple_type = CHAR_TRIPLE;
  triple->first = first;
  triple->second = second;
  triple->third = third;
}

static int CharPairHash(const void* pt) {
  const CharPair* pair = pt;
  return (1 << 16) + (pair->first << 8) + pair->second;
}

static int CharTripleHash(const void* pt) {
  const CharTriple* triple = pt;
  return (1 << 24) + (triple->first << 16) + (triple->second << 8) +
    triple->third;
}

static int TupleHash(const void* pt) {
  switch (*(TupleType*)pt) {
  case CHAR_PAIR: return CharPairHash(pt);
  case CHAR_TRIPLE: return CharTripleHash(pt);    
  }
}

void TupleHashSetInit(TupleHashSet* set) {
  PackedHashSetInit(set, TupleHash, /*free_fn=*/NULL);
}

void TupleHashSetDispose(TupleHashSet* set) {
  PackedHashSetDispose(set);
}

bool TupleHashSetInsertCharPair(TupleHashSet* set, CharPair* pair) {
  return PackedHashSetInsert(set, pair, sizeof(CharPair));
}

bool TupleHashSetInsertCharTriple(TupleHashSet* set, CharTriple* triple) {
  return PackedHashSetInsert(set, triple, sizeof(CharTriple));
}

const void* TupleHashSetGet(TupleHashSet* set, int index) {
  return PackedHashSetGet(set, index);
}

const void* TupleHashSetFind(TupleHashSet* set, const void* elem) {
  return PackedHashSetFind(set, elem);
}

void TupleHashSetSort(TupleHashSet* set) {
  PackedHashSetSortByHash(set);
}
