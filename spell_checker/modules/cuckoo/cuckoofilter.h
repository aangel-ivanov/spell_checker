#include <stdbool.h>

// CUCKOO_SUCCESS indicates successful execution of the cuckoo_insert or 
//   cuckoo_remove function.
extern const int CUCKOO_SUCCESS;
// CUCKOO_FAILURE indicates failed execution of the cuckoo_insert or 
//   cuckoo_remove function.
extern const int CUCKOO_FAILURE;

// a cuckoo filter for storing strings
struct cuckoofilter;

// requires: all functions require valid (non-NULL) parameters

// cuckoo_create(hash_func, hash_len, nof_entries, fp_len, max_num_kicks) 
//   creates a new cuckoo filter for strings with hash_func as the hash
//   function, hash_len the length of the result of a hash function (in
//   bits), nof_entries the number of entries in a bucket of the hash
//   table, and max_num_kicks the maximum number of relocation attempts.
// effects:  allocates heap memory; client must call cuckoo_destroy
// requires: hash_len, nof_entries, fp_len, and max_num_kicks must be positive
// time:     O(n * m), where n is length of hash table and m is number of 
//           entries in a bucket
struct cuckoofilter *cuckoo_create(int (*hash_func)(const char *, int), 
                                   int hash_len, int nof_entries, int fp_len, 
                                   int max_num_kicks);

// cuckoo_destroy(cf) frees all resources allocated by cuckoo filter cf.
// effects: invalidates cf
// time: O(n * m), where n is length of hash table in cf and m is number of
//       entries in a bucket
void cuckoo_destroy(struct cuckoofilter *cf);

// cuckoo_insert(cf, key) inserts string key into cuckoo filter cbf.
//   The function returns:
//   * CUCKOO_SUCCESS: if key was inserted into cf, or
//   * CUCKOO_FAILURE: if key was not inserted into cf.
// effects: modifies cf
// requires: key must be a non-empty null-terminated string [not asserted]
// time: O(m + s) where m is number of entries in a bucket and s is the length
//       of key
int cuckoo_insert(struct cuckoofilter *cf, const char *key);

// cuckoo_lookup(cf, key) returns true if the set membership test for string
//   key in cuckoo filter cf succeeds and false otherwise.
// requires: key must be a non-empty null-terminated string [not asserted]
// time: O(m + s) where m is number of entries in a bucket and s is the length
//       of key
bool cuckoo_lookup(const struct cuckoofilter *cf, const char *key);

// cuckoo_remove(cf, key) removes string key from cuckoo filter cf.
//   The function returns:
//   * CUCKOO_SUCCESS: if key was removed from cf, or
//   * CUCKOO_FAILURE: if key was not removed from cf.
// effects: modifies cf
// requires: key must be a non-empty null-terminated string [not asserted]
// time: O(m + s) where m is number of entries in a bucket and s is the length
//       of key
int cuckoo_remove(struct cuckoofilter *cf, const char *key);

// cuckoo_print(cf) prints the content of cuckoo filter cf to the 
//   console.
// effects: creates output
// time: O(n * m * s), where n is length of hash table in cf, m is number of
//       entries in a bucket, and s is the length of a fingerprint
void cuckoo_print(const struct cuckoofilter *cf);
