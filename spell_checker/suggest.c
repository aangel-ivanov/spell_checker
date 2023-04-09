#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "suggest.h"
#include "triple-s.h"

const int SUGGEST_MAX_STRLEN = 63;

// insert_char(str, index, c) adds character c at position index in string str
// requires str valid pointer to char and index <= strlen(str)
// time: O(n) where n is the length of str
void insert_char(char *str, int index, char c) {
  assert(str);
  assert(index <= strlen(str));
  int i = 0; 
  int len = strlen(str);
  char *temp = malloc((len + 2) * sizeof(char));
  // copy up to index
  for (; i < index; ++i) {
    temp[i] = str[i];
  }
  // insert char
  temp[index] = c;
  // copy the rest
  for (; i < len; ++i) {
    int j = i + 1;
    temp[j] = str[i];
  }
  temp[i + 1] = '\0';
  // copy back to str
  for (int k = 0; k <= len; ++k) {
    str[k] = temp[k];
  }
  free(temp);
}

// delete_char(str, index, c) adds character c at position index in string str
// requires str valid pointer to char and index < strlen(str)
// time: O(n) where n is the length of str
void delete_char(char *str, int index) {
  assert(str);
  assert(index < strlen(str));
  int len = strlen(str);
  int i = index;
  for (; i < len - 1; ++i) {
    str[i] = str[i + 1];
  }
  str[i] = '\0';
}

// sub_char(str, index, c) substitutes the character at index index with char
// requires str valid pointer to char and index < strlen(str)
// time: O(1)
void sub_char(char *str, int index, char c) {
  assert(str);
  assert(index < strlen(str));
  str[index] = c;
}

// swap_char(str, i, j) swaps the character at index i with index j
// requires str valid pointer to char and i, j < strlen(str)
// time: O(1)
void swap_chars(char* str, int i, int j) {
  assert(str);
  assert(i < strlen(str));
  assert(j < strlen(str));
  char temp = str[i];
  str[i] = str[j];
  str[j] = temp;
}

// my implementation of C function strncpy in string.h library
// time: O(n) where n is the length of src
char *mystrncpy(char *dest, const char *src, unsigned long len) {
    int j = 0;
    for (; j <= len; ++j) {
        dest[j] = src[j];
    }
    dest[j] = '\0';
    return dest;
}

// get_candidate_words(word) gets all candidate words from word
// i.e. gets all words different by a single deletion, insertion, substitution, and adjacent swap
// time: O(n^2) where n is the length of word
char **get_candidate_words(const char *word, int *num_candidates) {
  assert(word);
  assert(num_candidates);
  int len = strlen(word);

  char **candidates = malloc(4 * (27 * (len + 2)) * sizeof(char*));
  int pos = 0;

  // candidates different by an insertion
  // O(n^2)
  for (int i = 0; i <= len; ++i) {
    for (char c = 'a'; c <= 'z'; ++c) {
      char *candidate = malloc((len + 2) * sizeof(char));
      mystrncpy(candidate, word, len); // O(n)
      insert_char(candidate, i, c);    // O(n)
      candidates[pos] = candidate;
      ++pos;
    }
  }
  // candidates different by a deletion
  // O(n^2)
  for (int i = 0; i < len; ++i) {
    char *candidate = malloc( (len + 2) * sizeof(char) );
    mystrncpy(candidate, word, len);
    delete_char(candidate, i); // O(n)
    candidates[pos] = candidate;
    ++pos;
  }
  // candidates different by a substitution 
  // O(n^2)
  for (int i = 0; i < len; ++i) {
    for (char c = 'a'; c <= 'z'; ++c) {
      if (c != word[i]) {
        char *candidate = malloc((len + 2) * sizeof(char));
        mystrncpy(candidate, word, len); // O(n)
        sub_char(candidate, i, c);       // O(1)
        candidates[pos] = candidate;
        ++pos;
      }
    }
  }
  // candidates different by adjacent swap
  // O(n^2)
  for (int i = 0; i < len - 1; ++i) {
    char *candidate = malloc((len + 2) * sizeof(char));
    mystrncpy(candidate, word, len); // O(n)
    swap_chars(candidate, i, i + 1); // O(1)
    candidates[pos] = candidate;
    ++pos;
  }
  *num_candidates = --pos;
  return candidates;
}

// helper function to free allocated memeory
// time: O(n) where n is the number of candidates
void myfree(char **candidates, int num_candidates) {
  for(int i = 0; i <= num_candidates; ++i) {
    if (candidates[i]) {
      free(candidates[i]);
      candidates[i] = NULL;
    }
  }
  if(candidates) {
    free(candidates);
    candidates = NULL;
  }
}

// suggest(dict, word) creates candidate words and checks if each candidate is in dict
//  if it is then add it to the sss
// for candidate words we consider 1) deletions, 2) insertions, 3) substitutions, and 4) swaps
// time: O(n^2 * m) where n is the length of word and m is the number of hash tables in dict
struct triple_s *suggest(const struct cuckoofilter *dict, 
                         const char *word) {
  assert(dict);
  // assert(sizeof(dict) == sizeof(struct cuckoofilter));
  assert(word);
  struct triple_s *suggest = sss_create();
  int num_candidates = 0;
  char **candidates = get_candidate_words(word, &num_candidates); // O(n^2), n is the length of word
  // this for loop is O(n^2 * m) where n is the length of word and m is the number of hash tables in dict
  for (int i = 0; i < num_candidates; ++i) {
    if (cuckoo_lookup(dict, candidates[i])) { // O(m * n), where m is the number of hash tables in dict 
                                           //  and n is the length of candidates[i]
      sss_add(suggest, candidates[i]);     // O(m + n*logm) where m is number of strings in suggest 
                                           //  and n is the length of candidates[i]
    }
  }
  myfree(candidates, num_candidates);      // O(n) where n is the number of candidates
  return suggest;
}