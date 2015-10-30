/* chunks_nbdkit.c
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

// see also https://github.com/libguestfs/nbdkit/blob/master/docs/nbdkit-plugin.pod

#include <config.h>
#include <nbdkit-plugin.h>

#include <string.h> // strcmp(), strlen(), etc.
#include <stdint.h> // uint8_t, etc.
#include <stdbool.h> // bool
#include <stdio.h> // fopen(), etc.

#include "chunks_math.h"


#define THREAD_MODEL NBDKIT_THREAD_MODEL_SERIALIZE_ALL_REQUESTS


// Metadata about this block device (version 0).
// Version 0 only declares magic and the version number of the metadata format.
// We do this to guarantee that any version of this plugin will be able
// to parse at least enough of any metadata file to determine if it
// is supported.
struct _metadata_v0_t
{
    // a value which specifically identifies this as a "chunks" metadata file.
    uint8_t magic;

    // format version number of this metadata structure.
    uint8_t metadata_version;
};
typedef struct _metadata_v0_t metadata_v0_t;

#define CHUNKS_METADATA_MAGIC 53

// metadata about this block device (version 1).
struct _metadata_v1_t
{
    // a value which specifically identifies this as a "chunks" metadata file.
    uint8_t magic;

    // format version number of this metadata structure.
    uint8_t metadata_version;

    // size of the block device in bytes.  should be divisible by chunk_size.
    uint64_t dev_size;

    // size of each chunk in bytes.  must be a power of 2.
    uint64_t chunk_size;
};
typedef struct _metadata_v1_t metadata_v1_t;

#define CHUNKS_METADATA_MIN_SUPPORTED_VERSION 1
#define CHUNKS_METADATA_MAX_SUPPORTED_VERSION 1

union _metadata_t
{
    metadata_v0_t v0;
    metadata_v1_t v1;
};
typedef union _metadata_t metadata_t;


char *chunks_dir_path = NULL;
metadata_t metadata;


int chunks_config(const char *key, const char *value)
{
    if (strcmp(key, "dir") == 0)
    {
        chunks_dir_path = nbdkit_absolute_path(value);
        if (chunks_dir_path == NULL)
        {
            nbdkit_error("nbdkit_absolute_path() failed on dir '%s'", value);
            return -1;
        }

        return 0;
    }
    else
    {
        nbdkit_error("Unrecognized parameter: '%s'", key);
        return -1;
    }
}

bool metadata_dev_size_is_sane()
{
    return is_divisible_by(metadata.v1.dev_size, metadata.v1.chunk_size);
}

bool metadata_chunk_size_is_sane()
{
    return is_power_of_two(metadata.v1.chunk_size);
}

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

int chunks_config_complete()
{
    if (chunks_dir_path == NULL)
    {
        nbdkit_error("'dir' is a required parameter");
        return -1;
    }

    if (read_metadata() != 0)
    {
        return -1;
    }

    return 0;
}

static struct nbdkit_plugin plugin = {
  .name              = "chunks",
  .longname          = "nbdkit chunks plugin",
  .description       = "An nbdkit plugin which stores data in many small files, rather than one large backing file.",
  .version           = "0.0",
  .config            = chunks_config,
  .config_complete   = chunks_config_complete
};

NBDKIT_REGISTER_PLUGIN(plugin)
