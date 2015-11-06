/* chunks_nbdkit.c
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

// see also https://github.com/libguestfs/nbdkit/blob/master/docs/nbdkit-plugin.pod

#include <config.h>
#include <nbdkit-plugin.h> // nbdkit_error(), etc.

#include "chunks_dev_t.h"
#include "chunks_handle_t.h"

#include "chunks_config.h"
#include "chunks_can_write.h"
#include "chunks_pread.h"
#include "chunks_pwrite.h"

#define THREAD_MODEL NBDKIT_THREAD_MODEL_SERIALIZE_ALL_REQUESTS

// per block-device state:
chunks_dev_t dev;

// per-connection state:
chunks_handle_t handle;

void* chunks_open(int readonly)
{
    return (void*)(&handle);
}

int64_t chunks_get_size(void *passed_handle)
{
    return (int64_t)(dev.dev_size);
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

  .get_size          = chunks_get_size,

  .pread             = chunks_pread,
  .pwrite            = chunks_pwrite
};

NBDKIT_REGISTER_PLUGIN(plugin)