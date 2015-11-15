#include <stdio.h> // snprintf(), etc.
#include <stdint.h> // uint64_t, etc.
#include <string.h> // strlen(), etc.

#define UINT64_MAX_DIGITS 20

int8_t _uint64t_strlen(uint64_t i)
{
	char buf[UINT64_MAX_DIGITS+1];
	int retval = snprintf(buf, sizeof(buf), "%llu", i);
	if (retval < 0)
		return -1;

	int8_t len = strlen(buf);
	return len;
}

int path(uint64_t chunk_index,
	uint64_t chunks_per_dir,
	int max_chunk_index,
	char *out, int out_size)
{
	int8_t dir_len = _uint64t_strlen(chunks_per_dir-1);
	if (dir_len < 1)
		return -1;

	int8_t chunk_len = _uint64t_strlen(max_chunk_index);
	if (chunk_len < 1)
		return -1;

	if (chunk_len % dir_len > 0) {
		chunk_len += dir_len - (chunk_len % dir_len);
	}

	char fmt_str[8+1];
	int retval = snprintf(fmt_str, sizeof(fmt_str), "%%0.%illu", 20); // FIXME replace 20 with what you actually need
	if (retval < 0)
		return -1;

	char chunk_index_str[UINT64_MAX_DIGITS+1];
	retval = snprintf(chunk_index_str, sizeof(chunk_index_str), fmt_str, chunk_index);
	if (retval < 0)
		return -1;

	// FIXME left off here

	return 0;
}
