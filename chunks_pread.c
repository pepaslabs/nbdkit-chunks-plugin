/* chunks_pread.c
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

// see also https://github.com/libguestfs/nbdkit/blob/master/docs/nbdkit-plugin.pod

#include "chunks_pread.h"

#include "chunks_dev_t.h"

#include <nbdkit-plugin.h> // nbdkit_error(), etc.
#include <stdio.h> // fopen(), etc.

extern chunks_dev_t dev;

int _read_from_open_file(FILE *fp, char *filepath, uint32_t src_count, void *dest)
{
    size_t bytes_remaining = src_count;

    while(bytes_remaining > 0)
    {
        size_t bytes_read = fread(dest, sizeof(unsigned char), bytes_remaining, fp);
        if (ferror(fp) || feof(fp) || bytes_read == 0)
        {
            break;
        }

        bytes_remaining -= bytes_read;
        dest += bytes_read;
    }

    if (bytes_remaining)
    {
        nbdkit_error("Unable to fread '%s'", filepath);
        return -1;
    }

    return 0;
}

int _read_from_chunk_at_path(char *chunk_path, uint64_t src_offset, uint32_t src_count, void *dest, uint64_t dest_offset)
{
    FILE *fp = fopen(chunk_path, "r");
    if (fp == NULL)
    {
        nbdkit_error("Unable to fopen '%s'", chunk_path);
        return -1;
    }

    int ret = _read_from_open_file(fp, chunk_path, src_count, dest);

    if (fclose(fp) != 0)
    {
        nbdkit_error("Unable to fclose '%s'", chunk_path);
        return -1;
    }

    return ret;
}

int _chunk_path(uint64_t chunk_num)
{
    // dir/chunks/chunk_num

    // FIXME
}

int _read_from_chunk_num(uint64_t chunk_num, uint64_t offset, uint32_t count, void *buf)
{
    // FIXME
}

int chunks_pread(void *passed_handle, void *buf, uint32_t count, uint64_t offset)
{
    return -1;

    // split the read up into chunks
    // read each chunk

    uint64_t first_chunk_number = offset << dev.chunk_shift;
    uint64_t first_chunk_offset = offset % dev.chunk_size;

    // FIXME
}
