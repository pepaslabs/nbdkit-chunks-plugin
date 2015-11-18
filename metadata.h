/* metadata.h
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

#ifndef METADATA_H
#define METADATA_H

#include "metadata_t.h"
#include <stdbool.h> // bool, etc.

void metadata_v0_init(metadata_t *inout);

bool metadata_dev_size_is_sane(metadata_t *metadata);
bool metadata_chunk_size_is_sane(metadata_t *metadata);
bool metadata_chunks_per_subdir_is_sane(metadata_t *metadata);

#endif // METADATA_H
