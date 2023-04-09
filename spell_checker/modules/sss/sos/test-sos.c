// This is an I/O Test client for the SOS module

// THERE SHOULD BE NO NEED FOR YOU TO MODIFY THIS FILE

//////////////////////////////////////////////////////////////////////////////

// .in file format:

// [WORD] can be any string (without whitespace)
// [POS] is a valid position 

//   print                       calls sos_print
//   length                      prints "length: %d\n"
//   item_at [POS]               prints "%d: %s\n"
//   dup                         calls sos_dup, then sos_print, then sos_destroy

// The following commands modify the sos and produce no output:
//   insert_end [WORD]
//   insert_at [POS] [WORD]
//   replace_at [POS] [WORD]
//   remove_at [POS]
//   remove_end
//   swap [POS] [POS]

//////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sos.h"
#include "string-io.h"

int main(void) {
  struct sos *seq = sos_create();
  char *cmd = NULL;
  char *str = NULL;
  int pos = 0;
  int pos2 = 0;
  while (1) {
    cmd = read_word();
    if (cmd == NULL) {
      break;
    } else if (!strcmp(cmd, "length")) {
      printf("length: %d\n", sos_length(seq));
    } else if (!strcmp(cmd, "item_at")) {
      if (scanf("%d", &pos) != 1) {
        break;
      }
      printf("%d: %s\n", pos, sos_item_at(seq, pos));
    } else if (!strcmp(cmd, "insert_end")) {
      str = read_word();
      if (str == NULL) {
        break;
      }
      sos_insert_end(seq, str);
      free(str);
    } else if (!strcmp(cmd, "insert_at")) {
      if (scanf("%d", &pos) != 1) {
        break;
      }
      str = read_word();
      if (str == NULL) {
        break;
      }
      sos_insert_at(seq, pos, str);
      free(str);
    } else if (!strcmp(cmd, "replace_at")) {
      if (scanf("%d", &pos) != 1) {
        break;
      }
      str = read_word();
      if (str == NULL) {
        break;
      }
      sos_replace_at(seq, pos, str);
      free(str);
    } else if (!strcmp(cmd, "remove_at")) {
      if (scanf("%d", &pos) != 1) {
        break;
      }
      sos_remove_at(seq, pos);
    } else if (!strcmp(cmd, "remove_end")) {
      sos_remove_end(seq);
    } else if (!strcmp(cmd, "swap")) {
      if (scanf("%d", &pos) != 1) {
        break;
      }
      if (scanf("%d", &pos2) != 1) {
        break;
      }      
      sos_swap(seq, pos, pos2);
    } else if (!strcmp(cmd, "print")) {
      sos_print(seq);
    } else if (!strcmp(cmd, "dup")) {
      struct sos *dup = sos_dup(seq);
      sos_print(dup);
      sos_destroy(dup);
    } else {
      break;
    }
    free(cmd);
  }
  if (cmd) {
    free(cmd);
  }
  sos_destroy(seq);
}