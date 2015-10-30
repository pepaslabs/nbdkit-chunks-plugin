/* nbdkit-chunks-plugin
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

static struct nbdkit_plugin plugin = {
  .name              = "chunks",
  .longname          = "nbdkit chunks plugin",
  .version           = PACKAGE_VERSION,
};

NBDKIT_REGISTER_PLUGIN(plugin)
