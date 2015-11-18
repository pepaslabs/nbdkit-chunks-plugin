/* mkbd.chunks_errors.c
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

#ifndef MKBDCHUNKS_ERRORS_H
#define MKBDCHUNKS_ERRORS_H

enum {
    ERROR_parse_size_str_NO_LEADING_NUMERIC_CHARS = -1,
    ERROR_parse_size_str_WOULD_OVERFLOW_UINT64 = -2,
    ERROR_parse_suffix_UNRECOGNIZED_SUFFIX = -3,
    ERROR_parse_suffix_BAD_in_size = -4,

    ERROR_populate_metadata_from_args_SIZE_OFFSET = -10,
    ERROR_populate_metadata_from_args_SIZE_NO_LEADING_NUMERIC_CHARS = -11,
    ERROR_populate_metadata_from_args_SIZE_WOULD_OVERFLOW_UINT64 = -12,
    ERROR_populate_metadata_from_args_SIZE_UNRECOGNIZED_SUFFIX = -13,
    ERROR_populate_metadata_from_args_SIZE_BAD_in_size = -14,

    ERROR_populate_metadata_from_args_CHUNK_SIZE_OFFSET = -20,
    ERROR_populate_metadata_from_args_CHUNK_SIZE_NO_LEADING_NUMERIC_CHARS = -21,
    ERROR_populate_metadata_from_args_CHUNK_SIZE_WOULD_OVERFLOW_UINT64 = -22,
    ERROR_populate_metadata_from_args_CHUNK_SIZE_UNRECOGNIZED_SUFFIX = -23,
    ERROR_populate_metadata_from_args_CHUNK_SIZE_BAD_in_size = -24,

    ERROR_populate_metadata_from_args_CHUNKS_PER_SUBDIR_OFFSET = -30,
    ERROR_populate_metadata_from_args_CHUNKS_PER_SUBDIR_NO_LEADING_NUMERIC_CHARS = -31,
    ERROR_populate_metadata_from_args_CHUNKS_PER_SUBDIR_WOULD_OVERFLOW_UINT64 = -32,
    ERROR_populate_metadata_from_args_CHUNKS_PER_SUBDIR_UNRECOGNIZED_SUFFIX = -33,
    ERROR_populate_metadata_from_args_CHUNKS_PER_SUBDIR_BAD_in_size = -34,

    ERROR_populate_metadata_from_args_INVALID_SIZE = -41,
    ERROR_populate_metadata_from_args_INVALID_CHUNK_SIZE = -42,
    ERROR_populate_metadata_from_args_INVALID_CHUNKS_PER_SUBDIR = -43,

    ERROR_populate_metadata_file_pwrite_FAILED = -51,
    ERROR_create_metadata_file_snprintf_FAILED = -52,
    ERROR_create_metadata_file_open_FAILED = -53,
    ERROR_create_metadata_file_close_FAILED = -54,
    ERROR_create_chunks_directory_snprintf_FAILED = -55,
    ERROR_create_chunks_directory_mkdir_FAILED = -56,

    ERROR_create_directory_if_needed_mkdir_FAILED = -61,
    ERROR_create_directory_if_needed_stat_FAILED = -62,
    ERROR_create_directory_if_needed_stat_EXISTS_BUT_NOT_DIR = -63
};

#define RETURN_SUCCESS (0)
#define RETURN_FAILURE (-1)

char* error_message(int ok);
void print_error(int ok);

#endif // MKBDCHUNKS_ERRORS_H
