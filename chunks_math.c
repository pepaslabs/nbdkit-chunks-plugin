/* chunks_math.c
 * See https://github.com/pepaslabs/nbdkit-chunks-plugin
 * Copyright (C) 2015 Jason Pepas.
 * Released under the terms of the MIT license.  See https://opensource.org/licenses/MIT
 */

#include "chunks_math.h"

bool is_divisible_by(uint64_t x, uint64_t y)
{
    return x % y == 0;
}

bool is_power_of_two(uint64_t x)
{
    // thanks to http://stackoverflow.com/a/600306
    return (x & (x - 1)) == 0;
}
