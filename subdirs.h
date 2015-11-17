/* subdirs.h
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

#ifndef SUBDIRS_H
#define SUBDIRS_H

#include <stdint.h> // uint64_t, etc.

int chunk_path(
	uint64_t chunk_index,
	uint64_t chunks_per_dir,
	int max_chunk_index,
	char *out, int out_size);

#endif // SUBDIRS_H
