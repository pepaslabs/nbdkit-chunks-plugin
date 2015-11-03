// local redefinition of nbdkit_error() for use with command-line utilities.

#include "local_nbdkit_error.h"

#include <stdio.h>

void nbdkit_error(const char *fs, ...)
{
  va_list args;

  fprintf(stderr, "ERROR: ");

  va_start(args, fs);
  vfprintf(stderr, fs, args);
  va_end(args);

  fprintf(stderr, "\n");
}