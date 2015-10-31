/* chunks_metadata.h
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

#ifndef CHUNKS_METADATA_H
#define CHUNKS_METADATA_H

#include "chunks_dev_t.h"
#include "metadata_t.h"

#include <stdbool.h> // bool

#define CHUNKS_METADATA_MIN_SUPPORTED_VERSION 1
#define CHUNKS_METADATA_MAX_SUPPORTED_VERSION 1

int read_metadata_and_populate_chunks_dev(chunks_dev_t *dev);

#endif // CHUNKS_METADATA_H