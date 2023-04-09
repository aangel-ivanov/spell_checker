#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

static int hash_str( const char* input, const int num ) {

    const int num_buckets = 3;

    long hash = 0;
    const int len_string = strlen(input);
    for (int i = 0; i < len_string; i++) {
        hash += (long) pow(num_buckets, len_string - (i+1)) * input[i];
        hash = hash % num_buckets;
    }

    return (int) hash;
}

