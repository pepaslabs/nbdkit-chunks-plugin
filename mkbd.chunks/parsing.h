#ifndef PARSING_H
#define PARSING_H

#include <stddef.h> // size_t, etc.
#include <stdint.h> // uint64_t, etc.
#include <stdbool.h> // bool, etc.

enum _suffix_t { NONE=0, K, KB, M, MB, G, GB, T, TB };
typedef enum _suffix_t suffix_t;

// copy in to out, stopping at the first non-numeric character.
// returns the number of charaters copied, or -1.
// in_size is the length of 'in' plus the terminating '\0'.
int copy_numeric(char *in, size_t in_size, char *out, size_t out_size);

// translate a suffix string into a suffix_t enum.
// in_size is the length of 'in' plus the terminating '\0'.
// returns 0 on success, -1 on error.
int parse_suffix(char *in, size_t in_size, suffix_t *out);

uint64_t suffix_to_multiplier(suffix_t *suffix);

// in_size is the length of 'in' plus the terminating '\0'.
int parse_size_str(char *in, size_t in_size, uint64_t *out);

#endif // PARSING_H