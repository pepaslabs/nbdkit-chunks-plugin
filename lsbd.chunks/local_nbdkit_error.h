/* local_nbdkir_error.h
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

// this is a local redefinition of nbdkit_error() for use with command-line utilities.

#ifndef LOCAL_NBDKIT_ERROR_H
#define LOCAL_NBDKIT_ERROR_H

#include <stdarg.h>

void nbdkit_error(const char *fs, ...);

#endif // LOCAL_NBDKIT_ERROR_H
