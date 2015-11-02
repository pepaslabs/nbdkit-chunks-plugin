#include <stdlib.h> // EXIT_FAILURE, etc.
#include <stdio.h> // printf(), etc.
#include <string.h> // strlen(), etc.
#include <stdbool.h> // bool, etc.
#include <limits.h> // INT_MAX, etc.

#include "args.h"
#include "args_t.h"
#include "metadata_t.h"

// copy in to out, stopping at the first non-numeric character.
// returns the number of charaters copied, or -1.
// in_size is the length of 'in' plus the terminating '\0'.
int copy_numeric(char *in, size_t in_size, char *out, size_t out_size)
{
    if (in_size == 0 || out_size == 0) return 0;

    int i = 0;
    bool stop = false;
    while (i < in_size && i < out_size && i < INT_MAX && !stop)
    {
        if (i == in_size-1 || i == out_size-1)
        {
            stop = true;
            continue;
        }

        char ch = in[i];
        switch (ch)
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                out[i] = ch;
                i++;
                continue;

            default:
                stop = true;
                continue;
        }
    }
    out[i] = '\0';

    return i;
}

enum _suffix_t { NONE=0, K, KB, M, MB, G, GB, T, TB };
typedef enum _suffix_t suffix_t;

// translate a suffix string into a suffix_t enum.
// in_size is the length of 'in' plus the terminating '\0'.
// returns 0 on success, -1 on error.
int parse_suffix(char *in, size_t in_size, suffix_t *out)
{
    if (in_size == 0+1)
    {
        *out = NONE;
    }
    else if (in_size == 1+1)
    {
        if (strncasecmp(in, "k", 1) == 0) *out = K;
        else if (strncasecmp(in, "m", 1) == 0) *out = M;
        else if (strncasecmp(in, "g", 1) == 0) *out = G;
        else if (strncasecmp(in, "t", 1) == 0) *out = T;
    }
    else if (in_size == 2+1)
    {
        if (strncasecmp(in, "kb", 2) == 0) *out = KB;
        else if (strncasecmp(in, "mb", 2) == 0) *out = MB;
        else if (strncasecmp(in, "gb", 2) == 0) *out = GB;
        else if (strncasecmp(in, "tb", 2) == 0) *out = TB;
    }
    else
    {
        return -1;
    }

    return 0;
}

uint64_t suffix_to_multiplier(suffix_t *suffix)
{
    switch (*suffix)
    {
        case KB: return 1000;
        case K:  return 1024;
        case MB: return 1000*1000;
        case M:  return 1024*1024;
        case GB: return 1000*1000*1000;
        case G:  return 1024*1024*1024;
        case TB: return (uint64_t)1000*1000*1000*1000;
        case T:  return (uint64_t)1024*1024*1024*1024;
        default: return 1;
    }
}

// in_size is the length of 'in' plus the terminating '\0'.
int parse_size_str(char *in, size_t in_size, uint64_t *out)
{
    char int_buf[sizeof("18446744073709551615")];
    int chars_copied = copy_numeric(in, in_size, int_buf, sizeof(int_buf));
    if (chars_copied == 0)
    {
        return -1;
    }

    int base = 10;
    unsigned long long int size = strtoull(int_buf, NULL, base);

    char *suffix_in = in + chars_copied;
    size_t suffix_size = in_size - chars_copied;
    suffix_t suffix;
    if (parse_suffix(suffix_in, suffix_size, &suffix) == -1)
    {
        return -1;
    }

    uint64_t multiplier = suffix_to_multiplier(&suffix);

    bool would_overflow = (multiplier > (UINT64_MAX / size));
    if (would_overflow)
    {
        return -1;
    }

    *out = size * multiplier;
}

void populate_metadata_from_args()
{
    
}

args_t args;
metadata_t metadata;

int main(int argc, char *argv[])
{
    parse_args(argc, argv);

    printf("args.directory: %s\n", args.directory);
    printf("args.size_str: %s\n", args.size_str);
    printf("args.chunk_size_str: %s\n", args.chunk_size_str);

    uint64_t size;
    int ok = parse_size_str(args.size_str, strlen(args.size_str)+1, &size);
    if (ok == -1)
    {
        return EXIT_FAILURE;
    }

    uint64_t chunk_size;
    ok = parse_size_str(args.chunk_size_str, strlen(args.chunk_size_str)+1, &chunk_size);
    if (ok == -1)
    {
        return EXIT_FAILURE;
    }

    printf("size: %llu\n", size);
    printf("chunk_size: %llu\n", chunk_size);

    return EXIT_SUCCESS;
}
