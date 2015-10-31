/* chunks_nbdkit.c
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

// see also https://github.com/libguestfs/nbdkit/blob/master/docs/nbdkit-plugin.pod

#include <config.h>
#include <nbdkit-plugin.h>

#include "chunks_dev_t.h"
#include "chunks_handle_t.h"
#include "chunks_metadata.h"

#include "chunks_can_write.h"

#include <string.h> // strcmp(), etc.



#define THREAD_MODEL NBDKIT_THREAD_MODEL_SERIALIZE_ALL_REQUESTS


// per block-device state:
chunks_dev_t dev;


// per-connection state:
chunks_handle_t handle;


#define chunks_config_help "dir: absolute path to the directory where the metadata and chunks are stored."


int chunks_config(const char *key, const char *value)
{
    if (strcmp(key, "dir") == 0)
    {
        dev.dir_path = nbdkit_absolute_path(value);
        if (dev.dir_path == NULL)
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
    if (dev.dir_path == NULL)
    {
        nbdkit_error("'dir' is a required parameter");
        return -1;
    }

    if (read_metadata_and_populate_chunks_dev(&dev) != 0)
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
    return (int64_t)(dev.dev_size);
}


// --- pread


int chunks_pread(void *passed_handle, void *buf, uint32_t count, uint64_t offset)
{
    // split the read up into chunks
    // read each chunk

    uint64_t first_chunk_number = offset << dev.chunk_shift;
    uint64_t first_chunk_offset = offset % dev.chunk_size;
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
