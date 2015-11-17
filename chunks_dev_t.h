/* chunks_dev_t.h
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

#ifndef CHUNKS_DEV_T_H
#define CHUNKS_DEV_T_H

#include <stdint.h> // uint64_t, etc.

// per block-device state:
struct _chunks_dev_t
{
    char *dir_path;
    uint64_t dev_size;
    uint64_t chunk_size;
    uint8_t chunk_shift;
};
typedef struct _chunks_dev_t chunks_dev_t;

#endif // CHUNKS_DEV_T_H
