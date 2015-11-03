// local redefinition of nbdkit_error() for use with command-line utilities.

#ifndef LOCAL_NBDKIT_ERROR_H
#define LOCAL_NBDKIT_ERROR_H

#include <stdarg.h>

void nbdkit_error(const char *fs, ...);

#endif // LOCAL_NBDKIT_ERROR_H