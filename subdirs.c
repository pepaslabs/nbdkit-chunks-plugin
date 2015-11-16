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
	int retval;

	int8_t dir_len = _uint64t_strlen(chunks_per_dir-1);
	if (dir_len < 1)
		return -1;

	int8_t chunk_len = _uint64t_strlen(max_chunk_index);
	if (chunk_len < 1)
		return -1;

	if (chunk_len % dir_len > 0) {
		chunk_len += dir_len - (chunk_len % dir_len);
	}

	retval = snprintf(out, out_size, "chunks/");
	if (retval < 0)
		return -1;

	char *chunk_index_str = out + strlen(out);

	uint8_t subdir_count;
	uint8_t subdirs_strlen;
	if (max_chunk_index >= chunks_per_dir) {
		uint8_t subdir_count = (chunk_len / dir_len) - 1;
		uint8_t subdirs_strlen = subdir_count * (dir_len + sizeof('/'));
		chunk_index_str += subdirs_strlen;
	}

	char fmt_str[8+1];
	retval = snprintf(fmt_str, sizeof(fmt_str), "%%0.%illu", chunk_len);
	if (retval < 0)
		return -1;

	int chunk_index_str_buf_len = out_size - strlen(out);
	retval = snprintf(chunk_index_str, chunk_index_str_buf_len, fmt_str, chunk_index);
	if (retval < 0)
		return -1;

	if (max_chunk_index >= chunks_per_dir) {
		char *d_str = out + strlen("chunks/");
		char *c_str = chunk_index_str;
		for (int i=0; i < subdir_count; i++) {
			memcpy(d_str, c_str, dir_len + sizeof('/'));
			d_str += (dir_len + sizeof('/'));
			c_str += dir_len;
		}
	}

	// FIXME left off here.

	return 0;
}
