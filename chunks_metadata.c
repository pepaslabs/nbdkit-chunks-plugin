/* chunks_metadata.c
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

#include "chunks_metadata.h"
#include "chunks_math.h"
#include "chunks_dev_t.h"

#include <string.h> // strlen(), etc.
#include <stdio.h> // fopen(), snprintf(), etc.

#include <nbdkit-plugin.h> // nbdkit_error(), etc.


bool _metadata_dev_size_is_sane(metadata_t *metadata)
{
    return is_divisible_by(metadata->v1.dev_size, metadata->v1.chunk_size);
}

bool _metadata_chunk_size_is_sane(metadata_t *metadata)
{
    return is_power_of_two(metadata->v1.chunk_size);
}

void _calculate_chunk_shift(metadata_t *metadata, chunks_dev_t *dev)
{
    dev->chunk_shift = 0;
    uint64_t chunk_size = metadata->v1.chunk_size;
    while(chunk_size > 1)
    {
        chunk_size << 1;
        dev->chunk_shift += 1;
    }
}

int _read_metadata_from_open_file(FILE *fp, char *filepath, metadata_t *metadata)
{
    size_t bytes_read;

    bytes_read = fread(&(metadata->v0.magic), sizeof(metadata->v0.magic), 1, fp);
    if (bytes_read != sizeof(metadata->v0.magic))
    {
        nbdkit_error("Unable to fread '%s'", filepath);
        return -1;
    }

    if (metadata->v0.magic != CHUNKS_METADATA_MAGIC)
    {
        nbdkit_error("Bad magic in '%s'", filepath);
        return -1;
    }

    bytes_read = fread(&(metadata->v0.metadata_version), sizeof(metadata->v0.metadata_version), 1, fp);
    if (bytes_read != sizeof(metadata->v0.metadata_version))
    {
        nbdkit_error("Unable to fread '%s'", filepath);
        return -1;
    }

    if (metadata->v0.metadata_version < CHUNKS_METADATA_MIN_SUPPORTED_VERSION
        || metadata->v0.metadata_version > CHUNKS_METADATA_MAX_SUPPORTED_VERSION)
    {
        nbdkit_error("Unsupported metadata version in '%s'", filepath);
        return -1;
    }

    bytes_read = fread(&(metadata->v1.dev_size), sizeof(metadata->v1.dev_size), 1, fp);
    if (bytes_read != sizeof(metadata->v1.dev_size))
    {
        nbdkit_error("Unable to fread '%s'", filepath);
        return -1;
    }

    bytes_read = fread(&(metadata->v1.chunk_size), sizeof(metadata->v1.chunk_size), 1, fp);
    if (bytes_read != sizeof(metadata->v1.chunk_size))
    {
        nbdkit_error("Unable to fread '%s'", filepath);
        return -1;
    }

    if (!_metadata_dev_size_is_sane(metadata))
    {
        nbdkit_error("Invalid dev_size in '%s'", filepath);
        return -1;
    }

    if (!_metadata_chunk_size_is_sane(metadata))
    {
        nbdkit_error("Invalid chunk_size in '%s'", filepath);
        return -1;
    }

    return 0;
}

int read_metadata(chunks_dev_t *dev, metadata_t *metadata)
{
    size_t buff_size = strlen(dev->dir_path) + strlen("/metadata") + 1;
    char metadata_path[buff_size];
    snprintf(metadata_path, buff_size, "%s/metadata", dev->dir_path);

    FILE *fp = fopen(metadata_path, "r");
    if (fp == NULL)
    {
        nbdkit_error("Unable to fopen '%s'", metadata_path);
        return -1;
    }

    int ret = _read_metadata_from_open_file(fp, metadata_path, metadata);

    if (fclose(fp) != 0)
    {
        nbdkit_error("Unable to fclose '%s'", metadata_path);
        return -1;
    }

    return ret;
}

void populate_chunks_dev_from_metadata(chunks_dev_t *dev, metadata_t *metadata)
{
    dev->dev_size = metadata->v1.dev_size;
    dev->chunk_size = metadata->v1.chunk_size;
    _calculate_chunk_shift(metadata, dev);
}