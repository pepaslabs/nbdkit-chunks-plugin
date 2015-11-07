/* chunks_pwrite.c
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

// see also https://github.com/libguestfs/nbdkit/blob/master/docs/nbdkit-plugin.pod

#include "chunks_pwrite.h"

#include "chunks_dev_t.h"

// for fallocate()
#define _GNU_SOURCE

#include <nbdkit-plugin.h> // nbdkit_error(), etc.
#include <stdio.h> // snprintf(), etc.
#include <fcntl.h> // for open(), close(), fallocate(), etc.
// FIXME: I'm still seeing "warning: implicit declaration of function 'fallocate'"
#include <unistd.h> // for pread(), etc.
#include <string.h> // strlen(), etc.
#include <strings.h> // bzero(), etc.
#include <errno.h> // ENOENT, etc.

extern chunks_dev_t dev;

int _write_to_open_file(int fd, char *filepath, uint64_t offset, uint32_t count, const unsigned char *src)
{
    while(count > 0)
    {
        ssize_t bytes_written = pwrite(fd, src, count, offset);
        if (bytes_written < 1)
        {
            break;
        }

        count -= bytes_written;
        offset += bytes_written;
        src += bytes_written;
    }

    if (count > 0)
    {
        nbdkit_error("Unable to pwrite '%s': %m", filepath);
        return -1;
    }

    return 0;
}

#define CREATE_FAIL_IF_EXISTS (O_CREAT|O_EXCL)
#define CHMOD_RW_______ (S_IRUSR|S_IWUSR)

int _write_to_chunk_at_path(char *chunk_path, uint64_t offset, uint32_t count, const unsigned char *src)
{
    int fd = open(chunk_path, O_WRONLY);
    if (fd == -1)
    {
        if (errno == ENOENT)
        {
            fd = open(chunk_path, O_WRONLY|CREATE_FAIL_IF_EXISTS, CHMOD_RW_______);
            if (fd == -1)
            {
                nbdkit_error("Unable to create '%s': %m", chunk_path);
                return -1;
            }

            int err = fallocate(fd, 0, 0, dev.chunk_size);
            if (err == -1)
            {
                nbdkit_error("Unable to fallocate '%s': %m", chunk_path);

                if (close(fd) == -1)
                {
                    nbdkit_error("Unable to close '%s': %m", chunk_path);
                }

                return -1;
            }
        }
        else
        {
            nbdkit_error("Unable to open '%s': %m", chunk_path);
            return -1;
        }
    }

    int ret = _write_to_open_file(fd, chunk_path, offset, count, src);

    if (close(fd) == -1)
    {
        nbdkit_error("Unable to close '%s': %m", chunk_path);
        return -1;
    }

    return ret;
}

int _write_to_chunk_at_index(uint64_t chunk_index, uint64_t offset, uint32_t count, const unsigned char *src)
{
    size_t buff_size = strlen(dev.dir_path) + strlen("/chunks/18446744073709551615") + 1;
    char chunk_path[buff_size];
    int chars_written = snprintf(chunk_path, buff_size, "%s/chunks/%0.20llu", dev.dir_path, chunk_index);

    return _write_to_chunk_at_path(chunk_path, offset, count, src);
}

int chunks_pwrite(void *passed_handle, const void *buf, uint32_t count, uint64_t offset)
{
    while(count > 0)
    {
        uint64_t chunk_index = offset >> dev.chunk_shift;
        uint64_t chunk_offset = offset % dev.chunk_size;
        uint64_t chunk_remaining = dev.chunk_size - chunk_offset;
        uint32_t chunk_count = count <= chunk_remaining ? count : chunk_remaining;

        int ret = _write_to_chunk_at_index(chunk_index, chunk_offset, chunk_count, (const unsigned char*)buf);
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
