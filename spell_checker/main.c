// This is an I/O test client for the suggest module
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "words.h"
#include "suggest.h"
#include "hash.h"
#include "triple-s.h"
 
/*
#define _DEBUG_PRINT

char **get_candidate_words(const char *word, int *num_candidates); 
void myfree( char **candidates, int num_candidates );

int main(void) {
    const char* myword = "keyword";
    int num_candidates = 0;

    char** candidates = get_candidate_words(myword, &num_candidates);

    #ifdef _DEBUG_PRINT
    for(int i = 0; i < num_candidates; i++) {
        printf("<%d, %s>\n", i, candidates[i]);
    }

    printf("\n");
    printf("num_candidates=%d\n", num_candidates);
    #endif

    myfree(candidates, num_candidates);
    return 0;
}
*/

int main(void) {
  int hash_length = -1;
  assert(scanf("%d", &hash_length) == 1);
  int nof_entries = -1;
  assert(scanf("%d", &nof_entries) == 1);
  int fp_length = -1;
  assert(scanf("%d", &fp_length) == 1);
  int max_num_kicks = -1;
  assert(scanf("%d", &max_num_kicks) == 1);
  assert(2 <= hash_length && hash_length <= 16);

  struct cuckoofilter *cf = words_create(hash_str, hash_length, nof_entries,
    fp_length, max_num_kicks);

  char word[64] = "";
  while (scanf("%s", word) == 1) {
    printf("%s:", word);
    bool found = cuckoo_lookup(dict, word);
    if (!found) {
      struct triple_s *suggestions = suggest(dict, word);
      if (sss_count(suggestions)) {
        for (int i = 0; i < sss_count(suggestions); ++i) {
          printf(" %s", sss_get(suggestions, i));
        }
      } else {
        printf(" NONE");
      }
      sss_destroy(suggestions);
    } else {
      printf(" OK");
    }
    printf("\n");
  }
  cuckoo_destroy(dict);
}
