#include <stdlib.h> // EXIT_FAILURE, etc.
#include <stdio.h> // printf(), etc.

#include "args.h"
#include "args_t.h"
#include "metadata_t.h"

args_t args;
metadata_t metadata;

int main(int argc, char *argv[])
{
    parse_args(argc, argv);

    printf("directory: %s\n", args.directory);
    printf("size: %s\n", args.size);
    printf("chunk_size: %s\n", args.chunk_size);

    return EXIT_FAILURE;
}
