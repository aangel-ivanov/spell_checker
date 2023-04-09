// This is an I/O Test client for the sss module

// THERE SHOULD BE NO NEED FOR YOU TO MODIFY THIS FILE

//////////////////////////////////////////////////////////////////////////////

// .in file format:

// [WORD] can be any string (without whitespace)
// [INDEX] is any valid index

// count                         prints "count: %d\n"
// add [WORD]                    adds [WORD] to the sss (does not print)
// add_follows [WORD]            adds [WORD] to the sss (does not print)
// remove [WORD]                 removes [WORD] from the sss (does not print)
// search [WORD]                 searches the sss and prints "search: %d\n"
// get [INDEX]                   prints "get: %s\n"

//////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "cs136-trace.h"
#include "string-io.h"
#include "sss.h"

int main(void) {
  const int ADD = lookup_symbol("add");
  const int ADD_FOLLOWS = lookup_symbol("add_follows");
  const int REMOVE = lookup_symbol("remove");
  const int COUNT = lookup_symbol("count");
  const int SEARCH = lookup_symbol("search");
  const int GET = lookup_symbol("get");
  
  struct triple_s *sss = sss_create();
  
  while (1) {
    int cmd = read_symbol();
    if (cmd == ADD) {
      char *s = read_word();
      if (s == NULL) {
        break;
      }
      sss_add(sss, s);
      free(s);
    } else if (cmd == ADD_FOLLOWS) {
      char *s = read_word();
      if (s == NULL) {
        break;
      }
      sss_add_follows(sss, s);
      free(s);      
    } else if (cmd == REMOVE) {
      char *s = read_word();
      if (s == NULL) {
        break;
      }
      sss_remove(sss, s);
      free(s);
    } else if (cmd == COUNT) {
      printf("count: %d\n", sss_count(sss));
    } else if (cmd == SEARCH) {
      char *s = read_word();
      if (s == NULL) {
        break;
      }
      printf("search: %d\n", sss_search(sss, s));
      free(s);
    } else if (cmd == GET) {
      int idx = 0;
      if (scanf("%d", &idx) != 1) {
        break;
      }
      printf("get: %s\n", sss_get(sss, idx));
    } else {
      break;
    }
  }
  sss_destroy(sss);
}
