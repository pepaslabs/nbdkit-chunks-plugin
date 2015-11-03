#include "mkbd.chunks_errors.h"

#include <stdio.h> // fprintf(), etc.
#include <errno.h> // errno, etc.

char* error_message(int ok)
{
    switch (ok)
    {
        case ERROR_populate_metadata_from_args_SIZE_NO_LEADING_NUMERIC_CHARS:
            return "ERROR: Unable to parse size";
        case ERROR_populate_metadata_from_args_SIZE_WOULD_OVERFLOW_UINT64:
            return "ERROR: size must be smaller than UINT64_MAX";
        case ERROR_populate_metadata_from_args_SIZE_UNRECOGNIZED_SUFFIX:
            return "ERROR: Unable to parse size suffix";
        case ERROR_populate_metadata_from_args_SIZE_BAD_in_size:
            return "ERROR: Internal error #1.  Please file a bug";
        case ERROR_populate_metadata_from_args_CHUNK_SIZE_NO_LEADING_NUMERIC_CHARS:
            return "ERROR: Unable to parse chunk-size";
        case ERROR_populate_metadata_from_args_CHUNK_SIZE_WOULD_OVERFLOW_UINT64:
            return "ERROR: chunk-size must be smaller than UINT64_MAX";
        case ERROR_populate_metadata_from_args_CHUNK_SIZE_UNRECOGNIZED_SUFFIX:
            return "ERROR: Unable to parse chunk-size suffix";
        case ERROR_populate_metadata_from_args_CHUNK_SIZE_BAD_in_size:
            return "ERROR: Internal error #2.  Please file a bug";

        case ERROR_populate_metadata_file_pwrite_FAILED:
            return "ERROR: Unable to write to metadata file";
        case ERROR_create_metadata_file_snprintf_FAILED:
            return "ERROR: Internal error #3.  Please file a bug";
        case ERROR_create_metadata_file_open_FAILED:
            return "ERROR: Unable to create metadata file";
        case ERROR_create_metadata_file_close_FAILED:
            return "ERROR: close() failed for metadata file";
        case ERROR_create_chunks_directory_snprintf_FAILED:
            return "ERROR: Internal error #4.  Please file a bug";
        case ERROR_create_chunks_directory_mkdir_FAILED:
            return "ERROR: Unable to create 'chunks' directory";

        case ERROR_create_directory_if_needed_mkdir_FAILED:
            return "ERROR: Unable to create directory";
        case ERROR_create_directory_if_needed_stat_FAILED:
            return "ERROR: stat() failed for directory";
        case ERROR_create_directory_if_needed_stat_EXISTS_BUT_NOT_DIR:
            return "ERROR: directory path exists but is not a directory";

        default:
            return "ERROR: Unknown error";
    }
}

void print_error(int ok)
{
    char *message = error_message(ok);

    if (errno != 0)
    {
        perror(message);
    }
    else
    {
        fprintf(stderr, "ERROR: %s.\n", message);
    }
}
