/* chunks_pread.c
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

// see also https://github.com/libguestfs/nbdkit/blob/master/docs/nbdkit-plugin.pod

#include "chunks_pread.h"

#include "chunks_dev_t.h"
#include "hexify.h"

#include <nbdkit-plugin.h> // nbdkit_error(), etc.
#include <stdio.h> // fopen(), etc.
#include <string.h> // strlen(), etc.

extern chunks_dev_t dev;

int _read_from_open_file(FILE *fp, char *filepath, uint64_t offset, uint32_t count, unsigned char *dest)
{
    if (fseek(fp, offset, SEEK_SET) != 0)
    {
        nbdkit_error("Unable to fseek '%s'", filepath);
        return -1;
    }

    size_t bytes_remaining = count;

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

int _read_from_chunk_at_path(char *chunk_path, uint64_t offset, uint32_t count, unsigned char *dest)
{
    FILE *fp = fopen(chunk_path, "r");
    if (fp == NULL)
    {
        nbdkit_error("Unable to fopen '%s'", chunk_path);
        return -1;
    }

    int ret = _read_from_open_file(fp, chunk_path, offset, count, dest);

    if (fclose(fp) != 0)
    {
        nbdkit_error("Unable to fclose '%s'", chunk_path);
        return -1;
    }

    return ret;
}

void _uint64_t_to_hex(uint64_t in, char *buf, size_t buf_size)
{
    hexify((unsigned char*)(&in), sizeof(in), buf, buf_size);
}

int _read_from_chunk_num(uint64_t chunk_num, uint64_t offset, uint32_t count, unsigned char *dest)
{
    size_t buff_size = strlen(dev.dir_path) + strlen("/chunks/ffffffffffffffff") + 1;
    char chunk_path[buff_size];
    int chars_written = snprintf(chunk_path, buff_size, "%s/chunks/", dev.dir_path);
    _uint64_t_to_hex(chunk_num, chunk_path+chars_written, buff_size-chars_written);

    return _read_from_chunk_at_path(chunk_path, offset, count, dest);
}

int chunks_pread(void *passed_handle, void *buf, uint32_t count, uint64_t offset)
{
    while(count > 0)
    {
        uint64_t chunk_num = offset << dev.chunk_shift;
        uint64_t chunk_offset = offset % dev.chunk_size;
        uint64_t chunk_remaining = dev.chunk_size - chunk_offset;
        uint32_t chunk_count = count <= chunk_remaining ? count : chunk_remaining;

        int ret = _read_from_chunk_num(chunk_num, chunk_offset, chunk_count, (unsigned char*)buf);
        if (ret != 0)
        {
            return ret;
        }

        offset += chunk_count;
        count -= chunk_count;
        buf += chunk_count;
    }

    return 0;
}
