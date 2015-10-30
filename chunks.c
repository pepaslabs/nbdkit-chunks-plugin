/* nbdkit-chunks-plugin
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

// see https://github.com/libguestfs/nbdkit/blob/master/docs/nbdkit-plugin.pod

#include <config.h>
#include <nbdkit-plugin.h>

#define THREAD_MODEL NBDKIT_THREAD_MODEL_SERIALIZE_ALL_REQUESTS

static struct nbdkit_plugin plugin = {
  .name              = "chunks",
  .longname          = "nbdkit chunks plugin",
  .description       = "An nbdkit plugin which stores data in many small files, rather than one large backing file.",
  .version           = "0.0",
};

NBDKIT_REGISTER_PLUGIN(plugin)
