/* nbdkit-chunks-plugin
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

#include <config.h>
#include <nbdkit-plugin.h>

#define THREAD_MODEL NBDKIT_THREAD_MODEL_SERIALIZE_REQUESTS

static struct nbdkit_plugin plugin = {
  .name              = "chunks",
  .longname          = "nbdkit chunks plugin",
  .version           = PACKAGE_VERSION,
};

NBDKIT_REGISTER_PLUGIN(plugin)
