#include "mkbd.chunks_errors.h"

#include <stdio.h> // fprintf(), etc.

char* error_message(int ok)
{
    switch (ok)
    {
        case ERROR_populate_metadata_from_args_SIZE_NO_LEADING_NUMERIC_CHARS:
            return "Unable to parse size.";
        case ERROR_populate_metadata_from_args_SIZE_WOULD_OVERFLOW_UINT64:
            return "size must be smaller than UINT64_MAX.";
        case ERROR_populate_metadata_from_args_SIZE_UNRECOGNIZED_SUFFIX:
        case ERROR_populate_metadata_from_args_SIZE_BAD_in_size:
            return "Unable to parse size suffix.";
        case ERROR_populate_metadata_from_args_CHUNK_SIZE_NO_LEADING_NUMERIC_CHARS:
            return "Unable to parse chunk-size.";
        case ERROR_populate_metadata_from_args_CHUNK_SIZE_WOULD_OVERFLOW_UINT64:
            return "chunk-size must be smaller than UINT64_MAX.";
        case ERROR_populate_metadata_from_args_CHUNK_SIZE_UNRECOGNIZED_SUFFIX:
        case ERROR_populate_metadata_from_args_CHUNK_SIZE_BAD_in_size:
            return "Unable to parse chunk-size suffix.";
        default:
            return "Unknown error.";
    }
}

void print_error(int ok)
{
    char *message = error_message(ok);
    fprintf(stderr, "ERROR: %s\n", message);
}