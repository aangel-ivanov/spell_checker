// Implementation of a triple_s (SSS) ADT

// This is a "wrapper" ADT that provides a triple_s ADT interface, but all of the data
// is actually stored inside of an SOS ADT

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "sos.h"
#include "sss.h"

struct triple_s {
  struct sos *seq;
};

struct triple_s *sss_create(void) {
  struct triple_s *sss = malloc(sizeof(struct triple_s));
  sss->seq = sos_create();
  return sss;
}

void sss_destroy(struct triple_s *sss) {
  assert(sss);
  sos_destroy(sss->seq);
  free(sss);
}

int sss_count(const struct triple_s *sss) {
  assert(sss);
  return sos_length(sss->seq);
}

int sss_search(const struct triple_s *sss, const char *s) {
  assert(sss);
  int low = 0;
  int high = sos_length(sss->seq) - 1;
  while (low <= high) {
    int mid = low + (high - low) / 2;
    int result = strcmp(s, sos_item_at(sss->seq, mid));
    if (result == 0) {
      return mid;
    } else if (result > 0) {
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }
  return -1;
}

const char *sss_get(const struct triple_s *sss, int idx) {
  assert(sss);
  assert(idx >= 0);
  assert(idx < sss_count(sss));
  return sos_item_at(sss->seq, idx);
}

void sss_add(struct triple_s *sss, const char *s) {
  assert(sss);
  int len = sss_count(sss);
  for (int i = 0; i < len; ++i) {
    if (strcmp(sss_get(sss, i), s) == 0) {
      return;  // s already exists (this part is O(n))
    }
  }
  // get the index
  int index = 0;
  for (index = 0; (index < len) && (strcmp(sss_get(sss, index), s) < 0); ++index) {
  }
  // insert at index
  sos_insert_at(sss->seq, index, s);
}

void sss_add_follows(struct triple_s *sss, const char *s) {
  assert(sss);
  assert((sss_count(sss) == 0) || (strcmp(sss_get(sss, sss_count(sss) - 1), s) < 0));
  sos_insert_end(sss->seq, s);
}

void sss_remove(struct triple_s *sss, const char *s) {
  assert(sss);
  int idx = sss_search(sss, s);
  if (idx == -1) {
    return;
  }
  sos_remove_at(sss->seq, idx);
}