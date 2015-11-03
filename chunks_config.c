/* chunks_config.c
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

// see also https://github.com/libguestfs/nbdkit/blob/master/docs/nbdkit-plugin.pod

#include "chunks_config.h"

#include "chunks_dev_t.h"
#include "chunks_metadata.h" // read_metadata_and_populate_chunks_dev(), etc.

#include <string.h> // strcmp(), etc.
#include <nbdkit-plugin.h> // nbdkit_absolute_path(), etc.

extern chunks_dev_t dev;

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
    int ok;

    if (dev.dir_path == NULL)
    {
        nbdkit_error("'dir' is a required parameter");
        return -1;
    }

    metadata_t metadata;
    ok = read_metadata(&dev, &metadata);
    if (ok != 0) return -1;

    populate_chunks_dev_from_metadata(&dev, &metadata);

    return 0;
}