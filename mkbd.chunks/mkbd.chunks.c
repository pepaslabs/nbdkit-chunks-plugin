#include <stdlib.h> // EXIT_FAILURE, etc.
#include <stdio.h> // printf(), etc.
#include <string.h> // strlen(), etc.
#include <stdbool.h> // bool, etc.
#include <limits.h> // INT_MAX, etc.
#include <fcntl.h> // open(), etc.
#include <unistd.h> // pwrite(), etc.
#include <sys/stat.h> // mkdir(), etc. 
#include <errno.h> // errno, etc

#include "args.h"
#include "args_t.h"
#include "metadata.h"
#include "mkbd.chunks_errors.h"
#include "parsing.h"

args_t args;
metadata_t metadata;

int populate_metadata_from_args()
{
    int ok;

    metadata_v0_init(&metadata);

    uint64_t size;
    ok = parse_size_str(args.size_str, strlen(args.size_str)+1, &size);
    if (ok < 0) return ok + ERROR_populate_metadata_from_args_SIZE_OFFSET;
    metadata.v1.dev_size = size;

    uint64_t chunk_size;
    ok = parse_size_str(args.chunk_size_str, strlen(args.chunk_size_str)+1, &chunk_size);
    if (ok < 0) return ok + ERROR_populate_metadata_from_args_CHUNK_SIZE_OFFSET;
    metadata.v1.chunk_size = chunk_size;

    uint64_t chunks_per_subdir;
    ok = parse_size_str(args.chunks_per_subdir_str, strlen(args.chunks_per_subdir_str)+1, &chunks_per_subdir);
    if (ok < 0) return ok + ERROR_populate_metadata_from_args_CHUNKS_PER_SUBDIR_OFFSET;
    metadata.v1.chunks_per_subdir = chunks_per_subdir;

    if (!metadata_dev_size_is_sane(&metadata))
    {
        return ERROR_populate_metadata_from_args_INVALID_SIZE;
    }

    if (!metadata_chunk_size_is_sane(&metadata))
    {
        return ERROR_populate_metadata_from_args_INVALID_CHUNK_SIZE;
    }

    if (!metadata_chunks_per_subdir_is_sane(&metadata))
    {
        return ERROR_populate_metadata_from_args_INVALID_CHUNKS_PER_SUBDIR;
    }

    return RETURN_SUCCESS;
}

int populate_metadata_file(int fd)
{
    ssize_t bytes_written;
    off_t offset = 0;

    bytes_written = pwrite(fd, (void*)(&metadata.v0.magic), sizeof(metadata.v0.magic), offset);
    if (bytes_written != sizeof(metadata.v0.magic))
    {
        return ERROR_populate_metadata_file_pwrite_FAILED;
    }
    offset += bytes_written;

    bytes_written = pwrite(fd, (void*)(&metadata.v0.metadata_version), sizeof(metadata.v0.metadata_version), offset);
    if (bytes_written != sizeof(metadata.v0.metadata_version))
    {
        return ERROR_populate_metadata_file_pwrite_FAILED;
    }
    offset += bytes_written;
    
    bytes_written = pwrite(fd, (void*)(&metadata.v1.dev_size), sizeof(metadata.v1.dev_size), offset);
    if (bytes_written != sizeof(metadata.v1.dev_size))
    {
        return ERROR_populate_metadata_file_pwrite_FAILED;
    }
    offset += bytes_written;
    
    bytes_written = pwrite(fd, (void*)(&metadata.v1.chunk_size), sizeof(metadata.v1.chunk_size), offset);
    if (bytes_written != sizeof(metadata.v1.chunk_size))
    {
        return ERROR_populate_metadata_file_pwrite_FAILED;
    }
    offset += bytes_written;

    bytes_written = pwrite(fd, (void*)(&metadata.v1.chunks_per_subdir), sizeof(metadata.v1.chunks_per_subdir), offset);
    if (bytes_written != sizeof(metadata.v1.chunks_per_subdir))
    {
        return ERROR_populate_metadata_file_pwrite_FAILED;
    }
    offset += bytes_written;

    return 0;
}

#define CREATE_FAIL_IF_EXISTS (O_CREAT|O_EXCL)
#define CHMOD_RW_______ (S_IRUSR|S_IWUSR)
#define CHMOD_RWX______ (S_IRWXU)

int create_metadata_file()
{
    int ok;
    size_t buf_size;
    int chars_written;
    int fd;

    buf_size = strlen(args.directory) + strlen("/metadata") + 1;
    char metadata_path[buf_size];
    chars_written = snprintf(metadata_path, buf_size, "%s/metadata", args.directory);
    if (chars_written != (buf_size-1))
    {
        return ERROR_create_metadata_file_snprintf_FAILED;
    }

    fd = open(metadata_path, O_WRONLY|CREATE_FAIL_IF_EXISTS, CHMOD_RW_______);
    if (fd == -1)
    {
        return ERROR_create_metadata_file_open_FAILED;
    }

    ok = populate_metadata_file(fd);
    if (ok != 0)
    {
        close(fd);
        return ok;
    }

    ok = close(fd);
    if (ok == -1)
    {
        return ERROR_create_metadata_file_close_FAILED;
    }

    return 0;
}

int create_directory_if_needed()
{
    int fd;
    int ok;

    // thanks to http://stackoverflow.com/a/9314702
    struct stat s;
    ok = stat(args.directory, &s);
    if (ok == -1)
    {
        if (errno == ENOENT) // directory does not exist
        {
            // see http://techoverflow.net/blog/2013/04/05/how-to-use-mkdir-from-sysstat-h/ 
            fd = mkdir(args.directory, CHMOD_RWX______);
            if (fd == -1)
            {
                return ERROR_create_directory_if_needed_mkdir_FAILED;
            }

            return 0;
        }
        else // other error
        {
            ERROR_create_directory_if_needed_stat_FAILED;
        }
    }
    else
    {
        if (S_ISDIR(s.st_mode)) // exists and is a directory
        {
            return 0;
        }
        else // exists but is NOT a directory
        {
            return ERROR_create_directory_if_needed_stat_EXISTS_BUT_NOT_DIR;
        }
    }

    return 0;
}

int create_chunks_directory()
{
    size_t buf_size;
    int chars_written;
    int fd;

    buf_size = strlen(args.directory) + strlen("/chunks") + 1;
    char chunks_dir_path[buf_size];
    chars_written = snprintf(chunks_dir_path, buf_size, "%s/chunks", args.directory);
    if (chars_written != (buf_size-1))
    {
        return ERROR_create_chunks_directory_snprintf_FAILED;
    }

    // see http://techoverflow.net/blog/2013/04/05/how-to-use-mkdir-from-sysstat-h/ 
    fd = mkdir(chunks_dir_path, CHMOD_RWX______);
    if (fd == -1)
    {
        return ERROR_create_chunks_directory_mkdir_FAILED;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int ok;

    parse_args(argc, argv);

    ok = populate_metadata_from_args();
    if (ok < 0)
    {
        print_error(ok);
        return ok * -1;
    }

    printf("Creating chunked block device:\n");
    printf("  directory: %s\n", args.directory);
    printf("  size: %llu bytes\n", metadata.v1.dev_size);
    printf("  chunk_size: %llu bytes\n", metadata.v1.chunk_size);

    if (metadata.v1.chunks_per_subdir)
    {
        printf("  chunks_per_subdir: %llu\n", metadata.v1.chunks_per_subdir);
    }
    else
    {
        printf("  chunks_per_subdir: 0 (subdirectories disabled)\n");
    }

    ok = create_directory_if_needed();
    if (ok < 0)
    {
        print_error(ok);
        return ok * -1;
    }

    ok = create_metadata_file();
    if (ok < 0)
    {
        print_error(ok);
        return ok * -1;
    }

    ok = create_chunks_directory();
    if (ok < 0)
    {
        print_error(ok);
        return ok * -1;
    }

    return EXIT_SUCCESS;
}