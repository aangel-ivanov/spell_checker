// This is the implementation of the sequence of strings (sos) ADT

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "string-io.h"
#include "sos.h"

struct sos {
  int len;
  int maxlen;
  char **data;
};


// see sos.h
struct sos *sos_read(void) {
  struct sos *seq = sos_create();
  while (1) {
    char *line = read_line();
    if (line == NULL) {
      break;
    }
    sos_insert_end(seq, line);
    free(line);
  }
  return seq;
}

struct sos *sos_create(void) {
  struct sos *s = malloc(sizeof(struct sos));
  s->len = 0;
  s->maxlen = 1;
  s->data = malloc(s->maxlen * sizeof(char *));
  return s;
}


void sos_destroy(struct sos *seq) {
  assert(seq);
  int len = seq->len;
  for (int i = 0; i < len; ++i) {
    free(seq->data[i]);
  }
  free(seq->data);
  free(seq);
}


int sos_length(const struct sos *seq) {
  assert(seq);
  return seq->len;
}


void sos_insert_end(struct sos *seq, const char *s) {
  assert(seq);
  if (seq->len == seq->maxlen) {
    seq->maxlen *= 2;
    seq->data = realloc(seq->data, seq->maxlen * sizeof(char *));
  }
  seq->data[seq->len] = malloc(strlen(s) + 1);
  strcpy(seq->data[seq->len], s);
  seq->len += 1;
}


void sos_print(const struct sos *seq) {
  assert(seq);
  int len = seq->len;
  for (int i = 0; i < len; ++i) {
    for (int j = 0; (seq->data[i])[j] != '\0'; ++j) {
      printf("%c", (seq->data[i])[j]);
    }
    printf("\n");
  }
}

const char *sos_item_at(const struct sos *seq, int pos) {
  assert(seq);
  assert(pos >= 0);
  assert(pos < sos_length(seq));
  return seq->data[pos];
}


void sos_remove_end(struct sos *seq) {
  assert(seq);
  assert(0 < sos_length(seq));
  free(seq->data[seq->len - 1]);
  seq->len -= 1;
}


void sos_insert_at(struct sos *seq, int pos, const char *s) {
  assert(seq);
  assert(pos >= 0);
  assert(pos < sos_length(seq));

  if (seq->len == seq->maxlen) {
    seq->maxlen *= 2;
    seq->data = realloc(seq->data, seq->maxlen * sizeof(char *));
  }
  for (int i = seq->len; i > pos; --i) {
    seq->data[i] = seq->data[i - 1];
  }
  seq->data[pos] = malloc(strlen(s) + 1);
  strcpy(seq->data[pos], s);
  seq->len += 1;
}

void sos_remove_at(struct sos *seq, int pos) {
  assert(seq);
  assert(pos >= 0);
  assert(pos < sos_length(seq));
  free(seq->data[pos]);
  int len = seq->len - 1;
  for (int i = pos; i < len; ++i) {
    seq->data[i] = seq->data[i + 1];
  }
  seq->len -= 1;
}


void sos_replace_at(struct sos *seq, int pos, const char *s) {
  assert(seq);
  assert(pos >= 0);
  assert(pos < sos_length(seq));
  free(seq->data[pos]);
  seq->data[pos] = malloc(strlen(s) + 1);
  strcpy(seq->data[pos], s);
}


void sos_swap(struct sos *seq, int pos1, int pos2) {
  assert(seq);
  assert(pos1 >= 0);
  assert(pos1 < sos_length(seq));
  assert(pos2 >= 0);
  assert(pos2 < sos_length(seq));
  if (pos1 == pos2) {
    return;
  }
  char *temp = seq->data[pos1];
  seq->data[pos1] = seq->data[pos2];
  seq->data[pos2] = temp;
}


struct sos *sos_dup(const struct sos *seq) {
  assert(seq);
  struct sos *duplicate = malloc(sizeof(struct sos));
  duplicate->len = seq->len;
  duplicate->maxlen = seq->maxlen;
  duplicate->data = malloc(duplicate->maxlen * sizeof(char *));
  int len = duplicate->len;
  for (int i = 0; i < len; ++i) {
    duplicate->data[i] = malloc(strlen(seq->data[i]) + 1);
    strcpy(duplicate->data[i], seq->data[i]);
  }
  return duplicate;
}
