// This is the implementation of the cuckoo filter ADT.

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "cs136-trace.h"
#include "cuckoofilter.h"
#include "random.h"

// -----------------------------------------------------------------------
// DO NOT CHANGE THESE
// See cuckoofilter.h for documentation
const int CUCKOO_SUCCESS = 0;
const int CUCKOO_FAILURE = 1;
// -----------------------------------------------------------------------

struct bucket {
  char **entries; // array of strings
};

struct hashtable {
  int hash_len;
  int (*hash_func)(const char *, int);
  int nof_entries;
  struct bucket **buckets; // array of buckets
};

// See cuckoofilter.h for documentation
struct cuckoofilter {
  int fp_len;
  int max_num_kicks;
  struct hashtable *ht;
};

// See cuckoofilter.h for documentation
struct cuckoofilter *cuckoo_create(int (*hash_func)(const char *, int), 
                                  int hash_len, int nof_entries, int fp_len, 
                                  int max_num_kicks) {
  assert(hash_func);
  assert(hash_len > 0);
  assert(nof_entries > 0);
  assert(fp_len > 0);
  assert(max_num_kicks > 0);
  
  struct cuckoofilter *cf = malloc(sizeof(struct cuckoofilter));
  cf->fp_len = fp_len;
  cf->max_num_kicks = max_num_kicks;
  cf->ht = malloc(sizeof(struct hashtable));
  struct hashtable *ht = cf->ht;
  ht->hash_len = hash_len;
  ht->hash_func = hash_func;
  ht->nof_entries = nof_entries;
  int nof_buckets = 1 << hash_len; // 2^hash_len
  
  ht->buckets = malloc(nof_buckets * sizeof(struct bucket));
  for (int i = 0; i < nof_buckets; ++i) {
    ht->buckets[i] = malloc(sizeof(struct bucket));
    ht->buckets[i]->entries = malloc(nof_entries * sizeof(char *));
    for (int j = 0; j < ht->nof_entries; ++j) {
      ht->buckets[i]->entries[j] = NULL;
    }
  }
  return cf;
}

// See cuckoofilter.h for documentation
void cuckoo_destroy(struct cuckoofilter *cf) {
  assert(cf);
  struct hashtable *ht = cf->ht;
  int nof_buckets = 1 << ht->hash_len; // 2^hash_len
  for (int i = 0; i < nof_buckets; ++i) {
    // free(ht->buckets[i]);
    for (int j = 0; j < ht->nof_entries; ++j) {
      if (ht->buckets[i]->entries[j]) {
        free(ht->buckets[i]->entries[j]); ht->buckets[i]->entries[j] = NULL;
      }
    }
    if (ht->buckets[i]->entries) {
      free(ht->buckets[i]->entries); ht->buckets[i]->entries = NULL;
    }
    if (ht->buckets[i]) {
      free(ht->buckets[i]); ht->buckets[i] = NULL;
    }
  }
  free(ht->buckets); ht->buckets = NULL;
  free(ht); ht = NULL;
  free(cf); cf = NULL;
}

// See cuckoofilter.h for documentation
int cuckoo_insert(struct cuckoofilter *cf, const char *key) {
  assert(cf);
  assert(key);
  
  // get the fingerprint
  int key_len = strlen(key);
  int fp_len = cf->fp_len;
  char *fp = malloc(fp_len + 1);
  if (key_len >= fp_len) {
    strncpy(fp, key, fp_len);
    fp[fp_len] = '\0';
  } else {
    strncpy(fp, key, key_len);
    fp[fp_len] = '\0';
    fp = realloc(fp, (key_len + 1));
  }
  // printf("fp is %s\n", fp);
  struct hashtable *ht = cf->ht;
  // get the first index
  int idx_1 = ht->hash_func(key, ht->hash_len);
  // printf("idx_1 = %d\n", idx_1);
  // get the second index
  int idx_2 = idx_1 ^ ht->hash_func(fp, ht->hash_len);
  // printf("idx_2 = %d\n", idx_2);

  // int counter1 = 0;
  // for (int i = 0; i < ht->nof_entries; ++i) {
  //   if (ht->buckets[idx_1]->entries[i] == NULL) {
  //     ++counter1;
  //   }
  // }
  // printf("we have %d null entries in idx_1\n", counter1);

  // int counter2 = 0;
  // for (int i = 0; i < ht->nof_entries; ++i) {
  //   if (ht->buckets[idx_2]->entries[i] == NULL) {
  //     ++counter2;
  //   }
  // }
  // printf("we have %d null entries in idx_2\n", counter2);

  // check for empty entry
  for (int i = 0; i < ht->nof_entries; ++i) {
    if (ht->buckets[idx_1]->entries[i] == NULL) {
      ht->buckets[idx_1]->entries[i] = malloc(strlen(fp) + 1);
      strcpy(ht->buckets[idx_1]->entries[i], fp);
      free(fp);
      // printf("inserted into idx_1\n");
      return CUCKOO_SUCCESS;
    }
  }
  for (int i = 0; i < ht->nof_entries; ++i) {
    if (ht->buckets[idx_2]->entries[i] == NULL) {
      ht->buckets[idx_2]->entries[i] = malloc(strlen(fp) + 1);
      strcpy(ht->buckets[idx_2]->entries[i], fp);
      free(fp);
      // printf("inserted into idx_2\n");
      return CUCKOO_SUCCESS;
    }
  }
  // reallocate existing items
  int result = toin_coss();
  int idx;
  if (result == 0) {
    idx = idx_1;
  } else {
    idx = idx_2;
  }
  // printf("idx = %d\n", idx);
  for (int i = 0; i < cf->max_num_kicks; ++i) {
    char **bucket = ht->buckets[idx]->entries;
    // randomly select an entry from bucket[idx]
    char **entry = random_bucket_entry(bucket, ht->nof_entries);
    // printf("entry is %s\n", *entry);
    // swap fp and entry
    char *temp = malloc(strlen(*entry) + 1);
    strcpy(temp, *entry);
    strcpy(*entry, fp);
    strcpy(fp, temp);
    // printf("we made the swap!\n");
    // cuckoo_print(cf);
    free(temp);
    // get new index
    idx = idx ^ ht->hash_func(fp, ht->hash_len);
    printf("idx is now %d\n", idx);
    // check for empty entry
    for (int i = 0; i < ht->nof_entries; ++i) {
      if (ht->buckets[idx]->entries[i] == NULL) {
        ht->buckets[idx]->entries[i] = malloc(strlen(fp) + 1);
        strcpy(ht->buckets[idx]->entries[i], fp);
        free(fp);
        // printf("found a new empty entry!\n");
        return CUCKOO_SUCCESS;
      }
    }
    // printf("we didnt find a new entry, we are reallocating again\n");
  }
  // hashtable is full
  free(fp);
  return CUCKOO_FAILURE;
}

// See cuckoofilter.h for documentation
bool cuckoo_lookup(const struct cuckoofilter *cf, const char *key) {
  assert(cf);
  assert(key);
  // get the fingerprint
  int key_len = strlen(key);
  int fp_len = cf->fp_len;
  char *fp = malloc(fp_len + 1);
  if (key_len >= fp_len) {
    strncpy(fp, key, fp_len);
    fp[fp_len] = '\0';
  } else {
    strncpy(fp, key, key_len);
    fp[fp_len] = '\0';
    fp = realloc(fp, (key_len + 1));
  }
  // printf("fp is %s\n", fp);
  struct hashtable *ht = cf->ht;
  // get the first index
  int idx_1 = ht->hash_func(key, ht->hash_len);
  // printf("idx_1 = %d\n", idx_1);
  // get the second index
  int idx_2 = idx_1 ^ ht->hash_func(fp, ht->hash_len);
  // printf("idx_2 = %d\n", idx_2);
  // check for the key
  for (int i = 0; i < ht->nof_entries; ++i) {
    if (ht->buckets[idx_1]->entries[i]) {
      if (strcmp(ht->buckets[idx_1]->entries[i], fp) == 0) {
        free(fp);
        return true;
      }
    }
  }
  for (int i = 0; i < ht->nof_entries; ++i) {
    if (ht->buckets[idx_2]->entries[i]) {
      if (strcmp(ht->buckets[idx_2]->entries[i], fp) == 0) {
        free(fp);
        return true;
      }
    }
  }
  free(fp);
  return false;
}

// See cuckoofilter.h for documentation
int cuckoo_remove(struct cuckoofilter *cf, const char *key) {
  assert(cf);
  assert(key);
  // get the fingerprint
  int key_len = strlen(key);
  int fp_len = cf->fp_len;
  char *fp = malloc(fp_len + 1);
  if (key_len >= fp_len) {
    strncpy(fp, key, fp_len);
    fp[fp_len] = '\0';
  } else {
    strncpy(fp, key, key_len);
    fp[fp_len] = '\0';
    fp = realloc(fp, (key_len + 1));
  }
  // printf("fp is %s\n", fp);
  struct hashtable *ht = cf->ht;
  // get the first index
  int idx_1 = ht->hash_func(key, ht->hash_len);
  // printf("idx_1 = %d\n", idx_1);
  // get the second index
  int idx_2 = idx_1 ^ ht->hash_func(fp, ht->hash_len);
  // printf("idx_2 = %d\n", idx_2);
  // check for the key
  for (int i = 0; i < ht->nof_entries; ++i) {
    if (ht->buckets[idx_1]->entries[i]) {
      if (strcmp(ht->buckets[idx_1]->entries[i], fp) == 0) {
        free(ht->buckets[idx_1]->entries[i]);
        ht->buckets[idx_1]->entries[i] = NULL;
        free(fp);
        return CUCKOO_SUCCESS;
      }
    }
  }
  for (int i = 0; i < ht->nof_entries; ++i) {
    if (ht->buckets[idx_2]->entries[i]) {
      if (strcmp(ht->buckets[idx_2]->entries[i], fp) == 0) {
        free(ht->buckets[idx_1]->entries[i]);
        ht->buckets[idx_1]->entries[i] = NULL;
        free(fp);
        return CUCKOO_SUCCESS;
      }
    }
  }
  free(fp);
  return CUCKOO_FAILURE;
}

// See cuckoofilter.h for documentation
void cuckoo_print(const struct cuckoofilter *cf) {
  assert(cf);
  struct hashtable *ht = cf->ht;
  int nof_buckets = 1 << ht->hash_len; // 2^hash_len
  for (int i = 0; i < nof_buckets; ++i) {
    char **bucket = ht->buckets[i]->entries;
    printf("bucket %d:\n", i);
    for (int j = 0; j < ht->nof_entries; ++j) {
      if (bucket[j]) {
        printf("%3d: %s\n", j, bucket[j]);
      } else {
        printf("%3d: \n", j);
      }
    }
  }
}
