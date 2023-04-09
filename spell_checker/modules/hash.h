// hash_str_n(str, hash_length) calculates the hash value of string str with a
//   length of hash_length bits.
// examples: hash_str_n("Test", 4) returns a 4-bit hash value ([0, ..., 15])
//           hash_str_n("Test", 7) returns a 7-bit hash value ([0, ..., 127])
//           hash_str_n("Test", 10) returns a 10-bit hash value ([0, ..., 1023])
//           hash_str_n("Test", 16) returns a 16-bit hash value ([0, ...,65536])
// requires: hash_length must be between 2 and 16 (both inclusive)
// time:     O(n) where n is the length of the string
int hash_str_0(const char *str, int hash_length);

int hash_str_1(const char *str, int hash_length);

int hash_str_2(const char *str, int hash_length);

int hash_str_3(const char *str, int hash_length);

int hash_str_4(const char *str, int hash_length);
