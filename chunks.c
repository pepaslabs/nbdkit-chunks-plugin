/* nbdkit-chunks-plugin
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

// see https://github.com/libguestfs/nbdkit/blob/master/docs/nbdkit-plugin.pod

#include <config.h>
#include <nbdkit-plugin.h>

#include <string.h> // strcmp()
#include <stdint.h>


#define THREAD_MODEL NBDKIT_THREAD_MODEL_SERIALIZE_ALL_REQUESTS


// metadata about this block device.
struct _metadata_v0_t
{
	// format version number of this metadata structure.
	uint8_t metadata_version;
};
typedef struct _metadata_v0_t metadata_v0_t;


// metadata about this block device.
struct _metadata_v1_t
{
	// format version number of this metadata structure.
	uint8_t metadata_version;

	// size of the block device in bytes.  should be divisible by chunk_size.
	uint64_t dev_size;

	// size of each chunk in bytes.  must be a power of 2.
	uint64_t chunk_size;
};
typedef struct _metadata_v1_t metadata_v1_t;


int chunks_config(const char *key, const char *value)
{
	if (strcmp(key, "dir") == 0)
	{
		char *chunks_dir = nbdkit_absolute_path(value);
		if (chunks_dir == NULL)
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

static struct nbdkit_plugin plugin = {
  .name              = "chunks",
  .longname          = "nbdkit chunks plugin",
  .description       = "An nbdkit plugin which stores data in many small files, rather than one large backing file.",
  .version           = "0.0",
  .config            = chunks_config
};

NBDKIT_REGISTER_PLUGIN(plugin)
