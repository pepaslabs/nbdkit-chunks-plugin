/* chunks_pread.c
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

// see also https://github.com/libguestfs/nbdkit/blob/master/docs/nbdkit-plugin.pod

#include "chunks_pread.h"

#include "chunks_dev_t.h"

#include <nbdkit-plugin.h> // nbdkit_error(), etc.
#include <stdio.h> // snprintf(), etc.
#include <fcntl.h> // for open(), close(), etc.
#include <unistd.h> // for pread(), etc.
#include <string.h> // strlen(), etc.

extern chunks_dev_t dev;

int _read_from_open_file(int fd, char *filepath, uint64_t offset, uint32_t count, unsigned char *dest)
{
    while(count > 0)
    {
        ssize_t bytes_read = pread(fd, dest, count, offset);
        if (bytes_read < 1)
        {
            break;
        }

        count -= bytes_read;
        offset += bytes_read;
        dest += bytes_read;
    }

    if (count > 0)
    {
        nbdkit_error("Unable to pread '%s'", filepath);
        return -1;
    }

    return 0;
}

int _read_from_chunk_at_path(char *chunk_path, uint64_t offset, uint32_t count, unsigned char *dest)
{
    int fd = open(chunk_path, O_RDONLY);
    if (fd == -1)
    {
        nbdkit_error("Unable to open '%s'", chunk_path);
        return -1;
    }

    int ret = _read_from_open_file(fd, chunk_path, offset, count, dest);

    if (close(fd) == -1)
    {
        nbdkit_error("Unable to close '%s'", chunk_path);
        return -1;
    }

    return ret;
}

int _read_from_chunk_num(uint64_t chunk_num, uint64_t offset, uint32_t count, unsigned char *dest)
{
    size_t buff_size = strlen(dev.dir_path) + strlen("/chunks/18446744073709551615") + 1;
    char chunk_path[buff_size];
    int chars_written = snprintf(chunk_path, buff_size, "%s/chunks/%0.20llu", dev.dir_path, chunk_num);

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