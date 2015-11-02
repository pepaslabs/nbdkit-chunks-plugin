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
    ERROR_populate_metadata_from_args_CHUNK_SIZE_BAD_in_size = -24
};

#define RETURN_SUCCESS 0

char* error_message(int ok);
void print_error(int ok);

#endif // MKBDCHUNKS_ERRORS_H