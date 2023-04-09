// toin_coss "randomly" returns either zero or one.
// time: O(1)
int toin_coss();

// random_bucket_entry "randomly" chooses a non-empty entry in bucket
//   and returns its address
// requires: at least one of the entries in buckets must not be empty
//           [not asserted]
// time: O(1)
char** random_bucket_entry(char *bucket[], int nof_entries);