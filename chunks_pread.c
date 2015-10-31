/* chunks_pread.c
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

// see also https://github.com/libguestfs/nbdkit/blob/master/docs/nbdkit-plugin.pod

#include "chunks_pread.h"

#include "chunks_dev_t.h"

extern chunks_dev_t dev;

int chunks_pread(void *passed_handle, void *buf, uint32_t count, uint64_t offset)
{
    // split the read up into chunks
    // read each chunk

    uint64_t first_chunk_number = offset << dev.chunk_shift;
    uint64_t first_chunk_offset = offset % dev.chunk_size;
}