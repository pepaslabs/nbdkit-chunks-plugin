/* chunks_pread.h
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

// see also https://github.com/libguestfs/nbdkit/blob/master/docs/nbdkit-plugin.pod

#ifndef CHUNKS_PREAD_H
#define CHUNKS_PREAD_H

#include <stdint.h> // uint32_t, etc.

int chunks_pread(void *passed_handle, void *buf, uint32_t count, uint64_t offset);

#endif // CHUNKS_PREAD_H