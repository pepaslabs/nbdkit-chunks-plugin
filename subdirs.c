/* subdirs.c
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

#include <stdio.h> // snprintf(), etc.
#include <stdint.h> // uint64_t, etc.
#include <string.h> // strlen(), etc.

#define UINT64_MAX_DIGITS 20

int8_t uint64t_strlen(uint64_t i)
{
	char buf[UINT64_MAX_DIGITS+1];
	int retval = snprintf(buf, sizeof(buf), "%llu", i);
	if (retval < 0)
		return -1;

	int8_t len = strlen(buf);
	return len;
}

int chunk_relpath(
	uint64_t chunk_index,
	uint64_t chunks_per_subdir,
	int max_chunk_index,
	char *out, int out_size)
{
	int retval;

	int8_t dir_strlen = uint64t_strlen(chunks_per_subdir-1);
	if (dir_strlen < 1)
		return -1;

	int8_t chunk_strlen = uint64t_strlen(max_chunk_index);
	if (chunk_strlen < 1)
		return -1;

	if (chunk_strlen % dir_strlen > 0)
		chunk_strlen += dir_strlen - (chunk_strlen % dir_strlen);

	retval = snprintf(out, out_size, "chunks/");
	if (retval < 0)
		return -1;

	char *chunk_index_substr = out + strlen(out);

	uint8_t subdir_count;
	uint8_t subdirs_strlen;
	if (max_chunk_index >= chunks_per_subdir) {
		subdir_count = (chunk_strlen / dir_strlen) - 1;
		subdirs_strlen = subdir_count * (dir_strlen + strlen("/"));
		chunk_index_substr += subdirs_strlen;
	}

	char fmt_str[8+1];
	retval = snprintf(fmt_str, sizeof(fmt_str), "%%0.%illu", chunk_strlen);
	if (retval < 0)
		return -1;

	int chunk_index_substr_buf_len = out_size - strlen(out);
	retval = snprintf(chunk_index_substr, chunk_index_substr_buf_len, fmt_str, chunk_index);
	if (retval < 0)
		return -1;

	if (max_chunk_index >= chunks_per_subdir) {
		char *dir_substr = out + strlen(out);
		char *c_substr = chunk_index_substr;
		for (int i=0; i < subdir_count; i++) {
			memcpy(dir_substr, c_substr, dir_strlen);
			dir_substr += dir_strlen;
			c_substr += dir_strlen;
			memcpy(dir_substr, "/", 1);
			dir_substr += 1;
		}
	}

	return 0;
}
