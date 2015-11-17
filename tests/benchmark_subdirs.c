/* benchmark_subdirs.c
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

#include "subdirs.h"

#include <stdlib.h> // EXIT_SUCCESS, etc.
#include <assert.h> // assert(), etc.
#include <string.h> // strcmp(), etc.
#include <stdio.h> // printf(), etc.

int main(int argc, char *argv[])
{
    char buf[1024];

    for(uint32_t i=0; i<1000000; i++)
    {
        chunk_path(123456789, 1000, 999999999, buf, sizeof(buf));
    }

    return EXIT_SUCCESS;
}
