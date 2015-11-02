#include <stdlib.h> // EXIT_FAILURE, etc.
#include <stdio.h> // printf(), etc.
#include <string.h> // strlen(), etc.
#include <stdbool.h> // bool, etc.
#include <limits.h> // INT_MAX, etc.

#include "args.h"
#include "args_t.h"
#include "metadata_t.h"
#include "mkbd.chunks_errors.h"
#include "parsing.h"

args_t args;
metadata_t metadata;

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
    if (ok < 0)
    {
        print_error(ok);
        return ok * -1;
    }

    printf("size: %llu\n", metadata.v1.dev_size);
    printf("chunk_size: %llu\n", metadata.v1.chunk_size);

    return EXIT_SUCCESS;
}