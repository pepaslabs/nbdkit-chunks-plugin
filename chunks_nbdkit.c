/* chunks_nbdkit.c
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

// see also https://github.com/libguestfs/nbdkit/blob/master/docs/nbdkit-plugin.pod

#include <config.h>
#include <nbdkit-plugin.h>

#include "chunks_metadata.h"

#include <string.h> // strcmp(), etc.



#define THREAD_MODEL NBDKIT_THREAD_MODEL_SERIALIZE_ALL_REQUESTS


// per block-device state:
char *chunks_dir_path = NULL;
metadata_t metadata;
uint8_t chunk_shift;


// per-connection state:
struct _chunks_handle_t
{

};
typedef struct _chunks_handle_t chunks_handle_t;

chunks_handle_t handle;


#define chunks_config_help "dir: absolute path to the directory where the metadata and chunks are stored."


int chunks_config(const char *key, const char *value)
{
    if (strcmp(key, "dir") == 0)
    {
        chunks_dir_path = nbdkit_absolute_path(value);
        if (chunks_dir_path == NULL)
        {
            nbdkit_error("nbdkit_absolute_path() failed on dir '%s'", value);
            return -1;
        }

        return 0;
    }
    else
    {
        nbdkit_error("Unrecognized parameter: '%s'", key);
        return -1;
    }
}

int chunks_config_complete()
{
    if (chunks_dir_path == NULL)
    {
        nbdkit_error("'dir' is a required parameter");
        return -1;
    }

    if (read_metadata() != 0)
    {
        return -1;
    }

    return 0;
}

void* chunks_open(int readonly)
{
    return (void*)(&handle);
}

void chunks_close(void *passed_handle)
{
    ;
}

int64_t chunks_get_size(void *passed_handle)
{
    return (int64_t)(metadata.v1.dev_size);
}


// --- can_write


#include <stdio.h> // snprintf(), etc.
#include <stdlib.h> // mkdtemp(), etc.
#include <unistd.h> // unlink(), etc.

int can_make_dir()
{
    char *template = "/chunks/tmp.XXXXXX";
    size_t buff_size = strlen(chunks_dir_path) + strlen(template) + 1;
    char tmpd_path[buff_size];
    snprintf(tmpd_path, buff_size, "%s%s", chunks_dir_path, template);

    char *created_tmpd_path = mkdtemp(tmpd_path);
    if (created_tmpd_path == NULL)
    {
        nbdkit_error("Can't mkdtemp '%s'", tmpd_path);
        return -1;
    }

    if (unlink(created_tmpd_path) != 0)
    {
        nbdkit_error("Can't unlink '%s'", created_tmpd_path);
        return -1;
    }

    return 0;
}

int can_make_file()
{
    char *template = "/chunks/tmp.XXXXXX";
    size_t buff_size = strlen(chunks_dir_path) + strlen(template) + 1;
    char tmpf_path[buff_size];
    snprintf(tmpf_path, buff_size, "%s%s", chunks_dir_path, template);

    if (mkstemp(tmpf_path) == -1)
    {
        nbdkit_error("Can't mkstemp '%s'", tmpf_path);
        return -1;
    }

    if (unlink(tmpf_path) != 0)
    {
        nbdkit_error("Can't unlink '%s'", tmpf_path);
        return -1;
    }

    return 0;
}

int chunks_can_write(void *passed_handle)
{
    if (can_make_dir() != 0)
    {
        return -1;
    }

    if (can_make_file() != 0)
    {
        return -1;
    }

    return 0;
}


// --- pread


int chunks_pread(void *passed_handle, void *buf, uint32_t count, uint64_t offset)
{
    // split the read up into chunks
    // read each chunk

    uint64_t first_chunk_number = offset << chunk_shift;
    uint64_t first_chunk_offset = offset % metadata.v1.chunk_size;
}


static struct nbdkit_plugin plugin = {
  .name              = "chunks",
  .longname          = "nbdkit chunks plugin",
  .description       = "An nbdkit plugin which stores data in many small files, rather than one large backing file.",
  .version           = "0.0",

  .config            = chunks_config,
  .config_complete   = chunks_config_complete,
  .config_help       = chunks_config_help,

  .open              = chunks_open,
  .close             = chunks_close,

  .get_size          = chunks_get_size,
  .can_write         = chunks_can_write,

  .pread             = chunks_pread
};

NBDKIT_REGISTER_PLUGIN(plugin)
