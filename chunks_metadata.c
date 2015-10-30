/* chunks_metadata.h
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

#include "chunks_metadata.h"
#include "chunks_math.h"

#include <string.h> // strlen(), etc.
#include <stdio.h> // fopen(), etc.

#include <nbdkit-plugin.h> // nbdkit_error(), etc.


extern char *chunks_dir_path;
extern metadata_t metadata;


int read_metadata()
{
    size_t buff_size = strlen(chunks_dir_path) + strlen("/metadata") + 1;
    char metadata_path[buff_size];
    snprintf(metadata_path, buff_size, "%s/metadata", chunks_dir_path);

    FILE *fp = fopen(metadata_path, "r");
    if (fp == NULL)
    {
        nbdkit_error("Unable to fopen '%s'", metadata_path);
        return -1;
    }

    size_t bytes_read;

    bytes_read = fread(&(metadata.v0.magic), sizeof(metadata.v0.magic), 1, fp);
    if (bytes_read != sizeof(metadata.v0.magic))
    {
        nbdkit_error("Unable to fread '%s'", metadata_path);
        return -1;
    }

    if (metadata.v0.magic != CHUNKS_METADATA_MAGIC)
    {
        nbdkit_error("Bad magic in '%s'", metadata_path);
        return -1;
    }

    bytes_read = fread(&(metadata.v0.metadata_version), sizeof(metadata.v0.metadata_version), 1, fp);
    if (bytes_read != sizeof(metadata.v0.metadata_version))
    {
        nbdkit_error("Unable to fread '%s'", metadata_path);
        return -1;
    }

    if (metadata.v0.metadata_version < CHUNKS_METADATA_MIN_SUPPORTED_VERSION
        || metadata.v0.metadata_version > CHUNKS_METADATA_MAX_SUPPORTED_VERSION)
    {
        nbdkit_error("Unsupported metadata version in '%s'", metadata_path);
        return -1;
    }

    bytes_read = fread(&(metadata.v1.dev_size), sizeof(metadata.v1.dev_size), 1, fp);
    if (bytes_read != sizeof(metadata.v1.dev_size))
    {
        nbdkit_error("Unable to fread '%s'", metadata_path);
        return -1;
    }

    bytes_read = fread(&(metadata.v1.chunk_size), sizeof(metadata.v1.chunk_size), 1, fp);
    if (bytes_read != sizeof(metadata.v1.chunk_size))
    {
        nbdkit_error("Unable to fread '%s'", metadata_path);
        return -1;
    }

    if (!metadata_dev_size_is_sane())
    {
        nbdkit_error("Invalid dev_size in '%s'", metadata_path);
        return -1;
    }

    if (!metadata_chunk_size_is_sane())
    {
        nbdkit_error("Invalid chunk_size in '%s'", metadata_path);
        return -1;
    }

    if (fclose(fp) != 0)
    {
        nbdkit_error("Unable to fclose '%s'", metadata_path);
        return -1;
    }

    return 0;
}

bool metadata_dev_size_is_sane()
{
    return is_divisible_by(metadata.v1.dev_size, metadata.v1.chunk_size);
}

bool metadata_chunk_size_is_sane()
{
    return is_power_of_two(metadata.v1.chunk_size);
}
