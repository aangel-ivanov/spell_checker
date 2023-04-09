/*
  This module provides a "Sequence of Strings" ADT (aka SOS)

  Sequence of Strings
  ^           ^
  |           |
  |           +----------- this ADT stores strings
  +----------------------- the strings are in a sequence with positions 0, 1, 2...
  
  To create a new SOS:
    struct sos *seq = sos_create();      // sequence is now empty
  
  To add a string, insert it at a position
  Because this is the first string to be added, it must be at position 0
    sos_insert_at(seq, 0, "hi");       // sequence is now: hi
      
  A copy of the string is added to the SOS, so if a client modifies the
  contents of the original string that was added, the SOS is not affected
  Also, there is a function to insert at the end of the sequence
    char str[] = "cat";
    sos_insert_end(seq, str);          // sequence is now: hi, cat
    str[0] = 'b';                      // sequence is still: hi, cat

  A string may be inserted at any valid position, but it will change the position
  of strings that are now after the inserted string
    sos_insert_at(seq, 1, "black");    // sequence is now: hi, black, cat

  A (const) pointer to the string at any position can be retreived in O(1) time
  Note that it is a const pointer, so the string in the SOS cannot be modified
    const char *greeting = sos_item_at(seq, 0);    // greeting points at "hi"
    greeting[0] = 'H';                             // type error (not allowed)

  A string may also be removed from any valid position, which changes
  the positions of strings that were after the removed string
  Also, the string is freed so any pointers to the old string are now "dangling"
    sos_remove_at(seq, 0);             // sequence is now: black, cat
    printf("%s", greeting);            // memory error (greeting is dangling)

  Similaraly, strings that are replaced are freed and no longer valid
    sos_replace_at(seq, 0, "white");   // sequence is now: white, cat

  There are additional SOS functions described in the interface below

*/  

//////////////////////////////////////////////////////////////////////////////


// requires: all functions require valid (non-NULL) parameters

// times: n is the number of strings in the seq [sos_length(seq)]
//        m is the length of the string s [strlen(s)]
//        k is the length of the longest string in the seq

struct sos;

// sos_create() returns a new empty sequence of strings
// effects: allocates memory (client must call sos_destroy)
// time: O(1)
struct sos *sos_create(void);

// sos_destroy(seq) frees all memory for seq
// effects: seq is no longer valid
// time: O(n)
void sos_destroy(struct sos *seq);

// sos_length(seq) returns the number of items in seq
// time: O(1)
int sos_length(const struct sos *seq);

// sos_item_at(seq, pos) returns a (const) pointer to the string in seq at pos
// requires: 0 <= pos < sos_length(seq)
// time: O(1)
const char *sos_item_at(const struct sos *seq, int pos);

// sos_insert_at(seq, pos, s) inserts a copy of s at position pos in seq
// note: changes the position of items that were at position >= pos
// requires: 0 <= pos <= sos_length(seq)
// effects: modifies seq
// time: O(n + m)
void sos_insert_at(struct sos *seq, int pos, const char *s);

// sos_insert_end(seq, s) inserts a copy of s at the end of the seq
// note: is the same as sos_insert_at(seq, pos, sos_length(seq))
//       except that it is more efficient
// effects: modifies seq
// time: O(m) [amortized]
void sos_insert_end(struct sos *seq, const char *s);

// sos_remove_at(seq, pos) removes the string at position pos in seq
// notes: changes the position of items that were at position > pos
//        frees the string, so any exisiting pointers to it are now dangling
// requires: 0 <= pos < sos_length(seq)
// effects: modifies seq
//          the string that was at pos is no longer valid
// time: O(n)
void sos_remove_at(struct sos *seq, int pos);

// sos_remove_end(seq) removes the last string in seq
// note: is the same as sos_remove_at(seq, sos_length(seq) - 1)
//       except that it is more efficient
// requires: 0 < sos_length(seq)
// effects: modifies seq
//          the string that was at (sos_length(seq) - 1) is no longer valid
// time: O(1)
void sos_remove_end(struct sos *seq);

// sos_replace_at(seq, pos, s) replaces the string at position pos in seq
//   with a copy of s
// note: frees the old string, so any exisiting pointers to it are now dangling
//       this is equivalent to a remove then insert, but is more efficient
// requires: 0 <= pos < sos_length(seq)
// effects: modifies seq
//          the string that was at pos is no longer valid
// time: O(m)
void sos_replace_at(struct sos *seq, int pos, const char *s);

// sos_swap(seq, pos1, pos2) swaps the position of the two strings at
//   pos1 and pos2
// note: pos1 and pos2 may have the same value
// requires: 0 <= pos1,pos2 < sos_length(seq)
// effects: may modify seq
// time: O(1)
void sos_swap(struct sos *seq, int pos1, int pos2);

// sos_dup(seq) creates a new sos that is a duplicate of seq
// note: makes a copy of each and every string in seq
// effects: allocates memory (client must call sos_destroy)
// time: O(n * k)
struct sos *sos_dup(const struct sos *seq);

// sos_read() creates a new sos where each item in the sos
//   is a line from the input
// notes: returns an empty sos if EOF is encountered immediately
//        blank lines with no characters are empty strings in the sos
// effects: allocates memory (client must call sos_destroy)
// time: O(n * k) where n,k are in the generated sos
struct sos *sos_read(void);

// sos_print(seq) prints each string in seq
// notes: prints each string followed by a newline (\n)
//        prints nothing if seq has length 0
// effects: may produce output
// time: O(n * k)
void sos_print(const struct sos *seq);
