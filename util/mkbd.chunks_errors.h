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

    ERROR_populate_metadata_file_pwrite_FAILED = -31,
    ERROR_create_metadata_file_snprintf_FAILED = -32,
    ERROR_create_metadata_file_open_FAILED = -33,
    ERROR_create_metadata_file_close_FAILED = -34,
    ERROR_create_chunks_directory_snprintf_FAILED = -35,
    ERROR_create_chunks_directory_mkdir_FAILED = -36,

    ERROR_create_directory_if_needed_mkdir_FAILED = -41,
    ERROR_create_directory_if_needed_stat_FAILED = -42,
    ERROR_create_directory_if_needed_stat_EXISTS_BUT_NOT_DIR = -43
};

#define RETURN_SUCCESS (0)
#define RETURN_FAILURE (-1)

char* error_message(int ok);
void print_error(int ok);

#endif // MKBDCHUNKS_ERRORS_H
