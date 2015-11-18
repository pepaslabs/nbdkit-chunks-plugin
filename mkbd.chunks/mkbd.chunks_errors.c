#include "mkbd.chunks_errors.h"

#include <stdio.h> // fprintf(), etc.
#include <errno.h> // errno, etc.

char* error_message(int ok)
{
    switch (ok)
    {
        case ERROR_populate_metadata_from_args_SIZE_NO_LEADING_NUMERIC_CHARS:
            return "Unable to parse size";
        case ERROR_populate_metadata_from_args_SIZE_WOULD_OVERFLOW_UINT64:
            return "size must be smaller than UINT64_MAX";
        case ERROR_populate_metadata_from_args_SIZE_UNRECOGNIZED_SUFFIX:
            return "Unable to parse size suffix";
        case ERROR_populate_metadata_from_args_SIZE_BAD_in_size:
            return "Internal error #1.  Please file a bug";

        case ERROR_populate_metadata_from_args_CHUNK_SIZE_NO_LEADING_NUMERIC_CHARS:
            return "Unable to parse chunk-size";
        case ERROR_populate_metadata_from_args_CHUNK_SIZE_WOULD_OVERFLOW_UINT64:
            return "chunk-size must be smaller than UINT64_MAX";
        case ERROR_populate_metadata_from_args_CHUNK_SIZE_UNRECOGNIZED_SUFFIX:
            return "Unable to parse chunk-size suffix";
        case ERROR_populate_metadata_from_args_CHUNK_SIZE_BAD_in_size:
            return "Internal error #2.  Please file a bug";

        case ERROR_populate_metadata_from_args_CHUNKS_PER_SUBDIR_NO_LEADING_NUMERIC_CHARS:
            return "Unable to parse chunks-per-subdir";
        case ERROR_populate_metadata_from_args_CHUNKS_PER_SUBDIR_WOULD_OVERFLOW_UINT64:
            return "chunks-per-subdir must be smaller than UINT64_MAX";
        case ERROR_populate_metadata_from_args_CHUNKS_PER_SUBDIR_UNRECOGNIZED_SUFFIX:
            return "Unable to parse chunks-per-subdir suffix";
        case ERROR_populate_metadata_from_args_CHUNKS_PER_SUBDIR_BAD_in_size:
            return "Internal error #3.  Please file a bug";

        case ERROR_populate_metadata_file_pwrite_FAILED:
            return "Unable to write to metadata file";
        case ERROR_create_metadata_file_snprintf_FAILED:
            return "Internal error #4.  Please file a bug";
        case ERROR_create_metadata_file_open_FAILED:
            return "Unable to create metadata file";
        case ERROR_create_metadata_file_close_FAILED:
            return "close() failed for metadata file";
        case ERROR_create_chunks_directory_snprintf_FAILED:
            return "Internal error #5.  Please file a bug";
        case ERROR_create_chunks_directory_mkdir_FAILED:
            return "Unable to create 'chunks' directory";

        case ERROR_populate_metadata_from_args_INVALID_SIZE:
            return "Invalid size (must be divisble by chunk-size)";
        case ERROR_populate_metadata_from_args_INVALID_CHUNK_SIZE:
            return "Invalid chunk-size (must be a power of 2)";
        case ERROR_populate_metadata_from_args_INVALID_CHUNKS_PER_SUBDIR:
            return "Invalid chunks-per-subdir (must be a power of 10, or zero to disable subdirectories)";

        case ERROR_create_directory_if_needed_mkdir_FAILED:
            return "Unable to create directory";
        case ERROR_create_directory_if_needed_stat_FAILED:
            return "stat() failed for directory";
        case ERROR_create_directory_if_needed_stat_EXISTS_BUT_NOT_DIR:
            return "directory path exists but is not a directory";

        default:
            return "Unknown error";
    }
}

void print_error(int ok)
{
    char *message = error_message(ok);

    if (errno != 0)
    {
        fprintf(stderr, "\nERROR: %s: %m.\n", message);
    }
    else
    {
        fprintf(stderr, "\nERROR: %s.\n", message);
    }
}
