#include <stdlib.h> // EXIT_FAILURE, etc.
#include <stdio.h> // printf(), etc.
#include <string.h> // strlen(), etc.
#include <stdbool.h> // bool, etc.
#include <limits.h> // INT_MAX, etc.

#include "args.h"
#include "args_t.h"
#include "metadata_t.h"

enum {
    ERROR_parse_size_str_NO_LEADING_NUMERIC_CHARS = -1,
    ERROR_parse_size_str_WOULD_OVERFLOW_UINT64 = -2,
    ERROR_parse_suffix_BAD_in_size = -3,

    ERROR_populate_metadata_from_args_SIZE_OFFSET = -10,
    ERROR_populate_metadata_from_args_SIZE_NO_LEADING_NUMERIC_CHARS = -11,
    ERROR_populate_metadata_from_args_SIZE_WOULD_OVERFLOW_UINT64 = -12,
    ERROR_populate_metadata_from_args_SIZE_BAD_in_size = -13,

    ERROR_populate_metadata_from_args_CHUNK_SIZE_OFFSET = -20,
    ERROR_populate_metadata_from_args_CHUNK_SIZE_NO_LEADING_NUMERIC_CHARS = -21,
    ERROR_populate_metadata_from_args_CHUNK_SIZE_WOULD_OVERFLOW_UINT64 = -22,
    ERROR_populate_metadata_from_args_CHUNK_SIZE_BAD_in_size = -23
};

enum _suffix_t { NONE=0, K, KB, M, MB, G, GB, T, TB };
typedef enum _suffix_t suffix_t;

args_t args;
metadata_t metadata;

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

#define RETURN_SUCCESS 0

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
        return ERROR_parse_suffix_BAD_in_size;
    }

    return RETURN_SUCCESS;
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
    int ok;

    char int_buf[sizeof("18446744073709551615")];
    int chars_copied = copy_numeric(in, in_size, int_buf, sizeof(int_buf));
    if (chars_copied == 0)
    {
        return ERROR_parse_size_str_NO_LEADING_NUMERIC_CHARS;
    }

    int base = 10;
    unsigned long long int size = strtoull(int_buf, NULL, base);

    char *suffix_in = in + chars_copied;
    size_t suffix_size = in_size - chars_copied;
    suffix_t suffix;
    ok = parse_suffix(suffix_in, suffix_size, &suffix);
    if (ok < 0) return ok;

    uint64_t multiplier = suffix_to_multiplier(&suffix);

    bool would_overflow = (multiplier > (UINT64_MAX / size));
    if (would_overflow)
    {
        return ERROR_parse_size_str_WOULD_OVERFLOW_UINT64;
    }

    *out = size * multiplier;

    return RETURN_SUCCESS;
}

void metadata_init_v0(metadata_t *inout)
{
    inout->v0.magic = CHUNKS_METADATA_MAGIC;
    inout->v0.metadata_version = 1;
}

int populate_metadata_from_args()
{
    int ok;

    metadata_init_v0(&metadata);

    uint64_t size;
    ok = parse_size_str(args.size_str, strlen(args.size_str)+1, &size);
    if (ok < 0) return ok + ERROR_populate_metadata_from_args_SIZE_OFFSET;

    metadata.v1.dev_size = size;

    uint64_t chunk_size;
    ok = parse_size_str(args.chunk_size_str, strlen(args.chunk_size_str)+1, &chunk_size);
    if (ok < 0) return ok + ERROR_populate_metadata_from_args_CHUNK_SIZE_OFFSET;

    metadata.v1.chunk_size = chunk_size;

    return RETURN_SUCCESS;
}

int main(int argc, char *argv[])
{
    int ok;

    parse_args(argc, argv);

    printf("args.directory: %s\n", args.directory);
    printf("args.size_str: %s\n", args.size_str);
    printf("args.chunk_size_str: %s\n", args.chunk_size_str);

    ok = populate_metadata_from_args();
    if (ok < 0) return ok * -1;

    printf("size: %llu\n", metadata.v1.dev_size);
    printf("chunk_size: %llu\n", metadata.v1.chunk_size);

    return EXIT_SUCCESS;
}
