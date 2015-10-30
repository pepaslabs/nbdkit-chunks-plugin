/* chunks_metadata.h
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

#include <stdint.h> // uint8_t, etc.
#include <stdbool.h> // bool


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


union _metadata_t
{
    metadata_v0_t v0;
    metadata_v1_t v1;
};
typedef union _metadata_t metadata_t;


#define CHUNKS_METADATA_MIN_SUPPORTED_VERSION 1
#define CHUNKS_METADATA_MAX_SUPPORTED_VERSION 1


int read_metadata();

bool metadata_dev_size_is_sane();
bool metadata_chunk_size_is_sane();
