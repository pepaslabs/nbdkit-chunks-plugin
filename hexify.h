/* hexify.h
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

#ifndef HEXIFY_H
#define HEXIFY_H

#include <stddef.h> // size_t, etc.

int hexify(unsigned char *in, size_t in_size, char *out, size_t out_size);

#endif // HEXIFY_H