/* test_subdirs.c
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

// 10 chunks per subdir:

    chunk_path(0, 10, 9, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/0") == 0);

    chunk_path(1, 10, 9, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/1") == 0);

    chunk_path(9, 10, 9, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/9") == 0);

    chunk_path(0, 10, 99, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/0/00") == 0);

    chunk_path(123, 10, 999, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/1/2/123") == 0);

    chunk_path(12, 10, 99, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/1/12") == 0);

    chunk_path(98, 10, 99, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/9/98") == 0);

    chunk_path(99, 10, 99, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/9/99") == 0);

    chunk_path(100, 10, 999, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/1/0/100") == 0);

    chunk_path(102, 10, 999, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/1/0/102") == 0);

    chunk_path(0, 10, 999, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/0/0/000") == 0);

    chunk_path(1, 10, 999, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/0/0/001") == 0);

    chunk_path(992, 10, 999, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/9/9/992") == 0);

    chunk_path(1234, 10, 9999, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/1/2/3/1234") == 0);

// 100 chunks per subdir:

    chunk_path(0, 100, 999, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/00/0000") == 0);

    chunk_path(99, 100, 999, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/00/0099") == 0);

    chunk_path(100, 100, 999, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/01/0100") == 0);

    chunk_path(999, 100, 999, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/09/0999") == 0);

    chunk_path(1234, 100, 9999, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/12/1234") == 0);

    chunk_path(9876, 100, 9999, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/98/9876") == 0);

    chunk_path(9999, 100, 9999, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/99/9999") == 0);

    chunk_path(10000, 100, 99999, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/01/00/010000") == 0);

    chunk_path(123456, 100, 999999, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/12/34/123456") == 0);

    chunk_path(123456, 100, 9999999, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/00/12/34/00123456") == 0);

    chunk_path(123456, 100, 99999999, buf, sizeof(buf));
    assert(strcmp(buf, "chunks/00/12/34/00123456") == 0);

    return EXIT_SUCCESS;
}

