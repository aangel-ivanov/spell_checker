// This module provides tools suggesting words for a spellcheck

#include "cuckoofilter.h"

extern const int SUGGEST_MAX_STRLEN;

void insert_char(char *str, int index, char c);

// suggest(dict, word) creates a new SSS that contains suggestions
//   from the provided dict[ionary] for possible misspellings of word
// note: the word itself should not appear in the returned SSS
//       the returned SSS will be empty if no suggestions can be found
//         (the returned SSS must still be destroyed)
//       see assignment text for more details regarding
//         which words should be suggested
// requires: dict must be a valid cuckoo filter
//           word must be a non-empty null-terminated string [not asserted]
//           word only contains lowercase letters (or numbers) [not asserted]
//           length of word <= SUGGEST_MAX_STRLEN
// effects: allocates memory (an SSS) (you must call sss_destroy)
// time: ??? (provide this in your .c file)
struct triple_s *suggest(const struct cuckoofilter *dict, 
                         const char *word);
