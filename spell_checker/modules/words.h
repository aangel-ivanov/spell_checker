// This module provides a collection of about 9,200 words stored in a counting
//   Bloom filter

#include "countingbloomfilter.h"

// words_create() creates a new counting Bloom filter that stores about 9,200
//   English words
// effects: allocates memory (you must call cbf_destroy)
// time: O(n) where n is the number of hash tables in the counting Bloom filter
struct countingbloomfilter *words_create(int (**hash_funcs)(const char *, int),
  int hash_len, int nof_tables);
