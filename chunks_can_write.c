/* chunks_can_write.c
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

// see also https://github.com/libguestfs/nbdkit/blob/master/docs/nbdkit-plugin.pod

#include "chunks_can_write.h"

#include "chunks_dev_t.h"

#include <stdio.h> // snprintf(), etc.
#include <stdlib.h> // mkdtemp(), etc.
#include <unistd.h> // unlink(), etc.
#include <string.h> // strlen(), etc.

#include <nbdkit-plugin.h> // nbdkit_error(), etc.

extern chunks_dev_t dev;

int _can_make_dir()
{
    char *template = "/chunks/tmp.XXXXXX";
    size_t buff_size = strlen(dev.dir_path) + strlen(template) + 1;
    char tmpd_path[buff_size];
    snprintf(tmpd_path, buff_size, "%s%s", dev.dir_path, template);

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

int _can_make_file()
{
    char *template = "/chunks/tmp.XXXXXX";
    size_t buff_size = strlen(dev.dir_path) + strlen(template) + 1;
    char tmpf_path[buff_size];
    snprintf(tmpf_path, buff_size, "%s%s", dev.dir_path, template);

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
    if (_can_make_dir() != 0)
    {
        return -1;
    }

    if (_can_make_file() != 0)
    {
        return -1;
    }

    return 0;
}