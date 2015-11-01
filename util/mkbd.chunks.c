#include <stdlib.h> // EXIT_FAILURE, etc.
#include <stdio.h> // printf(), etc.
#include <string.h> // strlen(), etc.
#include <stdbool.h> // bool, etc.
#include <limits.h> // INT_MAX, etc.

#include "args.h"
#include "args_t.h"
#include "metadata_t.h"

args_t args;
metadata_t metadata;

// copy in to out, stopping at the first non-numeric character.
// returns the number of charaters copied, or -1.
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
// returns 0 on success, -1 on error.
int parse_suffix(char *in, size_t in_size, suffix_t *out)
{
    if (in_size == 0)
    {
        *out = NONE;
    }
    else if (in_size == 1)
    {
        if (strncasecmp(in, "k", 1) == 0) *out = K;
        else if (strncasecmp(in, "m", 1) == 0) *out = M;
        else if (strncasecmp(in, "g", 1) == 0) *out = G;
        else if (strncasecmp(in, "t", 1) == 0) *out = T;
    }
    else if (in_size == 2)
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
        case K:  return 1000;
        case KB: return 1024;
        case M:  return 1000*1000;
        case MB: return 1024*1024;
        case G:  return 1000*1000*1000;
        case GB: return 1024*1024*1024;
        case T:  return (long)1000*1000*1000*1000;
        case TB: return (long)1024*1024*1024*1024;
        default: return 1;
    }
}

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
    char suffix_buf[sizeof("XX")];
    if (parse_suffix(suffix_buf, sizeof(suffix_buf), &suffix) == -1)
    {
        return -1;
    }

    uint64_t multiplier = suffix_to_multiplier(&suffix);
    *out = size * multiplier;
}

void populate_metadata_from_args()
{
    
}

int main(int argc, char *argv[])
{
    parse_args(argc, argv);

    printf("directory: %s\n", args.directory);
    printf("size: %s\n", args.size_str);
    printf("chunk_size: %s\n", args.chunk_size_str);

    return EXIT_FAILURE;
}
