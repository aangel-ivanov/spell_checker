// A simple module for reading strings from I/O

// NOTE: read_word() is the same as readstr() from the notes

// read_word() reads in the next "word" from stdin
// notes: returns NULL if there are no more words before EOF
//        a "word" is the next sequence of non-whitespace chars
//        the returned string will never contain whitespace
// effects: allocates memory (caller must free)
//          reads input
char *read_word(void);

// read_line() reads in the next "line" from stdin
// notes: returns NULL if there are no more lines before EOF
//        a "line" is all chars up to but not including the next \n (or EOF)
//        if the first char encountered is a \n, returns an empty string ("")
// effects: allocates memory (caller must free)
//          reads input
char *read_line(void);
