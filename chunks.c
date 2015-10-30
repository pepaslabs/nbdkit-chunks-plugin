/* nbdkit-chunks-plugin
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

// see https://github.com/libguestfs/nbdkit/blob/master/docs/nbdkit-plugin.pod

#include <config.h>
#include <nbdkit-plugin.h>

#include <string.h> // strcmp()
#include <stdint.h> // uint8_t, etc
#include <stdbool.h> // bool


#define THREAD_MODEL NBDKIT_THREAD_MODEL_SERIALIZE_ALL_REQUESTS


static char *dir = NULL;


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


int chunks_config(const char *key, const char *value)
{
    if (strcmp(key, "dir") == 0)
    {
        char *chunks_dir = nbdkit_absolute_path(value);
        if (chunks_dir == NULL)
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

bool dir_sanity_dir_exists()
{
    // FIXME implement this
    nbdkit_error("dir '%s' does not exist.", dir);
    return false;
}

bool dir_sanity_dir_is_dir_or_symlink_to_dir()
{
    // FIXME implement this
    nbdkit_error("'%s' is not a directory (or symlink to a directory).", dir);
    return false;
}

bool dir_sanity_metadata_file_exists()
{
    // FIXME implement this
    nbdkit_error("'%s/metadata' does not exist.", dir);
    return false;
}

bool perform_dir_sanity_checks()
{
    return dir_sanity_dir_exists() \
    && dir_sanity_dir_is_dir_or_symlink_to_dir() \
    && dir_sanity_metadata_file_exists();
}

int chunks_config_complete()
{
    if (dir == NULL)
    {
        nbdkit_error("'dir' is a required parameter");
        return -1;
    }

    if (!perform_dir_sanity_checks())
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
