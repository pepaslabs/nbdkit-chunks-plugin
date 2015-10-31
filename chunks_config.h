/* chunks_config.h
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

// see also https://github.com/libguestfs/nbdkit/blob/master/docs/nbdkit-plugin.pod

#ifndef CHUNKS_CONFIG_H
#define CHUNKS_CONFIG_H

#define chunks_config_help "dir: absolute path to the directory where the metadata and chunks are stored."

int chunks_config(const char *key, const char *value);
int chunks_config_complete();

#endif // CHUNKS_CONFIG_H