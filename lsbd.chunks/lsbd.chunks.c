/* lsbd.chunks.c
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

#include <stdlib.h> // EXIT_FAILURE, etc.
#include <stdio.h> // fprintf, etc.

#include "metadata_t.h"
#include "chunks_metadata.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "DIRECTORY is a required parameter.\n");
        return EXIT_FAILURE;
    }

    char *dev_dir_path = argv[1];

    metadata_t metadata;
    int ok = read_metadata(dev_dir_path, &metadata);
    if (ok != 0) return EXIT_FAILURE;

    printf("metadata version: %i\n", metadata.v0.metadata_version);
    printf("block device size: %i bytes\n", metadata.v1.dev_size);
    printf("chunk size: %i bytes\n", metadata.v1.chunk_size);

	return EXIT_SUCCESS;
}
